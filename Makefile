TAR	= a.out
OBJ	= chapter8_2.o
SRC	= chapter8_2.c
LIBS	= 
#OBJ	+=
CC	= gcc
CFLAGS	= -Wall -O2 -g #-DWITH_FIELDS#-fno-stack-protector 
LANG	= C

all:$(OBJ) $(TAR)

$(OBJ):$(SRC)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(TAR):$(OBJ)
	$(CC) -o $@ $< -lm
	@rm -f *.o

clean:
	@rm -f *.o
