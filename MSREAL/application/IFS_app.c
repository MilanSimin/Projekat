#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "image.h"
#define MAX_PKT_SIZE 65536
#define MAX_IFS_SIZE 16384

//comment to send pixels as commands via regular write function of char driver
//leave uncommented to write directly to memory (faster)

#define MMAP


int main(void)
{
	int fp,fr;
	int *p,*r;
	int i,x,y;

	fp = open("/dev/image_conv", O_RDWR|O_NDELAY);
	if (fp < 0)
	{
		printf("Cannot open /dev/image_conv for write\n");
		return -1;
	}
	p=(int*)mmap(0,4, PROT_READ | PROT_WRITE, MAP_SHARED, fp, 0);
	if (p == NULL ) {
		printf ("\ncouldn't mmap\n");
		return 0;
	}

	fr = open("/dev/bram_image", O_RDWR|O_NDELAY);
	if (fr < 0)
	{
		printf("Cannot open /dev/bram_image for write\n");
		return -1;
	}
	r=(int*)mmap(0,MAX_PKT_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fr, 0);
	if (r == NULL ) {
		printf ("\ncouldn't mmap\n");
		return 0;
	}
	memcpy(r, image, 120);
	memcpy(p, image, 4);
	munmap(p, 4);
	munmap(r, MAX_PKT_SIZE);
	printf ("\nclosing file\n");
	close(fp);
	close(fr);
	if (fp < 0)
	{
		printf("Cannot close /dev/image_conv for write\n");
		return -1;
	}

	if(fr < 0)
	{
		printf("cannot close /dev/bram_image for write\n");
		return -1;
	}
	return 0;
}
