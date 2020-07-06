#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <ctype.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <ctype.h>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;



void error(const char *msg)
{
    perror(msg);
    exit(0);
}
//************************************************************************
void chooseImage (int select){

	Mat image, newImage;	
	int width = 120, height = 120;
	switch(select){

	case 1: 
		image = imread("lenna.png", CV_LOAD_IMAGE_GRAYSCALE);
		resize(image, newImage, Size(width,height));

		namedWindow("Original image", WINDOW_AUTOSIZE );
		imshow("Original image", newImage );
		waitKey(3000);

		break;
	case 2:
		image = imread("stelvio1.jpeg", CV_LOAD_IMAGE_GRAYSCALE);
		resize(image, newImage, Size(width,height));

		namedWindow("Original image", WINDOW_AUTOSIZE );
		imshow("Original image", newImage );
		waitKey(3000);

		break;

	case 3:
		image = imread("stelvio2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		resize(image, newImage, Size(width,height));

		namedWindow("Original image", WINDOW_AUTOSIZE );
		imshow("Original image", newImage );
		waitKey(0);

		break;

	case 4:
		image = imread("stelvio3.jpeg", CV_LOAD_IMAGE_GRAYSCALE);
		resize(image, newImage, Size(width,height));

		namedWindow("Original image", WINDOW_AUTOSIZE );
		imshow("Original image", newImage );
		waitKey(0);

		break;

	case 5:
		image = imread("stelvio5.jpeg", CV_LOAD_IMAGE_GRAYSCALE);
		resize(image, newImage, Size(width,height));

		namedWindow("Original image", WINDOW_AUTOSIZE );
		imshow("Original image", newImage );
		waitKey(0);

		break;

	default: 
		cerr<<"Invalid selection" << endl;

	break;

	}


	FILE *fp;
	fp =fopen("image.txt","w");
	if (fp==NULL)
	{
		printf("cannot open image.txt\n");
	}
	for (int x=0; x<newImage.cols; x++) {
		for (int y=0; y<newImage.rows; y++) {
			fprintf(fp,"%d ", newImage.at<uchar>(x,y));
		}
		fprintf(fp,"\n");
	}

	if(fclose(fp) == EOF)
	{
		printf("cannot close final_image.txt\n");
	}
}
//***************************************************************************

int getLines()
{
	FILE* f;
	int lines = 0;
	int columns = 0;
	char i;
	f = fopen("image.txt", "r");

	
	while((i=fgetc(f))!=EOF)
	{
		if (i == '\n')
		{
			lines++;
		}
	}
	fclose(f);
	return lines;
}
int getColumns()
{
	FILE* f;
	int lines = 0;
	int columns = 0;
	char i;
	f = fopen("image.txt", "r");

	while((i=fgetc(f))!='\n')
	{
		if (i == ' ')
		{
			columns++;
		}
	}
	
	fclose(f);
	return columns;
}

//***********************************************************************************************************

int main(int argc, char *argv[])
{
	int selectImage, selectKernel;
	int temp[120*120], temp2[120*120];
	
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	if (argc < 3)
	{
		fprintf(stderr,"usage %s hostname port\n", argv[0]);
		exit(0);
	}


	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
		server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
		error("ERROR connecting");


//*************************************************CREATING IMAGE.TXT*************************************************
	cout << "Izaberite sliku za obradu:\n 1. lenna.png \n 2. stelvio1.jpeg\n 3. stelvio2.jpeg\n 4. stelvio3.jpeg\n 5. stelvio5.jpeg\n "<< endl;
	cin >> selectImage;
	chooseImage(selectImage);

//**************************************************SELECTING KERNEL AND SENDING*******************************************
	cout<<"Izaberite tip obrade slike:\n 1. Identity Operator \n2. Edge detection \n3. Sharpening \n"<<endl;
	cin >> selectKernel;

	int kernel1[9], kernel2[9], kernel3[9];
	
	switch(selectKernel){

	case 1:
		kernel1[0] = 0;
		kernel1[1] = 0;
		kernel1[2] = 0;
		kernel1[3] = 0;
		kernel1[4] = 1;
		kernel1[5] = 0;
		kernel1[6] = 0;
		kernel1[7] = 0;
		kernel1[8] = 0;
		write(sockfd, kernel1, sizeof(int)*9);	
		break;

	case 2:
		kernel2[0] = -1;
		kernel2[1] = -1;
		kernel2[2] = -1;
		kernel2[3] = -1;
		kernel2[4] = 8;
		kernel2[5] = -1;
		kernel2[6] = -1;
		kernel2[7] = -1;
		kernel2[8] = -1;
		write(sockfd, kernel2, sizeof(int)*9);
		break;

	case 3:
		kernel3[0] = 0;
		kernel3[1] = -1;
		kernel3[2] = 0;
		kernel3[3] = -1;
		kernel3[4] = 5;
		kernel3[5] = -1;
		kernel3[6] = 0;
		kernel3[7] = -1;
		kernel3[8] = 0;
		write(sockfd, kernel3, sizeof(int)*9);
		break;

	default:
		cerr<<"Invalid selection" << endl;

	break;

	}
//**************************************************SENDING IMAGE TO SERVER*************************************************
	cout<<"sending image to server"<<endl;
	FILE *f;
	int words = lines*columns, k=0;
	char c, ch;
	f=fopen("image.txt","r");
	while((c=fgetc(f))!=EOF)			
	{	
		fscanf(f, "%d", temp);
		if(isspace(c) || c=='\n')
		words++;	
	}
	printf("Words = %d \n", words);	
	write(sockfd, &words, sizeof(int));
     	rewind(f);
	while(ch != EOF)
	{	
		fscanf(f , "%d" , &temp2[k]);		
		ch= fgetc(f);		
		k++;	
	}
	
	write(sockfd,temp2,sizeof(int)*words);
	if(fclose(f) == EOF)
	{
		printf("cannot close final_image.txt\n");
	}
	printf("The file was sent successfully to server\n");
//************************************************ Sending lines and columns ********************************************
	cout<<"lines and columns sent to server"<<endl;
	int lines,columns;
	lines = getLines();
	columns = getColumns();

	//cout<<"lines is "<<lines<<endl;
	write(sockfd, &lines, sizeof(int));

	//cout<<"columns is "<<columns<<endl;	
	write(sockfd, &columns, sizeof(int));


//*********************************************	RECEIVING IMAGE FROM SERVER*************************************************

	cout<<"receving image from server\n"<<endl;
	FILE *fs;
        int h = 0, num = 0;
	read(sockfd, &num, sizeof(int));
	fs = fopen("final_image.txt","w");
	if (fs==NULL)
	{
		printf("cannot open final_image.txt\n");
		return -1;
	}
	int pom[num];
	bzero(pom,num);
	char *buff = (char *)pom;
	size_t rem = sizeof(int)*num;

	while(rem){
		ssize_t recvd = read(sockfd,buff,rem);
		rem -= recvd;
		buff += recvd;
	}
	while(h != num)
       	{
	   	fprintf(fs, "%d ", pom[h]);
		h++;
		if((h%(lines-1)) == 0){
			fprintf(fs,"\n");
		}
	}
	printf("The new file created is final_image.txt\n");
	
	if(fclose(fs) == EOF)
	{
		printf("cannot close final_image.txt\n");
	}
	
	close(sockfd);
	Mat final_picture((columns-1),(lines-1),CV_8U);

	for(int x=0; x<columns-1;x++){
		for(int y=0; y<lines-1; y++){
			final_picture.at<uchar>(x,y) = pom[y+x*(columns-1)];
		}
	}
	


	namedWindow("Final image", WINDOW_AUTOSIZE );
	imshow("Final image", final_picture );
	waitKey(3000);
	

	return 0;
	
}



