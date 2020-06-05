//#include <linux/io.h> //iowrite ioread
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
//#include "lena_hex.h"
#define MAX_PKT_SIZE 65536
#define MAX_IFS_SIZE 16384

//comment to send pixels as commands via regular write function of char driver
//leave uncommented to write directly to memory (faster)

#define MMAP
int image[21440];
int ifs[4];

int main(void)
{
	int fd,fr;
	int *p,*r;
	int i,x,y;
	int lines=55, columns= 23;
	int start=0, ready=0;
	for (i = 0; i < 21440; i++)
		image[i] = i;
	ifs[0] = 22;
	ifs[1] = 34;
	ifs[2] = 0;
	ifs[3] = 0;
	fd = open("/dev/image_conv", O_RDWR|O_NDELAY);
	if (fd < 0)
	{
		printf("Cannot open /dev/image_conv for write\n");
		return -1;
	}
	p=(int*)mmap(0,MAX_IFS_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
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
	memcpy(r, image, MAX_PKT_SIZE);
	memcpy(p, ifs, 4);
	printf("p[0] is %d\n",p[0]);
	printf("p[1] is %d\n",p[1]);
	printf("p[2] is %d\n",p[2]);
	printf("p[3] is %d\n",p[3]);
	/*
	printf("r[10000] is %d\n",r[10000]);
	printf("r[15000] is %d\n",r[15000]);
	printf("r[15500] is %d\n",r[15500]);
	printf("r[16380] is %d\n",r[16380]);
	printf("r[16383] is %d\n",r[16383]);
	printf("r[16384] is %d\n",r[16384]);
	*/
	munmap(p, 4);
	munmap(r, MAX_PKT_SIZE);
	printf ("\nclosing file\n");
	close(fd);
	close(fr);
	if (fd < 0)
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
