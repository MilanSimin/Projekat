//#include <linux/io.h> //iowrite ioread
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
//#include "lena_hex.h"
#define MAX_PKT_SIZE 256*256

//comment to send pixels as commands via regular write function of char driver
//leave uncommented to write directly to memory (faster)
#define MMAP
int image[1000];
 
int main(void)
{
	int fd,fr;
	int *p, *r;
	int i,x,y;
	int ifs_reg[10];

	for (i = 0; i < 1000; i++)
	  image[i] = i + 1000;

	fd = open("/dev/image_conv", O_RDWR|O_NDELAY);
	if (fd < 0)
	{
		printf("Cannot open /dev/image_conv for write\n");
		return -1;
	}
	p=(int*)mmap(0,MAX_PKT_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
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

	printf("r[0] is %d\n",r[0]);
	printf("r[10] is %d\n",r[10]);
	printf("r[25] is %d\n",r[25]);
	printf("r[35] is %d\n",r[35]);
	printf("r[50] is %d\n",r[50]);

	printf("r[100] is %d\n",r[100]);
	printf("r[150] is %d\n",r[150]);
	printf("r[250] is %d\n",r[250]);
	printf("r[350] is %d\n",r[350]);
	printf("r[500] is %d\n",r[500]);
	printf("r[900] is %d\n",r[900]);

	munmap(p, MAX_PKT_SIZE);
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
