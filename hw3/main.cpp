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
#include<time.h>
#include<stdlib.h>

using namespace std;


vector<vector<string>> data2;
vector<string> data1;
vector<vec3> screen ;
int resolution;
vec3 horizongap;
vec3 verticalgap;
vec3 upleft;
vec3 eye;
vec3 origeye;
float radius;
vector<sphere> spherepoint;
vector<triangle> trianglepoint;
vec3 light;
vector<vector<float>> material;
float rand1, rand2;
int raynum = 5;
int totalr = 0, totalg=0, totalb=0;
bool hit = false;

bool is_floating(const string& str)
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
	srand(time(NULL));

	string word;
	
	ifstream infile("hw3.txt");
	infile >> word;

	data1.push_back(word);
	while (1) {
		infile >> word;

		if (infile.eof()) {
			data1.push_back(word);
			data2.push_back(data1);
			break;
		}
		if (is_floating(word)) {
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
	
	origeye = file_eye(data2);
	spherepoint = file_sphere(data2);
	
	radius = file_radius(data2);
	trianglepoint = file_triangle(data2);

	light = file_light(data2);
	material = file_Material(data2);
	float t=0;
	vec3 pixelcolor;
	
	PPM picture(resolution, resolution);

	for (int i = 0; i < resolution; ++i) {
		for (int j = 0; j < resolution; ++j) {
		
			hit = false;
			point = (upleft + j * horizongap + i * verticalgap);
		
			totalr = 0, totalb = 0, totalg = 0;
			for (int k = 0; k < raynum; ++k) {
				
			
				rand1 = 1.4 * rand()/RAND_MAX - 0.7;
				rand2 = sqrt(0.7 * 0.7 - rand1 * rand1);
				rand2 = 2*rand2 * rand() / RAND_MAX - abs(rand2);				
				
			
				eye.set(origeye[0]+rand1, origeye[1]+rand2, origeye[2]);

				
				//sphere
				for (int l = 0; l < spherepoint.size(); ++l) {
					t = hit_sphere(l, spherepoint, eye, point);

					if (t >= 0) {

						pixelcolor = spherecolor(point, eye, l, spherepoint, light, t, trianglepoint, 2);

						totalr += pixelcolor[0];
						totalg += pixelcolor[1];
						totalb += pixelcolor[2];
						break;
					}
				}
				//triangle
				if (t < 0) {
					for (int l = 0; l < trianglepoint.size(); ++l) {
						t = hit_triangle(l, trianglepoint, eye, point);
						if (t > 0) {

							pixelcolor = trianglecolor(point, eye, l, light, t, trianglepoint, spherepoint, 2);

							totalr += pixelcolor[0];
							totalg += pixelcolor[1];
							totalb += pixelcolor[2];
							break;
						}
					}
				}

				if (t > 0) {
					hit = true;
				}
				else {
					totalb += 255 ;
					totalg += 255 ;
					totalr += 255 ;
				}
			}
			if (hit==false) {
				totalb = totalg = totalr = 255*raynum;
			}

			picture.image[i][j].r = (unsigned char)(totalr/raynum);
			picture.image[i][j].g = (unsigned char)(totalg/raynum);
			picture.image[i][j].b = (unsigned char)(totalb/raynum);
		}
	}
	picture.save("hw3.ppm");

}





