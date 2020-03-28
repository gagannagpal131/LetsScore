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

void servicePlayers(int, int);

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

	servicePlayers(client1, client2);
 
	return 0;
}

void servicePlayers(int client1, int client2){

	char message[255], player1[100], player2[100];
	int score[2], obtainedScore;

	//reading the name of player 1
	if(!read(client1, player1, 100)){
    		close(client1);
    		fprintf(stderr,"Unable to receive data from client\n");
		exit(0); 
	}

	//reading the name of player 2
	if(!read(client2, player2, 100)){
    		close(client1);
    		fprintf(stderr,"Unable to receive data from client\n");
		exit(0); 
	}

	//sending message to player 1
	strcpy(message, "Referee - You can now play!\n");
	write(client1, message, strlen(message)+1);

	//reading score from player 1
	if(!read(client1, message, 255)){
    		close(client1);
    		fprintf(stderr,"Unable to receive data from client\n");
		exit(0); 
		}
	
	//conversion of score from char[] to int
	sscanf(message,"%d",&obtainedScore);
	printf("%s - Score obtained:%d\n",player1,obtainedScore);	

	sleep(5);

	//sending message to player 2
	strcpy(message, "Referee: You can now play!\n");
	write(client2, message, strlen(message)+1);

	//reading score from player 2
	if(!read(client2, message, 255)){
    		close(client2);
    		fprintf(stderr,"Unable to receive data from client\n");
		exit(0); 
	}
	
	//conversion of score from char[] to int
	sscanf(message,"%d",&obtainedScore);
	printf("%s - Score obtained:%d\n",player2,obtainedScore);	

}
