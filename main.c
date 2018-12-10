#define GAMEKINDNAME "Checkers"
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define PORTNUMBER 1357
#define BUF 256
#define INT_MAX 2147483647

//Bibliotheken einbinden
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>

//weitere Programmteile einbinden
#include "performConnection.c"



void printHilfe(){
		printf("Programmaufruf nur mit: -g <gameId> -p <{0,1}>\n");
}


int main(int argc,char** argv){



  char gid[13]; //Game-ID
  char player[2];//Player-ID
  char config[64] = "client.conf";

  // Kommandozeilenparameter einlesen
  int ret;
  int gFlag = 0;
  int pFlag = 0;
  while ((ret=getopt(argc, argv, "g:p:c:")) != -1) {
    switch (ret) {
      case 'g':
        if(gFlag == 0){
          strcpy(gid, optarg);
          gFlag++;
        } else {
          printHilfe();
          return -1;
        }
        break;
      case 'p':
        if(pFlag == 0 && (atoi(optarg) == 1 || atoi(optarg) == 0)){
          strcpy(player, optarg);
          pFlag++;
        } else {
          printHilfe();
          return -1;
        }
        break;
      case 'c':
        strcpy(config, optarg);
        break;
      default:
        printHilfe();
	return -1;
    }
  }
  if (pFlag == 0 || gFlag == 0){
    printHilfe();
    return -1;
  }

  //create a socket and connect to server
  int *sock;
  sock = (int *)malloc(4*sizeof(int));
  *sock = socket(AF_INET, SOCK_STREAM, 0);
  connectToServer(sock);
  printf("gameID %s\nSpieler %s\n", gid, player);
  performConnection(gid, player, sock);

  //close connection
  close(*sock);

  //Erstellen der Pipe
	int fd[2];
	if (pipe(fd) < 0) {
		printf("Fehler beim Erstellen der pipe\n");
		exit(EXIT_FAILURE);
	}

	pid_t pid = fork();
	if (pid<0) {
		printf("Fehler beim Gabeln der Prozesse\n");
		exit(EXIT_FAILURE);
	} else
	if (pid==0) {
		//Hier beginnt der Connector = Kindprozess

		//Schließen der Schreibseite
		close(fd[1]);
		//printf("Hallo ich bin der Kindprozess, der Connector\n");
    

    close(fd[0]);
  } else {
    //Hier beginnt der Thinker = ElternProzess

    //Schließen der Leseseite
    close(fd[0]);
    //printf("Hallo ich bin der ElternProzess, der Thinker\n");
    //think();
    close(fd[1]);

  }


  return EXIT_SUCCESS;
}
