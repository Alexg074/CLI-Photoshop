#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "load_image.h"
#include "functions.h"
#include "info.h"
#include "select.h"
#include "select_all.h"

// function which rotates a matrix with 90 degrees to the right
void rotate_90_right(image_info *image)
{
	int x1 = (*image).image_start.width;
	int y1 = (*image).image_start.height;
	int x2 = (*image).image_end.width;
	int y2 = (*image).image_end.height;
	// SELECT ALL case
	if (x1 == 0 && y1 == 0 &&
		x2 == (*image).image_size.width && y2 == (*image).image_size.height) {
		int width = (*image).image_size.width;
		int height = (*image).image_size.height;
		int nr_pixels = (*image).nr_pixels;

		double ***rotated_matrix = (malloc(width * sizeof(double **)));
		if (!rotated_matrix) {
			printf("malloc failed\n");
			free(rotated_matrix);
			return;
		}
		for (int i = 0; i < width; i++) {
			rotated_matrix[i] = malloc(height * sizeof(double *));
			if (!rotated_matrix[i]) {
				free_matrix(&rotated_matrix, i, width);
				return;
			}
			for (int j = 0; j < height; j++)
				rotated_matrix[i][j] = malloc((nr_pixels * sizeof(double)));
		}
		// creating the matrix rotated with 90 degrees clockwise
		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
				for (int k = 0; k < nr_pixels; k++) {
					double val = (*image).matrix[height - j - 1][i][k];
					rotated_matrix[i][j][k] = val;
				}
		free_matrix(&(*image).matrix, width, height);
		// updating the dimensions in the struct
		swap(&(*image).image_size.width, &(*image).image_size.height);
		swap(&(*image).image_start.width, &(*image).image_start.height);
		swap(&(*image).image_end.width, &(*image).image_end.height);
		(*image).matrix = rotated_matrix;
		select_all(image, 1);
	} else if (x2 - x1 == y2 - y1) { // square selection
		int nr_pixels = (*image).nr_pixels;
		int n = x2 - x1; // = width = height
		// allocating the square matrix according to the SELECT params
		double ***rotated_matrix = malloc(n * sizeof(double **));
		if (!rotated_matrix) {
			printf("malloc failed\n");
			free(rotated_matrix);
			return;
		}
		for (int i = 0; i < n; i++) {
			rotated_matrix[i] = malloc(n * sizeof(double *));
			if (!rotated_matrix[i]) {
				free_matrix(&rotated_matrix, i, n);
				return;
			}
			for (int j = 0; j < n; j++)
				rotated_matrix[i][j] = malloc(nr_pixels * sizeof(double));
		}
		int cnt_r = 0;// column in the original matrix => row in rotated_matrix
		for (int j = x1; j < x2; j++) {
			int cnt_c = 0;
			for (int i = y2 - 1; i >= y1 ; i--)  {
				for (int k = 0; k < nr_pixels; k++)
					rotated_matrix[cnt_r][cnt_c][k] = (*image).matrix[i][j][k];
				cnt_c++;
			}
			cnt_r++; // increasing the number of rows in the rotated_matrix
		}
		for (int i = y1; i < y2; i++) {
			for (int j = x1; j < x2; j++) {
				for (int k = 0; k < (*image).nr_pixels; k++) {
					double val = rotated_matrix[i - y1][j - x1][k];
					(*image).matrix[i][j][k] = val;
				}
			}
		}
		free_matrix(&rotated_matrix, n, n);
	}
}

void rotate_image(image_info *image, int loaded, char *given_angle)
{
	int x1 = (*image).image_start.width;
	int y1 = (*image).image_start.height;
	int x2 = (*image).image_end.width;
	int y2 = (*image).image_end.height;

	// coverting the string Angle into an integer
	int angle = atoi(given_angle);

	if (loaded == 0) {
		printf("No image loaded\n");
		return;
	}

	if (angle % 90 != 0 || angle > 360) {
		printf("Unsupported rotation angle\n");
		return;
	}

	if (!(x1 == 0 && y1 == 0))
		if (x2 == (*image).image_size.width && y2 == (*image).image_size.height)
			if (!(x2 - x1 == y2 - y1)) {
				printf("The selection must be square\n");
				return;
			}

	if (angle == 90 || angle == -270) {
		rotate_90_right(&(*image));
	} else if (angle == 180 || angle == -180) {
		rotate_90_right(&(*image));
		rotate_90_right(&(*image));
	} else if (angle == 270 || angle == -90) {
		rotate_90_right(&(*image));
		rotate_90_right(&(*image));
		rotate_90_right(&(*image));
	}

	printf("Rotated %d\n", angle);
}
