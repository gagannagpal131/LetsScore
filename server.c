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

void servicePlayers(int,int);
void checkScore(int[],int,int,char[],char[]);

int main(int argc, char const *argv[])
{
	int sd, client1, client2, portNumber, pid;
	socklen_t len;
	struct sockaddr_in servAdd;

 	if(argc != 2){
    		printf("Call model: %s <Port #>\n", argv[0]);
    		exit(0);
	}

	//creation of socket sd
	if ((sd=socket(AF_INET,SOCK_STREAM,0)) < 0) {
   		fprintf(stderr, "Cannot create socket\n");
		exit(1);
	}

	//setting IP address, port number and other details
	servAdd.sin_family = AF_INET;
	servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
	sscanf(argv[1], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber);

	//binding servAdd & sd
	bind(sd,(struct sockaddr*)&servAdd,sizeof(servAdd));
	listen(sd, 5);

	printf("\nWaiting for players!\n");

	while(1){

		//waiting for player 1
		client1=accept(sd,(struct sockaddr*)NULL,NULL);
    		printf("Player Connected\n");

		//waiting for player 2
		client2=accept(sd,(struct sockaddr*)NULL,NULL);
		printf("Player Connected\n");

		printf("\nTwo players connected, Let the game begin\n");

		//forking for child process
		pid = fork();
		if(pid == 0)
			servicePlayers(client1, client2);

		//closing both clients in parent process
		close(client1);
		close(client2);
	}

	return 0;
}

/*
Function that will be called in child process.
Function will act as a referee for both players.
Socket descriptors for both clients are passed as parameters.
*/
void servicePlayers(int client1, int client2){

	char message[255], player1[100], player2[100];
	int totalScore[2], obtainedScore;

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

	//setting initial values of score to 0
	for(int i = 0; i < 2; i++)
		totalScore[i] = 0;

	while(1){

		printf("\n-----------------------------------------------\n\n");

		//sending message to player 1
		strcpy(message,"You can now play");
		write(client1, message, strlen(message)+1);

		//reading score from player 1
		if(!read(client1, message, 255)){
    			close(client1);
    			fprintf(stderr,"Unable to receive data from client\n");
			exit(0);
		}

		//conversion of score from char[] to int
		sscanf(message,"%d",&obtainedScore);
		totalScore[0] += obtainedScore;
		printf("%s - Total Score = %d\n",player1,totalScore[0]);
		sleep(1);

		//sending message to player 2
		strcpy(message, "You can now play");
		write(client2, message, strlen(message)+1);

		//reading score from player 2
		if(!read(client2, message, 255)){
    			close(client2);
    			fprintf(stderr,"Unable to receive data from client\n");
			exit(0);
		}

		//conversion of score from char[] to int
		sscanf(message,"%d",&obtainedScore);
		totalScore[1] += obtainedScore;
		printf("%s - Total Score = %d\n",player2,totalScore[1]);
		sleep(1);

		//checking if any player has won the game
		checkScore(totalScore, client1, client2, player1, player2);
	}
}

/*
Function to check score of both players and check the winners.
Total score of both players and file descriptors of both clients are passed as parameters.
*/
void checkScore(int totalScore[], int client1, int client2, char player1[], char player2[]){

	char message[255];
	int maxScore = 100;

	//Player 1 Wins
	if (totalScore[0] >= maxScore){

		strcpy(message, "Game over: you won the game");
		write(client1, message, strlen(message)+1);

		strcpy(message, "Game over: you lost the game");
		write(client2, message, strlen(message)+1);

		close(client1);
		close(client2);

		printf("\n%s won the game!\n",player1);
		printf("\nWaiting for 2 new players to Connect!\n");
		exit(0);
	}

	//Player 2 wins
	if(totalScore[1] >= maxScore){

		strcpy(message, "Game over: you won the game");
		write(client2, message, strlen(message)+1);

		strcpy(message, "Game over: you lost the game");
		write(client1, message, strlen(message)+1);

		close(client1);
		close(client2);

		printf("\n%s won the game!\n",player2);
		printf("\nWaiting for 2 new players to Connect!\n");
		exit(0);
	}
}
