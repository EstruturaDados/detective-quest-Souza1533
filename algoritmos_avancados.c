#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura de uma sala (nó da árvore)
struct No {
    char valor[20];
    struct No* sala_esquerda;
    struct No* sala_direita;
};

// Cria uma nova sala
struct No* CriarSala(char* valor) {
    struct No* novo = (struct No*) malloc(sizeof(struct No));
    if (novo == NULL) {
        printf("Erro ao alocar memoria!\n");
        exit(1);
    }
    strcpy(novo->valor, valor);
    novo->sala_esquerda = NULL;
    novo->sala_direita = NULL;
    return novo;
}

// Explora as salas recursivamente
void ExplorarSalas(struct No* raiz) {
    if (raiz != NULL) {
        char d_[2];
        printf("\nVoce esta na %s\n", raiz->valor);
        printf("Digite 'd' para direita, 'e' para esquerda ou 's' para sair: ");
        scanf("%1s", d_);

        if (strcmp(d_, "d") == 0 || strcmp(d_, "D") == 0) {
            ExplorarSalas(raiz->sala_direita);
        } else if (strcmp(d_, "e") == 0 || strcmp(d_, "E") == 0) {
            ExplorarSalas(raiz->sala_esquerda);
        } else if (strcmp(d_, "s") == 0 || strcmp(d_, "S") == 0) {
            printf("Saindo da funcao.\n");
            return;
        } else {
            printf("Entrada invalida\n");
        }
    } else {
        printf("\nNao ha sala nessa direcao.\n");
    }
}

// Cria o mapa de salas
struct No* CriandoSalas() {
    struct No* raiz = CriarSala("Sala Principal");

    // Lado esquerdo
    raiz->sala_esquerda = CriarSala("Quarto");
    raiz->sala_esquerda->sala_esquerda = CriarSala("Dormitorio");
    raiz->sala_esquerda->sala_direita = CriarSala("Banheiro");

    // Lado direito
    raiz->sala_direita = CriarSala("Cozinha");
    raiz->sala_direita->sala_esquerda = CriarSala("Sala_de_estar");
    raiz->sala_direita->sala_direita = CriarSala("Copa");

    return raiz;
}

// Menu principal do jogo
void Menu() {
    int menu_choice;
    struct No* raiz = CriandoSalas();
    do {
        printf("\n==== Detetive Quest ===\n\n");
        printf("1) Explorar Salas\n");
        printf("2) Sair\n\n-> ");
        scanf("%d", &menu_choice);

        switch (menu_choice) {
            case 1:
                ExplorarSalas(raiz);
                break;
            case 2:
                printf("Encerrando o jogo...\n");
                break;
            default:
                printf("\nOpcao invalida!\n");
                break;
        }
    } while (menu_choice != 2);
}

int main() {
    Menu();
    return 0;
}
