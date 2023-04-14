#pragma once
#include "info.h"

void check_for_comments(FILE * f);
void common_read(char *image_file, image_info *image, long *position);
int load_image(char *image_file, image_info *image);
