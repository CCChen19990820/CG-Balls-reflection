#ifndef GEOH
#define GEOH
#include "ray.h"
using namespace std;
class material;
typedef struct hit_record {
	float t;
	vec3 p;
	vec3 nv;
	vec3 C;
}hit_record;

class hitable { //geometry parent class
public:
	virtual bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const = 0;
};

class sphere : public hitable {
public:
	sphere() {}
	sphere(vec3 c, float r,vec3 _kd=vec3(1.0,1.0,1.0), float w_ri = 0.0f, float w_ti = 0.0f) : 
		center(c), radius(r), kd(_kd), w_r(w_ri), w_t(w_ti) {};
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
	vec3 center;
	float radius;
	vec3 kd;
	float w_r;//reflected
	float w_t;//transmitted
};

bool sphere::hit(const ray &r, float tmin, float tmax, hit_record & rec) const {
	/*
	To-do:
		compute whether the ray intersect the sphere
	*/
	float a = dot(r.direction()-r.origin(), r.direction()-r.origin());
	vec3 OtoC = r.origin() - center;
	float b = 2 * dot(r.direction()-r.origin(), OtoC);
	float c = dot(OtoC, OtoC) - radius * radius;
	float determine = ((b*b) - (4*a*c));
	float t = 0.0f;

	if (determine >= 0.0f) {
		t = ((-1)*b - sqrt(determine)) / (2 * a);
		if(!(t>tmin && t<tmax)){
			t = ((-1)*b + sqrt(determine) / (2 * a));
			if (t >tmin && t < tmax) {
				if (t < rec.t) {
					rec.t = t;
					rec.p = r.point_at_parameter(t);
					rec.nv = rec.p - center;
					//rec.C = center;
				} return true;
			} return false;
		}
		if(t<rec.t){
			rec.t = t;
			rec.p = r.point_at_parameter(t);
			rec.nv = rec.p-center;
			//rec.C = center;
			return true;
		}
	}
	else{
		return false;
	}

}

vec3 reflect(const vec3 &d, const vec3 &nv) {
	//compute the reflect direction
	//To-do
	vec3 I = vec3(0,0,0) - d;
	vec3 N = nv;
	float NdotI = dot(N,I);
	/*
	if (NdotI>0) {
		N = N*(-1);
	}
	if(NdotI<=0){
		NdotI = NdotI*(-1);
	}*/
	vec3 R = 2 * N * dot(N,I) - I;
	//R.make_unit_vector();
	return R;
}

vec3 refract(const vec3& v, const vec3& n, float r) {
	//compute the refracted direction
	//To-do
	vec3 I = v;
	vec3 N = n;
	//I.make_unit_vector();
	//N.make_unit_vector();
	float cos = dot((vec3(0,0,0) - I) , N);
	//float cos = dot(I , N);
	if(cos < 0.0f){
		r = 1 / r;
		cos = -cos;
	}
	else{
		N = N * (-1);
	}
	float sin2 = 1 - cos * cos;
	if(1 - r * r * sin2){
		return (I +  cos * N) * r - sqrt(1 - r * r * sin2) * N;
	}
	else{
		return (I + cos * n) * r;
		//return vec3(0,0,0);
	}

}
#endif
