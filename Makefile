CFLAGS = -Wall -Wextra -Werror

all:
	gcc $(CFLAGS) -o sysprak-client main.c

GAME_ID?=1xup1rk2evcc6

PLAYER?=2

play:
	gcc $(CFLAGS) -o sysprak-client main.c
	./sysprak-client -g $(GAME_ID) -p $(PLAYER)
  
clean:
	rm -f *.o sysprak-client
