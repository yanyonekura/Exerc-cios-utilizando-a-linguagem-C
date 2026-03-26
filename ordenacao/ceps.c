#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_N 1000000
#define INCREMENTO 10000 // Altere para 1 se quiser a precisão máxima (muito lento)

// --- FUNÇÕES DE AUXÍLIO ---

int buscar_maior(int v[], int n) {
    if (n <= 0) return 0;
    int maior = v[0];
    for (int i = 1; i < n; i++) {
        if (v[i] > maior) maior = v[i];
    }
    return maior;
}

int processar_cep(char *linha) {
    char limpo[12] = "";
    for (int i = 0; linha[i] != '\0'; i++) {
        if (linha[i] >= '0' && linha[i] <= '9') {
            strncat(limpo, &linha[i], 1);
        }
    }
    return atoi(limpo);
}

// --- QUESTÃO 3.A: COUNTING SORT ---
void counting_sort(int v[], int n) {
    if (n <= 0) return;

    int k = buscar_maior(v, n);
    
    // Alocação dinâmica para gerenciar o grande intervalo de CEPs
    int *count = (int *)calloc(k + 1, sizeof(int));
    int *output = (int *)malloc(n * sizeof(int));

    if (!count || !output) {
        // Se a memória falhar, tentamos avisar (comum se k for muito alto)
        if(count) free(count);
        if(output) free(output);
        return;
    }

    for (int i = 0; i < n; i++) count[v[i]]++;
    for (int i = 1; i <= k; i++) count[i] += count[i - 1];
    
    for (int i = n - 1; i >= 0; i--) {
        output[count[v[i]] - 1] = v[i];
        count[v[i]]--;
    }

    for (int i = 0; i < n; i++) v[i] = output[i];

    free(count);
    free(output);
}

// --- QUESTÃO 3.B: RADIX SORT ---
void counting_sort_radix(int v[], int n, int exp) {
    int *output = (int *)malloc(n * sizeof(int));
    int count[10] = {0};

    for (int i = 0; i < n; i++)
        count[(v[i] / exp) % 10]++;

    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; i--) {
        output[count[(v[i] / exp) % 10] - 1] = v[i];
        count[(v[i] / exp) % 10]--;
    }

    for (int i = 0; i < n; i++) v[i] = output[i];
    free(output);
}

void radix_sort(int v[], int n) {
    if (n <= 0) return;
    int m = buscar_maior(v, n);
    for (int exp = 1; m / exp > 0; exp *= 10)
        counting_sort_radix(v, n, exp);
}

// --- MAIN ---
int main() {
    // static para não estourar a pilha de memória local
    static int ceps_base[MAX_N];
    static int v_teste[MAX_N];
    
    FILE *f_ceps = fopen("ceps.txt", "r");
    if (!f_ceps) {
        printf("Erro: Arquivo ceps.txt nao encontrado!\n");
        return 1;
    }

    char linha[25];
    int total_ceps = 0;
    while (fgets(linha, sizeof(linha), f_ceps) && total_ceps < MAX_N) {
        ceps_base[total_ceps++] = processar_cep(linha);
    }
    fclose(f_ceps);
    printf("Dados carregados: %d CEPs.\n", total_ceps);

    FILE *out_c = fopen("tempo_counting.txt", "w");
    FILE *out_r = fopen("tempo_radix.txt", "w");

    printf("Iniciando testes com incremento de %d...\n", INCREMENTO);

    for (int n = 1; n <= total_ceps; n += INCREMENTO) {
        
        // Medição Counting Sort
        memcpy(v_teste, ceps_base, n * sizeof(int));
        clock_t t1 = clock();
        counting_sort(v_teste, n);
        clock_t t2 = clock();
        double tempo_c = 1000.0 * (double)(t2 - t1) / CLOCKS_PER_SEC;
        fprintf(out_c, "%d %.6f\n", n, tempo_c);

        // Medição Radix Sort
        memcpy(v_teste, ceps_base, n * sizeof(int));
        clock_t t3 = clock();
        radix_sort(v_teste, n);
        clock_t t4 = clock();
        double tempo_r = 1000.0 * (double)(t4 - t3) / CLOCKS_PER_SEC;
        fprintf(out_r, "%d %.6f\n", n, tempo_r);

        if (n % (INCREMENTO * 10) == 0) {
            printf("Progresso: n = %d\n", n);
        }
    }

    fclose(out_c);
    fclose(out_r);
    printf("Concluido! Verifique os arquivos .txt gerados.\n");

    return 0;
}