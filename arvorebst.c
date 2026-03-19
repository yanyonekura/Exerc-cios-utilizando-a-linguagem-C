#include <stdio.h>
#include <stdlib.h>

// ====================================================
//                ESTRUTURA DO NÓ DA BST
// ====================================================
typedef struct No {
    int valor;
    struct No *esq;
    struct No *dir;
} No;

// ====================================================
//                 CRIAÇÃO DE UM NOVO NÓ
// ====================================================
No* criarNo(int valor) {
    No* novo = (No*) malloc(sizeof(No));
    if (novo == NULL) {
        printf("ERRO: Falha ao alocar memoria!\n");
        exit(1);
    }
    novo->valor = valor;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

// ====================================================
//                     INSERÇÃO NA BST
// ====================================================
No* inserir(No* raiz, int valor) {
    if (raiz == NULL)
        return criarNo(valor);

    if (valor < raiz->valor)
        raiz->esq = inserir(raiz->esq, valor);
    else if (valor > raiz->valor)
        raiz->dir = inserir(raiz->dir, valor);
    else
        printf("Valor repetido! Ignorando insercao.\n");

    return raiz;
}

// ====================================================
//                       BUSCA NA BST
// ====================================================
int buscar(No* raiz, int valor) {
    if (raiz == NULL) 
        return 0;
    if (valor == raiz->valor) 
        return 1;
    if (valor < raiz->valor) 
        return buscar(raiz->esq, valor);
    return buscar(raiz->dir, valor);
}

// ====================================================
//        ENCONTRAR O MÍNIMO DA SUBÁRVORE DIREITA
// ====================================================
No* minimo(No* raiz) {
    while (raiz->esq != NULL)
        raiz = raiz->esq;
    return raiz;
}

// ====================================================
//                     REMOÇÃO NA BST
// ====================================================
No* remover(No* raiz, int valor) {

    if (raiz == NULL)
        return NULL;

    if (valor < raiz->valor) {
        raiz->esq = remover(raiz->esq, valor);
    }
    else if (valor > raiz->valor) {
        raiz->dir = remover(raiz->dir, valor);
    }
    else {
        // Caso 1: Nó folha
        if (raiz->esq == NULL && raiz->dir == NULL) {
            free(raiz);
            return NULL;
        }
        // Caso 2: Nó com um filho
        if (raiz->esq == NULL) {
            No* temp = raiz->dir;
            free(raiz);
            return temp;
        }
        if (raiz->dir == NULL) {
            No* temp = raiz->esq;
            free(raiz);
            return temp;
        }
        // Caso 3: Nó com dois filhos
        No* sucessor = minimo(raiz->dir);
        raiz->valor = sucessor->valor;
        raiz->dir = remover(raiz->dir, sucessor->valor);
    }

    return raiz;
}

// ====================================================
//                      PERCURSOS
// ====================================================
void preOrdem(No* raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->valor);
        preOrdem(raiz->esq);
        preOrdem(raiz->dir);
    }
}

void emOrdem(No* raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esq);
        printf("%d ", raiz->valor);
        emOrdem(raiz->dir);
    }
}

void posOrdem(No* raiz) {
    if (raiz != NULL) {
        posOrdem(raiz->esq);
        posOrdem(raiz->dir);
        printf("%d ", raiz->valor);
    }
}

// ====================================================
//                LIBERAÇÃO DE TODA A ÁRVORE
// ====================================================
void liberarArvore(No* raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esq);
        liberarArvore(raiz->dir);
        free(raiz);
    }
}

// ====================================================
//                      MENU PRINCIPAL
// ====================================================
int main() {
    No* raiz = NULL;
    int opcao = -1, valor;

    while (opcao != 0) {
        printf("\n====== MENU PRINCIPAL ======\n");
        printf("1 - Inserir valor\n");
        printf("2 - Buscar valor\n");
        printf("3 - Remover valor\n");
        printf("4 - Percorrer arvore\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
        case 1:
            printf("Digite o valor a inserir: ");
            scanf("%d", &valor);
            raiz = inserir(raiz, valor);
            break;

        case 2:
            printf("Digite o valor a buscar: ");
            scanf("%d", &valor);
            if (buscar(raiz, valor))
                printf("Valor encontrado!\n");
            else
                printf("Valor NAO encontrado.\n");
            break;

        case 3:
            printf("Digite o valor a remover: ");
            scanf("%d", &valor);
            raiz = remover(raiz, valor);
            break;

        case 4: {
            int op2;
            printf("\n------ PERCURSOS ------\n");
            printf("1 - Pre-ordem\n");
            printf("2 - Em ordem\n");
            printf("3 - Pos-ordem\n");
            printf("Escolha: ");
            scanf("%d", &op2);

            switch (op2) {
                case 1: preOrdem(raiz); break;
                case 2: emOrdem(raiz); break;
                case 3: posOrdem(raiz); break;
                default: printf("Opcao invalida!\n");
            }
            printf("\n");
            break;
        }

        case 0:
            printf("Encerrando e liberando memoria...\n");
            liberarArvore(raiz);
            break;

        default:
            printf("Opcao invalida!\n");
        }
    }
    return 0;
}



