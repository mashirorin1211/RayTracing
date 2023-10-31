#include "objectfunction.h"

using namespace std;
triangle::triangle(void) {
	for (int i = 0; i < 3; i++) {
		vec3 use;
		use.set(0, 0, 0);
		trianglepoint0 = trianglepoint1 = trianglepoint2 = use;
	}
}
triangle::triangle(const vec3 point1, const vec3 point2, const vec3 point3,const std::vector<float> mat) {
	trianglepoint0 = point1;
	trianglepoint1 = point2;
	trianglepoint2 = point3;
	material = mat;
}
triangle::triangle(const triangle& v) {
	trianglepoint0 = v.trianglepoint0;
	trianglepoint1 = v.trianglepoint1;
	trianglepoint2 = v.trianglepoint2;
	material = v.material;
}

void triangle::set(vec3 x, vec3 y, vec3 z ,std:: vector<float> mat)     
{
	trianglepoint0 = x;
	trianglepoint1 = y; 
	trianglepoint2 = z;
	material = mat;
}

vec3 triangle::gettrianglepoint0() {
	return trianglepoint0;
}

vec3 triangle::gettrianglepoint1() {
	return trianglepoint1;
}

vec3 triangle::gettrianglepoint2() {
	return trianglepoint2;
}

std::vector<float> triangle::getmaterial() {
	return material;
}


float hit_triangle(int trianglenumber,std:: vector<triangle> trianglepoint, vec3 eyeposition, vec3 screenpoint) {
	vec3 direction = (screenpoint - eyeposition).normalize();
	vec3 p1 = trianglepoint[trianglenumber].gettrianglepoint0(), p2 = trianglepoint[trianglenumber].gettrianglepoint1(), p3 = trianglepoint[trianglenumber].gettrianglepoint2();
	vec3 normalvector = ((p2 - p1) ^ (p3 - p1)).normalize();
	
	float NdotRayDirection = normalvector * direction;

	if (fabs(NdotRayDirection) < 0.0000000000000001) {// almost 0 
		return -1.0;
	}

	float D = (normalvector * p1) * (-1);
	float t = -1 * ((normalvector * eyeposition) + D) / (NdotRayDirection);
	vec3 normalpoint = eyeposition + t * direction;

	if ((eyeposition + (normalvector / 100) - normalpoint).length() > (eyeposition - normalpoint).length()) {
		normalvector = -normalvector;
		if (t < 0) {
			return -1.0;
		}
		vec3 P = eyeposition + t * direction;
		vec3 C;

		vec3 edge = p2 - p1;
		vec3 vp = P - p1;
		C = edge ^ vp;
		if ((normalvector * C) > 0)return -1.0;

		edge = p3 - p2;
		vp = P - p2;
		C = edge ^ vp;
		if ((normalvector * C) > 0)return -1.0;

		edge = p1 - p3;
		vp = P - p3;
		C = edge ^ vp;
		if ((normalvector * C) > 0) {
			return -1.0;
		}
		return t;
	}
	
	if (t < 0) {
		return -1.0;
	}
	
	vec3 P = eyeposition + t * direction;
	vec3 C;

	vec3 edge = p2 - p1;
	vec3 vp = P - p1;
	C = edge ^ vp;
	if ((normalvector * C) < 0)return -1.0;

	edge = p3 - p2;
	vp = P - p2;
	C = edge ^ vp;
	if ((normalvector * C) < 0)return -1.0;

	edge = p1 - p3;
	vp = P - p3;
	C = edge ^ vp;
	if ((normalvector * C) < 0) {
		return -1.0;
	}
	return t;
}

