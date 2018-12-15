#include "defines.h"

int posicao, id, status, semCondutores, fila;
Condutor x;
Condutor * condutores;
MsgViagem m;

Condutor getCondutor(){
	Condutor c;
	char t[100];

	printf("Numero do Condutor: ");
	fgets(t, 100, stdin);
	c.numero = atoi(t);
	return c;
}

int checkCondutor(){
	down(semCondutores);
	int id = shmget(MEMCOND, 1000 * sizeof(Condutor), IPC_CREAT | 0666);
	exit_on_error(id, "error shmget");

	condutores = (Condutor *)shmat(id, 0, 0);
	if(condutores == NULL) {perror("error attaching"); exit(1);}

	int i = 0;
	while(condutores[i].numero != 0){
		if(condutores[i].numero == x.numero){
			posicao = i;
			x = condutores[i];
			up(semCondutores);
			return 1;
		} i++;
	} up(semCondutores); return 0;
}

void activateDriver(){
	down(semCondutores);
	condutores[posicao].activo = 1;
	condutores[posicao].disponivel_desde = time(NULL);
	condutores[posicao].PID = getpid();
	up(semCondutores);
}

void getAnswear(){
	id=msgget(MSGCHANNEL, 0666 | IPC_CREAT);
	exit_on_error(id, "msgget");
	status = msgrcv(id, &m, sizeof(m.dados), getpid(), 0);
	exit_on_error(status, "recepcao");
	printf("Nova Viagem!\n");
	printf("PID do Passageiro: %d\n", m.dados.pid_passageiro);
}

void dealWithPassenger(){
	m.tipo = m.dados.pid_passageiro;
	m.dados.pid_condutor = getpid();
	printf("Onde se encontra? ");
	char s[50];
	fgets(s, 50, stdin);
	strcpy(m.dados.local_encontro, s);
	status = msgsnd(id, &m, sizeof(m.dados), 0);
	exit_on_error(status, "envio de msg");

	// Wait da Viagem
	int r = (rand() % 20) + 1;
	sleep(r);

	//User Input
	char t[10];
	printf("Valor da Viagem: ");
	fgets(t, 10, stdin);
	m.dados.valor = atof(t);
	int value = -1;
	while(value > 10 || value < 0){
		printf("Pontuacao de 0 a 10 da Viagem: ");
		fgets(t, 10, stdin);
		value = atoi(t);
	}
	m.dados.pontos = value;
}

void sendMessageToServer(){
	m.tipo = 1;
	status = msgsnd(id, &m, sizeof(m.dados), 0);
	exit_on_error(status, "send to server");
}

void logoutUser(){
	down(semCondutores);
	condutores[posicao].activo = 0;
	up(semCondutores);
	printf("\nAdeus!\n");
	exit(1);
}

void handleSignal(int signum){
	switch(signum){
		case 2: logoutUser(); break;
		default: printf("Didnt recognize the signal!");
	}
}

void joinSem(){
	semCondutores = semget(SEMCOND, 1, IPC_CREAT | 0666);
	exit_on_error(semCondutores, "join SemC");
	fila = semget(SEMFILA, 1, IPC_CREAT | 0666);
	exit_on_error(fila, "join fila");
}

int main(){
	joinSem();
	x = getCondutor();
	if(checkCondutor() == 1){
		printf("Bem vindo %s!\n", x.nome);
		signal(SIGINT, handleSignal);
		activateDriver();
		while(1){
			up(fila);
			printf("Waiting for new Passenger...\n");
			getAnswear();
			dealWithPassenger();
			sendMessageToServer();
		}
	}
}
