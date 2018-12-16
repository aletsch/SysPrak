#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

//#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de\n"
//#define PORTNUMBER 1357
#define BUF 256


int hostname_to_ip(char* host, char *ip){
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_in *h;
  int rv;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if((rv = getaddrinfo(host, "http", &hints, &servinfo))){
    printf("could not resolve addressinfo\n");
    return 1;
  }

  //take the first possible result
  for(p = servinfo; p != NULL; p = p->ai_next){
    h = (struct sockaddr_in *) p->ai_addr;
    strcpy(ip, inet_ntoa(h->sin_addr));
  }

  freeaddrinfo(servinfo);
  return 0;
}


/*
int hostname_to_ip(char * hostname, char* ip){
  struct hostent *he = gethostbyname(hostname);
  struct in_addr **addr_list;
  int i;
  printf("hostname_to_ip start\n");
  if(he == NULL){
    herror("could not find host by name");
    return 1;
  }

  addr_list = (struct in_addr **) he->h_addr_list;
  printf("addr_list initialisiert\n");
  printf("%s",  inet_ntoa(*addr_list[0]));
  for(i = 0; addr_list[i] != NULL; i++){
    printf("%i\n", i);
    ip =  inet_ntoa(*addr_list[i]);
    return 0;
  }
  return 1;
}
*/

int readServer(int *socket, char *buffer){

  memset(buffer, 0, BUF);

  if(read(*socket, buffer, BUF) != 0){
    switch (*buffer){
      case '+': 
        //printf("Server: %s", buffer);
        return 0;
        break;
      case '-':
        printf("Ungültige Serveranfrage: %s", buffer);
        exit(EXIT_FAILURE);
        break;
      default:
        printf("%s\n", buffer);
        printf("Unerwartete Antwort des Servers\n");
        exit(EXIT_FAILURE);
    }
    } else {
    perror("Fehler beim Lesen vom Server: ");
    return -1;
    }
}

int writeServer(int *socket, char *buffer, char message[BUF]){

  strcpy(buffer, message);

  if(write(*socket, buffer, strlen(buffer)) != 0){
  //printf("Client: %s", buffer);
  return 0;
  }
  return -1;
}

int connectToServer(int* sock, char* host, int port){
  //get ip
  char *ip;
  ip = (char *)malloc(60*sizeof(char));
  char *hostname = host;
  if (hostname_to_ip(hostname, ip) == 1){
    return 1;
  }



  // address structure to connect to
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);
  server_address.sin_addr.s_addr = inet_addr(ip);

  int connection_status = connect(*sock, (struct sockaddr *) &server_address, sizeof(server_address));
  if (connection_status != 0){
    perror("Error: ");
    return -1;
  }

  return 0;
}


int spielerBereit(char* line){
  char* curLine = line;
  char *ptr;
  char* nextLine = strchr(curLine, '\n');
  char* output = malloc(sizeof(char)*30);

  curLine = nextLine ? (nextLine+1) : NULL;
  ptr = strtok(curLine, " +");
  strcpy(output, "Spieler ");
  strcat(output, ptr);
  ptr = strtok(NULL, " +\n");
  strcat(output, " (");
  strcat(output, ptr);
  strcat(output, ") ist ");
  ptr = strtok(NULL, " +\n");
  if (atoi(ptr)){
    strcat(output, "bereit.\n");
  } else {
    strcat(output, "noch nicht bereit.\n");
  }
  printf("%s", output);
  free(output);
  return 0;
}


int performConnection(char* gameID, char* player, char* gamekind, int* sock){

  //Kommunikation mit Server
  char buffer[BUF];

  //get Server version
  readServer(sock, buffer);


  //send Client Version
  writeServer(sock, buffer, "VERSION 2.0\n");


  //Version accepted, request gameID
  readServer(sock, buffer);

  //send gameID
  char* temp = malloc(BUF);
  strcpy(temp, "ID ");
  strcat(temp, gameID);
  strcat(temp, "\n");
  writeServer(sock, buffer, temp);



  //Playing Checkers
  readServer(sock, buffer);
  char *ptr;
  ptr = strtok(buffer, " +");
  ptr = strtok(NULL, " +\n");
  if(strcmp(ptr, gamekind) != 0){
    printf("wrong gamekind");
    exit(EXIT_FAILURE);
  }  

  //Game Name
  readServer(sock, buffer);

  //gewünschte Mitsplielernummer
  strcpy(temp, "PLAYER ");
  strcat(temp, player);
  strcat(temp, "\n");
  writeServer(sock, buffer, temp);
  free(temp);

  //Mitspielerantwort
  readServer(sock, buffer);
  readServer(sock, buffer);
  spielerBereit(buffer);
  
  //THINKING schicken, damit communication eine Nachricht erhält, die es verarbeiten kann
  if(atoi(player) == 0){
    writeServer(sock, buffer, "THINKING\n");
  }
  printf("Ab hier arbeitet communication\n");

  return 0;
}




/*int main(){

  printf("Ich mach was!\n");
  performConnection();
  return 0;
}*/
