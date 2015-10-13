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

void bph_matrix_identity(float * matrix)
#ifdef BPH_MATRIX_IMPLEMENTATION
{
	matrix[0] = 1.0f;
	matrix[1] = 0.0f;
	matrix[2] = 0.0f;
	matrix[3] = 0.0f;

	matrix[4] = 0.0f;
	matrix[5] = 1.0f;
	matrix[6] = 0.0f;
	matrix[7] = 0.0f;

	matrix[8] = 0.0f;
	matrix[9] = 0.0f;
	matrix[10] = 1.0f;
	matrix[11] = 0.0f;

	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = 0.0f;
	matrix[15] = 1.0f;
}
#else
;
#endif

/*
General purpose ortho matrix
You should probably use ortho_simple or ortho_symmetric
*/
void bph_matrix_ortho(float * matrix, float left, float right, float top, float bottom, float near, float far)
#ifdef BPH_MATRIX_IMPLEMENTATION
{
	matrix[0] = 2.0f / (right - left);
	matrix[1] = 0.0f;
	matrix[2] = 0.0f;
	matrix[3] = 0.0f;

	matrix[4] = 0.0f;
	matrix[5] = 2.0f / (top - bottom);
	matrix[6] = 0.0f;
	matrix[7] = 0.0f;

	matrix[8] = 0.0f;
	matrix[9] = 0.0f;
	matrix[10] = -2.0f / (far - near);
	matrix[11] = 0.0f;

	matrix[12] = -(right + left) / (right - left);
	matrix[13] = -(top + bottom) / (top - bottom);
	matrix[14] = -(far + near) / (far - near);
	matrix[15] = 1.0f;
}
#else
;
#endif

/*
Simple ortho where left = 0, top = 0, near = 0, far = 1
*/
void bph_matrix_ortho_simple(float * matrix, float width, float height)
#ifdef BPH_MATRIX_IMPLEMENTATION
{
	matrix[0] = 2.0f / width;
	matrix[1] = 0.0f;
	matrix[2] = 0.0f;
	matrix[3] = 0.0f;

	matrix[4] = 0.0f;
	matrix[5] = -2.0f / height;
	matrix[6] = 0.0f;
	matrix[7] = 0.0f;

	matrix[8] = 0.0f;
	matrix[9] = 0.0f;
	matrix[10] = -2.0f;
	matrix[11] = 0.0f;

	matrix[12] = -1.0f;
	matrix[13] =  1.0f;
	matrix[14] = -1.0f;
	matrix[15] = 1.0f;
}
#else
;
#endif

/*
Simple ortho where 0,0 in the center
*/
void bph_matrix_ortho_symmetric(float * matrix, float width, float height)
#ifdef BPH_MATRIX_IMPLEMENTATION
{
	matrix[0] = 2.0f / width;
	matrix[1] = 0.0f;
	matrix[2] = 0.0f;
	matrix[3] = 0.0f;

	matrix[4] = 0.0f;
	matrix[5] = -2.0f / height;
	matrix[6] = 0.0f;
	matrix[7] = 0.0f;

	matrix[8] = 0.0f;
	matrix[9] = 0.0f;
	matrix[10] = -2.0f;
	matrix[11] = 0.0f;

	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = -1.0f;
	matrix[15] = 1.0f;
}
#else
;
#endif

/*
General purpose perspective matrix
You should probably use bph_matrix_perspective_symmetric
*/
void bph_matrix_perspective(float * matrix, float left, float right, float top, float bottom, float near, float far)
#ifdef BPH_MATRIX_IMPLEMENTATION
{
	matrix[0] = ( 2.0f * near ) / (right - left);
	matrix[1] = 0.0f;
	matrix[2] = 0.0f;
	matrix[3] = 0.0f;

	matrix[4] = 0.0f;
	matrix[5] = (2.0f * near) / (top - bottom);
	matrix[6] = 0.0f;
	matrix[7] = 0.0f;

	matrix[8] = (right + left) / (right - left);
	matrix[9] = (top + bottom) / (top - bottom);
	matrix[10] = -(far + near) / (far - near);
	matrix[11] = -1.0f;

	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = ( -2.0f * far * near ) / (far - near);
	matrix[15] = 0.0f;
}
#else
;
#endif

/*
Symmetric perspective.
*/
void bph_matrix_perspective_symmetric(float * matrix, float width, float height, float near, float far)
#ifdef BPH_MATRIX_IMPLEMENTATION
{
	matrix[0] = near / (width * 0.5f);
	matrix[1] = 0.0f;
	matrix[2] = 0.0f;
	matrix[3] = 0.0f;

	matrix[4] = 0.0f;
	matrix[5] = near / (height * 0.5f);
	matrix[6] = 0.0f;
	matrix[7] = 0.0f;

	matrix[8] = 0.0f;
	matrix[9] = 0.0f;
	matrix[10] = -(far + near) / (far - near);
	matrix[11] = -1.0f;

	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = (-2.0f * far * near) / (far - near);
	matrix[15] = 0.0f;
}
#else
;
#endif

/*
Symmetric perspective with an infinite far plane.
*/
void bph_matrix_perspective_infinite_far(float * matrix, float width, float height, float near)
#ifdef BPH_MATRIX_IMPLEMENTATION
{
	matrix[0] = near / (width * 0.5f);
	matrix[1] = 0.0f;
	matrix[2] = 0.0f;
	matrix[3] = 0.0f;

	matrix[4] = 0.0f;
	matrix[5] = near / (height * 0.5f);
	matrix[6] = 0.0f;
	matrix[7] = 0.0f;

	matrix[8] = 0.0f;
	matrix[9] = 0.0f;
	matrix[10] = -1.0f;
	matrix[11] = -1.0f;

	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = -2.0f * near;
	matrix[15] = 0.0f;
}
#else
;
#endif
