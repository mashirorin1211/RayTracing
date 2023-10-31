#include <vector>
#include <string>
#include "algebra3.h"
#include <iostream>
#include "objectfunction.h"


#ifndef _FILETOVECTOR_H_
#define _FILETOVECTOR_H_

std::vector<vec3> file_screen(std::vector<std::vector<std::string>>& data2);
std::vector<float> file_resolution(std::vector<std::vector<std::string>>& data2);
std::vector<std::vector<float>> file_Material(std::vector<std::vector<std::string>>& data2);
vec3 file_eye(std::vector<std::vector<std::string>>& data2);
std::vector<sphere> file_sphere(std::vector<std::vector<std::string>>& data2);
float file_radius(std::vector<std::vector<std::string>>& data2);
float file_viewangle(std::vector<std::vector<std::string>>& data2);
vec3 file_viewdirection(std::vector<std::vector<std::string>>& data2);
vec3 file_viewupdirection(std::vector<std::vector<std::string>>& data2);
vec3 file_light(std::vector<std::vector<std::string>>& data2);
std::vector<triangle> file_triangle(std::vector<std::vector<std::string>>& data2);

#endif