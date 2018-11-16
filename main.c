/*Created: 16.11.2018
Creator: Alexander
last edit: 16.11.2018
last editor: Alexander
*/
 
#define GAMEKINDNAME "Checkers"
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define PORTNUMBER 1357
#define BUF 256
#define INT_MAX 2147483647


#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<"performConnection">



void printHilfe() {
		printf("Fehlende Parameter\nBitte erneut eingeben mit Parameter");
		printf("Parameter -g für GameId -p für SpielerNummer (nur 1 oder 2 moeglich)\n");
}	


int main(int argc,char argv){



char gid[13]; //Game-ID
int player = INT_MAX; //Player-ID

  // Kommandozeilenparameter einlesen
  int ret;
  while ((ret=getopt(argc, argv, "g:p")) != -1) {
    switch (ret) {
      case 'g':
	 strcpy(gid, optarg);
         break;
      case 'p':
         player = optarg;
	if(player == 1 || player == 2)
		break;	
	else
      default:
        printHilfe();
	
	printf("Bitte tätigen sie ihre Eingabe:\n");
        break;	
    }
  }


//Wenn das Programm ohne Argumente gestertet wird startet es erneut und bittet um die Parameter
	if (argc < 2) {
		printf("Fehlende Parameter\nBitte erneut eingeben mit Parameter");
		printf("Parameter -g für GameId -p für SpielerNummer (nur 1 oder 2 moeglich)\n");

	}

	

	// Socket vorbereiten
	struct sockaddr_in address;
	int sock = socket(PF_INET,SOCK_STREAM,0);
	int new_socket;     
	address.sin_family = AF_INET;
	address.sin_port = htons(PORTNUMBER);


	performConnection(address);


    return EXIT_SUCCESS;
}

