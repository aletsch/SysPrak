#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>


int think(char** field[8][8], int spieler) {

  switch (spieler) {
    case 0:     //grün
      for(int x=0, x<8, x++) {
        for (int y=7, y>=0, y--) {
          if (field [x][y] == ('w' || 'W')) {
            possiblemoves(spieler, );
          } 
        }  
      }





    case 1:     //schwarz 








  }

}

