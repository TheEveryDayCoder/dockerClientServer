CC = gcc
RM = rm
CFLAGS = -g -Wall

all: serverM serverA serverB serverC clientA clientB 

clientA: clientA.c socketHelper.h
	$(CC) $(CFLAGS) socketHelper.c clientA.c -o clientA 

clientB: clientB.c socketHelper.h
	$(CC) $(CFLAGS) socketHelper.c clientB.c -o clientB

serverM: serverM.c socketHelper.h
	$(CC) $(CFLAGS) socketHelper.c serverM.c -o serverM

serverA: serverA.c socketHelper.h
	$(CC) $(CFLAGS) socketHelper.c serverA.c -o serverA

serverB: serverB.c socketHelper.h
	$(CC) $(CFLAGS) socketHelper.c serverB.c   -o serverB

serverC: serverC.c socketHelper.h
	$(CC) $(CFLAGS) socketHelper.c serverC.c -o serverC


clean:
	$(RM) serverM
	$(RM) serverA
	$(RM) serverB
	$(RM) serverC

	$(RM) clientA
	$(RM) clientB
