#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

// =======================================
// Função para pausar e esperar ENTER
// =======================================
void Press_Enter() {
    int c;

    // Limpa qualquer lixo que possa estar no buffer
    // Isso é importante porque scanf deixa o '\n' no buffer
    while ((c = getchar()) != '\n' && c != EOF);

    // Mensagem para o usuário
    printf("\nPressione ENTER para continuar...");

    // Aguarda o usuário apertar ENTER
    getchar();
}

// =======================================
// Definições para a Árvore TRIE (armazenar palavras)
// =======================================
#define ALPHABET 26       // Número de letras do alfabeto
#define MAX_PALAVRAS 250  // Tamanho máximo do buffer de palavras

// Estrutura de nó da TRIE
typedef struct NO_TRIE {
    struct NO_TRIE* filhos[ALPHABET]; // Ponteiros para os próximos nós (26 letras)
    bool endword;                      // Indica se é fim de uma palavra
    int count;                         // Contagem de vezes que a palavra foi inserida
} NO_TRIE;

// =======================================
// Cria um novo nó da TRIE
// =======================================
NO_TRIE* CriarNoTrie() {
    NO_TRIE* novo = (NO_TRIE*)malloc(sizeof(NO_TRIE));
    novo->endword = false;   // Inicialmente não é final de palavra
    novo->count = 0;         // Nenhuma ocorrência ainda
    for (int i = 0; i < ALPHABET; i++)
        novo->filhos[i] = NULL; // Inicializa todos os filhos como NULL
    return novo;
}

// =======================================
// Insere uma palavra na TRIE
// =======================================
void InserirNoTrie(NO_TRIE* raiz_trie, char* palavra) {
    NO_TRIE* atual = raiz_trie;

    for (int i = 0; palavra[i] != '\0'; i++) {
        char c = tolower(palavra[i]);      // Converte para minúscula
        if (c < 'a' || c > 'z') continue;  // Ignora caracteres que não são letras
        int index = c - 'a';               // Calcula índice no array de filhos

        if (atual->filhos[index] == NULL) {
            atual->filhos[index] = CriarNoTrie(); // Cria nó se não existir
        }
        atual = atual->filhos[index]; // Avança para o próximo nó
    }

    atual->endword = true; // Marca como fim de palavra
    atual->count++;         // Incrementa contagem de vezes que a palavra foi inserida
}

// =======================================
// Lista todas as palavras da TRIE com contagem
// =======================================
void ListarPistas(NO_TRIE* raiz_trie, char buffer[], int nivel) {
    static int contador = 0; // Contador global para organizar saída

    // Se o nó atual marca fim de palavra
    if (raiz_trie->endword) {
        buffer[nivel] = '\0';                       // Finaliza string
        printf("%s(%d) ", buffer, raiz_trie->count); // Mostra palavra + quantidade
        contador++;

        if (contador % 4 == 0) // A cada 4 palavras
            printf("\n");      // Quebra a linha
    }

    // Percorre todos os filhos (a-z)
    for (int i = 0; i < 26; i++) {
        if (raiz_trie->filhos[i] != NULL) {
            buffer[nivel] = 'a' + i;               // Adiciona letra ao buffer
            ListarPistas(raiz_trie->filhos[i], buffer, nivel + 1); // Recursão
        }
    }
}

// =======================================
// Definições para Árvore de Salas
// =======================================
#define NAME 20

typedef struct NO {
    char name[NAME];   // Nome da sala
    struct NO* esquerda; // Ponteiro para sala esquerda
    struct NO* direita;  // Ponteiro para sala direita
} NO;

// =======================================
// Cria um novo nó (sala)
// =======================================
NO* CriarNo(char* new_name) {
    NO* novo = (NO*)malloc(sizeof(NO));
    if (novo == NULL) {
        printf("Erro ao alocar memoria\n");
        return NULL;
    }
    strcpy(novo->name, new_name); // Copia o nome
    novo->direita = NULL;
    novo->esquerda = NULL;
    return novo;
}

