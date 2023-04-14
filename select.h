#pragma once
#include "info.h"
#include "functions.h"

#define MAX_READ 1000

int valid_param(char  *s);
void select_image(image_info *image, int loaded, char *params[MAX_READ]);
