#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAM_TABELA 10

//---------------------------------------------
// ESTRUTURA DA ARVORE BINARIA DE COMODOS
//---------------------------------------------
typedef struct Sala {
    char nome[30];
    char pista[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

//---------------------------------------------
// ESTRUTURA DA ARVORE BST DE PISTAS
//---------------------------------------------
typedef struct NoPista {
    char pista[50];
    int quantidade; // Quantas vezes a pista foi achada
    struct NoPista* esquerda;
    struct NoPista* direita;
} NoPista;

//---------------------------------------------
// ESTRUTURA DA TABELA HASH DE SUSPEITOS
//---------------------------------------------
typedef struct {
    char pista[50];
    char suspeito[30];
} ItemHash;

ItemHash tabela_hash[TAM_TABELA];

//---------------------------------------------
// FUNCAO HASH
//---------------------------------------------
int hash(char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        soma += tolower(chave[i]);
    }
    return soma % TAM_TABELA;
}

//---------------------------------------------
// INSERE PISTA E SUSPEITO NA HASH
//---------------------------------------------
void inserirNaHash(char* pista, char* suspeito) {
    int indice = hash(pista);
    strcpy(tabela_hash[indice].pista, pista);
    strcpy(tabela_hash[indice].suspeito, suspeito);
}

//---------------------------------------------
// PROCURA SUSPEITO A PARTIR DE UMA PISTA
//---------------------------------------------
char* encontrarSuspeito(char* pista) {
    int indice = hash(pista);
    if (strcmp(tabela_hash[indice].pista, pista) == 0)
        return tabela_hash[indice].suspeito;
    return "Desconhecido";
}

//---------------------------------------------
// CONVERTE TEXTO PARA MINUSCULO
//---------------------------------------------
void paraMinusculo(char* texto) {
    for (int i = 0; texto[i]; i++)
        texto[i] = tolower(texto[i]);
}

//---------------------------------------------
// CRIA UMA SALA DINAMICAMENTE
//---------------------------------------------
Sala* criarSala(char* nome, char* pista) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    paraMinusculo(nova->pista);
    nova->esquerda = nova->direita = NULL;
    return nova;
}

//---------------------------------------------
// INSERE UMA PISTA NA BST (ou incrementa se ja existir)
//---------------------------------------------
NoPista* inserirPista(NoPista* raiz, char* pista) {
    if (raiz == NULL) {
        NoPista* novo = (NoPista*)malloc(sizeof(NoPista));
        strcpy(novo->pista, pista);
        novo->quantidade = 1;
        novo->esquerda = novo->direita = NULL;
        return novo;
    }

    int cmp = strcmp(pista, raiz->pista);
    if (cmp < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (cmp > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    else
        raiz->quantidade++;

    return raiz;
}

//---------------------------------------------
// MOSTRA TODAS AS PISTAS COLETADAS E QUANTIDADE
//---------------------------------------------
void listarPistas(NoPista* raiz) {
    if (raiz == NULL) return;
    
    listarPistas(raiz->esquerda);
    printf("- %s (x%d)\n", raiz->pista, raiz->quantidade);
    listarPistas(raiz->direita);
}

//---------------------------------------------
// CONTA QUANTAS PISTAS APONTAM PARA UM SUSPEITO
//---------------------------------------------
void contarPistasSuspeito(NoPista* raiz, char* suspeito, int* contador) {
    if (raiz == NULL) return;
    char* sus = encontrarSuspeito(raiz->pista);
    if (strcmp(sus, suspeito) == 0)
        (*contador) += raiz->quantidade;
    contarPistasSuspeito(raiz->esquerda, suspeito, contador);
    contarPistasSuspeito(raiz->direita, suspeito, contador);
}

//---------------------------------------------
// JULGAMENTO FINAL
//---------------------------------------------
void verificarSuspeitoFinal(NoPista* arvore_pistas) {
    char suspeito[30];
    printf("\nDigite o nome do suspeito que deseja acusar: ");
    scanf(" %[^\n]", suspeito);
    paraMinusculo(suspeito);

    int contador = 0;
    contarPistasSuspeito(arvore_pistas, suspeito, &contador);
    
    if (contador >= 3) {
        printf("\nVoce acertou! %s e o verdadeiro culpado!\n", suspeito);
    } else if (contador == 0) {
        printf("\nNao ha pistas que apontem para esse suspeito.\n");
    } else {
        printf("\n%d pistas apontam para %s\n", contador, suspeito);
        printf("\nAcusacao incorreta. %s nao tem pistas suficientes.\n", suspeito);
    }
}

//---------------------------------------------
// EXPLORA AS SALAS DA MANSAO
//---------------------------------------------
void explorarSalas(Sala* atual, NoPista** arvore_pistas) {
    if (atual == NULL) return;

    printf("\nVoce esta na sala: %s\n", atual->nome);
    printf("Pista encontrada: %s\n", atual->pista);
    *arvore_pistas = inserirPista(*arvore_pistas, atual->pista);

    char opcao;
    printf("\nEscolha o caminho: (e) esquerda, (d) direita, (s) sair: ");
    scanf(" %c", &opcao);

    if (opcao == 'e')
        explorarSalas(atual->esquerda, arvore_pistas);
    else if (opcao == 'd')
        explorarSalas(atual->direita, arvore_pistas);
    else if (opcao == 's') {
        printf("\nSaindo da exploracao...\n");
        return; // <- sai totalmente
    }

}

//---------------------------------------------
// FUNCAO PRINCIPAL
//---------------------------------------------
int main() {
    // Cria a estrutura de salas
    Sala* raiz = criarSala("Entrada", "Pegada suspeita");
    raiz->esquerda = criarSala("Biblioteca", "Luvas de couro");
    raiz->direita = criarSala("Cozinha", "Faca ensanguentada");
    raiz->esquerda->esquerda = criarSala("Sala secreta", "Chave dourada");
    raiz->esquerda->direita = criarSala("Quarto", "Carta misteriosa");
    raiz->direita->esquerda = criarSala("Jardim", "Pegada de bota");
    raiz->direita->direita = criarSala("Porao", "Relogio quebrado");

    // Cria a tabela hash
    inserirNaHash("pegada suspeita", "carlos");
    inserirNaHash("luvas de couro", "maria");
    inserirNaHash("faca ensanguentada", "carlos");
    inserirNaHash("chave dourada", "ana");
    inserirNaHash("carta misteriosa", "carlos");
    inserirNaHash("pegada de bota", "ana");
    inserirNaHash("relogio quebrado", "maria");

    NoPista* arvore_pistas = NULL;
    int opcao;

    do {
        printf("\n========== MENU ==========\n");
        printf("1. Explorar salas\n");
        printf("2. Mostrar pistas coletadas\n");
        printf("3. Acusar suspeito\n");
        printf("0. Sair\n");
        printf("==========================\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                explorarSalas(raiz, &arvore_pistas);
                break;
            case 2:
                if (arvore_pistas == NULL){
                    printf("Nenhuma pista coletada ainda.\n");
                }else{ 
                    printf("\nPistas coletadas:\n");
                    listarPistas(arvore_pistas);
                }
                break;

            case 3:
                verificarSuspeitoFinal(arvore_pistas);
                break;
            case 0:
                printf("\nEncerrando o jogo...\n");
                break;
            default:
                printf("\nOpcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}
