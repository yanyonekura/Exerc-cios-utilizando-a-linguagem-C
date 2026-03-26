#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_N 1000000
#define MIN_VAL 1
#define MAX_VAL 1000000
#define PASSO 1000 // Incremento para viabilizar o tempo de execução

// --- UTILITÁRIOS ---
void swap(int *a, int *b) {
    int t = *a; *a = *b; *b = t;
}

void gerar_aleatorio(int v[], int n) {
    for (int i = 0; i < n; i++) v[i] = MIN_VAL + rand() % (MAX_VAL - MIN_VAL + 1);
}

void gerar_crescente(int v[], int n) {
    for (int i = 0; i < n; i++) v[i] = i + 1;
}

void gerar_decrescente(int v[], int n) {
    for (int i = 0; i < n; i++) v[i] = n - i;
}

// --- ALGORITMOS DE ORDENAÇÃO ---

// 1. QuickSort (Pivô no Meio)
void quickSortMiddle(int v[], int left, int right) {
    if (left >= right) return;
    int i = left, j = right;
    int pivot = v[(left + right) / 2];
    while (i <= j) {
        while (v[i] < pivot) i++;
        while (v[j] > pivot) j--;
        if (i <= j) {
            swap(&v[i], &v[j]);
            i++; j--;
        }
    }
    quickSortMiddle(v, left, j);
    quickSortMiddle(v, i, right);
}

// 2. QuickSort (Pivô Aleatório)
void quickSortRandom(int v[], int left, int right) {
    if (left >= right) return;
    int i = left, j = right;
    int pivot = v[left + rand() % (right - left + 1)];
    while (i <= j) {
        while (v[i] < pivot) i++;
        while (v[j] > pivot) j--;
        if (i <= j) {
            swap(&v[i], &v[j]);
            i++; j--;
        }
    }
    quickSortRandom(v, left, j);
    quickSortRandom(v, i, right);
}

// 3. HeapSort
void heapify(int v[], int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < n && v[l] > v[largest]) largest = l;
    if (r < n && v[r] > v[largest]) largest = r;
    if (largest != i) {
        swap(&v[i], &v[largest]);
        heapify(v, n, largest);
    }
}

void heapSort(int v[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) heapify(v, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(&v[0], &v[i]);
        heapify(v, i, 0);
    }
}

// 4. CountingSort para o RadixSort (Ordenação Estável baseada em dígitos)
void countingSortRadix(int v[], int n, int exp) {
    int *output = (int *)malloc(n * sizeof(int));
    int count[10] = {0};

    for (int i = 0; i < n; i++) count[(v[i] / exp) % 10]++;
    for (int i = 1; i < 10; i++) count[i] += count[i - 1];
    for (int i = n - 1; i >= 0; i--) {
        output[count[(v[i] / exp) % 10] - 1] = v[i];
        count[(v[i] / exp) % 10]--;
    }
    for (int i = 0; i < n; i++) v[i] = output[i];
    free(output);
}

// 5. RadixSort
void radixSort(int v[], int n) {
    int max = v[0];
    for (int i = 1; i < n; i++) if (v[i] > max) max = v[i];
    for (int exp = 1; max / exp > 0; exp *= 10)
        countingSortRadix(v, n, exp);
}

// --- FUNÇÃO DE EXECUÇÃO E MEDIÇÃO ---
void executar_testes(const char* nome_algoritmo, void (*sort_func)(int[], int, int), int is_quick) {
    static int v[MAX_N];
    char filename[100];
    
    const char* tipos[3] = {"aleatorio", "crescente", "decrescente"};
    FILE *arquivos[3];

    for (int i = 0; i < 3; i++) {
        sprintf(filename, "%s_%s.txt", nome_algoritmo, tipos[i]);
        arquivos[i] = fopen(filename, "w");
        fprintf(arquivos[i], "N Tempo_ms\n"); // Cabeçalho para facilitar no Python
    }

    printf("Iniciando bateria de testes: %s...\n", nome_algoritmo);

    for (int tipo = 0; tipo < 3; tipo++) {
        for (int n = PASSO; n <= MAX_N; n += PASSO) {
            
            if (tipo == 0) gerar_aleatorio(v, n);
            else if (tipo == 1) gerar_crescente(v, n);
            else if (tipo == 2) gerar_decrescente(v, n);

            clock_t inicio = clock();
            
            // Diferencia chamadas baseadas na assinatura da função
            if (is_quick) ((void (*)(int[], int, int))sort_func)(v, 0, n - 1);
            else ((void (*)(int[], int))sort_func)(v, n);
            
            clock_t fim = clock();
            double tempo_ms = 1000.0 * (double)(fim - inicio) / CLOCKS_PER_SEC;

            fprintf(arquivos[tipo], "%d %.6f\n", n, tempo_ms);
        }
        printf("  -> Concluído array tipo %d.\n", tipo);
    }

    for (int i = 0; i < 3; i++) fclose(arquivos[i]);
}

int main() {
    srand((unsigned)time(NULL));

    // Executa e gera .txt para cada algoritmo
    executar_testes("QuickMiddle", (void (*)(int[], int, int))quickSortMiddle, 1);
    executar_testes("QuickRandom", (void (*)(int[], int, int))quickSortRandom, 1);
    executar_testes("HeapSort", (void (*)(int[], int, int))heapSort, 0);
    executar_testes("RadixSort", (void (*)(int[], int, int))radixSort, 0);

    printf("\nTodos os testes finalizados com sucesso! Arquivos .txt gerados.\n");
    return 0;
}