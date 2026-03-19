#include <stdio.h>
#include <stdlib.h>

// --------------------------------------------------
// ÁRVORE RUBRO-NEGRA (RED-BLACK TREE) EM C
// Implementa: Inserção, Busca, Remoção e Travessias
// --------------------------------------------------

typedef enum { VERMELHO, PRETO } Cor;

typedef struct No {
    int valor;
    Cor cor;
    struct No *esq, *dir, *pai;
} No;

No *raiz = NULL;

// ------------------------------
// Funções auxiliares
// ------------------------------

Cor cor(No *n) {
    return n == NULL ? PRETO : n->cor;
}

No* criarNo(int valor) {
    No *n = (No*)malloc(sizeof(No));
    n->valor = valor;
    n->cor = VERMELHO;
    n->esq = n->dir = n->pai = NULL;
    return n;
}

No* minimo(No *n) {
    while (n->esq != NULL)
        n = n->esq;
    return n;
}

// ------------------------------
// Rotações
// ------------------------------

void rotacaoEsquerda(No **raiz, No *x) {
    No *y = x->dir;
    x->dir = y->esq;

    if (y->esq)
        y->esq->pai = x;

    y->pai = x->pai;

    if (x->pai == NULL)
        *raiz = y;
    else if (x == x->pai->esq)
        x->pai->esq = y;
    else
        x->pai->dir = y;

    y->esq = x;
    x->pai = y;
}

void rotacaoDireita(No **raiz, No *y) {
    No *x = y->esq;
    y->esq = x->dir;

    if (x->dir)
        x->dir->pai = y;

    x->pai = y->pai;

    if (y->pai == NULL)
        *raiz = x;
    else if (y == y->pai->dir)
        y->pai->dir = x;
    else
        y->pai->esq = x;

    x->dir = y;
    y->pai = x;
}

// ------------------------------
// Inserção RB
// ------------------------------

