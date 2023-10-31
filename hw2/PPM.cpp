#include "PPM.h"

using namespace std;

PPM::~PPM() {
	delete_image();
}

PPM::PPM(int height, int width) {
	set_height(height);
	set_width(width);
	create_image();
}

void PPM::save(string name_file) {
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
					output.write((char*)&image[i][j], sizeof(RGB));
				}
			}
		}
		output.close();
	}
}

void PPM::read(string name_file) {
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
					input.read((char*)&image[i][j], sizeof(RGB));
				}
			}

		}
		input.close();
	}

}
void PPM::create_image() {
	if (image != nullptr) {
		delete_image();
	}

	image = new RGB * [height];

	for (int i = 0; i < height; i++) {
		image[i] = new RGB[width];

		for (int j = 0; j < width; j++) {
			image[i][j].r = 255;
			image[i][j].g = 255;
			image[i][j].b = 255;
		}
	}
}

void PPM::horizontal_flip() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width / 2; j++) {
			swap(image[i][j], image[i][width - 1 - j]);
		}
	}
}

void PPM::vertical_flip() {
	for (int i = 0; i < height / 2; i++) {
		for (int j = 0; j < width; j++) {
			swap(image[i][j], image[height - 1 - i][j]);
		}
	}
}

void PPM::gray_scale() {
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

void PPM::delete_image() {
	if (image != nullptr) {
		for (int i = 0; i < height; i++) {
			delete image[i];
		}
		delete image;
	}
}

void PPM::resize(int height, int width) {
	RGB** image_resized = new RGB * [height];
	for (int i = 0; i < height; i++) {
		image_resized[i] = new RGB[width];

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