#ifndef _MATRIX_H
#define _MATRIX_H
#include "vector.h"
class mat4x4 {
public:
	mat4x4(bool i) :major(i) {}
	float m[4][4] = { 0 };//column major
	float& get(int i, int j);
	void make_identity();
	void make_translation(float x, float y, float z);
	void make_projection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);
	static bool ROW_MAJOR;
	static bool COL_MAJOR;
	bool major;
};


vector4 matrix_multiply_vector(mat4x4& im, vector4& iv);
mat4x4 matrix_multiply_matrix(mat4x4& m1, mat4x4& m2);
#endif // !_MATRIX_H
