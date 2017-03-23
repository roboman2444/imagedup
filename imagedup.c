#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for memset
#include "stb_image.h"
#include "stb_image_resize.h"


typedef struct image_s {
	const char * filename; // just a pointer to argv
	int bpp; // maybe add a b&w filter?
	int origheight, origwidth;
	int newheight, newwidth;
	int status; // 0 is nothing,, 1 is loaded, 2 is rescaled
	unsigned char * resizedata;
} image_t;

#define WIDTH 16
#define HEIGHT 16



int main(const int argc, const char ** argv){
	if(argc < 3){
		printf("no images specified\n");
		return -1;
	}
	int num_img = argc-1;
	image_t * imagelist = malloc(num_img * sizeof(image_t));
	memset(imagelist, 0, num_img * sizeof(image_t));
	int i;
	for(i = 0; i < num_img; i++){
		int x, y, n;
		imagelist[i].filename = argv[i+1];
		unsigned char * data = stbi_load(imagelist[i].filename, &x, &y, &n, 0);
		if(!data){
			printf("unable to load %s\n", imagelist[i].filename);
			continue;
		}
		printf("loaded %s with X:%i Y:%i N:%i\n", imagelist[i].filename, x, y, n);
		imagelist[i].status = 1;
		imagelist[i].bpp = n;
		imagelist[i].origwidth = x;
		imagelist[i].origheight = y;

		imagelist[i].newheight = HEIGHT;
		imagelist[i].newwidth = WIDTH;
		imagelist[i].resizedata = malloc(WIDTH * HEIGHT *n);
		stbir_resize_uint8(data, x, y, 0, imagelist[i].resizedata, imagelist[i].newwidth, imagelist[i].newheight, 0, n);
		imagelist[i].status = 2;
		stbi_image_free(data);
	}
	//all images loaded
	int sizebize = WIDTH * HEIGHT;
	for(i = 0; i < num_img; i++){
		if(imagelist[i].status != 2) continue;
		unsigned char * origdata = imagelist[i].resizedata;
		int orign = imagelist[i].bpp;

		int bestk = -1;
		int bestval = 0;
		int k;
		for(k = 0; k < num_img; k++){
			if(imagelist[k].status != 2) continue;
			if(k == i) continue;
			unsigned char * newdata = imagelist[k].resizedata;
			int newn = imagelist[k].bpp;

			int b;
			int tdiff = 0;
			for(b = 0; b<sizebize; b++){
				int k;
				for(k = 0; k < 3; k++){
				//compare
					unsigned char origsamp = origdata[b*orign + k%orign];
					unsigned char newsamp = newdata[b*newn + k%newn];
					int diff = newsamp - origsamp;
					tdiff+= diff < 0 ? -diff : diff;
				}
			}
//			printf("%s vs %s diff %i\n", imagelist[i].filename, imagelist[k].filename, tdiff);
			if(tdiff < bestval || bestk<0){
				bestval = tdiff;
				bestk = k;
			}
		}
		printf("%s and %s are similar with a diff of %i\n", imagelist[i].filename, imagelist[bestk].filename, bestval);
	}
	return 0;
}
