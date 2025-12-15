#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

/* Funcao para processar a batalha entre dois territorios
   Recebe ponteiros para os dois territorios em conflito
   Desconta tropas do perdedor e exibe resultado */
void batalha(struct territorio *atacante, struct territorio *defensor){
    
    // Rolar dados (1d6)
    int dado_atacante = rand() % 6 + 1;
    int dado_defensor = rand() % 6 + 1;
    
    printf("*****************Resultado da batalha*****************\n\n");
    printf("Dado atacante (%s - Exercito %s) rolou: (%d)\n", atacante->nome, atacante->cor, dado_atacante);
    printf("Dado defensor (%s - Exercito %s) rolou: (%d)\n", defensor->nome, defensor->cor, dado_defensor);
    
    if (dado_atacante > dado_defensor) {
        printf("\n>>> VITÓRIA DO ATACANTE (%s)!\n", atacante->nome);
        defensor->tropa--;  // Desconta uma tropa do defensor
        printf(">>> %s perde 1 tropa! Tropas restantes: %d\n", defensor->nome, defensor->tropa);
        
        // Verifica se o defensor perdeu o territorio
        if (defensor->tropa <= 0) {
            printf("\n ALERTA: %s foi CONQUISTADO pelo exercito %s!\n", defensor->nome, atacante->cor);
            printf(" O territorio agora pertence ao exercito %s!\n\n", atacante->cor);
            strcpy(defensor->cor, atacante->cor);  // Muda a cor do territorio
            defensor->tropa = 0;  // Marca conquistado com 0 tropas (exibir 0 no mapa)
        }
        
    } else if (dado_defensor > dado_atacante) {
        printf("\n>>> VITÓRIA DO DEFENSOR (%s)!\n", defensor->cor);
        atacante->tropa--;  // Desconta uma tropa do atacante
        printf(">>> %s perde 1 tropa! Tropas restantes: %d\n", atacante->nome, atacante->tropa);
        
        // Verifica se o atacante perdeu o territorio
        if (atacante->tropa <= 0) {
            printf("\n ALERTA: %s foi CONQUISTADO pelo exercito %s!\n", atacante->nome, defensor->cor);
            printf(" O territorio agora pertence ao exercito %s!\n\n", defensor->cor);
            strcpy(atacante->cor, defensor->cor);  // Muda a cor do territorio
            atacante->tropa = 0;  // Marca conquistado com 0 tropas (exibir 0 no mapa)
        } else if (atacante->tropa == 0) {
            printf(">>> %s não pode atacar mais, sem tropas restantes!\n", atacante->nome);
        }else if(defensor->tropa == 0){
            printf(">>> %s não pode se defender mais, sem tropas restantes!\n", defensor->nome);
        }
        
    } else {
        printf("\n>>> EMPATE! Nenhuma tropa é perdida.\n");
    }
    printf("==========================================\n\n");
}

int main(){

    // ponteiro para armazenar os territorios (alocado dinamicamente)
    struct territorio *geral = calloc(MAX_TRERRITORIO, sizeof *geral);
    if (!geral) { perror("malloc"); return 1; }

    // inicializa o gerador de números aleatórios
    srand(time(NULL));

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

        printf("===========Mapa do Mundo - Estado atual=============\n");
            for (int i = 0; i < MAX_TRERRITORIO; i++ ) {
                printf("%d %s (Exercito %s, Tropas %d)\n", i + 1, geral[i].nome, geral[i].cor, geral[i].tropa);
            }
             printf("\nPressione Enter para continuar...");
            getchar();

        // Fase de ataque
        int atacante, defensor;
        do {
            printf("----------FASE DE ATAQUE ----------\n");
            printf("Escolha o territorio atacante (1-%d ou 0 para sair): ", MAX_TRERRITORIO);
            scanf("%d", &atacante);
            limparBuff();
            if (atacante == 0) break;
            if (atacante < 1 || atacante > MAX_TRERRITORIO) {
                printf("Territorio invalido!\n");
                continue;
            }
            // Impede que um territorio sem tropas seja usado como atacante
            if (geral[atacante-1].tropa == 0) {
                printf("Territorio '%s' sem tropas e nao pode atacar!\n", geral[atacante-1].nome);
                continue;
            }
            printf("Escolha o territorio defensor (1-%d): ", MAX_TRERRITORIO);
            scanf("%d", &defensor);
            limparBuff();
            if (defensor < 1 || defensor > MAX_TRERRITORIO || defensor == atacante) {
                printf("Territorio invalido ou mesmo que atacante!\n");
                continue;
            }
            // Impede que um territorio sem tropas seja escolhido como defensor
            if (geral[defensor-1].tropa == 0) {
                printf("Territorio '%s' sem tropas e nao pode ser defendido (ja dominado)!\n", geral[defensor-1].nome);
                continue;
            }
             printf("\nPressione Enter para continuar...");
            getchar();
            
            // Chama a funcao de batalha passando ponteiros para os dois territorios
            batalha(&geral[atacante-1], &geral[defensor-1]);
            
            // Exibe o mapa atualizado
            printf("===========Mapa do Mundo - Estado atual=============\n");
            for (int i = 0; i < MAX_TRERRITORIO; i++ ) {
                printf("%d %s (Exercito %s, Tropas %d)\n", i + 1, geral[i].nome, geral[i].cor, geral[i].tropa);
            }
            printf("===================================================\n\n");
        } while (1);
        
       
        //libera a memoria alocada
        free(geral);
        return 0;
}