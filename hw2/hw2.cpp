#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <typeinfo>
#include <cmath>
#include "algebra3.h"
#include "algebra3.cpp"
#include "PPM.h"
#include "PPM.cpp"
#include "filetovector.h"
#include "filetovector.cpp"
#include "objectfunction.h"
#include "objectfunction.cpp"
using namespace std;

vector<vector<string>> data2;
vector<string> data1;
vector<vec3> screen ;
int resolution;
vec3 horizongap;
vec3 verticalgap;
vec3 upleft;
vec3 eye;
float radius;
vector<sphere> spherepoint;
vector<triangle> trianglepoint;
vec3 light;
vector<vector<float>> material;


bool isfloating(const string& str)
{
	int i = 0;
	if (str[0] == '-') {
		++i;
	}
	bool isf = true;
	for (i; i < str.size(); ++i) {
		if (str[i] < '0' || str[i] > '9') {
			if (str[i] != '.' || isf == false) {
				return false;
			}
			else {
				isf = false;
			}
		}
	}
	return true;
}

int main() {
	
	string word;
	
	ifstream infile("hw2_input.txt");
	infile >> word;

	data1.push_back(word);
	while (1) {
		infile >> word;

		if (infile.eof()) {
			data1.push_back(word);
			data2.push_back(data1);
			break;
		}
		if (isfloating(word)) {
			data1.push_back(word);
		}
		else {
			data2.push_back(data1);
			data1.clear();
			data1.push_back(word);
		}

	}

	for (int i = 0; i < data2.size(); ++i) {
		for (int j = 0; j < data2[i].size(); ++j) {
			cout << data2[i][j] << " ";
		}cout << endl;
	}
	
	screen = file_screen(data2);
	resolution = (int)file_resolution(data2)[1];
	
	horizongap = (screen[1] - screen[0]) / (resolution - 1);
	verticalgap = (screen[2] - screen[0]) / (resolution - 1);
	vec3 point;
	
	upleft = screen[0];
	
	eye = file_eye(data2);
	spherepoint = file_sphere(data2);
	
	radius = file_radius(data2);
	trianglepoint = file_triangle(data2);
	vec3 use1, use2, use3;

	light = file_light(data2);
	material = file_Material(data2);
	float t;
	vec3 pixelcolor;
	
	PPM picture(resolution, resolution);

	for (int i = 0; i < resolution; ++i) {
		for (int j = 0; j < resolution; ++j) {
			
			point=(upleft + j * horizongap + i * verticalgap);
			
			t = hit_sphere(0,spherepoint , eye, point);
			
			if (t >= 0) {
				
				pixelcolor = spherecolor(point,eye,0,spherepoint, light,t,trianglepoint,2);
				
				picture.image[i][j].r = (unsigned char)pixelcolor[0];
				picture.image[i][j].g = (unsigned char)pixelcolor[1];
				picture.image[i][j].b = (unsigned char)pixelcolor[2];
			}
			t = hit_triangle(0, trianglepoint, eye, point);
			if (t > 0) {

				pixelcolor = trianglecolor(point, eye, 0, light, t, trianglepoint, spherepoint,2);

				picture.image[i][j].r = (unsigned char)pixelcolor[0];
				picture.image[i][j].g = (unsigned char)pixelcolor[1];
				picture.image[i][j].b = (unsigned char)pixelcolor[2];
			}
			t = hit_triangle(1, trianglepoint, eye, point);
			
			if (t > 0) {
				pixelcolor = trianglecolor(point, eye, 1, light, t ,trianglepoint,spherepoint,2);
				
				picture.image[i][j].r = (unsigned char)pixelcolor[0];
				picture.image[i][j].g = (unsigned char)pixelcolor[1];
				picture.image[i][j].b = (unsigned char)pixelcolor[2];
			}
			
		}
	}
	picture.save("hw2.ppm");

}





