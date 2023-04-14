#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "load_image.h"
#include "functions.h"
#include "info.h"
#define MAX_READ 1000

// function which saves a loaded image
void save_image(char *image_file, image_info image, int loaded, char *params)
{
	int width = image.image_size.width;
	int height = image.image_size.height;
	unsigned char max_value = image.max_value;
	int image_type = image.image_type;

	if (loaded == 0) {
		printf("No image loaded\n");
		return;
	}

	FILE * output = fopen(image_file, "w");

	if (!output) {
		printf("Failed to open%s\n", image_file);
		exit(-1);
	}
	// saving the loaded image as an ASCII file if the param "ascii" exists
	if (!strcmp(params, "ascii")) {
		if (image_type >= 4)
			image_type -= 3;
		// printing the magic word, dimensions and max_value for the image
		fprintf(output, "P%d\n%d %d\n%hhu\n", image_type, width, height,
				max_value);

		print_ascii_matrix(output, image);
		printf("Saved %s\n", image_file);
		fclose(output);

	} else {
		if (image_type <= 3)
			image_type += 3;
		// printing the magic word, dimensions and max_value for the image
		fprintf(output, "P%d\n%d %d\n%hhu\n", image_type, width,
				height, max_value);

		fclose(output);

		// the file is binary, so i close the ASCII file an open it in binary,
		// then i save it
		FILE *output = fopen(image_file, "ab");

		if (!output) {
			printf("Failed to open%s\n", image_file);
			exit(-1);
		}
		print_binary_matrix(output, image);
		printf("Saved %s\n", image_file);
		fclose(output);
	}
}

