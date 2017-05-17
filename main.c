#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "star.h"
#include "image.h"

int main(int argc, char *argv[])
{
	if (argc < 3) {
		fprintf(stderr, "syntax: %s size starlist [lines...]\n", argv[0]);
		return EXIT_FAILURE;
	}

	int size = atoi(argv[1]);
	if (size <= 0) {
		fprintf(stderr, "size of image cannot be <= 0\n");
		return EXIT_FAILURE;
	}

	struct image img;
	image_init(&img, size, size);

	// Read all the stars (from file with name in argv[2])
	FILE * star_file = fopen(argv[2], "r");
	int max = 20;
	int star_numbers = 0;
	struct star * stars = malloc(max * sizeof(stars[0]));
	while (star_read_from_file((stars + star_numbers), star_file) == 1) {
		star_numbers += 1;
		if (star_numbers == max) {
			max *= 2;
			stars = realloc(stars, max * sizeof(stars[0]));
		}
	}
	fclose(star_file);

	// Draw all the stars
	for (int i = 0; i < star_numbers; i++)
		star_plot((stars + i), &img);
	
	// open every constellation file
	for (int i = 3; i < argc; i++) {
		FILE* f = fopen(argv[i], "r");
		// if the file could not be opened, skip to the next one.
		if (f == NULL) {
			fprintf(stderr, "cannot open line file \"%s\"\n", argv[i]);
			continue;
		}
		draw_constellation_from_file(f, &img, stars, star_numbers);
		fclose(f);
	}

	// write the painted image to the output file.
	FILE* image_file = fopen("stars.pbm", "w");
	if (image_file == NULL) {
		fprintf(stderr, "cannot open output file.\n");
		return EXIT_FAILURE;
	}

	image_write_to_file(&img, image_file);

	free(stars);

	fclose(image_file);
	image_destroy(&img);
}
