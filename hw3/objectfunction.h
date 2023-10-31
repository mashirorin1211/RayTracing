#pragma once
#include<iostream>
#include "algebra3.h"
#include <vector>


#ifndef _OBJECTFUNCTION_H_
#define _OBJECTFUNCTION_H_



class triangle {
private :
	vec3 trianglepoint0;
	vec3 trianglepoint1;
	vec3 trianglepoint2;
	std::vector<float> material;
public:

	triangle(void);
	triangle(const vec3 point1, const vec3 point2,const vec3 point3,const std::vector<float> mat);
	triangle(const triangle& v);			// copy constructor
	void set(vec3 x, vec3 y, vec3 z,std::vector<float> mat);
	vec3 gettrianglepoint0();
	vec3 gettrianglepoint1();
	vec3 gettrianglepoint2();
	std::vector<float> getmaterial();
	
};


class triangle;

class sphere {
private:
	vec3 spherecenter;
	float sphereradius;
	std::vector<float> material;
public:

	sphere(void);
	sphere(const vec3 center, const float radius, const std::vector<float> material);
	sphere(const sphere& v);			// copy constructor
	void set(vec3 center, float radius, std::vector<float> mat);
	vec3& operator [] (int i);
	std::vector<float> getmaterial();
	vec3& getcenter();
	float& getradius();
};


float hit_triangle(int trianglenumber, std::vector<triangle> trianglepoint, vec3 eyeposition, vec3 screenpoint);
vec3 trianglecolor(vec3 screenpoint, vec3 eyeposition, int trianglenumber, vec3 light, float t, std::vector<triangle> trianglepoint, std::vector<sphere> sphere, int reflecttime);
vec3 depth_of_field_trianglecolor(vec3 screenpoint, vec3 eyeposition, int trianglenumber, vec3 light, float t, std::vector<triangle> trianglepoint, std::vector<sphere> sphere, int reflecttime, int doftime);

float hit_sphere(int spherenumber, std::vector<sphere> sphere, vec3 eyeposition, vec3 screenpoint);
vec3 spherecolor(vec3 screenpoint, vec3 eyeposition,int spherenumber, std::vector<sphere> sphere, vec3 light, float t, std::vector<triangle> trianglepoint, int reflecttime);
vec3 depth_of_field_spherecolor(vec3 screenpoint, vec3 eyeposition, int spherenumber, std::vector<sphere> sphere, vec3 light, float t, std::vector<triangle> trianglepoint, int reflecttime,int doftime);



#endif