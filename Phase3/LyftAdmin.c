#include "defines.h"

Condutor *listaDeCondutores;
int indiceListaDeCondutores = 0;
Passageiro *listaDePassageiros;
int indiceListaDePassageiros = 0;
int pidFilho = -1;
int semPassageiros;
int semCondutores;
int fila;

int checkIfFileExists(const char * filename){
    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL){
        fclose(file);
        return 0;
    }
    return 1;
}

int convertToInt(char *c){
  int n = atoi(&c[0]);
  return n;
}

float convertToFloat(char *c){
  float n = atof(&c[0]);
  return n;
}

void obter_substring(char linha[], char resultado[], char separador, int indice) {
 int i, j=0, meu_indice = 0;
 for (i=0; linha[i] != '\0'; i++) {
   if ( linha[i] == separador ) {
     meu_indice++;
   } else if (meu_indice == indice) {
     resultado[j++]=linha[i];
   }
 }
  resultado[j]='\0';
}

int count_lines(FILE *fp){
  int count = 0;
  int linelength = 1000;
  char linesize[linelength]; //had to define a max linesize for this ?
  while(fgets(linesize,linelength, fp) != NULL){
    count++;
  }
  return count;
}

void removeSpace(char c[]){
  for(int i=0;c[i]!='\0';i++){
   if(c[i]=='\n'){
      c[i]='\0';
    }
  }
}

void read_condutores_to_memory(){
	down(semCondutores);
  indiceListaDeCondutores = 0;
  FILE *condutores;
  condutores = fopen("condutores.txt", "r");
  char linha[200];
  while(fgets(linha, 200, condutores)){
    Condutor c;
    char temp[200];
    obter_substring(linha, temp, ':', 0);
    c.numero = convertToInt(temp);
    obter_substring(linha, c.nome, ':', 1);
    obter_substring(linha, c.turma, ':', 2);
    obter_substring(linha, c.telemovel, ':', 3);
    obter_substring(linha, c.email, ':', 4);
    obter_substring(linha, c.tipo, ':', 5);
    obter_substring(linha, c.marca, ':', 6);
    obter_substring(linha, c.matricula, ':', 7);
    obter_substring(linha, temp, ':', 8);
    c.viagens = convertToInt(temp);
    obter_substring(linha, temp, ':', 9);
    c.pontos = convertToInt(temp);
    obter_substring(linha, temp, ':', 10);
    c.saldo = convertToFloat(temp);
    listaDeCondutores[indiceListaDeCondutores] = c;
    indiceListaDeCondutores++;
  }
  fclose(condutores);
  up(semCondutores);
}

void read_passageiros_to_memory(){
  down(semPassageiros);
	FILE *passageiros;
  passageiros = fopen("passageiros.txt", "r");
  char linha[200];
  while(fgets(linha, 200, passageiros)){
    Passageiro p;
    char temp[200];
    obter_substring(linha, temp, ':', 0);
    p.numero = convertToInt(temp);
    obter_substring(linha, p.nome, ':', 1);
    obter_substring(linha, p.turma, ':', 2);
    obter_substring(linha, p.telemovel, ':', 3);
    obter_substring(linha, p.email, ':', 4);
    obter_substring(linha, p.c_credito, ':', 5);
    removeSpace(p.c_credito);
    listaDePassageiros[indiceListaDePassageiros] = p;
    indiceListaDePassageiros++;
  }
  fclose(passageiros);
  up(semPassageiros);
}

void saveCondutoresToFile(){
  FILE *fileCondutores = fopen("condutores.txt", "w");
  for (int i = 0; i < indiceListaDeCondutores; i++) {
    Condutor c = listaDeCondutores[i];
    fprintf(fileCondutores, "%d:%s:%s:%s:%s:%s:%s:%s:%d:%d:%f\n", c.numero, c.nome, c.turma, c.telemovel, c.email, c.tipo, c.marca, c.matricula, c.viagens, c.pontos, c.saldo);
  }
  fclose(fileCondutores);
}

void savePassageirosToFile(){
  FILE *filePassageiros = fopen("passageiros.txt", "w");
  for (int i = 0; i < indiceListaDePassageiros; i++) {
    Passageiro p = listaDePassageiros[i];
    fprintf(filePassageiros, "%d:%s:%s:%s:%s:%s\n", p.numero, p.nome, p.turma, p.telemovel, p.email, p.c_credito);
  }
  fclose(filePassageiros);
}

void write_pid(){
  FILE*fp;
  fp = fopen("lyftadmin.pid","w");
  if ( fp == NULL ) {
    printf("Error creating file\n");
    return;
  }
  fprintf (fp, "PID=%d\n", getpid());
  fclose(fp);
}

void readEnter(){
  while (getc(stdin) != '\n');
}

