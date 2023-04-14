#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "load_image.h"
#include "functions.h"
#include "info.h"
#include "select.h"
#include "select_all.h"

double ***crop_image(image_info *image, int loaded)
{
	if (loaded == 0) {
		printf("No image loaded\n");
		exit(-1);
	}

	int new_width = (*image).image_end.width - (*image).image_start.width;
	int new_height = (*image).image_end.height - (*image).image_start.height;

	// allocating the new (cropped) matrix
	double ***new_matrix = malloc(new_height * sizeof(double **));

	if (!new_matrix) {
		printf("malloc failed\n");
		free(new_matrix);
		exit(-1);
	}

	// allocating each cell from each row of my new matrix
	for (int i = 0; i < new_height; i++) {
		new_matrix[i] = malloc(new_width * sizeof(double *));
		if (!new_matrix[i]) {
			free_matrix(&new_matrix, new_width, i);
			exit(-1);
		}

		for (int j = 0; j < new_width; j++)
			new_matrix[i][j] = malloc(((*image).nr_pixels * sizeof(double)));
	}

	int width = (*image).image_size.width;
	int height = (*image).image_size.height;

	int x1 = (*image).image_start.width;
	int y1 = (*image).image_start.height;
	int x2 = (*image).image_end.width;
	int y2 = (*image).image_end.height;

	// creating the new matrix based on the selected area of the original matrix
	for (int i = y1; i <  y2; i++)
		for (int j = x1; j < x2; j++)
			for (int k = 0; k < (*image).nr_pixels; k++)
				new_matrix[i - y1][j - x1][k] = (*image).matrix[i][j][k];

	free_matrix(&(*image).matrix, width, height);
	// updating the dimensions in my struct
	(*image).image_size.width = new_width;
	(*image).image_size.height = new_height;

	// updating the coordinates of the selection in my struct
	(*image).image_start.width = 0;
	(*image).image_start.height = 0;
	(*image).image_end.width = new_width;
	(*image).image_end.height = new_height;

	printf("Image cropped\n");
	return new_matrix;
}
