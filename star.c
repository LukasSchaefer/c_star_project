#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "star.h"

int star_read_from_file(struct star* s, FILE* f)
{
	double z;
	int harvard;
 	if (fscanf (f, "%lf %lf %lf %d %lf %d", &((*s).x), &((*s).y), &z, &((*s).draper), &((*s).magnitude), &harvard) == 6)
		return 1;
	else
		return 0;

}

void star_coord_to_pixel(struct star const* s, struct image* const img, int* x, int* y)
{
	double x_Koord = (*s).x;
	double y_Koord = (*s).y;
	if ((x_Koord < -1.0) || (x_Koord > 1.0) || (y_Koord < -1.0) || (y_Koord > 1.0)) {
		printf("Invalid star coordinates! X and Y must be between -1 and 1");
		return;
	}
	x_Koord = (int)((x_Koord + 1.0) * (((*img).w / 2.0) -0.5));
	y_Koord = (int)((-1.0) * (y_Koord - 1.0) * (((*img).h / 2.0) -0.5));
	*x = x_Koord;
	*y = y_Koord;
}

void star_plot(struct star const* s, struct image* img)
{
	int x_Koord, y_Koord;
	star_coord_to_pixel(s, img, &x_Koord, &y_Koord);
	int koord = y_Koord * (*img).w + x_Koord;
	if ((koord < ((*img).w * (*img).h)) && (koord >= 0))
		(*img).data[koord] = 0xffffff;
	return;
	
	
}

void draw_constellation_from_file(FILE* f, struct image* img, struct star const* stars, int num_stars) {
	int d1, d2;
	struct star s1, s2;
	while (fscanf(f, "%d,%d", &d1, &d2) > 0) {
		int status = 0;			// 0 - no draper number already found, 1 - d1 found, 2 - d2 found, 3 - both found
		for (int counter = 0; counter < num_stars; counter++) {
			if ((stars[counter]).draper == d1) {
				if (status == 0) {
					s1 = stars[counter];
					status = 1;
				}
				else 
					if (status == 1) 
						printf("2 stars with same draper number %d! Continue and ignore it.\n", d1);
					else {
						s2 = stars[counter];
						status = 3;
						break;
					}
			}	
			else {

				if ((stars[counter]).draper == d2) {
					if (status == 0) {
						s1 = stars[counter];
						status = 2;
					}
					else 
						if (status == 2) 
							printf("2 stars with same draper number %d! Continue and ignore it.\n", d2);
						else {
							s2 = stars[counter];
							status = 3;
							break;
						}
				}
			}
			
		}
		if (status == 3) {
			int x1_k, y1_k;
			star_coord_to_pixel(&s1, img, &x1_k, &y1_k);
			int x2_k, y2_k;
			star_coord_to_pixel(&s2, img, &x2_k, &y2_k);		
			image_draw_line(img, 0xffff00, x1_k, y1_k, x2_k, y2_k);
		}
	}
	return;
}
