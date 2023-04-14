#pragma once

typedef struct {
	int width;
	int height;
} size;

// struct which contains details about my image
// image_type can be 1,2,3,4,5 or 6, depending on the magic word
typedef struct {
	int image_type;
	size image_size;

	// the coordinates of a selection i make
	size image_start;
	size image_end;
	// max ascii value for a pixel
	unsigned char max_value;
	double ***matrix;
	int nr_pixels;
	// nr_pixels = 3 for RGB image
	// nr_pixels = 1 for BW & grayscale images
} image_info;

