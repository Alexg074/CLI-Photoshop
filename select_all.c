#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "load_image.h"
#include "functions.h"
#include "info.h"

void select_all(image_info *image, int loaded)
{
	if (loaded == 0) {
		printf("No image loaded\n");
		return;
	}

	// selecting all the image

	(*image).image_start.width = 0;
	(*image).image_start.height = 0;
	(*image).image_end.width = (*image).image_size.width;
	(*image).image_end.height = (*image).image_size.height;
}
