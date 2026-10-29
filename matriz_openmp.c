#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

/* Tamanho das matrizes quadradas: N x N. */
#define N 4

/*
 * Exibe uma matriz na tela.
 * O parametro nome permite identificar qual matriz esta sendo impressa.
 */
void imprimirMatriz(const char *nome, const int matriz[N][N]) {
    printf("\n%s:\n", nome);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%4d ", matriz[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    int A[N][N];
    int B[N][N];
    int C_sequencial[N][N];
    int C_paralela[N][N];

    /* Inicializa o gerador de numeros pseudoaleatorios. */
    srand((unsigned int) time(NULL));

    /* Preenche as matrizes A e B com valores entre 0 e 9. */
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }
    }

    /* ---------------------------------------------------------
     * 1. Multiplicacao sequencial: uma iteracao por vez.
     * --------------------------------------------------------- */
    double inicio_sequencial = omp_get_wtime();

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int soma = 0;

            for (int k = 0; k < N; k++) {
                soma += A[i][k] * B[k][j];
            }

            C_sequencial[i][j] = soma;
        }
    }

    double fim_sequencial = omp_get_wtime();

    /* ---------------------------------------------------------
     * 2. Multiplicacao paralela usando OpenMP.
     * parallel for distribui as iteracoes entre as threads.
     * collapse(2) combina os dois lacos externos (i e j).
     * schedule(static) distribui as iteracoes em blocos fixos.
     * --------------------------------------------------------- */
    double inicio_paralelo = omp_get_wtime();

    #pragma omp parallel for collapse(2) schedule(static)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int soma = 0;

            for (int k = 0; k < N; k++) {
                soma += A[i][k] * B[k][j];
            }

            /* Cada iteracao grava em uma posicao diferente de C. */
            C_paralela[i][j] = soma;
        }
    }

    double fim_paralelo = omp_get_wtime();

    /* ---------------------------------------------------------
     * 3. Confere se os resultados sequencial e paralelo sao iguais.
     * --------------------------------------------------------- */
    int resultados_iguais = 1;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (C_sequencial[i][j] != C_paralela[i][j]) {
                resultados_iguais = 0;
                break;
            }
        }

        if (!resultados_iguais) {
            break;
        }
    }

    /* ---------------------------------------------------------
     * 4. Apresenta as informacoes e a matriz resultante.
     * --------------------------------------------------------- */
    printf("==========================================\n");
    printf(" MULTIPLICACAO DE MATRIZES COM OPENMP\n");
    printf("==========================================\n");
    printf("Tamanho das matrizes: %d x %d\n", N, N);
    printf("Maximo de threads configuradas: %d\n", omp_get_max_threads());
    printf("Tempo sequencial: %.9f segundos\n", fim_sequencial - inicio_sequencial);
    printf("Tempo paralelo:   %.9f segundos\n", fim_paralelo - inicio_paralelo);
    printf("Resultados iguais? %s\n", resultados_iguais ? "SIM" : "NAO");

    /* Imprime as matrizes para tamanhos pequenos, evitando excesso de texto. */
    if (N <= 10) {
        imprimirMatriz("Matriz A", A);
        imprimirMatriz("Matriz B", B);
        imprimirMatriz("Matriz resultante C = A x B", C_paralela);
    } else {
        printf("\nMatrizes completas nao exibidas porque N > 10.\n");
        printf("Primeiro elemento do resultado: C[0][0] = %d\n", C_paralela[0][0]);
    }

    printf("\n==========================================\n");

    /* Retorna codigo de erro caso os resultados sejam diferentes. */
    return resultados_iguais ? EXIT_SUCCESS : EXIT_FAILURE;
}
