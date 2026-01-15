#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TERRITORIO 5

#define VITORIA_ATACANTE 1
#define VITORIA_DEFENSOR -1
#define EMPATE 0

#define MISS_CONQUISTAR_2 1
#define MISS_ELIMINAR_EXERCITO 2
#define MISS_ELIMINAR_VERMELHO 3
#define MISS_3_VITORIAS 4
#define MISS_CONQUISTAR_5T 5

// ===== STRUCTS =====
struct territorio {
    char nome[30];
    char cor[10];
    int tropa;
};

struct missao {
    int tipo; //cada missão tem um tipo, que define a regra de vitória.
    char descricao[100];
};


// ===== PROTÓTIPOS void =====
void limparBuffer();
void cadastrarTerritorios(struct territorio *mapa);
void exibirTerritorios(struct territorio *mapa);
int batalha(struct territorio *atacante, struct territorio *defensor);
void sortearMissao(struct missao *m);
void exibirMissao(struct missao *m);
int menu();
int verificarMissao(struct missao *m, struct territorio *mapa, char *corJogador, int vitoriasConsecutivas);
void mostrarAndamentoMissao( struct missao *m, struct territorio *mapa, char *corjogador, int vitoriasConsecutivas);




int main() {
    // Função principal do programa - Controla o fluxo do jogo
    
    // Declaração das variáveis principais
    struct territorio mapa[MAX_TERRITORIO];  // Array de territórios do jogo
    struct missao missaoAtual;               // Missão que o jogador deve cumprir para vencer

    int vitoriasConsecutivas = 0; // Contador de vitórias seguidas (para a missão de 3 vitórias)
    int atacante, defensor;       // Índices dos territórios que atacam e defendem
    int jogadorAtivo = 1;         // Flag que controla se o jogo continua rodando
    char corJogador[10] = "vermelho"; // Cor do exército do jogador (exemplo)
    
    // Inicializa o gerador de números aleatórios com a hora atual
    srand(time(NULL));

    // ===== INICIALIZAÇÃO DO JOGO =====
    
    // Cadastra todos os territórios do mapa
    cadastrarTerritorios(mapa);

    exibirTerritorios(mapa);
    
    // Sorteia uma missão aleatória para o jogador
    sortearMissao(&missaoAtual);
    
    // Exibe a missão sorteada na tela
    exibirMissao(&missaoAtual);

    while (jogadorAtivo) {

    

    // Exibe o menu e aguarda a escolha do jogador
    int opcao = menu();

    // Switch que controla as ações principais do jogo
    switch (opcao)
    {
    case 1:
        // CASO 1: Iniciar o modo de ataque
            printf("----------FASE DE ATAQUE ----------\n");
            printf("Escolha o territorio ATACANTE (1-%d): ", MAX_TERRITORIO);
            scanf("%d", &atacante);
            limparBuffer();

            if (atacante == 0) break;

            if (atacante < 1 || atacante > MAX_TERRITORIO || mapa[atacante-1].tropa == 0) {
                printf("Atacante invalido!\n");
                break;
            }

           
            printf("Escolha o territorio DEFENSOR (1-%d): ", MAX_TERRITORIO);
            scanf("%d", &defensor);
            limparBuffer();

            if (defensor < 1 || defensor > MAX_TERRITORIO || defensor == atacante || mapa[defensor-1].tropa == 0) {
                printf("Defensor invalido !\n");
                break;
            }

            int resultado = batalha(&mapa[atacante-1], &mapa[defensor -1]);
            
            // Se o jogador é o ATACANTE
            if (resultado == VITORIA_ATACANTE) {
            vitoriasConsecutivas++;
            } else {
            vitoriasConsecutivas = 0;
            }


        // Se o jogador é o defensor
            if (strcmp(mapa[defensor-1].cor, corJogador) == 0) {
                if(resultado == VITORIA_DEFENSOR) {
                    vitoriasConsecutivas++;
                } else {
                    vitoriasConsecutivas = 0;
                }
            } else {
                vitoriasConsecutivas = 0;
            }

            
            
            // Exibe o mapa atualizado
            printf("==============Mapa Atual================\n");
            for (int i = 0; i < MAX_TERRITORIO; i++ ) {
                printf("%-2d %-20s (Exercito %-10s, Tropas %d)\n", 
                    i + 1, 
                    mapa[i].nome, 
                    mapa[i].cor,
                    mapa[i].tropa);
            }
        
        break;
        
    case 2:
        // CASO 2: exibe a missao atual e andamento
        mostrarAndamentoMissao( &missaoAtual, mapa, corJogador, vitoriasConsecutivas);
        break;

    case 0:
        // CASO 0: Encerrar o jogo imediatamente
        jogadorAtivo = 0;
        break;
        default: 
        printf("Opção invalida!!");

    }
    
    // Verifica se a missão inicial foi concluída
    if(verificarMissao(&missaoAtual, mapa, corJogador, vitoriasConsecutivas)) {
        printf("\n🏆 MISSÃO CONCLUÍDA!\n");
        printf("VENCEDOR: Exército %s\n", corJogador);
        jogadorAtivo = 0;
    }
    
}
        
        // Exibe o mapa atualizado após a batalha
        printf("===========Estado atual=============\n");
        for (int i = 0; i < MAX_TERRITORIO; i++ ) {
            printf("%-2d. %-20s (Exercito %-10s | Tropas %d)\n", 
                i + 1, 
                mapa[i].nome, 
                mapa[i].cor, 
                mapa[i].tropa);
        }
        printf("===================================================\n\n");
    }



