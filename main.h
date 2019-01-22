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

extern void error(char message[BUF]);
extern void signalHandler(int signal);

struct Spieldaten {
  char gameName[BUF];
  int playerNumber;
  int playerCount;
  pid_t thinker;
  pid_t connector;
  char field[8][8];
  int fdWrite;

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


#endif
