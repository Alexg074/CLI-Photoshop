#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "load_image.h"
#include "functions.h"
#include "info.h"

#define MAX_READ 1000

int valid_param(char  *s)
{
	int len = strlen(s);

	if (s[0] != '-' && !(isdigit(s[0])))
		return 0;

	for (int i = 1; i < len; i++) {
		if (!isdigit(s[i]))
			return 0;
	}
	// in case all the characters of the string are digits
	return 1;
}

void select_image(image_info *image, int loaded, char *params[MAX_READ])
{
	if (loaded == 0) {
		printf("No image loaded\n");
		return;
	}

	// in case i don t have 4 parameters
	if (!strcmp(params[0], "") || !strcmp(params[1], "") ||
		!strcmp(params[2], "") || !strcmp(params[3], "")) {
		printf("Invalid command\n");
		return;
	}

	// in case the parameters are not digits
	if (valid_param(params[0]) == 0 || valid_param(params[1]) == 0 ||
		valid_param(params[2]) == 0 || valid_param(params[3]) == 0) {
		printf("Invalid command\n");
		return;
	}

	int width = (*image).image_size.width;
	int height = (*image).image_size.height;
	// atoi() function - converts a string to an integer
	int x1 = atoi(params[0]);
	int y1 = atoi(params[1]);
	int x2 = atoi(params[2]);
	int y2 = atoi(params[3]);

	// making sure that x1 and x2 values don t exceed their max possible value
	// (width)
	if ((x1 < 0 || x1 > width) || (x2 < 0 || x2 > width)) {
		printf("Invalid set of coordinates\n");
		return;
	}
	// making sure that y1 and y2 values don t exceed their max possible value
	// (height)
	if ((y1 < 0 || y1 > height) || (y2 < 0 || y2 > height)) {
		printf("Invalid set of coordinates\n");
		return;
	}

	// the correct selection is made only if x1 < x2 and y1 < y2
	if (x1 > x2)
		swap(&x1, &x2);
	if (y1 > y2)
		swap(&y1, &y2);

	(*image).image_start.width = x1;
	(*image).image_start.height = y1;
	(*image).image_end.width = x2;
	(*image).image_end.height = y2;

	printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
}