vec3 trianglecolor(const vec3 screenpoint,const vec3 eyeposition, int trianglenumber, vec3 light, float t, std::vector<triangle> trianglepoint, std::vector<sphere> sphere,int reflecttime){
	vec3 finalcolor;
	vec3 normalvector, viewdirection, lightdirection;
	vec3 normalpoint;
	vec3 rgb, reflect(0, 0, 0),shadow(0,0,0);
	vec3 p1 = trianglepoint[trianglenumber].gettrianglepoint0(), p2 = trianglepoint[trianglenumber].gettrianglepoint1(), p3 = trianglepoint[trianglenumber].gettrianglepoint2();
	vector<float> material = trianglepoint[trianglenumber].getmaterial();
	float I;
	
	viewdirection = (screenpoint - eyeposition).normalize();
	normalpoint = eyeposition + t * viewdirection;
	normalvector = ((p2 - p1) ^ (p3 - p2)).normalize();;
	if ((eyeposition+(normalvector/100)-normalpoint).length() > (eyeposition - normalpoint).length()) {
		normalvector = -normalvector;
	}
	

	lightdirection = (light - normalpoint).normalize();


	I = material[3] + (material[4] * (normalvector * lightdirection)) + material[5] * pow(((lightdirection - viewdirection) * 0.5) * normalvector, material[6]);

	for (int i = 0; i < 3; i++) {
		rgb[i] = (material[i] + I) * 255;
	}
	
	//shadow

	t = hit_sphere(0, sphere, (light-normalpoint), normalpoint);
	if (t > 0) {
		shadow.set(25,25,25);
	}
	t = hit_sphere(1, sphere, (light - normalpoint), normalpoint);
	if (t > 0) {
		shadow.set(25, 25, 25);
	}
	t = hit_sphere(2, sphere, (light - normalpoint), normalpoint);
	if (t > 0) {
		shadow.set(25, 25, 25);
	}
	else {
		t = hit_triangle(0, trianglepoint, normalpoint, (light - normalpoint));
		if (t > 0) {
			shadow.set(25, 25, 25);
		}
		else {
			t = hit_triangle(1, trianglepoint, normalpoint, (light - normalpoint));
			if (t > 0) {
				shadow.set(25, 25, 25);
			}
		}
	}

	//reflection
	if (reflecttime > 0 && material[7] != 0) {
		
		t = hit_triangle(0, trianglepoint, normalpoint, normalpoint + 2 * normalvector);
		if (t >= 0) {
			reflect = material[7] * trianglecolor(normalpoint + viewdirection + 2 * normalvector, normalpoint, 0, light, t, trianglepoint, sphere, reflecttime - 1);
		}

		if (t < 0) {
			t = hit_triangle(1, trianglepoint, normalpoint, normalpoint + 2 * normalvector);
			if (t >= 0) {
				reflect = material[7] * trianglecolor(normalpoint + viewdirection + 2 * normalvector, normalpoint, 1, light, t, trianglepoint, sphere, reflecttime - 1);
			}
		}
		if (t < 0) {
			t = hit_sphere(0, sphere, normalpoint + viewdirection + 2 * normalvector, normalpoint);
			if (t >= 0) {
				reflect = material[7] * spherecolor(normalpoint + viewdirection + 2 * normalvector, normalpoint, 0, sphere, light, t, trianglepoint, reflecttime - 1);
			}
		}
		if (t < 0) {
			t = hit_sphere(1, sphere, normalpoint + viewdirection + 2 * normalvector, normalpoint);
			if (t >= 0) {
				reflect = material[7] * spherecolor(normalpoint + viewdirection + 2 * normalvector, normalpoint, 1, sphere, light, t, trianglepoint, reflecttime - 1);
			}
		}
		if (t < 0) {
			t = hit_sphere(2, sphere, normalpoint + viewdirection + 2 * normalvector, normalpoint);
			if (t >= 0) {
				reflect = material[7] * spherecolor(normalpoint + viewdirection + 2 * normalvector, normalpoint, 2, sphere, light, t, trianglepoint, reflecttime - 1);
			}
		}
	}
	rgb += reflect;
	rgb -= shadow;
	for (int i = 0; i < 3; i++) {
		if (rgb[i] > 255) {
			rgb[i] = 255;
		}
		else if (rgb[i] < 0)
		{
			rgb[i] = 0;
		}
	}
	finalcolor.set(rgb[0], rgb[1], rgb[2]);
	

	return finalcolor;
}


