#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>


#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define PORTNUMBER 1357
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
    printf("%s\n", inet_ntoa(h->sin_addr));
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
  printf("Server: %s\n", buffer);
  return 0;
  }
  return -1;
}


int performConnection(){

  //get ip
  char * ip;
  ip=(char *)malloc(60*sizeof(char));
  char *hostname = HOSTNAME;
  if (hostname_to_ip(hostname, ip) == 1){
    exit(EXIT_FAILURE);
  }

  printf("Die ip ist:%s\n", ip);
  //create a socket
  int sock;
  sock = socket(AF_INET, SOCK_STREAM, 0);

  // address structure to connect to
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORTNUMBER);
  server_address.sin_addr.s_addr = inet_addr(ip);

  printf("Struktur erstellt: %i\n",server_address.sin_addr.s_addr);

  int connection_status = connect(sock, (struct sockaddr *) &server_address, sizeof(server_address));
  if (connection_status != 0){
    printf("Could not connect to server\n");
  }

  //Kommunikation mit Server
  char buffer[BUF];

  //get Server version
  readServer(&sock, buffer);


  strcpy(buffer, "VERSION 2.0\n");
  write(sock, buffer, strlen(buffer));
  printf("%s", buffer);


  //Version accepted, request GAME ID
  readServer(&sock, buffer);


  strcpy(buffer, "ID 3ir44rz0u65kj\n");
  write(sock, buffer, strlen(buffer));
  printf("%s\n", buffer);


  //Playing Checkers
  readServer(&sock, buffer);

  //Game Name
  readServer(&sock, buffer);

  //close connection
  close(sock);

  return 0;
}




/*int main(){

  printf("Ich mach was!\n");
  performConnection();
  return 0;
}*/