// ===== IMPLEMENTAÇÃO DOS VOIDS ==============================

void limparBuffer() {
    // Função que limpa o buffer de entrada do teclado
    // Necessária após usar scanf() para remover a quebra de linha
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void cadastrarTerritorios(struct territorio *mapa) {
    // Função que registra os dados de cada território (nome, cor, número de tropas)
    // Recebe um ponteiro para o array de territórios
    
    for (int i = 0; i < MAX_TERRITORIO; i++) {
        printf("\nCadastro do território %d\n", i + 1);

        // Lê o nome do território usando fgets (mais seguro que scanf)
        printf("Nome: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0'; // Remove quebra de linha

        // Lê a cor do exército que controla este território
        printf("Cor: ");
        fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0'; // Remove quebra de linha

        // Lê o número inicial de tropas do território
        printf("Tropas: ");
        scanf("%d", &mapa[i].tropa);
        limparBuffer();
}

}

void exibirTerritorios(struct territorio *mapa) {
    // Função que exibe todos os territórios cadastrados (CORREÇÃO: Esta função estava misturada com a anterior)
    printf("\n=== TERRITÓRIOS CADASTRADOS ===\n");

    for (int i = 0; i < MAX_TERRITORIO; i++) {
        printf("%-3d. %-20s (Exército: %-10s | Tropas: %d)\n", 
                i + 1, 
                mapa[i].nome, 
                mapa[i].cor, 
                mapa[i].tropa); 
        // Formatação com espaçamento para melhor legibilidade:
        // %-3d  : Número do território (3 caracteres, alinhado à esquerda)
        // %-20s : Nome do território (20 caracteres, alinhado à esquerda)
        // %-10s : Cor do exército (10 caracteres, alinhado à esquerda)
        // %d    : Número de tropas
    }
}
    
void sortearMissao(struct missao *m) {
    // Função que seleciona aleatoriamente uma missão para o jogador
    // A missão define as condições de vitória do jogo

    // Array com todas as missões disponíveis
    struct missao missoes[] = {
        {MISS_CONQUISTAR_2, "conquiste 2 territorios inimigos"},
        {MISS_ELIMINAR_EXERCITO, "Elimine todas as tropas de um inimigo"},
        {MISS_ELIMINAR_VERMELHO, "Domine o territorio vermelho"},
        {MISS_3_VITORIAS, "Vença 3 batalhas consecutivas"},
        {MISS_CONQUISTAR_5T, "Conquiste um territorio com mais de 5 tropas"}
    };

    // Calcula o número total de missões disponíveis
    int total = sizeof(missoes) / sizeof(missoes[0]);
    
    // Sorteia um índice aleatório entre 0 e total-1
    int sorteio = rand() % total;

    // Copia a missão sorteada para o ponteiro m (enviada como parâmetro)
    *m = missoes[sorteio];
}

void exibirMissao(struct missao *m) {
    // Função que exibe a missão sorteada para o jogador
    // Mostra qual é o objetivo para vencer o jogo
    printf("\n===== MISSÃO SORTEADA =====\n");
    printf("Objetivo: %s\n", m->descricao);
    printf("============================\n");
}

void mostrarAndamentoMissao( struct missao *m, struct territorio *mapa, char *corjogador, int vitoriasConsecutivas) {
    printf("\n===Missão Atual===\n");
    printf("Objetivo: %s\n", m->descricao);

    //ANDAMENTO
    if (m->tipo == MISS_CONQUISTAR_2) {
        int count = 0;
        for(int i = 0; i < MAX_TERRITORIO; i++) {
            if (strcmp(mapa[i].cor, corjogador) == 0) {
                count++;
            }
        }
        printf("Progresso: %d / 2 territorios conquistados\n", count);
    }

    else if (m->tipo == MISS_3_VITORIAS) {
        printf("Progresso: %d / 3 vitorias consecutivas\n", vitoriasConsecutivas);
    }

    else if (m->tipo == MISS_CONQUISTAR_5T) {
        int atingiu = 0;
        for (int i = 0; i < MAX_TERRITORIO; i++) {
            if (strcmp(mapa[i].cor, corjogador) == 0 && mapa[i].tropa >= 5) {
                atingiu = 1;
            }
        }
        printf("Progresso: %s\n", atingiu ? "Objetivo atingido" : "Ainda não atingiu o objetivo");
    }

    else if (m->tipo == MISS_ELIMINAR_EXERCITO) {
        int inimigoRestante = 0;

        for (int i = 0; i < MAX_TERRITORIO; i++) {
            if (strcmp(mapa[i].cor, corjogador) != 0 && mapa[i].tropa > 0) {
                inimigoRestante++;
            }
        }

        printf("Progresso: %d territorio inimigos ainda ativos\n", inimigoRestante);
    }

    else if (m->tipo == MISS_ELIMINAR_VERMELHO) {
        int vermelhoRestantes = 0;

        for (int i = 0; i < MAX_TERRITORIO; i++){
            
            if(strcmp(mapa[i].cor, "vermelho") == 0 && mapa[i].tropa > 0) {
                vermelhoRestantes++;
            }
        }

        // se o jogador for veremlho, essa missao é invalida
        if (strcmp(corjogador, "vermelho") == 0) {
            printf("Esta missão não pode ser concluida por um exército vermelho.\n");
        }else {
             printf("Progresso: %d territorios vermelhos ainda ativos\n", vermelhoRestantes);
        }
    }

    //status final
    if (verificarMissao(m, mapa, corjogador, vitoriasConsecutivas)) {
        printf("STATUS: MISSÃO CONCLUIDA\n");
    }else {
        printf("STATUS: MISSÃO EM ANDAMENTO\n");
    }
    printf("========================================\n\n");

}


int batalha(struct territorio *atacante, struct territorio *defensor){
    // Função que simula uma batalha entre dois territórios
    // Usa rolagem de dados para determinar o vencedor
    
    // Rolar dados (1-6 para cada combatente)
    int dado_atacante = rand() % 6 + 1;
    int dado_defensor = rand() % 6 + 1;
    
    printf("*****************Resultado da batalha*****************\n\n");
    printf("Dado atacante (%s - Exercito %s) rolou: (%d)\n", atacante->nome, atacante->cor, dado_atacante);
    printf("Dado defensor (%s - Exercito %s) rolou: (%d)\n", defensor->nome, defensor->cor, dado_defensor);
    
    // Verifica quem venceu comparando os dados
    if (dado_atacante > dado_defensor) {
        printf("\n>>> VITÓRIA DO ATACANTE (%s)!\n", atacante->nome);
        defensor->tropa--;  // Desconta uma tropa do defensor
        printf(">>> %s perde 1 tropa! Tropas restantes: %d\n", defensor->nome, defensor->tropa);
        
        // Se o defensor perde todas as tropas, o atacante conquista o território
        if (defensor->tropa <= 0) {
            printf("\n ALERTA: %s foi CONQUISTADO pelo exercito %s!\n", defensor->nome, atacante->cor);
            printf(" O territorio agora pertence ao exercito %s!\n\n", atacante->cor);
            strcpy(defensor->cor, atacante->cor);  // Muda a cor do territorio para a do atacante
            defensor->tropa = 0;  // Marca conquistado com 0 tropas
        }
        return 1;
        
    } else if (dado_defensor > dado_atacante) {
        printf("\n>>> VITÓRIA DO DEFENSOR (%s)!\n", defensor->cor);
        atacante->tropa--;  // Desconta uma tropa do atacante
        printf(">>> %s perde 1 tropa! Tropas restantes: %d\n", atacante->nome, atacante->tropa);
        
        // Se o atacante perde todas as tropas, o defensor conquista o território
        if (atacante->tropa <= 0) {
            printf("\n ALERTA: %s foi CONQUISTADO pelo exercito %s!\n", atacante->nome, defensor->cor);
            printf(" O territorio agora pertence ao exercito %s!\n\n", defensor->cor);
            strcpy(atacante->cor, defensor->cor);  // Muda a cor do territorio para a do defensor
            atacante->tropa = 0;  // Marca conquistado com 0 tropas

        } else if (atacante->tropa == 0) {
            printf(">>> %s não pode atacar mais, sem tropas restantes!\n", atacante->nome);
        } else if(defensor->tropa == 0) {
            printf(">>> %s não pode se defender mais, sem tropas restantes!\n", defensor->nome);
        }

        return -1;

    } else {
        printf("\n>>> EMPATE! Nenhuma tropa é perdida.\n");
    }
    printf("==========================================\n\n");

    return 0;

}


int menu() {
    // Função que exibe o menu principal de ações do jogo
    // Permite ao jogador escolher entre atacar, ver o mapa ou encerrar
    int opcao;

    printf("\n========= MENU DE AÇÕES =========\n");
    printf("1 - Atacar\n");
    printf("2 - Status da missão\n");
    printf("0 - Encerrar jogo\n");
    printf("Escolha: ");
    scanf("%d", &opcao);
    limparBuffer(); // Limpa o buffer após a entrada

    return opcao; // Retorna a opção escolhida pelo jogador
}


int verificarMissao(struct missao *m, struct territorio *mapa, char *corjogador, int vitoriasConsecutivas) {
    // Função que verifica se a missão foi concluída de acordo com o tipo
    // Retorna 1 se missão completa, 0 se não completa

    int contador = 0;

    // MISSÃO TIPO 1: Conquistar 2 territórios inimigos==============
    if (m->tipo == MISS_CONQUISTAR_2) {
        // Conta quantos territórios o jogador possui
        for (int i = 0; i < MAX_TERRITORIO; i++) {
            if (strcmp(mapa[i].cor, corjogador) == 0) {
                contador++;
            }
        }
        // Se conquistou 2 ou mais, missão completa
        if (contador >= 2) {
            return 1; // Missão concluída
        }
    }

    // MISSÃO TIPO 2: Vencer 3 batalhas consecutivas================
    if (m->tipo == MISS_3_VITORIAS) {
        // Verifica se o jogador venceu 3 batalhas seguidas
        if(vitoriasConsecutivas >= 3) {
            return 1; // Missão concluída
        }
    }

    // MISSÃO TIPO 3: Conquistar um território com mais de 5 tropas===================
    if(m->tipo == MISS_CONQUISTAR_5T) {
        // Procura um território que pertence ao jogador E tem mais de 5 tropas
        for (int i = 0; i < MAX_TERRITORIO; i++) {
            if (strcmp(mapa[i].cor, corjogador) == 0 && mapa[i].tropa > 5) {
                return 1; // Missão concluída
            }
        }
    }

    //elimina todo o vermelho========================
    if(m->tipo == MISS_ELIMINAR_VERMELHO) {
        for (int i = 0; i < MAX_TERRITORIO; i++) {
            if(strcmp(mapa[i].cor, "vermelho") == 0 && strcmp(corjogador, "vermelho") != 0) {
                    return 0;
            }
        }
        return 1;
    }

    //eliminar todo o exercito===========================
    if (m->tipo == MISS_ELIMINAR_EXERCITO) {
        for (int i = 0; i < MAX_TERRITORIO; i++) {

            //se tiver algum territorio inimigo com tropas
            if (strcmp(mapa[i].cor, corjogador) != 0 && mapa[i].tropa > 0) {
               return 0; // nao concluida
            }
            
        }
        //se saiu do loop nao tem mais exercito inimigo
        return 1;
    }


    return 0; // Nenhuma missão foi concluída
}