#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <unistd.h>
#include <sys/msg.h>
#include <time.h>
#define exit_on_error(s,m) if (s<0) {perror(m); exit(1);}

void down(int id){
	struct sembuf DOWN = {0,-1,0};
	int status = semop(id, &DOWN, 1);
	exit_on_error(status, "down");
}

void up(int id){
	struct sembuf UP = {0,1,0};
	int status = semop(id, &UP, 1);
	exit_on_error(status, "up");
}
