#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

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
//numero nome turma telemovel email tipo marca matricula viagens pontos saldo activo disponivel_desde PID - aux para copiar
// %d %c %c %c %c %c %c %c %d %d %f %d %d %li %d - aux para copiar

typedef struct{
  int numero;
  char nome[50];
  char turma[10];
  char telemovel[15];
  char email[40];
  char c_credito[20];
} Passageiro;
//numero nome turma telemovel email c_credito - aux para copiar
// %d %c %c %c %c %c - aux para copiar

Condutor listaDeCondutores[1000];
int indiceListaDeCondutores = 0;
Passageiro listaDePassageiros[1000];
int indiceListaDePassageiros = 0;

int convertToInt(char *c){
  int n = atoi(&c[0]);
  return n;
}

float convertToFloat(char *c){
  int n = atof(&c[0]);
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

void read_condutores_to_memory(){
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
}

void read_passageiros_to_memory(){
  FILE *passageiros;
  passageiros = fopen("condutores.txt", "r");
  char linha[200];
  while(fgets(linha, 200, passageiros)){
    Passageiro p;
    char temp[200];

    obter_substring(linha, temp, ':', 0);
    p.numero = convertToInt(temp);
    obter_substring(linha, p.nome, ':', 1);
    obter_substring(linha, p.turma, ':', 1);
    obter_substring(linha, p.telemovel, ':', 1);
    obter_substring(linha, p.email, ':', 4);
    obter_substring(linha, p.c_credito, ':', 5);

    listaDePassageiros[indiceListaDePassageiros] = p;
    indiceListaDePassageiros++;
  }
  fclose(passageiros);
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
  //escrever passageiros no txt com base em Tpassageiro
}

//2 criar liftadmin.pid e colocar la o seu PID
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

//3
void signals(){ //???????
  kill(getpid(),SIGUSR1);
  kill(getpid(),SIGTERM);
}

void updatesignal(int signal){
  if(signal == SIGUSR1){
    //ler coundutores.txt mas so acualizar pontos, viagens e saldos nos Tcondutores
  }
  //signal(SIGUSR1, updatesignal);
}

void terminatesignal(int signal){
  //Tratar SIGTERM

  if(signal == SIGTERM){
    saveCondutoresToFile();
    savePassageirosToFile();
    //terminar?
  }

//  signal(SIGTERM,terminatesignal);
}

void read_enter(){
  while ( getc(stdin) != '\n');
}

void printmemory(){
  //imprimir memoria
    //listar condutores e passageiros em memoria e indicar qtos são
}

void modifypassenger(){
  //alterar passageiro
    //ler numero de estudante, se existe em Tpassageiro, re introduzir campos
}
void modifydriver(){
  //alterar condutor
    //ler numero de estudante, se existe em Tcondutor, re introduzir tudo menos viagens, pontos e saldo
}

//TO DO
//5 alarm() de 60 em 60 segundos,fork() e execturar Lyfttask

int main(){
  read_condutores_to_memory();
  read_passageiros_to_memory();
  write_pid();
  //signals();
  int option;
    do {

      printf ("1. Imprimir memória\n");
      printf ("2. Alterar passageiro\n");
      printf ("3. Alterar condutor\n");
      printf ("0. Sair\n");
      printf ("Escolha a opcao pretendida: ");
      scanf ("%d", &option );

      read_enter();

      switch(option){
          case 1:
          printmemory();
          break;

          case 2:
          modifypassenger();
          break;

          case 3:
          modifydriver();
          break;

          case 0:
          return 0;

          default:
          printf("Insira uma opção válida!\n");
          break;
      }
    } while(option != 0);
    return 0;
    //alarm() ?
}
