#include "defines.h"

typedef struct{
	long tipo;
	struct{
		int pid_passageiro;
		int pid_condutor;
		char local_encontro[100];
		long data;
		float pontos;
		float valor;
	} dados;
} MsgViagem;

typedef struct{
	int numero;
	char nome[50];
	char turma[10];
	char telemovel[15];
	char email[40];
	char c_credito[20];
} Passageiro;

Passageiro x;
char local[100];
int id, status;
	
Passageiro getPassageiro(){
	Passageiro p;
	char t[100];

	printf("Numero do Condutor: ");
	fgets(t, 100, stdin);
	p.numero = atoi(t);
	return p;
}

int checkPassageiro(){

	int id = shmget(10002, 1000 * sizeof(Passageiro), IPC_CREAT | 0666);
	exit_on_error(id, "error shmget");

	Passageiro* passageiros = (Passageiro *)shmat(id, 0, 0);
	if(passageiros == NULL) {perror("error attaching"); exit(1);}

	// For Testing Purposes
	int i;
	for(i = 0; i < 5; i++){
		printf("%d\n", passageiros[i].numero);
	}
	
	// For Debugging Purposes
	return 1;
}

void askLocation(){
	printf("local de encontro: ");
	fgets(local, 100, stdin);
}

void sendMessage(){
	id = msgget(10003, 0666 | IPC_CREAT);
	exit_on_error(id, "msgget");

	MsgViagem m;
	m.tipo = 1;
	m.dados.pid_passageiro = getpid();
	// m.dados.local_encontro = local;
	// m.dados.data

	status = msgsnd(id, &m, sizeof(m.dados), 0);
	exit_on_error(status, "envio");

	printf("sucesso");
}

void getAnswear(){
	MsgViagem m;
	status = msgrcv(id, &m, sizeof(m.dados), getpid(), 0);
	exit_on_error(status, "recepcao");
	printf("PID do Condutor: %d\n", m.dados.pid_condutor);
}

int main(){
	x = getPassageiro();
	if(checkPassageiro() != 1){
		printf("erro na autenticacao!");
		exit(1);
	}
	askLocation();
	sendMessage();
	getAnswear();
}




