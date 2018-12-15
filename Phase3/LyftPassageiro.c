#include "defines.h"

Passageiro x;
char local[100];
int id, status, semPassageiros, fila;
Passageiro * passageiros;

Passageiro getPassageiro(){
	Passageiro p;
	char t[100];

	printf("Numero do Passageiro: ");
	fgets(t, 100, stdin);
	p.numero = atoi(t);
	return p;
}

int checkPassageiro(){
	int id = shmget(MEMPASS, 1000 * sizeof(Passageiro), 0600);
	exit_on_error(id, "error shmget");

	down(semPassageiros);
	passageiros = (Passageiro *)shmat(id, 0, 0);
	if(passageiros == NULL) {perror("error attaching"); exit(1);}

	int i = 0;
	while(passageiros[i].numero != 0){
		if(passageiros[i].numero == x.numero){
			x = passageiros[i];
			up(semPassageiros);
			return 1;
		} i++;
	} up(semPassageiros); return 0;
}

void askLocation(){
	printf("local de encontro: ");
	fgets(local, 100, stdin);
}

void sendMessage(){
	down(fila);
	id = msgget(MSGCHANNEL, IPC_CREAT | 0666);
	exit_on_error(id, "msgget");

	MsgViagem m;
	m.tipo = 1;
	m.dados.pid_passageiro = getpid();
	m.dados.pid_condutor = 0;
	strcpy(m.dados.local_encontro, local);
	m.dados.data = time(NULL);
	status = msgsnd(id, &m, sizeof(m.dados), 0);
	exit_on_error(status, "envio");
}

void getAnswear(){
	MsgViagem m;
	status = msgrcv(id, &m, sizeof(m.dados), getpid(), 0);
	exit_on_error(status, "recepcao");
	printf("PID do Condutor: %d\nLocal do Condutor: %s\n", m.dados.pid_condutor, m.dados.local_encontro);
}

void joinSem(){
	semPassageiros = semget(SEMPASS, 1, IPC_CREAT | 0666);
	exit_on_error(semPassageiros, "join SemP");
	fila = semget(SEMFILA, 1, IPC_CREAT | 0666);
	exit_on_error(fila, "join fila");
}

int main(){
	joinSem();
	x = getPassageiro();
	if(checkPassageiro() == 1){
		printf("Bem vindo %s!\n", x.nome);
		askLocation();
		sendMessage();
		getAnswear();
	}
}
