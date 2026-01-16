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

struct jogador
{
    char nome[20];
    char cor[10];
    struct missao missaoAtual;
    int vitoriasConsecutivas;
};



// ===== PROTÓTIPOS void =====
void limparBuffer();
void cadastrarTerritorios(struct territorio *mapa);
void exibirTerritorios(struct territorio *mapa, struct jogador *jogadores);
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
    struct jogador jogadores[2];

    
    int jogadorDaVez = 0;// 0 para Azul, 1 para Vermelho
    int atacante, defensor;       // Índices dos territórios que atacam e defendem
    int jogadorAtivo = 1;         // Flag que controla se o jogo continua rodando
    int turno = 0; // 0 = jogador 1 | 1 = jogador 2

    srand(time(NULL));

    strcpy(jogadores[0].nome, "jogador 1");
    strcpy(jogadores[0].cor, "vermelho");
    jogadores[0].vitoriasConsecutivas = 0;
    sortearMissao(&jogadores[0].missaoAtual);

    strcpy(jogadores[1].nome, "jogador 2");
    strcpy(jogadores[1].cor, "azul");
    jogadores[1].vitoriasConsecutivas = 0;
    sortearMissao(&jogadores[1].missaoAtual);
  
    
    // Inicializa o gerador de números aleatórios com a hora atual

    // ===== INICIALIZAÇÃO DO JOGO =====
    
    // Cadastra todos os territórios do mapa
    cadastrarTerritorios(mapa);

    exibirTerritorios(mapa, jogadores);
    

    
    printf("\n>>> DEFINICAO DE OBJETIVOS <<<\n");

printf("%s (%s): %s\n",
       jogadores[0].nome,
       jogadores[0].cor,
       jogadores[0].missaoAtual.descricao);

printf("%s (%s): %s\n",
       jogadores[1].nome,
       jogadores[1].cor,
       jogadores[1].missaoAtual.descricao);


    while (jogadorAtivo) {
        struct jogador *jogadorAtual = &jogadores[turno];

        printf("\n================================\n");
        printf("   VEZ DO JOGADOR: %s  (%s)\n", jogadorAtual->nome, jogadorAtual->cor);
        printf("\n================================\n");

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
                printf("Erro: Territorio invalido ou sem tropas suficientes!\n");
                break;
            }

            // VALIDAÇÃO 2: O território pertence ao jogador da vez?
            if (strcmp(mapa[atacante-1].cor, jogadorAtual->cor) != 0) {
            printf("Erro: Voce so pode atacar com territorios da cor %s!\n", jogadorAtual->cor);
            break;
            }


           
            printf("Escolha o territorio DEFENSOR (1-%d): ", MAX_TERRITORIO);
            scanf("%d", &defensor);
            limparBuffer();

            // VALIDAÇÃO 3: Não pode atacar a si mesmo
            if (strcmp(mapa[defensor-1].cor, jogadorAtual->cor) == 0) {
            printf("Erro: Voce nao pode atacar seu proprio territorio!\n");
            break;
            }

            if (defensor < 1 || defensor > MAX_TERRITORIO || defensor == atacante || mapa[defensor-1].tropa == 0) {
                printf("Defensor invalido !\n");
                break;
            }

             int resultado = batalha(&mapa[atacante-1], &mapa[defensor -1]);

            if (resultado == VITORIA_ATACANTE) {
             jogadorAtual->vitoriasConsecutivas++;
            }
            else if (resultado == VITORIA_DEFENSOR) {
            jogadorAtual->vitoriasConsecutivas = 0;
            }
        // empate não muda

            // Exibe o mapa atualizado
            printf("==============Mapa Atual================\n");
            for (int i = 0; i < MAX_TERRITORIO; i++ ) {
                printf("%-2d %-20s (Exercito %-10s, Tropas %d)\n", 
                    i + 1, 
                    mapa[i].nome, 
                    mapa[i].cor,
                    mapa[i].tropa);
            }
            printf("===================================\n");

            turno = (turno + 1) % 2;

        
        break;
        
        
    case 2:
        // CASO 2: exibe a missao atual e andamento
        mostrarAndamentoMissao( &jogadorAtual->missaoAtual, mapa, jogadorAtual->cor, jogadorAtual->vitoriasConsecutivas);
        break;

    case 0:
        // CASO 0: Encerrar o jogo imediatamente
        jogadorAtivo = 0;
        break;
        default: 
        printf("Opção invalida!!");

    }
    
    // Verifica se a missão inicial foi concluída
    if(verificarMissao(&jogadorAtual->missaoAtual, mapa, jogadorAtual->cor, jogadorAtual->vitoriasConsecutivas)) {

        printf("\n🏆 MISSÃO CONCLUÍDA!\n");
        printf("VENCEDOR:  %s\n", jogadorAtual->cor);

        jogadorAtivo = 0;
    }
    
}
        
}



