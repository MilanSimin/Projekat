//#include <linux/io.h> //iowrite ioread
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
//#include "lena_hex.h"
#define MAX_PKT_SIZE 400

//comment to send pixels as commands via regular write function of char driver
//leave uncommented to write directly to memory (faster)
#define MMAP
int image[100];
 
int main(void)
{
	int fd;
	int *p;
	int i,x,y;
	int ifs_reg[10];
	
	for (i = 0; i < 100; i++)
	  image[i] = i + 100;

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
	printf("p[0] is %d\n", p[0]);	
	printf("p[1] is %d\n", p[1]);
	printf("p[2] is %d\n", p[2]);
	printf("p[3] is %d\n", p[3]);
	//printf("p[0] is %ld", p[0]);
	printf("p is: %x",ioread(p));
	//p[0] = 0xf;
	munmap(p, MAX_PKT_SIZE);

	printf ("\nclosing file\n");
	close(fd);
	if (fd < 0)
	{
		printf("Cannot close /dev/image_conv for write\n");
		return -1;
	}
	return 0;
}
