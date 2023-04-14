#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "info.h"
#include "functions.h"

#define MAX_READ 1000

// function which checks for comments in the input file
void check_for_comments(FILE *f)
{
	char character, trash;
	fscanf(f, "%c", &character);

	if (character != '#') {
		fseek(f, -1, SEEK_CUR);
	} else {
		// if a comment is detected, i disregard characters until
		// a "new line" character appears
		do {
			fscanf(f, "%c", &trash);
		} while (trash != '\n');
	}
}

void common_read(char *image_file, image_info *image, long *position)
{
	FILE *f = fopen(image_file, "r");
	if (!f) {
		printf("Failed to load %s\n", image_file);
		return;
	}

	char *s = malloc(MAX_READ * (sizeof(char)));

	check_for_comments(f);

	fscanf(f, "%s", s);
	if (s[0] == 'P') {
		(*image).image_type = s[1] - '0';
		if ((*image).image_type % 3 == 0)
			(*image).nr_pixels = 3;
		else
			(*image).nr_pixels = 1;
	}
	free(s);

	check_for_comments(f);

	int width, height;

	// oading the dimensions and the max_value
	fscanf(f, "%d", &width);
	fscanf(f, "%d", &height);

	(*image).image_size.width = width;
	(*image).image_size.height = height;

	check_for_comments(f);

	if ((*image).image_type % 3 != 1) {
		fscanf(f, "%hhu", &(*image).max_value);
		check_for_comments(f);
	}

	// storing the position of the last character before reading
	// the matrix
	*position = ftell(f);

	fclose(f);
}

int load_image(char *image_file, image_info *image)
{
	long position = 0;
	common_read(image_file, image, &position);

	if (position == 0)
		return 0;

	if ((*image).image_type < 4)
		read_ascii_matrix(image_file, &(*image), position);
	else
		read_binary_matrix(image_file, &(*image), position);

	printf("Loaded %s\n", image_file);

	return 1;
}
