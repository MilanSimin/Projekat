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


int main(int argc, char **argv)
{
	FILE *fp;
	Mat image, newImage;
	int kernel[9];
	int selectImage, selectKernel;
	int width = 120, height = 120;
	if( argc != 2)
	{
		cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
		return -1;
	}

	

	int sockfd=0, n=0;
	char recvBuff[1024];
	struct sockaddr_in serv_addr;
	char ans;
	/* klijentska aplikacija se poziva sa ./ime_aplikacija ip_adresa_servera slika za obradu */
	/* kreiraj socket za komunikaciju sa serverom */
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Error : Could not create socket \n");
		return 1;
	}
	memset(&serv_addr, 0, sizeof(serv_addr)); 

	/*podaci neophodi za komunikaciju sa serverom*/
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5001);

	/* inet_pton konvertuje ip adresu iz stringa u format
	neophodan za serv_addr strukturu */
	if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
	{ 
		printf("\n inet_pton error occured\n");
		return 1;
	} 

	/* povezi se sa serverom definisanim preko ip adrese i porta */
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\n Error : Connect Failed \n");
		return 1;
	}

	n = read (sockfd, recvBuff, 1024);
	recvBuff[n] = 0; //terminiraj primljeni string kako bi ga mogao ispisati
	cout << "Izaberite sliku za obradu:\n 1. lenna.png \n 2. stelvio3.jpeg\n 3. stelvio5.jpeg\n" << endl;
	cin >> selectImage;

	switch(selectImage){
	
	case 1:
		image = imread("lenna.png", CV_LOAD_IMAGE_GRAYSCALE);
		resize(image, newImage, Size(width,height));

		namedWindow("Original image", WINDOW_AUTOSIZE );
		imshow("Original image", newImage );
		waitKey(0);

		break;

	case 2:
		image = imread("stelvio3.jpeg", CV_LOAD_IMAGE_GRAYSCALE);
		resize(image, newImage, Size(width,height));

		namedWindow("Original image", WINDOW_AUTOSIZE );
		imshow("Original image", newImage );
		waitKey(0);
		
		break;
	case 3: 

		image = imread("stelvio5.jpeg", CV_LOAD_IMAGE_GRAYSCALE);
		resize(image, newImage, Size(width,height));

		namedWindow("Original image", WINDOW_AUTOSIZE );
		imshow("Original image", newImage );
		waitKey(0);
		
		break;

	default:
		cerr << "Invalid selection" << endl;
	
	break;

	}

	fp =fopen("image.h","w");
	if (fp==NULL)
	{
		printf("cannot open image.txt\n");
		return -1;
	}
	fprintf(fp,"int image[] = { ");
	for (int x=0; x<newImage.cols; x++) {
		for (int y=0; y<newImage.rows; y++) {
			fprintf(fp,"%d,", newImage.at<uchar>(x,y));
		}
		fprintf(fp,"\n");
	}
	fprintf(fp,"};");

	if(fclose(fp) == EOF)
	{
		printf("cannot close final_image.txt\n");
		return -1;
	}
	cout<<"Izaberite tip obrade slike:\n 1. Identity Operator \n2. Edge detection \n3. Sharpening \n"<<endl;
	cin >> selectKernel;

	switch(selectKernel){
	
	case 1:
		kernel[9]={0,0,0,0,1,0,0,0,0};
		break;

	case 2:
		kernel[9]={-1,-1,-1,-1,8,-1,-1,-1,-1};
		break;
	case 3: 

		kernel[9]={0, -1, 0,-1, 5, -1, 0, -1, 0};
		break;

	default:
		cerr << "Invalid selection" << endl;
	
	break;

	}
	
	cout<<"Ako zelite da prekinete komunikaciju sa serverom ukucajte 'Q'\n"<<endl;

	while(1)
	{
		ans = getchar();
		write (sockfd, &ans,1);

	if (ans == 'q') 
	{
		return 0;
	}



	}
	return 0;
        
}