// ===== IMPLEMENTAÇÃO DOS VOIDS ==============================

void limparBuffer() {
    // Função que limpa o buffer de entrada do teclado
    // Necessária após usar scanf() para remover a quebra de linha
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void cadastrarTerritorios(struct territorio *mapa) {
    printf("\n+++++Um jogador é azul e outro é vermelho+++++\n\n");
    printf("\n+++++Na opção 2 do menu você consulta sua missao+++++\n");
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

void exibirTerritorios(struct territorio *mapa, struct jogador *jogadores) {
    // Função que exibe todos os territórios cadastrados)
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
    int existeVermelho = 0;

    for (int i = 0; i < MAX_TERRITORIO; i++) {
        if (strcmp(mapa[i].cor, "vermelho") == 0) {
            existeVermelho++;
            break;
        }
    }

    printf("Territórios vermelhos restantes: %d\n", existeVermelho);
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

            defensor->tropa = 1;  // O novo território ganha 1 exército
            atacante->tropa--;    // O atacante perde 1 exército que foi para lá
            return 1;
        }
        
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

    switch (m->tipo) {

        //=========================================================

        case MISS_CONQUISTAR_2:// MISSÃO TIPO 1: Conquistar 2 territórios inimigos

        for (int i = 0; i < MAX_TERRITORIO; i++) {
            // Conta quantos territórios o jogador possui
            if (strcmp(mapa[i].cor, corjogador) == 0) {
                contador++;
            }
        }
        // Se conquistou 2 ou mais, missão completa
        return (contador >= 2);

        //=========================================================
        
        case MISS_3_VITORIAS:
        return (vitoriasConsecutivas >= 3);

        //=========================================================

        case MISS_CONQUISTAR_5T:
         // Procura um território que pertence ao jogador E tem mais de 5 tropas
        for (int i = 0; i < MAX_TERRITORIO; i++) {
            if (strcmp(mapa[i].cor, corjogador) == 0 && mapa[i].tropa >= 5) {
                return 1; // Missão concluída
            }
        }
        return 0;

        //=========================================================

        case MISS_ELIMINAR_EXERCITO:
        for (int i = 0; i < MAX_TERRITORIO; i++) {

            //se tiver algum territorio inimigo com tropas
            if (strcmp(mapa[i].cor, corjogador) != 0 && mapa[i].tropa > 0) {
               return 0; // nao concluida
            }
            
        }
        //se saiu do loop nao tem mais exercito inimigo
        return 1;

        //=========================================================

        case MISS_ELIMINAR_VERMELHO:
        if(strcmp(corjogador, "vermelho") == 0 ) {

            return 0;// jogador vermelho não pode ganhar essa missão
        }

         for (int i = 0; i < MAX_TERRITORIO; i++) {

            if(strcmp(mapa[i].cor, "vermelho") == 0 && 
            mapa[i].tropa > 0) {
                return 0;
            }
        }
        return 1;

        //==========================================================
        default:
        return 0;

    }

}