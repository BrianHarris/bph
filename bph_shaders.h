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

 typedef void(*bph_shader_error_callback)(void * user, const char * message);

GLuint bph_shader_load(GLenum type, const GLchar *shaderSrc, bph_shader_error_callback callback, void * user)
#ifdef BPH_SHADERS_IMPLEMENTATION
{
	GLuint shader = glCreateShader(type);
	if (shader == 0) {
		if (callback != 0) {
			callback(user, "glCreateShader Failed");
		}
		return 0;
	}

	glShaderSource(shader, 1, &shaderSrc, 0);
	glCompileShader(shader);

	GLint compiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (compiled == 0) {
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1 && callback != 0) {
			char * infoLog = (char *)malloc(sizeof(char) * infoLen);
			glGetShaderInfoLog(shader, infoLen, 0, infoLog);
			callback(user, infoLog);
			free(infoLog);
		}
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}
#else
;
#endif

GLuint bph_program_load(GLuint vertexShader, GLuint fragmentShader, const GLchar ** attribLocations, bph_shader_error_callback callback, void * user)
#ifdef BPH_SHADERS_IMPLEMENTATION
{
	GLuint program = glCreateProgram();
	if (program == 0) {
		if (callback != 0) {
			callback(user, "glCreateProgram Failed");
		}
		return 0;
	}

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	if (attribLocations != 0) {
		for (GLuint i = 0; attribLocations[i] != 0; i++) {
			glBindAttribLocation(program, i, attribLocations[i]);
		}
	}

	glLinkProgram(program);

	GLint linked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (linked == 0) {
		GLint infoLen = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1 && callback != 0) {
			char* infoLog = (char *)malloc(sizeof(char) * infoLen);
			glGetProgramInfoLog(program, infoLen, NULL, infoLog);
			callback(user, infoLog);
			free(infoLog);
		}
		glDeleteProgram(program);
		return 0;
	}
	return program;
}
#else
;
#endif
