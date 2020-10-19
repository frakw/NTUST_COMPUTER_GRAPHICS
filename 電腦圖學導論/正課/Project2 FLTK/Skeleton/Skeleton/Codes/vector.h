#ifndef _MYVECTOR_H
#define _MYVECTOR_H

class vector4 {
public:
	vector4(float ix, float iy, float iz, float iw) :x(ix), y(iy), z(iz), w(iw) {}
	vector4(float ix, float iy, float iz) :x(ix), y(iy), z(iz){}
	vector4() {}
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1;
	
};

vector4 vector_add(vector4& v1, vector4& v2);
vector4 vector_sub(vector4& v1, vector4& v2);
vector4 vector_mul(vector4& v1, float k);
vector4 vector_div(vector4& v1, float k);
float vector_dot_product(vector4& v1, vector4& v2);
float vector_length(vector4& v);
vector4 vector_normalize(vector4& v);
vector4 vector_cross_product(vector4& v1, vector4& v2);
vector4 vector_intersect_plane(vector4& plane_p, vector4& plane_n, vector4& lineStart, vector4& lineEnd);


#endif // !_VECTOR_H