vec3 depth_of_field_trianglecolor(const vec3 screenpoint, const vec3 eyeposition, int trianglenumber, vec3 light, float t, std::vector<triangle> trianglepoint, std::vector<sphere> sphere, int reflecttime , int doftime) {
	vec3 finalcolor,eyepositionchange;
	vec3 normalvector, viewdirection, lightdirection;
	vec3 normalpoint;
	vec3 rgb, reflect(0, 0, 0), shadow(0, 0, 0);
	vec3 p1 = trianglepoint[trianglenumber].gettrianglepoint0(), p2 = trianglepoint[trianglenumber].gettrianglepoint1(), p3 = trianglepoint[trianglenumber].gettrianglepoint2();
	vector<float> material = trianglepoint[trianglenumber].getmaterial();
	float I;


	viewdirection = (screenpoint - eyeposition).normalize();
	normalpoint = eyeposition + t * viewdirection;
	normalvector = ((p2 - p1) ^ (p3 - p2)).normalize();;
	
	if ((eyeposition + (normalvector / 100) - normalpoint).length() > (eyeposition - normalpoint).length()) {
		normalvector = -normalvector;
	}


	lightdirection = (light - normalpoint).normalize();


	I = material[3] + (material[4] * (normalvector * lightdirection)) + material[5] * pow(((lightdirection - viewdirection) * 0.5) * normalvector, material[6]);

	for (int i = 0; i < 3; i++) {
		rgb[i] = (material[i] + I) * 255;
	}

	//shadow
	t = hit_sphere(0, sphere, (light - normalpoint), normalpoint);
	if (t > 0) {
		shadow.set(25, 25, 25);
	}
	t = hit_sphere(1, sphere, (light - normalpoint), normalpoint);
	if (t > 0) {
		shadow.set(25, 25, 25);
	}
	t = hit_sphere(2, sphere, (light - normalpoint), normalpoint);
	if (t > 0) {
		shadow.set(25, 25, 25);
	}
	else {
		t = hit_triangle(0, trianglepoint, normalpoint, (light - normalpoint));
		if (t > 0) {
			shadow.set(25, 25, 25);
		}
		else {
			t = hit_triangle(1, trianglepoint, normalpoint, (light - normalpoint));
			if (t > 0) {
				shadow.set(25, 25, 25);
			}
		}
	}

	//reflection
	if (reflecttime > 0 && material[7] != 0) {

		t = hit_triangle(0, trianglepoint, normalpoint, normalpoint + 2 * normalvector);
		if (t >= 0) {
			reflect = material[7] * trianglecolor(normalpoint + viewdirection + 2 * normalvector, normalpoint, 0, light, t, trianglepoint, sphere, reflecttime - 1);
		}

		if (t < 0) {
			t = hit_triangle(1, trianglepoint, normalpoint, normalpoint + 2 * normalvector);
			if (t >= 0) {
				reflect = material[7] * trianglecolor(normalpoint + viewdirection + 2 * normalvector, normalpoint, 1, light, t, trianglepoint, sphere, reflecttime - 1);
			}
		}
		if (t < 0) {
			t = hit_sphere(0, sphere, normalpoint + viewdirection + 2 * normalvector, normalpoint);
			if (t >= 0) {
				reflect = material[7] * spherecolor(normalpoint + viewdirection + 2 * normalvector, normalpoint, 0, sphere, light, t, trianglepoint, reflecttime - 1);
			}
		}
		if (t < 0) {
			t = hit_sphere(1, sphere, normalpoint + viewdirection + 2 * normalvector, normalpoint);
			if (t >= 0) {
				reflect = material[7] * spherecolor(normalpoint + viewdirection + 2 * normalvector, normalpoint, 1, sphere, light, t, trianglepoint, reflecttime - 1);
			}
		}
		if (t < 0) {
			t = hit_sphere(2, sphere, normalpoint + viewdirection + 2 * normalvector, normalpoint);
			if (t >= 0) {
				reflect = material[7] * spherecolor(normalpoint + viewdirection + 2 * normalvector, normalpoint, 2, sphere, light, t, trianglepoint, reflecttime - 1);
			}
		}
	}
	rgb += reflect;
	rgb -= shadow;
	for (int i = 0; i < 3; i++) {
		if (rgb[i] > 255) {
			rgb[i] = 255;
		}
		else if (rgb[i] < 0)
		{
			rgb[i] = 0;
		}
	}
	finalcolor.set(rgb[0], rgb[1], rgb[2]);


	return finalcolor;
}

