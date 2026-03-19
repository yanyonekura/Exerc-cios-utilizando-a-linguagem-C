#include <stdio.h>
#include <stdlib.h>

#define T 2   // grau mínimo (2-3 tree)

typedef struct Node23 {
    int n;                     
    int keys[2*T-1];           
    struct Node23* child[2*T]; 
    int isLeaf;
} Node23;


// -------------------- UTILIDADES -----------------------

Node23* createNode(int isLeaf) {
    Node23* node = (Node23*)malloc(sizeof(Node23));
    node->n = 0;
    node->isLeaf = isLeaf;
    for(int i=0;i<2*T;i++)
        node->child[i]=NULL;
    return node;
}


// -------------------- BUSCA -----------------------

int search23(Node23* root, int key){
    if(!root) return 0;

    int i=0;
    while(i<root->n && key>root->keys[i]) i++;

    if(i<root->n && root->keys[i]==key)
        return 1;

    if(root->isLeaf)
        return 0;

    return search23(root->child[i], key);
}


// -------------------- SPLIT -----------------------

void splitChild(Node23* parent, int i){
    Node23* y = parent->child[i];
    Node23* z = createNode(y->isLeaf);

    z->n = T-1;

    for(int j=0;j<T-1;j++)
        z->keys[j] = y->keys[j+T];

    if(!y->isLeaf){
        for(int j=0;j<T;j++)
            z->child[j] = y->child[j+T];
    }

    y->n = T-1;

    for(int j=parent->n;j>=i+1;j--)
        parent->child[j+1] = parent->child[j];

    parent->child[i+1] = z;

    for(int j=parent->n-1;j>=i;j--)
        parent->keys[j+1] = parent->keys[j];

    parent->keys[i] = y->keys[T-1];
    parent->n++;
}


// -------------------- INSERÇÃO -----------------------

void insertNonFull(Node23* node, int key){
    int i = node->n-1;

    if(node->isLeaf){
        while(i>=0 && key<node->keys[i]){
            node->keys[i+1]=node->keys[i];
            i--;
        }
        node->keys[i+1]=key;
        node->n++;
    }
    else{
        while(i>=0 && key<node->keys[i])
            i--;
        i++;

        if(node->child[i]->n == 2*T-1){
            splitChild(node,i);
            if(key > node->keys[i])
                i++;
        }

        insertNonFull(node->child[i],key);
    }
}

Node23* insert23(Node23* root, int key){
    if(!root){
        root=createNode(1);
        root->keys[0]=key;
        root->n=1;
        return root;
    }

    if(root->n == 2*T-1){
        Node23* s = createNode(0);
        s->child[0]=root;
        splitChild(s,0);

        int i=0;
        if(s->keys[0] < key)
            i++;

        insertNonFull(s->child[i],key);
        return s;
    }
    else{
        insertNonFull(root,key);
        return root;
    }
}


// -------------------- TRAVESSIA EM ORDEM -----------------------

void inorder23(Node23* root){
    if(!root) return;

    int i;
    for(i=0;i<root->n;i++){
        inorder23(root->child[i]);
        printf("%d ",root->keys[i]);
    }
    inorder23(root->child[i]);
}


// -------------------- MENU -----------------------

int main(){
    Node23* root=NULL;
    int op,val;

    do{
        printf("\n--- ARVORE 2-3 ---\n");
        printf("1 - Inserir\n");
        printf("2 - Buscar\n");
        printf("3 - Em ordem\n");
        printf("0 - Sair\nOpcao: ");
        scanf("%d",&op);

        switch(op){
            case 1:
                printf("Valor: ");
                scanf("%d",&val);
                root=insert23(root,val);
                break;

            case 2:
                printf("Valor: ");
                scanf("%d",&val);
                printf(search23(root,val)?"Existe\n":"Nao existe\n");
                break;

            case 3:
                inorder23(root);
                printf("\n");
                break;
        }

    }while(op!=0);

    return 0;
}
