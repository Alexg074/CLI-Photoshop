#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "info.h"
#include "functions.h"

#define MAX_READ 1000

// basic swap
void swap(int *a, int *b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}

char *command_type(char *params[MAX_READ], char s[MAX_READ])
{
	// searching for '\n' character and replacing it with '\0'
	char *string_end = strchr(s, '\n');
	if (string_end)
		*string_end = '\0';

	// places the holder right after the first word of my string s
	// (basically separates the first word from the first space)
	char *word = strtok(s, " ");
	char *words_string[MAX_READ];
	int nr_words = 0;

	// words_string stores each word of the "s" string
	while (word) {
		words_string[nr_words++] =  word;
		word = strtok(NULL, " ");
	}

	if (strcmp(words_string[0], "LOAD") == 0) {
		params[0] = words_string[1];
	} else if (strcmp(words_string[0], "SELECT") == 0) {
		if (strcmp(words_string[1], "ALL")) {
			// storing each param needed for the case SELECT <x1><y1><x2><y2>
			for (int i = 0; i < nr_words - 1; i++)
				params[i] = words_string[i + 1];
		} else {
			params[0] = words_string[1];
		}
	} else if (strcmp(words_string[0], "ROTATE") == 0) {
		params[0] = words_string[1];
	} else if (strcmp(words_string[0], "CROP") == 0) {
		params[0] = "\0";
	} else if (strcmp(words_string[0], "APPLY") == 0) {
		params[0] = words_string[1];
	} else if (strcmp(words_string[0], "SAVE") == 0) {
		params[0] = words_string[1];
		// if there are more then 2 words read, then i have to
		// store in "params" string the "[ascii]" param as well
		if (nr_words > 2)
			params[1] = words_string[2];
		else
			params[1] = "\0";
	} else if (strcmp(words_string[0], "EXIT")) {
		params[0] = "\0";
	}

	// the function returns the command itself (load, save etc)
	return words_string[0];
}

// function which frees an image matrix
void free_matrix(double ****matrix, int width, int height)
{
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++)
			free((*matrix)[i][j]);
		free((*matrix)[i]);
	}
	free(*matrix);
}

// function which frees a simple matrix
void free_simple_matrix(double **matrix, int rows)
{
	for (int i = 0; i < rows; i++)
		free((matrix)[i]);
	free(matrix);
}

// function which allocates a simple matrix
double **alloc_simple_matrix(int rows, int columns)
{
	double **matrix = malloc(rows * sizeof(double *));

	if (!matrix) {
		printf("malloc failed\n");
		free(matrix);
		return NULL;
	}

	for (int i = 0; i < rows; i++) {
		matrix[i] = malloc(columns * sizeof(double));

		if (!matrix[i]) {
			free_simple_matrix(matrix, i);
			return NULL;
		}
	}

	return matrix;
}

// allocating and reading my matrix from an ASCII file
void read_ascii_matrix(char *image_file, image_info *image, long position)
{
	FILE *f = fopen(image_file, "r");
	if (!f) {
		printf("Failed to open %s\n", image_file);
		return;
	}

	// placing the cursor back on the correct position
	fseek(f, position, SEEK_SET);

	int width = (*image).image_size.width;
	int height = (*image).image_size.height;
	int nr_pixels = (*image).nr_pixels;

	(*image).matrix = malloc(height * sizeof(double **));

	if (!(*image).matrix) {
		printf("malloc failed\n");
		free((*image).matrix);
		return;
	}

	fseek(f, 1, SEEK_CUR);

	// allocating each cell from each row of my matrix
	for (int i = 0; i < height; i++) {
		(*image).matrix[i] = malloc(width * sizeof(double *));

		if (!(*image).matrix[i]) {
			free_matrix(&(*image).matrix, i, height);
			exit(-1);
		}

		// allocating memory for each cell of my matrix and thus for each
		// string of pixels stored in each cell of my matrix
		for (int j = 0; j < width; j++) {
			(*image).matrix[i][j] = malloc(nr_pixels * sizeof(double));

			// reading each pixel value from each string of pixels
			for (int k = 0; k < nr_pixels; k++)
				fscanf(f, "%lf", &(*image).matrix[i][j][k]);
		}
	}

	fclose(f);
}

// allocating and reading my matrix from a binary file
void read_binary_matrix(char *image_file, image_info *image, long position)
{
	FILE *f = fopen(image_file, "rb");
	if (!f) {
		printf("Failed to open %s\n", image_file);
		return;
	}

	fseek(f, position, SEEK_SET);

	int width = (*image).image_size.width;
	int height = (*image).image_size.height;
	int nr_pixels = (*image).nr_pixels;

	(*image).matrix = malloc(height * sizeof(double **));

	if (!(*image).matrix) {
		printf("malloc failed\n");
		free((*image).matrix);
		exit(-1);
	}

	fseek(f, 1, SEEK_CUR);

	// allocating each cell from each row of my matrix
	for (int i = 0; i < height; i++) {
		(*image).matrix[i] = malloc(width * sizeof(double *));

		if (!(*image).matrix[i]) {
			free_matrix(&(*image).matrix, i, height);
			exit(-1);
		}

		// allocating memory for each cell of my matrix and thus for each
		// string of pixels stored in each cell of my matrix
		for (int j = 0; j < width; j++) {
			(*image).matrix[i][j] = malloc((nr_pixels * sizeof(double)));

			// reading each pixel value from each string of pixels
			for (int k = 0; k < nr_pixels; k++) {
				unsigned char pixel;
				fread(&pixel, sizeof(unsigned char), 1, f);
				(*image).matrix[i][j][k] = (double)pixel;
			}
		}
	}

	fclose(f);
}

void print_ascii_matrix(FILE *output, image_info image)
{
	//my "output" file is already opened in save_image.c
	int width = image.image_size.width;
	int height = image.image_size.height;
	int nr_pixels = image.nr_pixels;

	/* depending on the image type,
	nr_pixels = 1 (BW & grayscale) or 3 for RGB */
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			for (int k = 0; k < nr_pixels; k++)
				fprintf(output, "%hhu ", (unsigned char)image.matrix[i][j][k]);
		}
	fprintf(output, "\n");
	}
}

void print_binary_matrix(FILE *output, image_info image)
{
	int width = image.image_size.width;
	int height = image.image_size.height;
	int nr_pixels = image.nr_pixels;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			for (int k = 0; k < nr_pixels; k++) {
				// each pixel has a max 255 value => unsigned char
				unsigned char pixel;
				pixel = (unsigned char)image.matrix[i][j][k];
				fwrite(&pixel, sizeof(unsigned char), 1, output);
			}
		}
	}
}
