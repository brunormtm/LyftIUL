#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>


#define CONDUTORES "path para condutores" #TODO
#define PASSAGEIROS "path para passageiros.txt" #TODO


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
  //estes abaixo nao existem no txt original da fase 1!!!
  int activo;
  long disponivel_desde;
  int PID;
} Tcondutor;
//numero nome turma telemovel email tipo marca matricula viagens pontos saldo activo disponivel_desde PID - aux para copiar
// %d %c %c %c %c %c %c %c %d %d %f %d %d %li %d - aux para copiar
typedef struct{
int numero;
char nome[50];
char turma[10];
char telemovel[15];
char email[40];
char c_credito[20];
} Tpassageiro;
//numero nome turma telemovel email c_credito - aux para copiar
// %d %c %c %c %c %c - aux para copiar


//1 ler ficheiros e carregar em memoria
void read_databasefiles(){
  read_condutores_to_memory();
  read_passageiros_to_memory();
}

void read_condutores_to_memory(){
  FILE *fp;
  struct Tcondutor condutor;

  //abrir ficheiro - r for read
  fp = fopen(CONDUTORES,"r");
  //condutores.txt path missing
  if (fp == NULL){
    perror("File not found!");
    exit(1);
  }

  while (fread (&condutor, sizeof (struct Tcondutor), count_lines(fp), fp))
  {
    fscanf(fp, "%d %c %c %c %c %c %c %c %d %d %f %d %d %li %d", condutor.numero, condutor.nome, condutor.turma, condutor.telemovel, condutor.email, condutor.tipo, condutor.marca, condutor.matricula, condutor.viagens,
    condutor.pontos, condutor.saldo, condutor.activo, condutor.disponivel_desde, condutor.PID )
    //activo,disponivel_desde nao existe no file original, o PID vem de outro file - remover???.
    //separar por :?
  }
  fclose(fp);

}
void read_passageiros_to_memory(){
  FILE *fp;
  struct Tpassageiro passageiro;

  //abrir ficheiro - r for read
  fp = fopen(PASSAGEIROS,"r");
  //passageiros.txt path missing
  if (fp == NULL){
    perror("File not found!");
    exit(1);
  }

  while (fread (&condutor, sizeof (struct Tcondutor), count_lines(fp), fp))
  {
    fscanf(fp, "%d %c %c %c %c %c", passageiro.numero, passageiro.nome, passageiro.turma, passageiro.telemovel, passageiro.email, passageiro.c_credito)
  }
  fclose(fp);

}

//countlines of a file aux
int count_lines(FILE *fp){
  int count = 0;
  int linelength = 1000;
  char linesize[linelength]; //had to define a max linesize for this ?

  while(fgets(linesize,linelength, fp) != null){
    count++
  }
  return count;
}


//2 criar liftadmin.pid e colocar la o seu PID
void write_pid(){
  FILE*fp;

  fp = fopen("lyftadmin.pid","w")
  if ( fp == NULL ) {
      printf("Error creating file\n");
      return;
    }

  //while?
  fprintf (fp, "PID=%d\n", getpid() )

  fclose(fp);
}

//3
void signals(){ //???????
  kill(getpid(),SIGUSR1);
  kill(getpid(),SIGTERM);
}
void updatesignal(int signal){

    if(signal = SIGUSR1){
      //ler coundutores.txt mas so acualizar pontos, viagens e saldos nos Tcondutores
    }
    signal(SIGUSR1, updatesignal);
}
void terminatesignal(int signal){
  //Tratar SIGTERM

    if(signal = SIGTERM){
      write_passageiros_to_txt();
      write_condutores_to_txt();
      #TODO
      //terminar?
    }

  signal(SIGTERM,terminatesignal);
}

#TODO
void write_condutores_to_txt(){
  //escrever coundutores no txt com base em Tcoundutor
}
void write_passageiros_to_txt(){
  //escrever passageiros no txt com base em Tpassageiro
}


//4 menu (main?)
main(){
  read_databasefiles();
  write_pid();
  signals();
  int option;
    do {

      printf ("1. Imprimir memória\n");
      printf ("2. Alterar passageiro\n");
      printf ("3. Alterar condutor\n");

      //nao e printf mas n me lembro da funçao certa
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

          default:
          printf("Insira uma opção válida!");
          break;
      }
    } while  ( opcao != 0 );

    //alarm() ?
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

#TODO
//5 alarm() de 60 em 60 segundos,fork() e execturar Lyfttask