void printMemory(){
  printf("\nCondutores\n\n");
	for (int i = 0; i < indiceListaDeCondutores; i++) {
    Condutor c = listaDeCondutores[i];
    printf("%d:%s:%s:%s:%s:%s:%s:%s:%d:%d:%f:%d\n", c.numero, c.nome, c.turma, c.telemovel, c.email, c.tipo, c.marca, c.matricula, c.viagens, c.pontos, c.saldo, c.activo);
	}
	printf("\nPassageiros\n\n");
  for (int i = 0; i < indiceListaDePassageiros; i++) {
    Passageiro p = listaDePassageiros[i];
    printf("%d:%s:%s:%s:%s:%s\n", p.numero, p.nome, p.turma, p.telemovel, p.email, p.c_credito);
  }
  printf("\nNumero de Condutores: %d\n", indiceListaDeCondutores);
  printf("Numero de passageiros: %d\n\n", indiceListaDePassageiros);
}

void modifyPassenger(){
  down(semPassageiros);
	int num;
  printf("\nIntroduza o numero de Aluno: ");
  scanf ("%d", &num);
  readEnter();
  int indice;
  Passageiro p;
  int found = 0;
  for (int i = 0; i < indiceListaDePassageiros; i++) {
    p = listaDePassageiros[i];
    if (p.numero == num) {
      indice = i;
      found = 1;
    }
  }
  if(found){
    p = listaDePassageiros[indice];
    // FAZER FUNCAO PARA FAZER CHECK AOS ARGUMENTOS
    printf("\nNumero de aluno antigo: %d", p.numero);
    printf("\nNumero de aluno novo: ");
    scanf("%d", &p.numero);
    readEnter();
    printf("\nNome antigo: %s", p.nome);
    printf("\nNome novo: ");
    fgets(p.nome, 50, stdin);
    removeSpace(p.nome);
    printf("\nTurma antiga: %s", p.turma);
    printf("\nTurma nova: ");
    fgets(p.turma, 10, stdin);
    removeSpace(p.turma);
    printf("\nTelemovel antigo: %s", p.telemovel);
    printf("\nTelemovel novo: ");
    fgets(p.telemovel, 15, stdin);
    removeSpace(p.telemovel);
    printf("\nEmail antigo: %s", p.email);
    printf("\nEmail novo: ");
    fgets(p.email, 40, stdin);
    removeSpace(p.email);
    printf("\nCC antigo: %s", p.c_credito);
    printf("\nCC novo: ");
    fgets(p.c_credito, 20, stdin);
    removeSpace(p.c_credito);
    listaDePassageiros[indice] = p;
  } else {
    printf("\nNao existe o Aluno na memoria");
  }
  up(semPassageiros);
}
void modifyDriver(){
	down(semCondutores);
  int num;
  printf("\nIntroduza o numero de Aluno: ");
  scanf ("%d", &num);
  readEnter();
  int indice;
  Condutor c;
  int found = 0;
  for (int i = 0; i < indiceListaDeCondutores; i++) {
    c = listaDeCondutores[i];
    if (c.numero == num) {
      indice = i;
      found = 1;
    }
  }
  if(found){
    c = listaDeCondutores[indice];
    printf("\nNumero de aluno antigo: %d", c.numero);
    printf("\nNumero de aluno novo: ");
    scanf("%d", &c.numero);
    readEnter();
    printf("\nNome antigo: %s", c.nome);
    printf("\nNome novo: ");
    fgets(c.nome, 50, stdin);
    removeSpace(c.nome);
    printf("\nTurma antiga: %s", c.turma);
    printf("\nTurma nova: ");
    fgets(c.turma, 10, stdin);
    removeSpace(c.turma);
    printf("\nTelemovel antigo: %s", c.telemovel);
    printf("\nTelemovel novo: ");
    fgets(c.telemovel, 15, stdin);
    removeSpace(c.telemovel);
    printf("\nEmail antigo: %s", c.email);
    printf("\nEmail novo: ");
    fgets(c.email, 40, stdin);
    removeSpace(c.email);
    printf("\nTipo de veiculo antigo: %s", c.tipo);
    printf("\nTipo de veiculo novo: ");
    fgets(c.tipo, 20, stdin);
    removeSpace(c.tipo);
    printf("\nMarca antiga: %s", c.marca);
    printf("\nMarca nova: ");
    fgets(c.marca, 20, stdin);
    removeSpace(c.marca);
    printf("\nMatricula antiga: %s", c.matricula);
    printf("\nMatricula nova: ");
    fgets(c.matricula, 20, stdin);
    removeSpace(c.matricula);
    listaDeCondutores[indice] = c;
  } else {
    printf("\nNao existe o Aluno na memoria");
  }
  up(semCondutores);
}

void leave(){
  saveCondutoresToFile();
  savePassageirosToFile();
  kill(pidFilho, SIGKILL);
  exit(0);
}

void runLyftTask(){
  if(fork() == 0){
    execl("LyftTask", NULL);
    printf("Failed to exec LyftTask\n");
  }
  alarm(60);
}

