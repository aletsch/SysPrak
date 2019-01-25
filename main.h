#ifndef MAIN_HEADER_H
#define MAIN_HEADER_H

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define BUF 256
#define KEY 6543
#define SHMSIZE 2*sizeof(int)+BUF+2*sizeof(pid_t)+160

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/socket.h>


extern void error(char message[BUF]);
extern void signalHandler(int signal);

void printBoard();
struct Configuration setConfig(char* file);
int connectToServer(int* sock, char* host, int port);
int performConnection(char* gameID, char* player, char* gamekind, int* sock);
int communication(int *socket, int pipe);
char* think();
int readServer(int *socket, char *buffer);
int writeServer(int *socket, char *buffer, char message[BUF]);

struct Spieldaten {
  char gameName[BUF];
  int playerNumber;
  int playerCount;
  pid_t thinker;
  pid_t connector;
  char field[8][8];
  int thinkFlag;

};

struct Configuration{
  char hostname[64];
  int portnumber;
  char gamekind[64];
};

struct moeglicherZug{
  char zug[64];
  int gewichtung;
};

struct queenData{
  struct moeglicherZug bestMove;
  int x;   //akt Position
  int y;   //akt Position
  int success; //erfolg 1 (also schlagen) sonst 0
  char moveATM[64];
  char field[8][8];
  char enemyColour[2];
  char ownColour;
};

#endif
