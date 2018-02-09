#ifndef VECTOR3
#define VECTOR3
#include <math.h>
typedef struct v3{
	float x;
	float y;
	float z;
}vector3;

//adds v2 to v1
vector3 add(const vector3* v1,const vector3* v2){
	vector3 v3;
	v3.x = v1->x + v2->x;
	v3.y = v1->y + v2->y;
	v3.z = v1->z + v2->z;
	return v3;
}

void addEqual( vector3* v1, vector3* v2){
	v1->x += v2->x;
	v1->y += v2->y;
	v1->z += v2->z;
}

//subtracts v2 from v1
vector3 sub(vector3* v1, vector3* v2){
	vector3 v3;
	v3.x = v1->x - v2->x;
	v3.y = v1->y - v2->y;
	v3.z = v1->z - v2->z;
	return v3;
}

void subEqual( vector3* v1, vector3* v2){
	v1->x -= v2->x;
	v1->y -= v2->y;
	v1->z -= v2->z;
}

vector3 mul(vector3* v1, vector3* v2){
	vector3 v3;
	v3.x = v1->x * v2->x;
	v3.y = v1->y * v2->y;
	v3.z = v1->z * v2->z;
	return v3;
}

vector3 mulScalar(vector3* v1, float s){
	vector3 v3;
	v3.x = v1->x * s;
	v3.y = v1->y * s;
	v3.z = v1->z * s;
	return v3;
}

float dot(const vector3* v1, const vector3* v2){
	return (v1->x * v2->x + v1->y * v2->y + v1->z * v2->z);
}

vector3 cross(const vector3* v1, const vector3* v2){
	vector3 v3;
	v3.x = v1->y * v2->z - v1->z * v2->y;
	v3.y = v1->z * v2->x - v1->x * v2->z;
	v3.z = v1->x * v2->y - v1->y * v2->x;
	return v3;
}

vector3 normalize(const vector3* v){
	vector3 vr;
	float l = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
	vr.x = v->x / l;
	vr.y = v->y / l;
	vr.z = v->z / l;
	return vr;
}

#endif
