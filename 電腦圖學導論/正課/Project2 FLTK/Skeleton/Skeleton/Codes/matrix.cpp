#include "matrix.h"
#include <memory.h>
#include <math.h>
bool mat4x4::ROW_MAJOR = false;
bool mat4x4::COL_MAJOR = true;
float& mat4x4::get(int i, int j) {
	if (major)
		return m[j][i];
	else
		return m[i][j];
}
void mat4x4::make_identity() {
	memset((void*)m, 0, 16 * sizeof(float));
	m[0][0] = 1.0f;
	m[1][1] = 1.0f;
	m[1][1] = 1.0f;
	m[1][1] = 1.0f;
}
void mat4x4::make_translation(float x, float y, float z) {
	make_identity();
	get(3, 0) = x;
	get(3, 1) = y;
	get(3, 2) = z;
}
void mat4x4::make_projection(float fFovDegrees, float fAspectRatio, float fNear, float fFar) {
	float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159f);
	get(0, 0) = fAspectRatio * fFovRad;
	get(1, 1) = fFovRad;
	get(2, 2) = fFar / (fFar - fNear);
	get(3, 2) = (-fFar * fNear) / (fFar - fNear);
	get(2, 3) = 1.0f;
	get(3, 3) = 0.0f;
}

vector4 matrix_multiply_vector(mat4x4& im, vector4& iv) {
	vector4 v;
	v.x = iv.x * im.get(0, 0) + iv.y * im.get(1, 0) + iv.z * im.get(2, 0) + iv.w * im.get(3, 0);
	v.y = iv.x * im.get(0, 1) + iv.y * im.get(1, 1) + iv.z * im.get(2, 1) + iv.w * im.get(3, 1);
	v.z = iv.x * im.get(0, 2) + iv.y * im.get(1, 2) + iv.z * im.get(2, 2) + iv.w * im.get(3, 2);
	v.w = iv.x * im.get(0, 3) + iv.y * im.get(1, 3) + iv.z * im.get(2, 3) + iv.w * im.get(3, 3);
	return v;
}
mat4x4 matrix_multiply_matrix(mat4x4& m1, mat4x4& m2) {
	mat4x4 matrix(m1.major);
	for (int c = 0; c < 4; c++)
		for (int r = 0; r < 4; r++)
			matrix.get(r, c) = m1.get(r, 0) * m2.get(0, c) + m1.get(r, 1) * m2.get(1, c) + m1.get(r, 2) * m2.get(2, c) + m1.get(r, 3) * m2.get(3, c);

	return matrix;
}