sphere::sphere(void) {
	spherecenter[0] = 0, spherecenter[1] = 0 , spherecenter[2] = 0 ;
	sphereradius = 0;

}
sphere::sphere(const vec3 center, const float radius, const std::vector<float> mat) {
	spherecenter = center;
	sphereradius = radius;
	material = mat;
}
sphere::sphere(const sphere& v) {
	spherecenter = v.spherecenter;
	sphereradius = v.sphereradius;
	material = v.material;
}

void sphere::set(vec3 center, float radius, std::vector<float> mat)
{
	spherecenter = center; sphereradius = radius; material = mat;
}

vec3& sphere::operator [] (int i) {
	if (i < 0 || i > 3)
		VEC_ERROR("vec3 [] operator: illegal access");

	return spherecenter;
}

vec3& sphere::getcenter() {
	return spherecenter;
}
float& sphere::getradius() {
	return sphereradius;
}


std::vector<float> sphere::getmaterial() {
	return material;
}

float hit_sphere(int spherenumber,std::vector<sphere> sphere, vec3 eyeposition, vec3 screenpoint) {
	vec3 unitdirection = (screenpoint - eyeposition).normalize();
	vec3 center = sphere[spherenumber].getcenter();
	float radius = sphere[spherenumber].getradius();
	float a = unitdirection * unitdirection;
	float b = (unitdirection * (eyeposition - center)) * 2.0;
	float c = ((eyeposition - center) * (eyeposition - center)) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		return -1.0;
	}
	else {
		return (-b - sqrt(discriminant)) / (2.0 * a);
	}
}

vec3 spherecolor(vec3 screenpoint, vec3 eyeposition, int spherenumber, std::vector<sphere> sphere, vec3 light, float t, std::vector<triangle> trianglepoint, int reflecttime) {

	vec3 finalcolor;
	vec3 normalvector, viewdirection, lightdirection;
	vec3 normalpoint;
	vec3 rgb,reflect(0, 0, 0), shadow(0, 0, 0);
	float I;
	vector<float> material = sphere[spherenumber].getmaterial();
	vec3 spherecenter = sphere[spherenumber].getcenter();
	float radius = sphere[spherenumber].getradius();
	viewdirection = (screenpoint - eyeposition).normalize();
	normalpoint = (eyeposition + t * viewdirection);
	normalvector = (normalpoint - spherecenter).normalize();
	lightdirection = (light - normalpoint).normalize();

	I = material[3] + (material[4] * (normalvector * lightdirection)) + material[5] * pow(((lightdirection - viewdirection) * 0.5) * normalvector, material[6]);
	for (int i = 0; i < 3; i++) {
		rgb[i] = (material[i] + I) * 255;
		if (rgb[i] > 255) {
			rgb[i] = 255;
		}
		else if (rgb[i] < 0)
		{
			rgb[i] = 0;
		}
	}
	
	//shadow
	
	for (int l = 0; l < trianglepoint.size(); ++l) {
		t = hit_sphere(l, sphere, normalpoint + viewdirection + 2 * normalvector, normalpoint);
		if (t >= 0) {
			shadow.set(25, 25, 25);
			break;
		}
	}
	if (t < 0) {
		for (int l = 0; l < trianglepoint.size(); ++l) {
			t = hit_triangle(l, trianglepoint, normalpoint, (light - normalpoint));
			if (t >= 0) {
				shadow.set(25, 25, 25);
				break;
			}
		}
	}
	
	
	
	//reflection
	if (reflecttime > 0 && material[7] != 0) {

		for (int l = 0; l < trianglepoint.size(); ++l) {
			t = hit_triangle(l, trianglepoint, normalpoint, normalpoint + 2 * normalvector);
			if (t >= 0) {
				reflect = material[7] * trianglecolor(normalpoint + viewdirection + 2 * normalvector, normalpoint, 0, light, t, trianglepoint, sphere, reflecttime - 1);
				break;
			}
		}
		if (t < 0) {
			for (int l = 0; l < sphere.size(); ++l) {
				t = hit_sphere(l, sphere, normalpoint + viewdirection + 2 * normalvector, normalpoint);
				if (t >= 0) {
					reflect = material[7] * spherecolor(normalpoint + viewdirection + 2 * normalvector, normalpoint, 0, sphere, light, t, trianglepoint, reflecttime - 1);
					break;
				}
			}
		}
	}
	rgb += reflect;
	rgb -= shadow;

	for (int i = 0; i < 3; i++) {
		if (rgb[i] > 255) {
			rgb[i] = 255;
		}
		else if (rgb[i] < 0)
		{
			rgb[i] = 0;
		}
	}

	finalcolor.set(rgb[0], rgb[1], rgb[2]);

	return finalcolor;
}

