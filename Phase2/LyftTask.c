#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

void fillListaDeCondutores(){
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

void fillListaDeViagens(){
  FILE *viagens;
  viagens = fopen("viagens.txt", "r");
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

    printf("%d\n", v.pontos);

    listaDeViagens[indiceListaDeViagens] = v;
    indiceListaDeViagens++;
  }
  fclose(viagens);
}

int main() {

  fillListaDeCondutores();
  fillListaDeViagens();

  FILE *condutores;
  FILE *viagens;

  condutores = fopen("condutores.txt", "r+");
  viagens = fopen("viagens.txt", "r+");

  fclose(condutores);
  fclose(viagens);

  return 0;

}
