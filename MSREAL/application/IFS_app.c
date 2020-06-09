#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define MAX_PKT_SIZE 65536
#define MAX_IFS_SIZE 16384

//comment to send pixels as commands via regular write function of char driver
//leave uncommented to write directly to memory (faster)

#define MMAP
int image[21440];
int ifs[4];
size_t num_of_bytes = 1000;

int main(void)
{
	FILE *fp;
	int fd,fr;
	int *p,*r;
	int i,x,y,number;
	char *my_line;
	for (i = 0; i < 21440; i++)
		image[i] = i;

	fp = fopen ("/root/Projekat/MSREAL/application/image.txt","r");
	if (fp == NULL)
	{
		printf("Cannot open image.txt\n");
		return -1;
	}
	my_line = (char*)malloc(num_of_bytes +1);
	getline(&my_line,&num_of_bytes,fp);

	/*fd = open("/dev/image_conv", O_RDWR|O_NDELAY);
	if (fd < 0)
	{
		printf("Cannot open /dev/image_conv for write\n");
		return -1;
	}
	p=(int*)mmap(0,4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
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
	memcpy(p, image, 4);*/
	printf("my_line[0] is %d\n",my_line[0]);
	printf("my_line[1] is %d\n",my_line[1]);
	printf("my_line[2] is %d\n",my_line[2]);
	printf("my_line[3] is %d\n",my_line[3]);
	printf("my_line[4] is %d\n",my_line[4]);
	printf("my_line[5] is %d\n",my_line[5]);
	printf("my_line[6] is %d\n",my_line[6]);
	printf("my_line[7] is %d\n",my_line[7]);
	printf("my_line[8] is %d\n",my_line[8]);
	printf("my_line[9] is %d\n",my_line[9]);
	printf("my_line[10] is %d\n",my_line[10]);
	printf("my_line[11] is %d\n",my_line[11]);
	printf("my_line[12] is %d\n",my_line[12]);
	printf("my_line[13] is %d\n",my_line[13]);
	printf("my_line[14] is %d\n",my_line[14]);
	printf("my_line[15] is %d\n",my_line[15]);
	
	printf("my_line[800] is %d\n",my_line[800]);
	printf("my_line[910] is %d\n",my_line[910]);
	printf("my_line[920] is %d\n",my_line[920]);
	printf("my_line[1000] is %d\n",my_line[1000]);
	
	/*
	printf("r[10000] is %d\n",r[10000]);
	printf("r[15000] is %d\n",r[15000]);
	printf("r[15500] is %d\n",r[15500]);
	printf("r[16380] is %d\n",r[16380]);
	printf("r[16383] is %d\n",r[16383]);
	printf("r[16384] is %d\n",r[16384]);
	*/
	/*munmap(p, 4);
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

	}*/
	if(fclose(fp))
	{
		printf("Cannot close image.txt  for read\n");
		return -1;
	}
	return 0;
}
