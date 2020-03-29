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

	char message[255], myScore[255];
	int server, portNumber, score;
	socklen_t len;
	struct sockaddr_in servAdd;

 	if(argc != 4){
  		printf("Call model:%s <IP> <Port#> <Player Name>\n",argv[0]);
  		exit(0);
	}

	//creation of socket sd
	if((server = socket(AF_INET, SOCK_STREAM, 0))<0){
  		fprintf(stderr, "Cannot create socket\n");
		exit(1);
	}

	//setting IP address, port number and other details
	servAdd.sin_family = AF_INET;
	sscanf(argv[2], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber);

	//representing
	if(inet_pton(AF_INET,argv[1],&servAdd.sin_addr) < 0){
		fprintf(stderr, " inet_pton() has failed\n");
		exit(2);
	}

	//Connecting to ther server
	if(connect(server,(struct sockaddr *)&servAdd,sizeof(servAdd)) < 0){
 		fprintf(stderr, "connect() has failed, exiting\n");
		exit(3);
	}

	printf("%s - Player Connected to Server!!\n", argv[3]);

	//sending player name to server
	write(server, argv[3], strlen(argv[3])+1);

	while(1){

		printf("\n-----------------------------------------------\n");

		//reading message from server
		if(read(server, message, 255) < 0) {
   			fprintf(stderr, "read() error\n");
			exit(3);
		}

		//Player can roll the dice
		if((strcmp(message,"\nYou can now play\n") == 0)){

				printf("%s",message);

				//generation of random numbers
				score = (int)time(NULL)%6 + 1;

				//Converting score from int to char[]
				sprintf(myScore, "%d", score);
				printf("%s - Obtained Score: %s\n",argv[3],myScore);
				strcpy(message,myScore);

				//sending score to server
				write(server, message, strlen(message)+1);
		}

		//player wins the game
		if((strcmp(message,"\nGame over: you won the game\n") == 0)){

			printf("\nI won the game\n\n");
			close(server);
			exit(0);
		}

		//player lost the game
		if((strcmp(message,"\nGame over: you lost the game\n") == 0)){

			printf("\nI lost the game\n\n");
			close(server);
			exit(0);
		}
	}
}