void corrigirInsercao(No **raiz, No *z) {
    while (z != *raiz && cor(z->pai) == VERMELHO) {
        if (z->pai == z->pai->pai->esq) {
            No *tio = z->pai->pai->dir;
            if (cor(tio) == VERMELHO) {
                z->pai->cor = PRETO;
                tio->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->dir) {
                    z = z->pai;
                    rotacaoEsquerda(raiz, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacaoDireita(raiz, z->pai->pai);
            }
        } else {
            No *tio = z->pai->pai->esq;
            if (cor(tio) == VERMELHO) {
                z->pai->cor = PRETO;
                tio->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->esq) {
                    z = z->pai;
                    rotacaoDireita(raiz, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacaoEsquerda(raiz, z->pai->pai);
            }
        }
    }
    (*raiz)->cor = PRETO;
}

void inserir(int valor) {
    No *z = criarNo(valor);
    No *y = NULL;
    No *x = raiz;

    while (x) {
        y = x;
        if (valor < x->valor)
            x = x->esq;
        else if (valor > x->valor)
            x = x->dir;
        else {
            printf("Valor duplicado!\n");
            free(z);
            return;
        }
    }

    z->pai = y;

    if (!y)
        raiz = z;
    else if (valor < y->valor)
        y->esq = z;
    else
        y->dir = z;

    corrigirInsercao(&raiz, z);
}

// ------------------------------
// Busca
// ------------------------------

No* buscar(No *r, int valor) {
    if (!r || r->valor == valor)
        return r;
    if (valor < r->valor)
        return buscar(r->esq, valor);
    return buscar(r->dir, valor);
}

// ------------------------------
// Remoção RB
// ------------------------------

void corrigirRemocao(No **raiz, No *x) {
    while (x != *raiz && cor(x) == PRETO) {
        if (x == x->pai->esq) {
            No *w = x->pai->dir;
            if (cor(w) == VERMELHO) {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoEsquerda(raiz, x->pai);
                w = x->pai->dir;
            }
            if (cor(w->esq) == PRETO && cor(w->dir) == PRETO) {
                w->cor = VERMELHO;
                x = x->pai;
            } else {
                if (cor(w->dir) == PRETO) {
                    w->esq->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacaoDireita(raiz, w);
                    w = x->pai->dir;
                }
                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                w->dir->cor = PRETO;
                rotacaoEsquerda(raiz, x->pai);
                x = *raiz;
            }
        } else {
            No *w = x->pai->esq;
            if (cor(w) == VERMELHO) {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoDireita(raiz, x->pai);
                w = x->pai->esq;
            }
            if (cor(w->dir) == PRETO && cor(w->esq) == PRETO) {
                w->cor = VERMELHO;
                x = x->pai;
            } else {
                if (cor(w->esq) == PRETO) {
                    w->dir->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacaoEsquerda(raiz, w);
                    w = x->pai->esq;
                }
                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                w->esq->cor = PRETO;
                rotacaoDireita(raiz, x->pai);
                x = *raiz;
            }
        }
    }
    if (x)
        x->cor = PRETO;
}

void remover(int valor) {
    No *z = buscar(raiz, valor);
    if (!z) {
        printf("Valor não encontrado!\n");
        return;
    }

    No *y = z;
    No *x;
    Cor corOriginal = y->cor;

    if (!z->esq) {
        x = z->dir;
        if (z->pai == NULL)
            raiz = z->dir;
        else if (z == z->pai->esq)
            z->pai->esq = z->dir;
        else
            z->pai->dir = z->dir;
        if (z->dir)
            z->dir->pai = z->pai;
    } else if (!z->dir) {
        x = z->esq;
        if (z->pai == NULL)
            raiz = z->esq;
        else if (z == z->pai->esq)
            z->pai->esq = z->esq;
        else
            z->pai->dir = z->esq;
        if (z->esq)
            z->esq->pai = z->pai;
    } else {
        y = minimo(z->dir);
        corOriginal = y->cor;
        x = y->dir;

        if (y->pai == z) {
            if (x)
                x->pai = y;
        } else {
            if (y->pai) {
                if (y == y->pai->esq)
                    y->pai->esq = y->dir;
                else
                    y->pai->dir = y->dir;
            }
            if (y->dir)
                y->dir->pai = y->pai;

            y->dir = z->dir;
            y->dir->pai = y;
        }

        if (z->pai == NULL)
            raiz = y;
        else if (z == z->pai->esq)
            z->pai->esq = y;
        else
            z->pai->dir = y;

        y->pai = z->pai;
        y->esq = z->esq;
        y->esq->pai = y;
        y->cor = z->cor;
    }

    free(z);

    if (corOriginal == PRETO && x)
        corrigirRemocao(&raiz, x);
}

// ------------------------------
// Travessias
// ------------------------------

void preOrdem(No *r) {
    if (r) {
        printf("%d(%c) ", r->valor, r->cor == VERMELHO ? 'R' : 'B');
        preOrdem(r->esq);
        preOrdem(r->dir);
    }
}

void emOrdem(No *r) {
    if (r) {
        emOrdem(r->esq);
        printf("%d(%c) ", r->valor, r->cor == VERMELHO ? 'R' : 'B');
        emOrdem(r->dir);
    }
}

void posOrdem(No *r) {
    if (r) {
        posOrdem(r->esq);
        posOrdem(r->dir);
        printf("%d(%c) ", r->valor, r->cor == VERMELHO ? 'R' : 'B');
    }
}

// ------------------------------
// Menu
// ------------------------------

void menu() {
    int op, valor;
    do {
        printf("\n--- Árvore Rubro-Negra ---\n");
        printf("1 - Inserir\n");
        printf("2 - Buscar\n");
        printf("3 - Remover\n");
        printf("4 - Pré-Ordem\n");
        printf("5 - Em-Ordem\n");
        printf("6 - Pós-Ordem\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &op);

        switch (op) {
            case 1:
                printf("Valor: ");
                scanf("%d", &valor);
                inserir(valor);
                break;
            case 2:
                printf("Valor: ");
                scanf("%d", &valor);
                if (buscar(raiz, valor))
                    printf("Valor encontrado!\n");
                else
                    printf("Valor não encontrado!\n");
                break;
            case 3:
                printf("Valor: ");
                scanf("%d", &valor);
                remover(valor);
                break;
            case 4:
                preOrdem(raiz);
                printf("\n");
                break;
            case 5:
                emOrdem(raiz);
                printf("\n");
                break;
            case 6:
                posOrdem(raiz);
                printf("\n");
                break;
        }
    } while (op != 0);
}

int main() {
    menu();
    return 0;
}
