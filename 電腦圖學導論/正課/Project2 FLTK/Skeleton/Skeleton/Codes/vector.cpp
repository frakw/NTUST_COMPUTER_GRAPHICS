#include "vector.h"
#include <fl\math.h>

vector4 vector_add(vector4& v1, vector4& v2)
{
	return vector4( v1.x + v2.x, v1.y + v2.y, v1.z + v2.z );
}

vector4 vector_sub(vector4& v1, vector4& v2)
{
	return vector4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z );
}

vector4 vector_mul(vector4& v1, float k)
{
	return vector4(v1.x * k, v1.y * k, v1.z * k );
}

vector4 vector_div(vector4& v1, float k)
{
	return vector4(v1.x / k, v1.y / k, v1.z / k );
}

float vector_dot_product(vector4& v1, vector4& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float vector_length(vector4& v)
{
	return sqrtf(vector_dot_product(v, v));
}

vector4 vector_normalize(vector4& v)
{
	float l = vector_length(v);
	return { v.x / l, v.y / l, v.z / l };
}

vector4 vector_cross_product(vector4& v1, vector4& v2)
{
	vector4 v;
	v.x = v1.y * v2.z - v1.z * v2.y;
	v.y = v1.z * v2.x - v1.x * v2.z;
	v.z = v1.x * v2.y - v1.y * v2.x;
	return v;
}

vector4 vector_intersect_plane(vector4& plane_p, vector4& plane_n, vector4& lineStart, vector4& lineEnd)
{
	plane_n = vector_normalize(plane_n);
	float plane_d = -vector_dot_product(plane_n, plane_p);
	float ad = vector_dot_product(lineStart, plane_n);
	float bd = vector_dot_product(lineEnd, plane_n);
	float t = (-plane_d - ad) / (bd - ad);
	vector4 lineStartToEnd = vector_sub(lineEnd, lineStart);
	vector4 lineToIntersect = vector_mul(lineStartToEnd, t);
	return vector_add(lineStart, lineToIntersect);
}
