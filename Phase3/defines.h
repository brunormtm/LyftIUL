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
#define MSGCHANNEL 10003
#define MEMCOND 10001
#define MEMPASS 10002
#define SEMCOND 10005
#define SEMPASS 10004
#define SEMFILA 10006


typedef struct{
  int numero;
  char nome[50];
  char turma[10];
  char telemovel[15];
  char email[40];
  char tipo[20];
  char marca[20];
  char matricula[15];
  int viagens;
  int pontos;
  float saldo;
  int activo;
  long disponivel_desde;
  int PID;
} Condutor;

typedef struct{
  int numero;
  char nome[50];
  char turma[10];
  char telemovel[15];
  char email[40];
  char c_credito[20];
} Passageiro;

typedef struct{
	long tipo;
	struct {
		int pid_passageiro;
		int pid_condutor;
		char local_encontro[100];
		long data;
		float pontos;
		float valor;
	} dados;
} MsgViagem;

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
