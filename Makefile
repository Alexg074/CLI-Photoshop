# Copyright 2022-2023 Georgescu Mihai-Alexandru, 322CA

CFLAGS=-Wall -Wextra -g -std=c99

build: image_editor

image_editor: image_editor.o functions.o load_image.o save_image.o select.o select_all.o rotate_image.o crop_image.o filters.o
	gcc $(CFLAGS) $^ -o image_editor

%.o: %.c
	gcc $(CFLAGS) -c $^

pack:
	zip -FSr 322CA_GeorgescuMihai-Alexandru_Tema3.zip README Makefile *.c *.h

clean:
	rm -f image_editor *.o

.PHONY: clean pack