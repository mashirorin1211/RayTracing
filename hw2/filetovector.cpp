
#include "filetovector.h"

using namespace std;
std::vector<vec3> file_screen(std::vector<std::vector<std::string>>& data2) {
	float angle = 90;
	vec3 eye, viewdirection, eyeup, updirection, angledirection, leftdirection, upleft, upright, lowleft, lowright;
	vector<vec3> frame;

	for (int i = 0; i < data2.size(); ++i) {
		if (data2[i][0] == "E") {
			eye.set(stof(data2[i][1]), stof(data2[i][2]), stof(data2[i][3]));
			break;
		}
	}

	for (int i = 0; i < data2.size(); ++i) {
		if (data2[i][0] == "F") {
			angle = stof(data2[i][data2[i].size() - 1]) / 2;
			break;
		}
	}

	for (int i = 0; i < data2.size(); ++i) {
		if (data2[i][0] == "V") {
			viewdirection.set(stof(data2[i][1]), stof(data2[i][2]), stof(data2[i][3]));
			eyeup.set(stof(data2[i][4]), stof(data2[i][5]), stof(data2[i][6]));
			viewdirection.normalize();
			eyeup.normalize();
			break;
		}
	}


	leftdirection = (viewdirection ^ eyeup);
	leftdirection = viewdirection.length() * tan(angle * 3.1415926 / 180) * (viewdirection ^ eyeup);
	vec3 use = leftdirection;
	use.normalize();
	updirection = viewdirection.length() * tan(angle * 3.1415926 / 180) * (use ^ viewdirection);

	//cout << viewdirection << " " << use << endl;

	upleft = (eye + viewdirection + leftdirection + updirection);
	upright = (eye + viewdirection - leftdirection + updirection);
	lowleft = (eye + viewdirection + leftdirection - updirection);
	lowright = (eye + viewdirection - leftdirection - updirection);
	//cout << leftdirection << updirection << endl;
	//cout << upleft << " " << upright << " " << lowleft << " " << lowright;

	frame.push_back(upleft);
	frame.push_back(upright);
	frame.push_back(lowleft);
	frame.push_back(lowright);
	return frame;
}

std::vector<float> file_resolution(std::vector<std::vector<std::string>>& data2) {
	vector<float> output;
	for (int i = 0; i < data2.size(); ++i) {
		if (data2[i][0] == "R") {
			for (int j = 1; j < data2[i].size(); ++j) {
				output.push_back(stof(data2[i][j]));
			}
		}
	}
	return output;
}

std::vector<std::vector<float>> file_Material(std::vector<std::vector<std::string>>& data2) {
	vector<float> use;
	vector<vector<float>> output;
	for (int i = 0; i < data2.size(); ++i) {
		if (data2[i][0] == "M") {
			if (output.empty()) {
				for (int j = 1; j < data2[i].size(); ++j) {
					use.push_back(stof(data2[i][j]));
				}output.push_back(use);
			}
			else {
				for (int j = 1; j < data2[i].size(); ++j) {
					use.push_back(stof(data2[i][j]));
				}output.push_back(use);
			}
		}
	}
	return output;
}

vec3 file_eye(std::vector<std::vector<std::string>>& data2) {
	vec3 output;
	for (int i = 0; i < data2.size(); ++i) {
		if (data2[i][0] == "E") {
			output.set(stof(data2[i][1]), stof(data2[i][2]), stof(data2[i][3]));
			break;
		}
	}
	return output;
}

std::vector<sphere> file_sphere(std::vector<std::vector<std::string>>& data2) {
	vector<sphere> output;
	sphere s;
	vec3 point;
	float radius;
	vector<float> mat;
	for (int i = 0; i < data2.size(); ++i) {
		if (data2[i][0] == "S") {
			
			point.set(stof(data2[i][1]), stof(data2[i][2]), stof(data2[i][3]));
			radius = stof(data2[i][4]);
			for (int j = 1; j < data2[i - 1].size(); ++j) {
				mat.push_back(stof(data2[i - 1][j ]));
			}
			s.set(point, radius, mat);
			output.push_back(s);
		}
	}
	return output;
}

float file_radius(std::vector<std::vector<std::string>>& data2) {
	for (int i = 0; i < data2.size(); ++i) {
		if (data2[i][0] == "S") {
			return stof(data2[i][data2[i].size() - 1]);
		}
	}
}

float file_viewangle(std::vector<std::vector<std::string>>& data2) {
	for (int i = 0; i < data2.size(); ++i) {
		if (data2[i][0] == "F") {
			return stof(data2[i][data2[i].size() - 1]);
		}
	}
}

vec3 file_viewdirection(std::vector<std::vector<std::string>>& data2) {
	vec3 output;
	for (int i = 0; i < data2.size(); ++i) {
		if (data2[i][0] == "V") {
			output.set(stof(data2[i][1]), stof(data2[i][2]), stof(data2[i][3]));
			break;
		}
	}
	return output;
}

vec3 file_viewupdirection(std::vector<std::vector<std::string>>& data2) {
	vec3 output;
	for (int i = 0; i < data2.size(); ++i) {
		if (data2[i][0] == "V") {
			output.set(stof(data2[i][4]), stof(data2[i][5]), stof(data2[i][6]));
			break;
		}
	}
	return output;
}

vec3 file_light(std::vector<std::vector<std::string>>& data2) {
	vec3 output;
	for (int i = 0; i < data2.size(); ++i) {
		if (data2[i][0] == "L") {
			output.set(stof(data2[i][1]), stof(data2[i][2]), stof(data2[i][3]));
			break;
		}
	}
	return output;
}

std::vector<triangle> file_triangle(std::vector<std::vector<std::string>>& data2) {
	vector<triangle> output;
	triangle t ;
	vector<vec3> point(3);
	vector<float> mat;
	bool use=true;

	for (int i = 0; i < data2.size(); ++i) {
		if (data2[i][0] == "T") {
			for (int j = 0; j < 3; ++j) {
				
				point[j].set(stof(data2[i][3 * j + 1]), stof(data2[i][3 * j + 2]), stof(data2[i][3 * j + 3]));
			}
			if (use = true) {
				for (int j = 1; j < data2[i - 1].size(); ++j) {
					mat.push_back(stof(data2[i - 1][j]));
					
				}
				use = false;
			}
			t.set(point[0], point[1], point[2],mat);
			output.push_back(t);
		}
	}
	return output;
}

