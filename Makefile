CFLAGS = -Wall -Wextra -Werror

GAME_ID?=2z8hf3o9bvl5s

PLAYER?=1

sysprak-client: main.o performConnection.o think.o communication.o conf.o
	gcc -o sysprak-client main.o performConnection.o think.o communication.o conf.o

performConnection.o: performConnection.c main.h
	gcc $(CFLAGS) -c performConnection.c

think.o: think.c main.h
	gcc $(CFLAGS) -c think.c

communication.o: communication.c main.h
	gcc $(CFLAGS) -c communication.c

conf.o: conf.c main.h
	gcc $(CFLAGS) -c conf.c
	
	
main.o: main.c main.h
	gcc $(CFLAGS) -c main.c

play: sysprak-client
	./sysprak-client -g $(GAME_ID) -p $(PLAYER)
  
clean:
	rm -f *.o sysprak-client test

rebuild: clean sysprak-client
