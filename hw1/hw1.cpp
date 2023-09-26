#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <typeinfo>
#include <cmath>
#include "algebra3.cpp"
#include "algebra3.h"

using namespace std;


class colorimage{   
public:
    ~colorimage();
    colorimage(int height,int width);
    colorimage() {}    

    struct pixel{
        unsigned char r;
        unsigned char g;
        unsigned char b;
    };

    // 設置
    void  set_width(int width){ this->width = width;}
    void  set_height(int height){ this->height = height;}
    void  set_version(string version){ this->version = version;}
    // 存讀
    void save_data(string name_file);
    void read_data(string name_file);

    // 圖像處理
	void horizontal_flip();
	void vertical_flip();
	void gray_scale();
	void resize(int height, int width);

    void delete_image();
    pixel** image = nullptr;

private:
    int width = 0;
    int height = 0;
    string version = "P6";

    void create_image();
};




colorimage::~colorimage() {
	delete_image();
}

colorimage::colorimage(int height, int width) {
	set_height(height);
	set_width(width);
	create_image();
}

void colorimage::save_data(string name_file) {
	ofstream output(name_file, ios::binary);

	if (output.is_open()) {
		output << version << endl;
		output << width << " " << height << endl;
		output << 255 << endl;

		if (version == "p3") {
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					output << (int)image[i][j].r << " ";
					output << (int)image[i][j].g << " ";
					output << (int)image[i][j].b << '\n';
				}
			}
		}
		else {
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					output.write((char*)&image[i][j], sizeof(pixel));
				}
			}
		}
		output.close();
	}
}

void colorimage::read_data(string name_file) {
	ifstream input(name_file, ios::binary);

	if (input.is_open()) {
		int color;
		char ver[3];

		input.read(ver, 2);
		version = ver;
		input >> width;
		input >> height;
		input >> color;
		input.read(ver, 1);

		create_image();

		int box;
		if (version == "P3") {
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					input >> box;
					image[i][j].r = box;
					input >> box;
					image[i][j].g = box;
					input >> box;
					image[i][j].b = box;
				}
			}
		}
		else {
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					input.read((char*)&image[i][j], sizeof(pixel));
				}
			}
		}
		input.close();
	}
}


void colorimage::create_image() {
	if (image != nullptr) {
		delete_image();
	}

	image = new pixel * [height];

	for (int i = 0; i < height; i++) {
		image[i] = new pixel[width];

		for (int j = 0; j < width; j++) {
			image[i][j].r = 255;
			image[i][j].g = 255;
			image[i][j].b = 255;
		}
	}
}

void colorimage::horizontal_flip() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width / 2; j++) {
			swap(image[i][j], image[i][width - 1 - j]);
		}
	}
}

void colorimage::vertical_flip() {
	for (int i = 0; i < height / 2; i++) {
		for (int j = 0; j < width; j++) {
			swap(image[i][j], image[height - 1 - i][j]);
		}
	}
}

void colorimage::gray_scale() {
	const float r = 0.299f;
	const float g = 0.587f;
	const float b = 0.114f;
	float greyscaleValue;

	for (int i = 0; i < height / 2; i++) {
		for (int j = 0; j < width; j++) {
			greyscaleValue = image[i][j].r * r + image[i][j].g * g + image[i][j].b * b;
			image[i][j].r = greyscaleValue;
			image[i][j].g = greyscaleValue;
			image[i][j].b = greyscaleValue;
		}
	}
}

void colorimage::delete_image() {
	if (image != nullptr) {
		for (int i = 0; i < height; i++) {
			delete image[i];
		}
		delete image;
	}
}

void colorimage::resize(int height, int width) {
	pixel** image_resized = new pixel * [height];
	for (int i = 0; i < height; i++) {
		image_resized[i] = new pixel[width];

		for (int j = 0; j < width; j++) {
			image_resized[i][j].r = 255;
			image_resized[i][j].g = 255;
			image_resized[i][j].b = 255;
		}
	}
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			image_resized[i][j] = image[i * this->height / height][j * this->width / width];
		}
	}

	delete_image();
	image = image_resized;

	this->height = height;
	this->width = width;
}


