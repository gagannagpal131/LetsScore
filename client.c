#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[]) {	
	int server, portNumber; socklen_t len;
	struct sockaddr_in servAdd;

 	if(argc != 3){
  		printf("Call model:%s <IP> <Port#>\n",argv[0]);
  		exit(0);
	}

	if((server = socket(AF_INET, SOCK_STREAM, 0))<0){
  		fprintf(stderr, "Cannot create socket\n");
		exit(1); 
	}

	servAdd.sin_family = AF_INET;
	sscanf(argv[2], "%d", &portNumber); 
	servAdd.sin_port = htons((uint16_t)portNumber);

	if(inet_pton(AF_INET,argv[1],&servAdd.sin_addr) < 0){ 
		fprintf(stderr, " inet_pton() has failed\n"); 
		exit(2);
	}

	if(connect(server,(struct sockaddr *)&servAdd,sizeof(servAdd)) < 0){
 		fprintf(stderr, "connect() has failed, exiting\n");
		exit(3);
	}

	printf("Player Connected to Server!!\n");


}

