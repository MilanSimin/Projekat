#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "image.h"

#define MAX_BRAM_SIZE 57600
#define MAX_IFS_SIZE 16384
#define MAX_KERNEL_SIZE 4096
#define IFS_SEND 16
#define KERNEL_SEND 36
#define MMAP


int main(void){
	int kernel[9]={-1,-1,-1,-1,8,-1,-1,-1,-1};
	int lines=120,columns=121,start=1,num_of_bytes=10;
	int ifs_reg[4]={columns,lines,start,0};
	int *final_image, *ready;
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
        ready = (int *) malloc(num_of_bytes);
	p=(int*)mmap(0, MAX_IFS_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fp, 0);
	if (p == NULL ) {
		printf ("\ncouldn't mmap\n");
		return 0;
	}

	memcpy(p, ifs_reg, IFS_SEND);
	//memcpy(ready, p, IFS_SEND/4);
	munmap(p, IFS_SEND);
	printf("image_conv done\n");
	//printf("ready is %d\n",&ready);
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
	r=(int*)mmap(0,MAX_BRAM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fr, 0);
	if (r == NULL ) {
		printf ("\ncouldn't mmap\n");
		return 0;
	}
	memcpy(r, image,MAX_BRAM_SIZE);
	munmap(r, MAX_BRAM_SIZE);
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
	k=(int*)mmap(0,MAX_KERNEL_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fk, 0);
	if (k == NULL ) {
		printf ("\ncouldn't mmap\n");
		return 0;
	}

	memcpy(k, kernel, KERNEL_SEND);
	munmap(k, KERNEL_SEND);
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
	final_image = (int *) malloc(MAX_BRAM_SIZE);
	n=(int*)mmap(0,MAX_BRAM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fn, 0);
	if (n == NULL ) {
		printf ("\ncouldn't mmap\n");
		return 0;
	}
	
	memcpy(final_image, n, MAX_BRAM_SIZE);
	munmap(n, MAX_BRAM_SIZE);
	printf("bram_after_conv done\n");
	close(fn);
	if(fn < 0)
	{
		printf("cannot close /dev/bram_after_conv for write\n");
		return -1;
	}

	/*for(i=72; i>62; i--){
		printf("final_image[%d] is %d\n",i,final_image[i]);
	}*/

	fm =fopen("final_image.txt","w");
	if (fm==NULL)
	{
		printf("cannot open final_image.txt\n");
		return -1;
	}
	printf("final_image opened\n");
	for(i = 0; i<118; i++)
	{
		for(j=0; j<119; j++)
		{
			fprintf(fm,"%d,",final_image[j+i*118]);
			fflush(fm);
		}
		fprintf(fm,"\n");
	}
	printf("final_image written\n");

	if(fclose(fm) == EOF)
	{
		printf("cannot close final_image.txt\n");
		return -1;
	}
	return 0;


}