// 檢查浮點數
bool check_floating_number(const string& str){
    int i = 0;
    if(str[0]=='-'){
        ++i;
    }
    bool checkF = true;
    for(i; i < str.size(); ++i){
        if(str[i] < '0' || str[i] > '9'){
            if(str[i]!='.' || checkF == false){
                return false;
            }
            else{
                checkF = false;
            }
        }
    }
    return true;
}

// camera位置
vec3 file_eye(vector<vector<string>>& data2) {
	vec3 output;
	for (int i = 0; i < data2.size(); ++i) {
		if (data2[i][0] == "E") {
			output.set(stof(data2[i][1]), stof(data2[i][2]), stof(data2[i][3]));
			break;
		}
	}
	return output;
}


// screen位置
vector<float> file_screen(vector<vector<string>>& data2) {
	vector<float> output;
	for (int i = 0; i < data2.size(); ++i) {
		if (data2[i][0] == "O") {
			for (int j = 1; j < data2[i].size(); ++j) {
				output.push_back(stof(data2[i][j]));
			}
		}
	}
	return output;
}

//分辨率
vector<float> file_resolution(vector<vector<string>>& data2) {
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


// 球體三維座標
vec3 file_sphere(vector<vector<string>>& data2) {
	vec3 output;
	for (int i = 0; i < data2.size(); ++i) {
		if (data2[i][0] == "S") {
			output.set(stof(data2[i][1]), stof(data2[i][2]), stof(data2[i][3]));
			break;
		}
	}
	return output;
}

// 球體半徑
float file_radius(vector<vector<string>>& data2) {
	for (int i = 0; i < data2.size(); ++i) {
		if (data2[i][0] == "S") {
			return stof(data2[i][data2[i].size() - 1]);
		}
	}
	return 0;

}

// 三角形座標
vector<vec3> file_triangle(vector<vector<string>>& data2) {
	vector<vec3> output;
	vec3 point;
	for (int i = 0; i < data2.size(); ++i) {
		if (data2[i][0] == "T") {
			for (int j = 0; j < 3; ++j) {
				// 3*j+1 表示第j個x座標，3*j+2表示第j個y座標，3*j+3表示第j個z座標
				point.set(stof(data2[i][3 * j + 1]), stof(data2[i][3 * j + 2]), stof(data2[i][3 * j + 3]));
				output.push_back(point);
			}
		}
	}
	return output;
}


// 判斷光線與三角形
bool hit_triangle(vec3 trianglepoint1, vec3 trianglepoint2, vec3 trianglepoint3, vec3 eyeposition, vec3 screenpoint) {
	vec3 direction = screenpoint - eyeposition;
	vec3 normalvector = (trianglepoint2 - trianglepoint1) ^ (trianglepoint3 - trianglepoint1);

	float NdotRayDirection = direction * normalvector;
	if (fabs(NdotRayDirection) <  0.0000000000000001) {
		return false;
	}

	float D = (( normalvector * trianglepoint1)) * (1);
	float t = (-(normalvector * eyeposition) + D) / (normalvector * direction);
	if (t < 0) {
		return false;
	}
   	else if (t == 0) {
       vec3 P = eyeposition;
       vec3 C;

       vec3 edge = trianglepoint2 - trianglepoint1;
       vec3 vp = P - trianglepoint1;
       C = edge ^ vp;
       if ((normalvector * C) < 0) return false;

       edge = trianglepoint3 - trianglepoint2;
       vp = P - trianglepoint2;
       C = edge ^ vp;
       if ((normalvector * C) < 0) return false;

       edge = trianglepoint1 - trianglepoint3;
       vp = P - trianglepoint3;
       C = edge ^ vp;
       if ((normalvector * C) < 0) return false;

       return true;
   }

	vec3 P = eyeposition + t * direction;
	vec3 C;

	vec3 edge = trianglepoint2 - trianglepoint1;
	vec3 vp = P - trianglepoint1;
	C = edge ^ vp;
	if ((normalvector * C) < 0)return false;

	edge = trianglepoint3 - trianglepoint2;
	vp = P - trianglepoint2;
	C = edge ^ vp;
	if ((normalvector * C) < 0)return false;

	edge = trianglepoint1 - trianglepoint3;
	vp = P - trianglepoint3;
	C = edge ^ vp;
	if ((normalvector * C) < 0) {
		return false;
	}
	return true;
	
}


// 判斷光線與球體
bool hit_sphere(const vec3& center, float radius, const vec3& eyeposition, const vec3& screenpoint) {
	vec3 unitdirection = (screenpoint - eyeposition) / (screenpoint - eyeposition).length();
	float a = unitdirection * unitdirection;
	float b = (unitdirection * (eyeposition - center)) * 2.0;
	float c = ((eyeposition - center) * (eyeposition - center)) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	return (discriminant >= 0);
}




int main(){

    string hw1_input;
	vector<vector<string>> data2;
	vector<string> data1;

	ifstream infile("hw1_input.txt");
	infile >> hw1_input;

	data1.push_back(hw1_input);

	while (1) {
		infile >> hw1_input;
		if (infile.eof()) {
			data1.push_back(hw1_input); 
			data2.push_back(data1);
			break;
		}
		if (check_floating_number(hw1_input)) {
			data1.push_back(hw1_input);
		}
		else {
			data2.push_back(data1);
			data1.clear();
			data1.push_back(hw1_input);

		}

	}
	
	//資料測試
    for (int i = 0; i < data2.size(); ++i) {
		for (int j = 0; j < data2[i].size(); ++j) {
			cout << data2[i][j] << " ";
		}
		cout << endl;
	}


	vector<float> screen = file_screen(data2);
	int resolution = (int)file_resolution(data2)[1];


	float horizonxgap = (screen[3] - screen[0]) / (resolution - 1), horizonygap = (screen[4] - screen[1]) / (resolution - 1), horizonzgap = (screen[5] - screen[2]) / (resolution - 1);
	float verticalxgap = (screen[6] - screen[0]) / (resolution - 1), verticalygap = (screen[7] - screen[1]) / (resolution - 1), verticalzgap = (screen[8] - screen[2]) / (resolution - 1);
	float x, y, z;
	vec3 eye = file_eye(data2);
	vec3 sphere_point = file_sphere(data2);
	float radius = file_radius(data2);
	vector<vec3> triangle_point = file_triangle(data2);
	colorimage photo(resolution, resolution);


	for (int i = 0; i < resolution; ++i) {
		for (int j = 0; j < resolution; ++j) {
			x = screen[0] + j * horizonxgap + 0.5 * horizonxgap + i * verticalxgap + 0.5 * verticalxgap;
			y = screen[1] + j * horizonygap + 0.5 * horizonygap + i * verticalygap + 0.5 * verticalygap;
			z = screen[2] + j * horizonzgap + 0.5 * horizonzgap + i * verticalzgap + 0.5 * verticalzgap;
			vec3 point(x, y, z);
			if (hit_sphere(sphere_point, radius, eye, point)) {
				photo.image[i][j].r = (unsigned char)(255*(x+1.0)/2.0);
				photo.image[i][j].g = (unsigned char)(255*(y+1.0)/2.0);
				photo.image[i][j].b = (unsigned char)(255*(z+1.0)/2.0);


			}
			int k = 0;
			for (k = 0; (k < triangle_point.size() / 3); ++k) {
				if (hit_triangle(triangle_point[3 * k], triangle_point[3 * k + 1], triangle_point[3 * k + 2], eye, point) == true) {
					photo.image[i][j].r = (unsigned char)(255*(z+1.0)/5.0);
					photo.image[i][j].g = (unsigned char)(255*(z+1.0)/5.0);
					photo.image[i][j].b = (unsigned char)(255*(z+1.0)/5.0);
				}
			}
		}
	}
    photo.save_data("raytracing_hw1.ppm");
			
}


