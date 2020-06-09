#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "image.h"
#define MAX_PKT_SIZE 65536
#define MAX_IFS_SIZE 16384
#define MMAP



int main(void)
{
	int fp,fr,fk;
	int *p,*r,*k;
	int kernel[9]={0,0,0,0,1,0,0,0,0};
	int ifs[4]={11,11,1,0};

	fp = open("/dev/image_conv", O_RDWR|O_NDELAY);
	if (fp < 0)
	{
		printf("Cannot open /dev/image_conv for write\n");
		return -1;
	}
	p=(int*)mmap(0, MAX_IFS_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fp, 0);
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

	fk = open("/dev/bram_kernel", O_RDWR|O_NDELAY);
	if (fk < 0)
	{
		printf("Cannot open /dev/bram_kernel for write\n");
		return -1;
	}
	k=(int*)mmap(0,9, PROT_READ | PROT_WRITE, MAP_SHARED, fk, 0);
	if (k == NULL ) {
		printf ("\ncouldn't mmap\n");
		return 0;
	}

	memcpy(p, ifs, 4);
	memcpy(r, image, 120);
	memcpy(k, kernel, 9);

	munmap(p, 4);
	munmap(r, MAX_PKT_SIZE);
	munmap(k, 9);

	close(fp);
	close(fr);
	close(fk);
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

	if(fk < 0)
	{
		printf("cannot close /dev/bram_kernel for write\n");
		return -1;
	}
	return 0;


}
