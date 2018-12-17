CFLAGS = -Wall -Wextra -Werror

all:
	gcc $(CFLAGS) -o play main.c
  
clean:
	rm -f *.o play
