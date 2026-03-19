#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int valor;
    int altura;
    struct No *esq, *dir;
} No;

// Altura do nó
int altura(No *no) {
    return no ? no->altura : 0;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

// Cria um novo nó
No* novoNo(int valor) {
    No* no = (No*)malloc(sizeof(No));
    no->valor = valor;
    no->esq = no->dir = NULL;
    no->altura = 1;
    return no;
}

// Fator de balanceamento
int fatorBalanceamento(No *no) {
    return no ? altura(no->esq) - altura(no->dir) : 0;
}

// ---------------- ROTAÇÕES ----------------

No* rotacaoDireita(No *y) {
    No *x = y->esq;
    No *T2 = x->dir;

    x->dir = y;
    y->esq = T2;

    y->altura = max(altura(y->esq), altura(y->dir)) + 1;
    x->altura = max(altura(x->esq), altura(x->dir)) + 1;

    return x;
}

No* rotacaoEsquerda(No *x) {
    No *y = x->dir;
    No *T2 = y->esq;

    y->esq = x;
    x->dir = T2;

    x->altura = max(altura(x->esq), altura(x->dir)) + 1;
    y->altura = max(altura(y->esq), altura(y->dir)) + 1;

    return y;
}

// ---------------- INSERÇÃO ----------------

No* inserir(No* no, int valor) {
    if (no == NULL)
        return novoNo(valor);

    if (valor < no->valor)
        no->esq = inserir(no->esq, valor);
    else if (valor > no->valor)
        no->dir = inserir(no->dir, valor);
    else
        return no;

    no->altura = 1 + max(altura(no->esq), altura(no->dir));

    int fb = fatorBalanceamento(no);

    if (fb > 1 && valor < no->esq->valor)
        return rotacaoDireita(no);

    if (fb < -1 && valor > no->dir->valor)
        return rotacaoEsquerda(no);

    if (fb > 1 && valor > no->esq->valor) {
        no->esq = rotacaoEsquerda(no->esq);
        return rotacaoDireita(no);
    }

    if (fb < -1 && valor < no->dir->valor) {
        no->dir = rotacaoDireita(no->dir);
        return rotacaoEsquerda(no);
    }

    return no;
}

// ---------------- REMOÇÃO ----------------

No* menorNo(No *no) {
    while (no->esq != NULL)
        no = no->esq;
    return no;
}

No* remover(No* raiz, int valor) {
    if (raiz == NULL)
        return raiz;

    if (valor < raiz->valor)
        raiz->esq = remover(raiz->esq, valor);
    else if (valor > raiz->valor)
        raiz->dir = remover(raiz->dir, valor);

    else {
        if (raiz->esq == NULL || raiz->dir == NULL) {
            No *temp = raiz->esq ? raiz->esq : raiz->dir;

            if (temp == NULL) {
                free(raiz);
                return NULL;
            } else {
                No *aux = raiz;
                raiz = temp;
                free(aux);
            }
        } else {
            No* temp = menorNo(raiz->dir);
            raiz->valor = temp->valor;
            raiz->dir = remover(raiz->dir, temp->valor);
        }
    }

    raiz->altura = 1 + max(altura(raiz->esq), altura(raiz->dir));

    int fb = fatorBalanceamento(raiz);

    if (fb > 1 && fatorBalanceamento(raiz->esq) >= 0)
        return rotacaoDireita(raiz);

    if (fb > 1 && fatorBalanceamento(raiz->esq) < 0) {
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
    }

    if (fb < -1 && fatorBalanceamento(raiz->dir) <= 0)
        return rotacaoEsquerda(raiz);

    if (fb < -1 && fatorBalanceamento(raiz->dir) > 0) {
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

// ---------------- TRAVESSIAS ----------------

void preOrdem(No *raiz) {
    if (raiz) {
        printf("%d ", raiz->valor);
        preOrdem(raiz->esq);
        preOrdem(raiz->dir);
    }
}

void emOrdem(No *raiz) {
    if (raiz) {
        emOrdem(raiz->esq);
        printf("%d ", raiz->valor);
        emOrdem(raiz->dir);
    }
}

void posOrdem(No *raiz) {
    if (raiz) {
        posOrdem(raiz->esq);
        posOrdem(raiz->dir);
        printf("%d ", raiz->valor);
    }
}

// ---------------- MAIN ----------------

int main() {
    No *raiz = NULL;
    int opcao, valor;

    do {
        printf("\n--- ARVORE AVL ---\n");
        printf("1 - Inserir valor\n");
        printf("2 - Remover valor\n");
        printf("3 - Exibir pre-ordem\n");
        printf("4 - Exibir em ordem\n");
        printf("5 - Exibir pos-ordem\n");
        printf("0 - Sair\n");
        printf("Escolha: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida!\n");
            while (getchar() != '\n');
            continue;
        }

        switch (opcao) {
            case 1:
                printf("Valor: ");
                scanf("%d", &valor);
                raiz = inserir(raiz, valor);
                break;

            case 2:
                printf("Valor: ");
                scanf("%d", &valor);
                raiz = remover(raiz, valor);
                break;

            case 3:
                preOrdem(raiz);
                printf("\n");
                break;

            case 4:
                emOrdem(raiz);
                printf("\n");
                break;

            case 5:
                posOrdem(raiz);
                printf("\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}
