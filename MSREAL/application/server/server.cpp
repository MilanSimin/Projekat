#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <iostream>

#define MAX_BRAM_SIZE 57600
#define MAX_IFS_SIZE 16384
#define MAX_KERNEL_SIZE 4096
#define IFS_SEND 16
#define KERNEL_SEND 36
#define MMAP

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

string sock_read(int sockfd){
	const int MAX =256;
	char buff[MAX];
	bzero(buff, MAX);

	read(sockfd, buff, sizeof(buff));
	string message(buff);

	return message;

}

int main(int argc, char *argv[])
{
//**********************************client/server creating socket and connecting***************************************************
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
		error("ERROR on binding");
	cout<< "Waiting for client ..."<<endl;
	listen(sockfd,1);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0) 
		error("ERROR on accept");

	int lines,columns,start=1;
	int kernel[9], i =0, j=0;
	int *final_image;
	int fk, fb, fc, fr;
	int *k, *b, *c, *r;
	string command;

while(command !="q\n") {
	command = sock_read(newsockfd);

	//if(command=="q\n"){
	//	return 0;
	//} else if (command == "y" ){
	cout<<"command is: "<<command<<endl;
//**********************************READING KERNEL AND SENDING TO BRAM_KERNEL***********************************************
	read(newsockfd, &kernel, sizeof(int)*9);
	cout<<"Read kernel"<<endl;
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
	cout<<"kernel sent to bram_kernel"<<endl;

//****************************************READING IMAGE AND SENDING TO BRAM_IMAGE*************************************************
	FILE *fp;
        int ch = 0, words;
	int image[MAX_BRAM_SIZE];
	bzero(image,MAX_BRAM_SIZE);
        fp = fopen("image.txt","w");
	read(newsockfd, &words, sizeof(int));
	cout<<"Reading image"<<endl;
	char *buff = (char *)image;
	size_t rem = sizeof(int)*words;

	while(rem){
		ssize_t recvd = read(newsockfd,buff,rem);
		rem -= recvd;
		buff += recvd;
	}

	cout<<"while loop"<<endl;


	while(ch != words)
       	{
	   	fprintf(fp, "%d, ", image[ch]);
		ch++;
		if((ch%120) == 0){
			fprintf(fp,"\n");
		}
	}
	printf("The new file created is image.txt\n");

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
	memcpy(r, image, MAX_BRAM_SIZE);
	munmap(r, MAX_BRAM_SIZE);
	printf("bram_image done\n");
	close(fr);
	if(fr < 0)
	{
		printf("cannot close /dev/bram_image for write\n");
		return -1;
	}
	cout<<"image sent to bram_image"<<endl;

//****************************************READING LINES AND COLUMNS AND SENDING TO IMAGE_CONV*************************************
	read(newsockfd, &lines, sizeof(int));
	read(newsockfd, &columns, sizeof(int));
	cout<<"lines and columns read"<<endl;
	cout<<"lines is "<<lines<<endl;
	cout<<"columns is "<<columns<<endl;
	int ifs_reg[4]={columns,lines,start,0};

	fc = open("/dev/image_conv", O_RDWR|O_NDELAY);
	if (c < 0)
	{
		printf("Cannot open /dev/image_conv for write\n");
		return -1;
	}
	c=(int*)mmap(0, MAX_IFS_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fc, 0);
	if (c == NULL ) {
		printf ("\ncouldn't mmap\n");
		return 0;
	}

	memcpy(c, ifs_reg, IFS_SEND);
	munmap(c, IFS_SEND);
	printf("image_conv done\n");
	close(fc);
	if (fc < 0)
	{
		printf("Cannot close /dev/image_conv for write\n");
		return -1;
	}
	cout<<"data sent to image_conv" <<endl;
//**************************************************READING FROM BRAM_AFTER_CONV***************************************************
	cout<<"reading image from bram_after_conv"<<endl;
	fb = open("/dev/bram_after_conv", O_RDWR|O_NDELAY);
	if (fb < 0)
	{
		printf("Cannot open /dev/bram_after_conv for write\n");
		return -1;
	}
	final_image = (int *) malloc(MAX_BRAM_SIZE);
	b=(int*)mmap(0,MAX_BRAM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
	if (b == NULL ) {
		printf ("\ncouldn't mmap\n");
		return 0;
	}
	
	memcpy(final_image, b, MAX_BRAM_SIZE);
	munmap(b, MAX_BRAM_SIZE);
	printf("bram_after_conv done\n");
	close(fb);
	if(fb < 0)
	{
		printf("cannot close /dev/bram_after_conv for write\n");
		return -1;
	}
	cout<<"image read from bram to final_image"<<endl;

	FILE *fm;
	fm =fopen("final_image.txt","w");
	if (fm==NULL)
	{
		printf("cannot open final_image.txt\n");
		return -1;
	}
	printf("final_image opened\n");
	for(i = 0; i<lines-1; i++)//lines-1
	{
		for(j=0; j<columns-1; j++)//columns-1 
		{
			fprintf(fm,"%d ",final_image[j+i*118]);
			fflush(fm);
		}
		fprintf(fm,"\n");
	}
	fprintf(fm,"\n");
	printf("final_image written\n");

	if(fclose(fm) == EOF)
	{
		printf("cannot close final_image.txt\n");
		return -1;
	}

//**************************************************SENDING FILE TO CLIENT**********************************************************
	cout<<"sending final_image.txt to client"<<endl;	
	FILE *f;
	int num = 0, x=0;
	int pom[120*120], pom2[120*120];
	int  h;
	f=fopen("final_image.txt","r");
	if(f==NULL){
		cout<<"cannot open final_image.txt"<<endl;
		return -1;
	}
	while(h!=EOF)
	{
		fscanf(f, "%d", pom);
		num++;
		h=fgetc(f);	
	}
	write(newsockfd, &num, sizeof(int));
     	rewind(f);
	while(x != num)
	{
		fscanf(f , "%d" , &pom2[x]);
		x++;
	}
	write(newsockfd,pom2,sizeof(int)*num);
	printf("The file was sent successfully\n");
	
	ifs_reg[0]=0;
	ifs_reg[1]=0;
	ifs_reg[2]=0;
	ifs_reg[3]=0;

	fc = open("/dev/image_conv", O_RDWR|O_NDELAY);
	if (c < 0)
	{
		printf("Cannot open /dev/image_conv for write\n");
		return -1;
	}
	c=(int*)mmap(0, MAX_IFS_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fc, 0);
	if (c == NULL ) {
		printf ("\ncouldn't mmap\n");
		return 0;
	}
	
	memcpy(c, ifs_reg, IFS_SEND);
	munmap(c, IFS_SEND);
	printf("image_conv done\n");
	close(fc);
	if (fc < 0)
	{
		printf("Cannot close /dev/image_conv for write\n");
		return -1;
	}
//	} 
}

	close(newsockfd);
	close(sockfd);
	return 0; 
}