// =======================================
// Sorteia uma pista aleatória com chance de não achar nada
// =======================================
char* SortearPista() {
    char* pistas[] = {
        "Chaves", "Sapatos", "Escovas",
        "Livro", "PenDrive", "Cartao",
        "Mouse", "Celular"
    };
    int total = sizeof(pistas) / sizeof(pistas[0]);
    int chance = rand() % (total + 2); // Chance extra de não achar

    if (chance >= total) // Não encontrou pista
        return NULL;

    return pistas[chance]; // Retorna a pista sorteada
}

// =======================================
// Função principal para explorar salas
// =======================================
void ExplorarSalas(struct NO* raiz, NO_TRIE* raiz_trie) {
    if (raiz != NULL) {
        char d_[2];
        printf("\nVoce esta na %s\n", raiz->name);
        printf("Digite 'd' para direita, 'e' para esquerda ou 's' para sair: ");
        scanf("%1s", d_); // Lê apenas 1 caractere

        if (strcmp(d_, "d") == 0 || strcmp(d_, "D") == 0) {
            char* pista_encontrada = SortearPista();
            if (pista_encontrada != NULL) {
                printf("Voce encontrou a pista: %s!\n", pista_encontrada);
                InserirNoTrie(raiz_trie, pista_encontrada);
            } else {
                printf("Voce procurou, mas nao encontrou nenhuma pista.\n");
            }
            ExplorarSalas(raiz->direita, raiz_trie); // Recursão para próxima sala

        } else if (strcmp(d_, "e") == 0 || strcmp(d_, "E") == 0) {
            char* pista_encontrada = SortearPista();
            if (pista_encontrada != NULL) {
                printf("Voce encontrou a pista: %s!\n\n", pista_encontrada);
                InserirNoTrie(raiz_trie, pista_encontrada);
            } else {
                printf("Voce procurou, mas nao encontrou nenhuma pista.\n");
            }
            ExplorarSalas(raiz->esquerda, raiz_trie); // Recursão para próxima sala

        } else if (strcmp(d_, "s") == 0 || strcmp(d_, "S") == 0) {
            printf("Saindo da funcao.\n");
            return; // Sai da função
        } else {
            printf("Entrada invalida.\n");
        }
    } else {
        printf("\nNao ha sala nessa direcao.\n");
    }
}

// =======================================
// Cria a árvore de salas do jogo
// =======================================
NO* CriandoSalas() {
    NO* raiz = CriarNo("Entrada");

    // Lado esquerdo
    raiz->esquerda = CriarNo("Quarto");
    raiz->esquerda->esquerda = CriarNo("Dormitorio");
    raiz->esquerda->direita = CriarNo("Banheiro");

    // Lado direito
    raiz->direita = CriarNo("Cozinha");
    raiz->direita->esquerda = CriarNo("Sala de estar");
    raiz->direita->direita = CriarNo("Copa");

    return raiz;
}

// =======================================
// Menu principal do jogo
// =======================================
void Menu() {
    int menu_choice;
    char buffer[MAX_PALAVRAS];
    int nivel = 0;
    srand(time(NULL)); // Inicializa gerador de números aleatórios

    // Criação das estruturas principais
    NO_TRIE* raiz_trie = CriarNoTrie();
    NO* raiz = CriandoSalas();

    do {
        printf("\n============( Detetive Quest )============\n\n");
        printf("1) Explorar Salas\n");
        printf("2) Listar Pistas Encontradas\n");
        printf("3) Sair\n\n-> ");
        scanf("%d", &menu_choice); // Lê escolha do usuário

        switch (menu_choice) {
            case 1:
                ExplorarSalas(raiz, raiz_trie);
                break;
            case 2:
                printf("\n-----------( Pistas Coletadas )-----------\n\n");
                ListarPistas(raiz_trie, buffer, nivel);
                printf("\n\n------------------------------------------\n");
                Press_Enter(); // Pausa para o usuário ver as pistas
                break;
            case 3:
                printf("Encerrando o jogo...\n");
                break;
            default:
                printf("\nOpcao invalida!\n");
                Press_Enter(); // Pausa caso digite opção inválida
                break;
        }
    } while (menu_choice != 3);
}

// =======================================
// Função principal
// =======================================
int main() {
    Menu();
    return 0;
}
