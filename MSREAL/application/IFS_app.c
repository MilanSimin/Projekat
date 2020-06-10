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

	/*printf("final_image[0]: %d\n",final_image[0]);
	printf("final_image[1]: %d\n",final_image[1]);
	printf("final_image[2]: %d\n",final_image[2]);
	printf("final_image[3]: %d\n",final_image[3]);
	printf("final_image[4]: %d\n",final_image[4]);
	printf("final_image[5]: %d\n",final_image[5]);
	printf("final_image[6]: %d\n",final_image[6]);
	printf("final_image[7]: %d\n",final_image[7]);
	printf("final_image[8]: %d\n",final_image[8]);
	printf("final_image[9]: %d\n",final_image[9]);
	printf("final_image[10]: %d\n",final_image[10]);
	printf("final_image[11]: %d\n",final_image[11]);
	printf("final_image[12]: %d\n",final_image[12]);
	printf("final_image[13]: %d\n",final_image[13]);
	printf("final_image[14]: %d\n",final_image[14]);
	printf("final_image[15]: %d\n",final_image[15]);


	printf("final_image[57]: %d\n",final_image[57]);
	printf("final_image[58]: %d\n",final_image[58]);
	printf("final_image[59]: %d\n",final_image[59]);
	printf("final_image[60]: %d\n",final_image[60]);
	printf("final_image[61]: %d\n",final_image[61]);
	printf("final_image[62]: %d\n",final_image[62]);
	printf("final_image[63]: %d\n",final_image[63]);
	printf("final_image[64]: %d\n",final_image[64]);
	printf("final_image[65]: %d\n",final_image[65]);
	printf("final_image[66]: %d\n",final_image[66]);
	printf("final_image[67]: %d\n",final_image[67]);
	printf("final_image[68]: %d\n",final_image[68]);
	printf("final_image[69]: %d\n",final_image[69]);
	printf("final_image[70]: %d\n",final_image[70]);
	printf("final_image[71]: %d\n",final_image[71]);
	printf("final_image[72]: %d\n",final_image[72]);*/

	fm =fopen("final_image.txt","w");
	if (fm==NULL)
	{
		printf("cannot open final_image.txt\n");
		return -1;
	}
	printf("final_image opened\n");
	//for(i = 0; i<9; i++)
	//{
	//	fprintf(fm,"\n");
		for(j=0; i<10; j++)
		{
			fprintf(fm,"%d",final_image[j]);
		}
	//}
	printf("final_image written\n");

	if(fclose(fm) == EOF)
	{
		printf("cannot close final_image.txt\n");
		return -1;
	}
	return 0;


}