vec3 depth_of_field_spherecolor(vec3 screenpoint, vec3 eyeposition, int spherenumber, std::vector<sphere> sphere, vec3 light, float t, std::vector<triangle> trianglepoint, int reflecttime, int doftime) {

	vec3 finalcolor;
	vec3 normalvector, viewdirection, lightdirection;
	vec3 normalpoint;
	vec3 rgb, reflect(0, 0, 0), shadow(0, 0, 0);
	float I;
	vector<float> material = sphere[spherenumber].getmaterial();
	vec3 spherecenter = sphere[spherenumber].getcenter();
	float radius = sphere[spherenumber].getradius();
	viewdirection = (screenpoint - eyeposition).normalize();
	normalpoint = (screenpoint + t * viewdirection);
	normalvector = (normalpoint - spherecenter).normalize();
	lightdirection = (light - normalpoint).normalize();

	I = material[3] + (material[4] * (normalvector * lightdirection)) + material[5] * pow(((lightdirection - viewdirection) * 0.5) * normalvector, material[6]);
	for (int i = 0; i < 3; i++) {
		rgb[i] = (material[i] + I) * 255;
		if (rgb[i] > 255) {
			rgb[i] = 255;
		}
		else if (rgb[i] < 0)
		{
			rgb[i] = 0;
		}
	}

	//shadow
	for (int l = 0; l < trianglepoint.size(); ++l) {
		t = hit_sphere(l, sphere, normalpoint + viewdirection + 2 * normalvector, normalpoint);
		if (t >= 0) {
			shadow.set(25, 25, 25);
			break;
		}
	}
	if (t < 0) {
		for (int l = 0; l < trianglepoint.size(); ++l) {
			t = hit_triangle(l, trianglepoint, normalpoint, (light - normalpoint));
			if (t >= 0) {
				shadow.set(25, 25, 25);
				break;
			}
		}
	}


	//reflection
	if (reflecttime > 0 && material[7] != 0) {

		for (int l = 0; l < trianglepoint.size(); ++l) {
			t = hit_triangle(l, trianglepoint, normalpoint, normalpoint + 2 * normalvector);
			if (t >= 0) {
				reflect = material[7] * trianglecolor(normalpoint + viewdirection + 2 * normalvector, normalpoint, 0, light, t, trianglepoint, sphere, reflecttime - 1);
				break;
			}
		}
		if (t < 0) {
			for (int l = 0; l < sphere.size(); ++l) {
				t = hit_sphere(l, sphere, normalpoint + viewdirection + 2 * normalvector, normalpoint);
				if (t >= 0) {
					reflect = material[7] * spherecolor(normalpoint + viewdirection + 2 * normalvector, normalpoint, 0, sphere, light, t, trianglepoint, reflecttime - 1);
					break;
				}
			}
		}
	}
	rgb += reflect;
	rgb -= shadow;

	for (int i = 0; i < 3; i++) {
		if (rgb[i] > 255) {
			rgb[i] = 255;
		}
		else if (rgb[i] < 0)
		{
			rgb[i] = 0;
		}
	}

	finalcolor.set(rgb[0], rgb[1], rgb[2]);

	return finalcolor;
}
