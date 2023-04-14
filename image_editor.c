#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "load_image.h"
#include "functions.h"
#include "info.h"
#include "save_image.h"
#include "select_all.h"
#include "select.h"
#include "rotate_image.h"
#include "crop_image.h"
#include "filters.h"

#define MAX_READ 1000

int main(void)
{
	int loaded = 0;
	image_info image;
	char *params[MAX_READ];
	char s[MAX_READ];
	char command[MAX_READ] = "";
	// - the string "params" stores each parameter for the command i read
	// - the string "commands" stores the command itself (load, save etc)
	// - the string "s" it's used to read each command and its params
	while (strcmp(command, "EXIT")) {
		fgets(s, MAX_READ, stdin);
		strcpy(command, command_type(params, s));
		if (strcmp(command, "LOAD") == 0) {
			// if there's already one image loaded, then i free the matrix
			if (loaded == 1)
				free_matrix(&image.matrix, image.image_size.width,
							image.image_size.height);
			loaded = load_image(params[0], &image);
		}
		if (strcmp(command, "SAVE") == 0) {
			if (loaded == 0)
				printf("No image loaded\n");
			else
				save_image(params[0], image, params[1]);
		}
		if (strcmp(command, "SELECT") == 0) {
			if (loaded == 0) {
				printf("No image loaded\n");
			} else {
				if (strcmp(params[0], "ALL") == 0) {
					select_all(&image, loaded);
					printf("Selected ALL\n");
				} else {
					select_image(&image, loaded, params);
				}
			}
		}
		if (strcmp(command, "ROTATE") == 0) {
			if (loaded == 0)
				printf("No image loaded\n");
			else
				rotate_image(&image, loaded, params[0]);
		}
		if (strcmp(command, "CROP") == 0) {
			if (loaded == 0)
				printf("No image loaded\n");
			else
				image.matrix = crop_image(&image, loaded);
		}
		if (strcmp(command, "APPLY") == 0) {
			if (loaded == 0)
				printf("No image loaded\n");
				// apply_filter(&image, loaded, params[0]);
		}
	}
	// case EXIT
	if (loaded == 0)
		printf("No image loaded\n");
	else
		free_matrix(&image.matrix, image.image_size.width,
					image.image_size.height);
	return 0;
}
