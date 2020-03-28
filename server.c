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

int main(int argc, char const *argv[])
{
	int sd, client1, client2, portNumber;
	socklen_t len;
	struct sockaddr_in servAdd;

 	if(argc != 2){
    		printf("Call model: %s <Port #>\n", argv[0]);
    		exit(0);
	}

	if ((sd=socket(AF_INET,SOCK_STREAM,0))<0){
   		fprintf(stderr, "Cannot create socket\n");
		exit(1); 
		}

	servAdd.sin_family = AF_INET; 
	servAdd.sin_addr.s_addr = htonl(INADDR_ANY); 
	sscanf(argv[1], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber); 
	
	bind(sd,(struct sockaddr*)&servAdd,sizeof(servAdd)); 
	listen(sd, 5);

	client1=accept(sd,(struct sockaddr*)NULL,NULL);
    	printf("1st Player Connected\n");

	client2=accept(sd,(struct sockaddr*)NULL,NULL);
	printf("2nd Player Connected\n");    

	return 0;
}
