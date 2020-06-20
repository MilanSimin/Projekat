#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <iostream>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>        
#include <fcntl.h>           
//sve potrebne biblioteke za rad


using namespace std;




int main(){
	//PRVO KOPIRATI SVE OKO OTVARANJE BRAM-OVA I IFS
	//int *buffer;
	int fd;
	int nizid[4]={0}; 
	int pid;
	char buffer[1024];

	//definisanje socketa i potrebnih promenljivih
	int sockfd, newsockfd, portno, clilen;
	struct sockaddr_in serv_addr, cli_addr;    
	
	/*prvo pozivamo socket() da bi se registrovao socket:
	  AF_INET je neophodan kada se zahteva komunikacija bilo koja dva hosta na internetu;
	  drugi argument definise tip socketa, SOCK_STREAM ili SOCK_DGRAM;
	  treci argument je zapravo protokol koji se koristi, najcesce stavljamo 0 sto znaci da OS sam odabere
	  podrazumevane protokole za dati tip socketa (TCP ZA SOCK_STREAM)
	*/ 
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		perror("ERROR opening socket");
		exit(1);
	} 

	/* Inicijalizacija strukture socket-a */    
	bzero((char *) &serv_addr, sizeof(serv_addr));     
	portno = 5001;     
	serv_addr.sin_family = AF_INET; //mora biti AF_INET    
	/* ip adresa host-a. INADDR_ANY vraca ip adresu masine na kojoj se startovao server */    
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	/* broj porta-ne sme se staviti kao broj vec se mora konvertovati u tzv. network byte order funkcijom htons*/  
  	serv_addr.sin_port = htons(portno);

	/* Sada bind-ujemo adresu sa prethodno kreiranim socket-om */    
	if (bind(sockfd, (struct sockaddr *) &serv_addr,  sizeof(serv_addr)) < 0) 
	{          
		perror("ERROR on binding");          
		exit(1);     
	}
	
	cout<<"Server started.. waiting for clients ...\n"<<endl;
	/* postavi prethodno kreirani socket kao pasivan socket koji ce prihvatati zahteve za konekcijom od klijenata koriscenjem accept funkcije */
	listen(sockfd,1); //maksimalno 1 klijent moze da koristi aplikaciju   
	clilen = sizeof(cli_addr);


	newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
	if(newsockfd < 0)
	{
		perror("ERROR on accept"); 
		exit(1);
	}

	FILE *f;
	int ch = 0;
	f = fopen("image_server.h", "w");
	int words;
	
	read(newsockfd, &words, sizeof(long));

	while (ch != words)
	{
		read(newsockfd, buffer, 255);
		fprintf(f, "%s ", buffer);
		ch++;
	}
	cout<<"receving success\n"<<endl;
	
	close(newsockfd);
	close(sockfd);
	return 0;
}
