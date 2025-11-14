#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 
cadastrar um numero fixo de territorios(nome, cor do exercito
tropas)
usamos buffers com tamanho fixo e limpamos com limparBuff()
para remover
*/

#define MAX_TRERRITORIO 5 //tamanho de 5 territorios no total
struct territorio 
{
    char nome[30]; //nome territorio ate 29 + 1
    char cor[10];//cor do exercito
    int tropa;//numero de tropas
};

/* limpa o buffer de entrada ate o fim da linha para 
aliminar o scanf para consumir o /n evitando q o fgets leia ]
linhas vazias */

void limparBuff(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main(){
    //array para armazenar os territorios cadastrados
    struct territorio geral[MAX_TRERRITORIO];

    //mensagem inicial
    printf("=======================================\n");
    printf("VAMOS CADASTRAR OS %d TERRITORIOS\n", MAX_TRERRITORIO);
    printf("=========================================\n");

    //loop de cadastro, nome, cor e tropa de cada 1
        for(int i = 0; i < MAX_TRERRITORIO; i++){

            printf("Cadastrando territorio %d\n", i + 1);

        // Lê o nome do território. Usar `sizeof geral[i].nome` garante que
        // não lemos mais caracteres do que o buffer comporta.
        printf("Nome do Territorio: ");
        fgets(geral[i].nome, sizeof geral[i].nome, stdin);

        printf("Cor do exercito (ex azul, vermelho): ");
        fgets(geral[i].cor, sizeof geral[i].cor, stdin);

        
        // `fgets` pode manter o '\n' no final do buffer; removemos usando
        // `strcspn` para substituir o primeiro '\n' por terminador '\0'.
        geral[i].nome[strcspn(geral[i].nome, "\n")] = '\0';
        geral[i].cor[strcspn(geral[i].cor, "\n")] = '\0';

         // Lê o número de tropas como inteiro. Após `scanf`, chamamos
        // `limparBuff()` para descartar o '\n' sobrante.
        printf("Número da tropa: ");
        scanf("%d", &geral[i].tropa);
        limparBuff();
        printf("\n");
        }

        //exibe o cadastro completo com loops
        printf("\n");
        for(int i = 0; i < MAX_TRERRITORIO; i++){
            printf("---------------------------------\n");
            printf("Territorio %d\n", i + 1);
            printf("-- Nome do territorio: %s\n", geral[i].nome);
            printf("-- Dominado por: exercito %s\n", geral[i].cor);
            printf("-- Número da tropa: %d\n", geral[i].tropa);
        }
        printf("-------------------------------------\n");
        printf("\n");
        return 0;
}

