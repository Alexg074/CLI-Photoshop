#pragma once
#include "info.h"
#define MAX_READ 1000

void swap(int *a, int *b);
char *command_type(char *params[MAX_READ], char s[MAX_READ]);
double **alloc_simple_matrix(int rows, int columns);
void free_simple_matrix(double **matrix, int rows);
void free_matrix(double ****matrix, int height, int width);
void read_ascii_matrix(char *image_file, image_info *image, long position);
void read_binary_matrix(char *image_file, image_info *image, long position);
void print_ascii_matrix(FILE *output, image_info image);
void print_binary_matrix(FILE *output, image_info image);
