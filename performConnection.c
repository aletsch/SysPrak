#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "main.h"
//#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de\n"
//#define PORTNUMBER 1357

char* getLine(char* line){
  char * curLine = line;
  char * nextLine = strchr(line, '\n');
  if (nextLine) *nextLine = '\0';  // temporarily terminate the current line
  return curLine;
}


int hostname_to_ip(char* host, char *ip){
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_in *h;
  int rv;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if((rv = getaddrinfo(host, "http", &hints, &servinfo))){
    printf("could not resolve addressinfo\n");
    //Kill signal schicken + shm freigeben
    struct Spieldaten *spieldaten;
    spieldaten = (struct Spieldaten *) shmat(shmID, NULL, 0);
    kill(spieldaten -> thinker, SIGTERM);
    shmdt(spieldaten);
    return -1;
  }

  //take the first possible result
  for(p = servinfo; p != NULL; p = p->ai_next){
    h = (struct sockaddr_in *) p->ai_addr;
    strcpy(ip, inet_ntoa(h->sin_addr));
  }

  freeaddrinfo(servinfo);
  return 0;
}

int readServer(int *socket, char *buffer){

  struct Spieldaten *spieldaten;
  spieldaten = (struct Spieldaten *) shmat(shmID, NULL, 0);

  memset(buffer, 0, BUF);

  if(read(*socket, buffer, BUF) != 0){
    switch (*buffer){
      case '+':
        printf("Server: %s", buffer);
        return 0;
        break;
      case '-':
        printf("Ungültige Serveranfrage: %s", buffer);
        printf("PID thinker %i\nPID connector %i\n", spieldaten->thinker, spieldaten->connector);
        //kill(spieldaten -> thinker, SIGTERM);
        shmdt(spieldaten);
        return -1;
      default:
        printf("%s\n", buffer);
        printf("Unerwartete Antwort des Servers\n");
        shmdt(spieldaten);
        //kill(spieldaten -> thinker, SIGTERM);
        return -1;
    }
  } else {
    perror("Fehler beim Lesen vom Server: ");
    //kill(spieldaten -> thinker, SIGTERM);
    shmdt(spieldaten);
    return -1;
  }
}

int writeServer(int *socket, char *buffer, char message[BUF]){

  
  memset(buffer, 0, BUF);
  strcpy(buffer, message);

  if(write(*socket, buffer, strlen(buffer)) != 0){
    //printf("Client: %s", buffer);
    return 0;
  }
  return -1;
}

int connectToServer(int* sock, char* host, int port){
  //get ip
  char *ip = malloc(60*sizeof(char));
  char *hostname = host;
  //failed to load IP
  if (hostname_to_ip(hostname, ip) == -1){
    free(ip);
    return -1;
  }



  // address structure to connect to
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);
  server_address.sin_addr.s_addr = inet_addr(ip);

  int connection_status = connect(*sock, (struct sockaddr *) &server_address, sizeof(server_address));
  if (connection_status != 0){
    perror("Error: ");
    free(ip);
    return -1;
  }

  free(ip);
  return 0;
}


int performConnection(char* gameID, char* player, char* gamekind, int* sock){

  //Kommunikation mit Server
  char buffer[BUF] = "";

  //get Server version
  if(readServer(sock, buffer) == -1)
    return -1;

  //send Client Version
  if(writeServer(sock, buffer, "VERSION 2.0\n") == -1)
    return -1;


  //Version accepted, request gameID
  if(readServer(sock, buffer) == -1)
    return -1;

  //send gameID
  char* temp = malloc(BUF);
  strcpy(temp, "ID ");
  strcat(temp, gameID);
  strcat(temp, "\n");
  if(writeServer(sock, buffer, temp) == -1){
    free(temp);
    return -1;
}



  //Playing Checkers
  if(readServer(sock, buffer) == -1){
    free(temp);
    return -1;
  }
  char *ptr;
  ptr = strtok(buffer, " +");
  ptr = strtok(NULL, " +\n");
  if(strcmp(ptr, gamekind) != 0){
    printf("wrong gamekind");
    free(temp);
    return -1;
  }

  //Game Name
  if(readServer(sock, buffer) == -1){
    free(temp);
    return -1;
  }

  //gewünschte Mitsplielernummer
  strcpy(temp, "PLAYER ");
  strcat(temp, player);
  strcat(temp, "\n");
  if(writeServer(sock, buffer, temp) == -1){
    free(temp);
    return -1;
  }
  free(temp);

  //Mitspielerantwort: YOU...
  if(readServer(sock, buffer) == -1)
    return -1;

  //THINKING schicken, damit communication eine Nachricht erhält, die es verarbeiten kann
  // if(atoi(player) == 0){
  //   readServer(sock, buffer);
  //   writeServer(sock, buffer, "THINKING\n");
  //   readServer(sock, buffer);
  //   writeServer(sock, buffer, "PLAY E3:F4\n");
  //   readServer(sock, buffer);
  // }
  printf("Ab hier arbeitet communication\n");

  return 0;
}




/*int main(){

  printf("Ich mach was!\n");
  performConnection();
  return 0;
}*/
