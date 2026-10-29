# Multiplicação de Matrizes com OpenMP

## Sobre o projeto

Este projeto foi desenvolvido para a aula prática da disciplina **Arquiteturas Paralelas e Distribuídas**, Unidade 3, Seção 3.3. A atividade consiste em implementar, em linguagem C, a multiplicação de duas matrizes quadradas utilizando o **OpenMP** para distribuir parte do processamento entre múltiplas threads.

O programa gera duas matrizes com valores pseudoaleatórios, calcula o produto entre elas de forma sequencial e paralela, compara os resultados e apresenta informações sobre as execuções.

## Objetivos

- Aplicar conceitos básicos de programação paralela.
- Utilizar o OpenMP em um programa escrito em C.
- Distribuir as iterações de laços entre threads.
- Calcular o produto de duas matrizes quadradas.
- Comparar o resultado paralelo com o resultado sequencial.
- Observar os tempos de execução em diferentes configurações.

## Tecnologias utilizadas

- **Linguagem:** C
- **Compilador:** GCC
- **Biblioteca/API:** OpenMP
- **Medição de tempo:** função `omp_get_wtime()`

## Estrutura do projeto

```text
.
├── matriz_openmp.c   # Código-fonte do programa
└── README.md         # Documentação do projeto
```

## Requisitos

É necessário ter um compilador GCC com suporte a OpenMP. Em distribuições baseadas em Ubuntu, como o Linux Mint, instale as ferramentas de compilação com:

```bash
sudo apt update
sudo apt install build-essential
```

## Como compilar

No terminal, acesse a pasta onde está o arquivo `matriz_openmp.c` e execute:

```bash
gcc -std=c11 -Wall -Wextra -O2 -fopenmp matriz_openmp.c -o matriz_openmp
```

A opção `-fopenmp` habilita o suporte ao OpenMP durante a compilação.

## Como executar

Para executar o programa utilizando quatro threads:

```bash
OMP_NUM_THREADS=4 ./matriz_openmp
```

Para executar com uma única thread e comparar os resultados e tempos:

```bash
OMP_NUM_THREADS=1 ./matriz_openmp
```

A quantidade de threads pode ser alterada por meio da variável de ambiente `OMP_NUM_THREADS`.

## Como funciona

### 1. Criação e preenchimento das matrizes

O programa utiliza a constante `N` para definir as dimensões das matrizes quadradas `A`, `B` e das matrizes de resultado. Inicialmente, `N` está definido como `4`, portanto, as matrizes possuem quatro linhas e quatro colunas.

As matrizes de entrada são preenchidas com valores pseudoaleatórios entre `0` e `9`, gerados com `rand()`.

### 2. Multiplicação sequencial

A versão sequencial utiliza três laços. Os dois primeiros percorrem as linhas e colunas do resultado; o terceiro calcula a soma dos produtos correspondentes:

\[
C[i][j] = \sum_{k=0}^{N-1} A[i][k] \times B[k][j]
\]

O resultado é armazenado em `C_sequencial`.

### 3. Multiplicação paralela com OpenMP

A versão paralela utiliza a diretiva:

```c
#pragma omp parallel for collapse(2) schedule(static)
```

- `parallel for`: distribui as iterações do laço entre as threads.
- `collapse(2)`: combina os dois laços externos para que suas iterações possam ser distribuídas conjuntamente.
- `schedule(static)`: distribui as iterações de forma estática entre as threads.

Cada combinação de índices `i` e `j` calcula uma posição distinta de `C_paralela`. A variável `soma` é local à iteração, e cada posição de resultado é escrita por uma única iteração, evitando que duas threads escrevam simultaneamente na mesma célula.

### 4. Comparação dos resultados

Depois dos cálculos, o programa compara cada elemento de `C_sequencial` com o elemento correspondente de `C_paralela`. A mensagem `Resultados iguais? SIM` indica que as duas implementações produziram a mesma matriz para as entradas geradas naquela execução.

### 5. Medição de tempo

O programa usa `omp_get_wtime()` para medir o tempo de cada implementação. Os valores podem mudar entre execuções e dependem do computador, da quantidade de threads e do tamanho das matrizes.

## Configurar o tamanho das matrizes

No início do arquivo `matriz_openmp.c`, altere a definição de `N`:

```c
#define N 4
```

Por exemplo, para usar matrizes de dimensão `100 × 100`:

```c
#define N 100
```

Depois de alterar o valor, compile o programa novamente e execute-o. Para matrizes maiores que `10 × 10`, o programa não imprime as matrizes completas; exibe apenas o primeiro elemento da matriz resultante, além das informações de execução.

## Testes sugeridos

| Teste | Valor de `N` | Threads | Verificação |
|---|---:|---:|---|
| 1 | 4 | 1 | Conferir a saída e a igualdade dos resultados. |
| 2 | 4 | 4 | Conferir se a versão paralela corresponde à sequencial. |
| 3 | 100 | 4 | Testar matrizes maiores e comparar os tempos. |
| 4 | 500 | 4 | Observar o comportamento com uma carga de trabalho maior. |

Para cada tamanho diferente de `N`, recompile o programa. O número de threads pode ser alterado sem modificar o código, usando `OMP_NUM_THREADS`.

> **Observação:** a execução paralela não será necessariamente mais rápida em todos os testes. Em matrizes pequenas, o custo de iniciar e coordenar as threads pode superar o ganho obtido com a execução simultânea.

## Exemplo de saída

A saída varia porque as matrizes são preenchidas com valores pseudoaleatórios. O programa apresenta informações semelhantes a estas:

```text
==========================================
 MULTIPLICACAO DE MATRIZES COM OPENMP
==========================================
Tamanho das matrizes: 4 x 4
Maximo de threads configuradas: 4
Tempo sequencial: ... segundos
Tempo paralelo:   ... segundos
Resultados iguais? SIM

Matriz A:
...

Matriz B:
...

Matriz resultante C = A x B:
...
==========================================
```

Os tempos e os valores das matrizes são ilustrativos e podem ser diferentes em cada execução.

## Arquivo para entrega

O arquivo principal da atividade é `matriz_openmp.c`. O `README.md` documenta os objetivos, os comandos de compilação e execução, a lógica implementada e os testes sugeridos. Caso sejam solicitadas evidências, inclua capturas do terminal mostrando a execução e a mensagem de igualdade dos resultados.
