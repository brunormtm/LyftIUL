#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

typedef struct {
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
} Condutor;

typedef struct {
  char data[50];
  int nCondutor;
  int nPassageiro;
  int pontos;
  float valor;
} Viagem;

Condutor listaDeCondutores[100];
int indiceListaDeCondutores = 0;
Viagem listaDeViagens[100];
int indiceListaDeViagens = 0;
int hasChange = 0;

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

void fillListaDeCondutores(){
  FILE *condutores;
  condutores = fopen("condutores.txt", "r");
  if(condutores == NULL){
    fclose(condutores);
    printf("ERROR: condutores.txt not found\n");
    exit(0);
  }
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

void fillListaDeViagens(){
  FILE *viagens;
  viagens = fopen("viagens.txt", "r");

  if (viagens == NULL) {
    fclose(viagens);
    return;
  }

  char linha[200];
  while(fgets(linha, 200, viagens)){
    Viagem v;
    char temp[200];
    obter_substring(linha, v.data, ':', 0);
    obter_substring(linha, temp, ':', 1);
    v.nCondutor = convertToInt(temp);
    obter_substring(linha, temp, ':', 2);
    v.nPassageiro = convertToInt(temp);
    obter_substring(linha, temp, ':', 3);
    v.pontos = convertToInt(temp);
    obter_substring(linha, temp, ':', 4);
    v.valor = convertToFloat(temp);
    listaDeViagens[indiceListaDeViagens] = v;
    indiceListaDeViagens++;
  }
  fclose(viagens);
  hasChange = 1;
}

void updateCondutores(){
  for (int i = 0; i < indiceListaDeViagens; i++) {
    Viagem v = listaDeViagens[i];
    for (int j = 0; j < indiceListaDeCondutores; j++) {
      Condutor c = listaDeCondutores[j];
      if (v.nCondutor == c.numero) {
        c.pontos = c.pontos + v.pontos;
        c.viagens = c.viagens + 1;
        c.saldo = c.saldo + v.valor;
        listaDeCondutores[j] = c;
        i++;
      }
    }
  }
}

void saveCondutoresToFile(){
  FILE *fileCondutores = fopen("condutores.txt", "w");
  for (int i = 0; i < indiceListaDeCondutores; i++) {
    Condutor c = listaDeCondutores[i];
    fprintf(fileCondutores, "%d:%s:%s:%s:%s:%s:%s:%s:%d:%d:%f\n", c.numero, c.nome, c.turma, c.telemovel, c.email, c.tipo, c.marca, c.matricula, c.viagens, c.pontos, c.saldo);
  }
  fclose(fileCondutores);
}

void deleteViagensFile(){
  FILE *viagens = fopen("viagens.txt", "r");
  if (viagens == NULL) {
    return;
  } else {
    if (remove("viagens.txt") == 0)
        return;
     else
        printf("Unable to delete the file\n");
  }
}

void sendSignal(){
  if(hasChange){
    FILE *f = fopen("lyftadmin.pid", "r");
    if (f != NULL) {
      char str[20];
      char tmp[20];
      fgets(tmp, 20, f);
      obter_substring(tmp, str, '=', 1);
      int val = atoi(str);
      kill(val, SIGUSR1);
    }
  }
}

int main() {
  fillListaDeCondutores();
  fillListaDeViagens();
  updateCondutores();
  saveCondutoresToFile();
  deleteViagensFile();
  sendSignal();
  return 0;
}
