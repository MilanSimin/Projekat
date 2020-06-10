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

int kernel[9]={0,0,0,0,1,0,0,0,0};
int ifs[4]={11,11,1,0};
int *final_image;
int num_of_bytes = 500;

int main(void)
{
	FILE *fm;
	int fp,fr,fk,fn;
	int *p,*r,*k,*n;
	int i=0,j=0;

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
	memcpy(p, ifs, 16);
	munmap(p, 16);
	printf("image_conv done\n");
	close(fp);
	if (fp < 0)
	{
		printf("Cannot close /dev/image_conv for write\n");
		return -1;
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

	memcpy(r, image, 500);
	munmap(r, 500);
	printf("bram_image done\n");
	close(fr);
	if(fr < 0)
	{
		printf("cannot close /dev/bram_image for write\n");
		return -1;
	}

	fk = open("/dev/bram_kernel", O_RDWR|O_NDELAY);
	if (fk < 0)
	{
		printf("Cannot open /dev/bram_kernel for write\n");
		return -1;
	}
	k=(int*)mmap(0,4096, PROT_READ | PROT_WRITE, MAP_SHARED, fk, 0);
	if (k == NULL ) {
		printf ("\ncouldn't mmap\n");
		return 0;
	}

	memcpy(k, kernel, 36);
	munmap(k, 36);
	printf("bram_kernel done\n");
	close(fk);
	if(fk < 0)
	{
		printf("cannot close /dev/bram_kernel for write\n");
		return -1;
	}


	fn = open("/dev/bram_after_conv", O_RDWR|O_NDELAY);
	if (fn < 0)
	{
		printf("Cannot open /dev/bram_after_conv for write\n");
		return -1;
	}
	final_image = (int *) malloc(num_of_bytes+1);
	n=(int*)mmap(0,MAX_PKT_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fn, 0);
	if (n == NULL ) {
		printf ("\ncouldn't mmap\n");
		return 0;
	}
	
	memcpy(final_image, n, 500);
	munmap(n, 500);
	printf("bram_after_conv done\n");
	close(fn);
	if(fn < 0)
	{
		printf("cannot close /dev/bram_after_conv for write\n");
		return -1;
	}

	for(i=72; i<62; i--){
		printf("final_image[%d] is %d\n",i,final_image[i]);
	}

	fm =fopen("final_image.txt","w");
	if (fm==NULL)
	{
		printf("cannot open final_image.txt\n");
		return -1;
	}
	printf("final_image opened\n");
	for(i = 0; i<9; i++)
	{
		fprintf(fm,"\n");
		for(j=0; j<9; j++)
		{
			fprintf(fm,"%d,",final_image[j+i*9]);
			fflush(fm);
		}
	}
	printf("final_image written\n");

	if(fclose(fm) == EOF)
	{
		printf("cannot close final_image.txt\n");
		return -1;
	}
	return 0;


}
