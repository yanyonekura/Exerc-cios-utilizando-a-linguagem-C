#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_CEPS 1000000
#define INCREMENTO 10000 

// --- Funções do HeapSort (Questão 2) ---
void heapify(int arr[], int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;
    if (largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        heapify(arr, i, 0);
    }
}

// --- Funções do QuickSort (Questão 1) ---
void quickSort(int arr[], int low, int high) {
    int i = low, j = high;
    int pivot = arr[low + (high - low) / 2];
    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;
        if (i <= j) {
            int t = arr[i]; arr[i] = arr[j]; arr[j] = t;
            i++; j--;
        }
    }
    if (low < j) quickSort(arr, low, j);
    if (i < high) quickSort(arr, i, high);
}

int main() {
    int *base_ceps = (int*)malloc(MAX_CEPS * sizeof(int));
    int *arr_teste = (int*)malloc(MAX_CEPS * sizeof(int));
    
    FILE *f_entrada = fopen("ceps.txt", "r");
    if (!f_entrada) {
        printf("Erro: Arquivo 'ceps.txt' nao encontrado!\n");
        free(base_ceps); free(arr_teste);
        return 1;
    }

    printf("Lendo arquivo de CEPs...\n");
    for (int i = 0; i < MAX_CEPS; i++) {
        if (fscanf(f_entrada, "%d", &base_ceps[i]) == EOF) break;
    }
    fclose(f_entrada);

    FILE *f_quick = fopen("tempo_quick_ceps.txt", "w");
    FILE *f_heap = fopen("tempo_heap_ceps.txt", "w");

    printf("Iniciando bateria de testes (Quick e Heap)...\n");

    for (int n = INCREMENTO; n <= MAX_CEPS; n += INCREMENTO) {
        // Teste QuickSort
        for (int i = 0; i < n; i++) arr_teste[i] = base_ceps[i];
        clock_t start = clock();
        quickSort(arr_teste, 0, n - 1);
        double t_quick = ((double)(clock() - start)) / CLOCKS_PER_SEC * 1000;
        fprintf(f_quick, "%d %.6f\n", n, t_quick);

        // Teste HeapSort
        for (int i = 0; i < n; i++) arr_teste[i] = base_ceps[i];
        start = clock();
        heapSort(arr_teste, n);
        double t_heap = ((double)(clock() - start)) / CLOCKS_PER_SEC * 1000;
        fprintf(f_heap, "%d %.6f\n", n, t_heap);

        if (n % 100000 == 0) printf("Progresso: %d CEPs processados\n", n);
    }

    fclose(f_quick);
    fclose(f_heap);
    free(base_ceps);
    free(arr_teste);

    printf("\nPronto! Arquivos 'tempo_quick_ceps.txt' e 'tempo_heap_ceps.txt' gerados.\n");
    return 0;
}