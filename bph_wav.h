/*
 * Copyright (c) 2015, NVIDIA CORPORATION. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

// Return the number of bytes read
// Return 0 for eof, -1 for any other errors
typedef int(*bph_wav_read_callback)(void * user, void * buffer, int size);

// This is called with information about the sound file
// Return false to abort reading this file
// A channel contains info for a single speaker
// A sample is a single quanta of data for a channel
// A frame contains a sample for each channel
// numChannels is the number of channels (and samples per frame)
// sampleRate is the number of frames per second
// frameSize is the size (in bytes) of each frame
// bitsPerSample is the number of bits per sample
// frameSize is normally numChannels * bitsPerSample / 8, but it could be larger
typedef bool(*bph_wav_header_callback)(void * user, int numChannels, int sampleRate, int frameSize, int bitsPerSample);

// This is called when the actual sound data is located
// You should read 'size' bytes into your buffer
// Return false to abort reading this file
typedef bool(*bph_wav_data_callback)(void * user, int size);

// Location is always absolute (equivalent to SEEK_SET)
// Return false if there was an error seeking there (for example, it's past the end of the file)
typedef bool(*bph_wav_seek_callback)(void * user, int location);

// This is called prior to returning false, so you can log more detailed error messages
typedef void(*bph_wav_error_callback)(void * user, const char * message);

bool bph_wav_load(bph_wav_read_callback read, bph_wav_header_callback header, bph_wav_data_callback data, bph_wav_seek_callback seek, bph_wav_error_callback error, void * user)
#ifdef BPH_WAV_IMPLEMENTATION
{
	struct {
		int id;
		int size;
		int format;
	} riff;

	if (read(user, &riff, sizeof(riff)) != sizeof(riff)) {
		error(user, "Could not read RIFF header");
		return false;
	}

	if (riff.id != 0x46464952) {
		error(user, "Not a RIFF file");
		return false;
	}
	if (riff.size <= 36) {
		error(user, "Invalid riff file (header size too small)");
		return false;
	}
	if (riff.format != 0x45564157) {
		error(user, "Not a WAVE file");
		return false;
	}

	int i = sizeof(riff);
	while (i < sizeof(riff.format) + riff.size && seek(user, i)) {
		struct {
			int id;
			int size;
		} chunk;

		if (read(user, &chunk, sizeof(chunk)) != sizeof(chunk)) {
			error(user, "Could not read chunk");
			return false;
		}

		switch (chunk.id) {
			case 0x20746d66: // fmt
			{
				struct {
					short format;
					short numChannels;
					int sampleRate;
					int byteRate;
					short frameSize;
					short bitsPerSample;
				} fmt;
				if (read(user, &fmt, sizeof(fmt)) != sizeof(fmt)) {
					error(user, "Could not read format chunk");
					return false;
				}
				if (fmt.format != 1) {
					error(user, "Not a PCM file");
					return false;
				}
				if (!header(user, fmt.numChannels, fmt.sampleRate, fmt.frameSize, fmt.bitsPerSample)) {
					error(user, "Unsupported format");
					return false;
				}
				break;
			}
			case 0x61746164: // data
			{
				if (!data(user, chunk.size)) {
					error(user, "Could not read data");
					return false;
				}
				break;
			}
		}
		i += sizeof(chunk) + chunk.size;
	}
	return true;
}
#else
;
#endif