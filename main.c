//#define GAMEKINDNAME "Checkers"
//#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
//#define PORTNUMBER 1357

//Bibliotheken einbinden
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

//weitere Programmteile einbinden
#include "performConnection.c"
#include "conf.c"
#include "communication.c"


// struct Spieldaten {
//   char gameName[BUF];
//   int playerNumber;
//   int playerCount;
//   pid_t thinker;
//   pid_t connector;
//   char field[8][8];
//
// } spieldaten;

void error(char message[BUF])
{
    printf("Error: %s\n", message);
    exit(EXIT_FAILURE);
}


void printHilfe(){
		printf("Programmaufruf nur mit: -g <gameId> -p <{1,2}>\n");
}


int main(int argc,char** argv){



  //struct shm shm;

  char gid[14]; //Game-ID
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
        if(pFlag == 0 && (atoi(optarg) == 2 || atoi(optarg) == 1)){
          if(atoi(optarg) == 1) {
            strcpy(player, "0");
          } else {
            strcpy(player, "1");
          }
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

  //config Datei auslesen
  struct Configuration configStruct = setConfig(config);

  //create the shared memory
  int shmID;
  int shmSize = 2*sizeof(int)+BUF+2*sizeof(pid_t)+160;
  shmID = shmget(6543, shmSize ,IPC_CREAT | 0666);

  //attach shared memory to processes
  struct Spieldaten *spieldaten;
  spieldaten = (struct Spieldaten * ) shmat(shmID, NULL, 0);


  //test
  // spieldaten.playerNumber =  atoi(player);
  // printf("Player im SHM: %i\n", spieldaten.playerNumber);
  // printf("SHM-ID: %i\n", shmID);

  //Erstellen der Pipe
	int fd[2];
	if (pipe(fd) < 0) {
		error("Fehler beim Erstellen der pipe");
	}

	pid_t pid = fork();
	if (pid<0) {
		error("Fehler beim Gabeln der Prozesse");
	} else
	if (pid==0) {
		//Hier beginnt der Connector = Kindprozess
                                    //shmData = shmat(getpid(), NULL, 0);
    //spieldaten.connector = getpid();

		//Schließen der Schreibseite
		close(fd[1]);
    //create a socket and connect to server
    int *sock;
    sock = (int *)malloc(4*sizeof(int));
    *sock = socket(AF_INET, SOCK_STREAM, 0);
    connectToServer(sock, configStruct.hostname, configStruct.portnumber);
    performConnection(gid, player, configStruct.gamekind, sock);

    communication(sock, fd[0]);


    //close connection
    close(*sock);

    close(fd[0]);
  } else {
    //Hier beginnt der Thinker = ElternProzess
    //spieldaten.thinker = getpid();

    //Schließen der Leseseite
    close(fd[0]);



    char message[BUF];
    strcpy(message, "B6:C5\n");
    write(fd[1], message, strlen(message) + 1);

    wait(NULL);

    //ab hier Test SHM
    //struct Spieldaten *spieldaten;

    //spieldaten = (struct Spieldaten *) shmat(shmID, NULL, 0);

    for(int i = 7; i >= 0; i--){
      for(int j = 0; j <= 7; j++){
        printf("%c ", spieldaten -> field[j][i]);
      }
      printf("\n");
    }
    //bis hier Test SHM


    //think();
    close(fd[1]);

  }


  return EXIT_SUCCESS;
}
