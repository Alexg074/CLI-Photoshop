 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <math.h>

 #include "load_image.h"
 #include "functions.h"
 #include "info.h"
 #include "select.h"
 #include "select_all.h"
 #include "filters.h"

 // function which keeps a value within [0,255] interval
 double clamp(double x)
 {
 	if (x < 0)
 		return 0;
 	else if (x > 255)
 		return 255;
 	else
 		return x;
 }

 // function which returns the kernel matrix specific to a certain filter
 double **get_kernel_matrix(char *filter)
 {
 	double edge_detection_kernel[3][3] = {{-1, -1, -1}, {-1, 8, -1},
 										{-1, -1, -1}};

 	double **edge_detection = alloc_simple_matrix(3, 3);
 	for (int i = 0; i < 3; i++)
 		for (int j = 0; j < 3; j++)
 			edge_detection[i][j] = edge_detection_kernel[i][j];

 	double sharpen_kernel[3][3] = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};

 	double **sharpen = alloc_simple_matrix(3, 3);
 	for (int i = 0; i < 3; i++)
 		for (int j = 0; j < 3; j++)
 			sharpen[i][j] = sharpen_kernel[i][j];

 	double box_blur_kernel[3][3] =  {{0.1111, 0.1111, 0.1111},
 									{0.1111, 0.1111, 0.1111},
 									{0.1111, 0.1111, 0.1111}};

 	double **box_blur = alloc_simple_matrix(3, 3);

 	for (int i = 0; i < 3; i++)
 		for (int j = 0; j < 3; j++)
 			box_blur[i][j] = box_blur_kernel[i][j];

 	double gaussian_blur_kernel[3][3] =  {{0.0625, 0.125, 0.0625},
 											{0.125, 0.25, 0.125},
 											{0.0625, 0.125, 0.0625}};

 	double **gaussian_blur = alloc_simple_matrix(3, 3);
 	for (int i = 0; i < 3; i++)
 		for (int j = 0; j < 3; j++)
 			gaussian_blur[i][j] = gaussian_blur_kernel[i][j];

 	if (strcmp(filter, "EDGE") == 0)
 		return edge_detection;
 	else if (strcmp(filter, "SHARPEN") == 0)
 		return sharpen;
 	else if (strcmp(filter, "BLUR") == 0)
 		return box_blur;
 	else if (strcmp(filter, "GAUSSIAN_BLUR") == 0)
 		return gaussian_blur;
 	else
 		return NULL;
 }

 void filter_pixel(image_info *image, double ****filtered_matrix,
 				  int row, int column, int color, char *filter)
 {
 	double **kernel_matrix = get_kernel_matrix(filter);

 	// initialising the pixel that will be filtered
 	(*filtered_matrix)[row][column][color] = 0;
 	// going through the current pixel's neighbours
 	for (int l = row - 1; l <= row + 1; l++)
 		for (int m = column - 1; m <= column + 1; m++)
 			(*filtered_matrix)[row][column]
 			[color] += kernel_matrix[l - row + 1][m - column + 1]
 						* (*image).matrix[l][m][color];

 	// keeping the value within [0,255] interval
 	(*filtered_matrix)[row][column][color] = clamp((*filtered_matrix)[row]
 													[column][color]);
 }

 void apply_filter(image_info *image, int loaded, char *filter)
 {
 	int nr_pixels = (*image).nr_pixels;

 	// dealing with the potential error cases
 	if (loaded == 0) {
 		printf("No image loaded\n");
 		return;
 	}

 	if (nr_pixels == 1) {
 		printf("Easy, Charlie Chaplin\n");
 		return;
 	}

 	if (strcmp(filter, "EDGE") != 0 && strcmp(filter, "SHARPEN") != 0 &&
 		strcmp(filter, "BLUR") != 0 && strcmp(filter, "GAUSSIAN_BLUR") != 0) {
 		printf("APPLY parameter invalid\n");
 		return;
 	}

 	// starting the "APPLY" action itself
 	// declaring and allocating the new filtered matrix
 	int height = (*image).image_size.height;
 	int width = (*image).image_size.width;

 	double ***filtered_matrix = malloc(height * sizeof(double **));
 	if (!filtered_matrix) {
 		printf("malloc failed\n");
 		free(filtered_matrix);
 		return;
 	}

 	for (int i = 0; i < height; i++) {
 		filtered_matrix[i] = malloc(width * sizeof(double *));
 		if (!filtered_matrix[i]) {
 			free_matrix(&filtered_matrix, i, width);
 			return;
 		}

 		for (int j = 0; j < width; j++) {
 			filtered_matrix[i][j] = malloc((nr_pixels * sizeof(double)));
 			// keeping the original values of the pixels for the image's borders
 			for (int k = 0; k < nr_pixels; k++)
 				filtered_matrix[i][j][k] = (*image).matrix[i][j][k];
 		}
 	}

 	// applying the actual kernel matrix to the pixels
 	for (int i = image->image_start.height; i < image->image_end.height; i++) {
 		for (int j = image->image_start.width; j < image->image_end.width;
 			j++) {
 			if (!(i != 0 && j != 0 && i != height - 1 && j != width - 1))
 				continue;
 			for (int k = 0; k < 3; k++) {
 				// going through the neighbours of the pixel and applying
 				// the filter
 				filter_pixel(image, &filtered_matrix, i, j, k, filter);
 			}
 		}
 	}

 	// applying the new RGB values to the original matrix
 	// the filters won't be applied to the edges of the full image
 	for (int i = image->image_start.height; i < image->image_end.height; i++)
 		for (int j = image->image_start.width; j < image->image_end.width; i++)
 			if (i != 0 && j != 0 && i != height - 1 && j != width - 1)
 				for (int k = 0; k < 3; k++)
 					(*image).matrix[i][j][k] = filtered_matrix[i][j][k];

 	free_matrix(&filtered_matrix, height, width);
 	printf("APPLY %s done\n", filter);
 }