void handleSignal(int signum){
  switch(signum){
    case 30: read_condutores_to_memory(); break;
    case 15: leave(); break;
    case 14: runLyftTask(); break;
    default: printf("Didn't recognize the signal\n");
  }
}

void checkFiles(){
  if (checkIfFileExists("condutores.txt") == 0 || checkIfFileExists("passageiros.txt") == 0) {
    printf("ERROR: condutores.txt OR passageiros.txt not found!\n");
    exit(0);
  }
}

void makeSharedMemory(){
  int id = shmget(MEMCOND, 1000 * sizeof(Condutor) , IPC_CREAT | 0666);
  exit_on_error(id, "shmget");

  int di = shmget(MEMPASS, 1000 * sizeof(Passageiro) , IPC_CREAT | 0666);
  exit_on_error(di, "shmget");

  listaDeCondutores = (Condutor *)shmat(id, 0, 0);
  if (listaDeCondutores == NULL) {perror("error attaching"); exit(1);}

  listaDePassageiros = (Passageiro *)shmat(di, 0, 0);
  if (listaDePassageiros == NULL) {perror("error attaching"); exit(1);}

}

void dealWithNewPassenger(int msgid, MsgViagem m){
	down(semCondutores);
	int i = 0;
	int t = -1;
	while(listaDeCondutores[i].numero != 0 || i == 1000){
		if(listaDeCondutores[i].activo == 1){
			if(listaDeCondutores[t].numero == -1 || listaDeCondutores[t].disponivel_desde < listaDeCondutores[i].disponivel_desde){
				t = i;
			}
		} i++;
	}
	if(t != -1){
		listaDeCondutores[t].activo = 0;
		m.tipo = listaDeCondutores[t].PID;
		int resposta;
		resposta = msgsnd(msgid, &m, sizeof(m.dados), 0);
		exit_on_error(resposta, "new passenger");
	}
	up(semCondutores);
}

void dealWithReceipt(MsgViagem m){
	down(semCondutores);
	int i = 0;
	while(listaDeCondutores[i].numero != 0 || i == 1000){
		if(listaDeCondutores[i].PID == m.dados.pid_condutor){
			listaDeCondutores[i].saldo += m.dados.valor;
			listaDeCondutores[i].pontos += m.dados.pontos;
			listaDeCondutores[i].disponivel_desde = time(NULL);
			listaDeCondutores[i].viagens++;
			listaDeCondutores[i].activo = 1;
			break;
		} i++;
	}
	up(semCondutores);
}

void listenForMessages(){
	pidFilho = fork();
	if(pidFilho == 0){
		int msgid, status;
		MsgViagem m;
		msgid = msgget(MSGCHANNEL, IPC_CREAT | 0666);
		exit_on_error(msgid, "criacao de lista de msg");
		while(1){
			status = msgrcv(msgid, &m, sizeof(m.dados), 1, 0);
			exit_on_error(status, "erro de recepcao");
			if(m.dados.pid_condutor == 0){
				dealWithNewPassenger(msgid, m);
			} else{
				dealWithReceipt(m);
			}
		}
	}
}

void makeSems(){
	int status;

	semPassageiros = semget(SEMPASS, 1, IPC_CREAT | 0666);
	exit_on_error(semPassageiros, "Criacao de SemP");
	status = semctl(semPassageiros, 0, SETVAL, 1);
	exit_on_error(status, "Ligar SemP");

	semCondutores = semget(SEMCOND, 1, IPC_CREAT | 0666);
	exit_on_error(semCondutores, "Criacao de SemC");
	status = semctl(semCondutores, 0, SETVAL, 1);
	exit_on_error(status, "Ligar SemC");

	fila = semget(SEMFILA, 1, IPC_CREAT | 0666);
	exit_on_error(fila, "criacao da fila");
	status = semctl(fila, 0, SETVAL, 0);
	exit_on_error(status, "ligar fila");
}

int main(){
  checkFiles();
  makeSharedMemory();
  makeSems();
  read_condutores_to_memory();
  read_passageiros_to_memory();
  signal(SIGUSR1, handleSignal);
  signal(SIGTERM, handleSignal);
  signal(SIGALRM, handleSignal);
  alarm(60);
  write_pid();
  listenForMessages();

  int option;
    do {
	option = 9;
      printf ("\n1. Imprimir memória\n");
      printf ("2. Alterar passageiro\n");
      printf ("3. Alterar condutor\n");
      printf ("0. Sair\n");
      printf ("Escolha a opcao pretendida: ");
      scanf("%d", &option);
      readEnter();
      switch(option){
          case 1: printMemory(); makeSharedMemory(); break;
          case 2: modifyPassenger(); makeSharedMemory(); break;
          case 3: modifyDriver(); makeSharedMemory(); break;
          case 0: makeSharedMemory(); leave(); break;
          default: printf("Insira uma opção válida!\n");
      }
    } while(option != 0);
    return 0;
}
