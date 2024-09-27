/*
    3ª Avaliação de AEDS II - Trabalho Prático
    O Problema do Caixeiro Viajante - Força Bruta

    Nomes:  Alexander Neves Barbosa Júnior
            Maurílio Rittershaussen Novaes
            Samuel Viana Santana
    
    Data: 08/08/2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

// define uma variável global de comparações
int COMPARACOES = 0;

/*
    Bloco responsável pala criação de uma matriz simétrica com elementos aleatórios
    É uma função chamada pela função inicializaMatriz
    Note que são gerados números de 1 a 9, a menos da diagonal principal
*/

int **criaMatrizDeAdjascencias(int tamanho){
    int **matriz = (int**) malloc(tamanho*sizeof(int*));

    for(int i=0; i<tamanho; i++){
        matriz[i] = (int*) malloc(tamanho*sizeof(int));
        for(int j=0; j<tamanho; j++){
            if(j==i){
                matriz[i][j]=0;
            }
            else{
                if(i<j){
                    matriz[i][j]=rand()%9 +1;
                }
                else{
                    matriz[i][j]=matriz[j][i];
                }
            }
        }
    }

    return matriz;
}

/*
    Bloco responsável pala inicialização de uma matriz simétrica com elementos aleatórios
    Chama a função criaMatrizDeAdjascencias e define o valor do tamanho da matriz
*/
int **inicializaMatriz(int *tamanho){
    srand(time(0));
    printf("Digite o tamanho da matriz: ");
    scanf("%i", tamanho);

    while(*tamanho<2){
        printf("Por favor, digite um tamanho de, no mínimo, 2: ");
        scanf("%i", tamanho);
    }
    
    return criaMatrizDeAdjascencias(*tamanho);
}

/*
    Função responsável por imprimir a matriz no terminal
*/
void imprimeMatriz(int **matriz, int tamanho){
    printf("\n");
    for(int i=0; i<tamanho; i++){
        for(int j=0; j<tamanho; j++){
            printf("%i ", matriz[i][j]);
        }
        printf("\n");
    }

    return;
}

/*
    Função responsável pelo cálculo do custo do caminho atual
*/
int calculaCusto(int **matriz, int *caminho, int tamanho) {
    int custo = 0;

    // o caminho percorrido na matriz depende da relação de dois elementos consecutivos no vetor caminho
    for (int i = 0; i < tamanho - 1; i++) {
        custo += matriz[caminho[i]][caminho[i + 1]];
    }
    
    // soma o custo de voltar ao primeiro vértice da matriz por meio do último elemento do caminho
    custo += matriz[caminho[tamanho - 1]][caminho[0]];
    return custo;
}

void trocar(int *x, int *y) {
    int aux = *x;
    *x = *y;
    *y = aux;
}

/*
    Função que auxilia nas permutações, invertendo o caminho
*/
void trasPraFrente(int *inicio, int *fim) {
    while ((inicio != fim) && (inicio != --fim)) {
        trocar(inicio++, fim);
    }
}

/*
    Função que gera a próxima permutação, chamando as funções de troca e de inverter de trás pra fernte
    Ela permita os elementos no próprio vetor, evitando criar diversos vetores auxiliares
*/
int fazProximaPermutacao(int *inicio, int *fim) {
    int *fimDoVetor = fim;

    // verifica se está na última permutação possível
    if (inicio == fim){
        return 0;
    }

    if (inicio == --fimDoVetor){
        return 0;
    } 

    // realiza as permutações
    while (1) {
        int *elementoAtual = fimDoVetor;

        if (*--fimDoVetor < *elementoAtual) {
            int *sucessor = fim;

            while (!(*fimDoVetor < *--sucessor));

            trocar(fimDoVetor, sucessor);
            trasPraFrente(elementoAtual, fim);

            return 1;
        }
        if (fimDoVetor == inicio) {
            trasPraFrente(inicio, fim);
            return 0;
        }
    }
}

/*
    Função responsável por realizar todas as permutações possíveis e analisar o custo de cada
*/
int *forcaBrutaCaixeiroViajante(int **matriz, int tamanho, int *menorCusto) {
    int caminho[tamanho];
    int *melhorCaminho= (int*) malloc((tamanho+1)*sizeof(int));

    for (int i = 0; i < tamanho; i++) {
        caminho[i] = i;
    }   

    // Gerar todas as permutações do caminho, avaliando qual é o menor custo dentre elas
    do {
        int custoAtual = calculaCusto(matriz, caminho, tamanho);
        COMPARACOES+=1;

        // salva o menor custo e grava o melhor caminho
        if (custoAtual < *menorCusto) {
            *menorCusto = custoAtual;

            for(int i=0; i< tamanho; i++){
                melhorCaminho[i]=caminho[i];
            }
            melhorCaminho[tamanho]=melhorCaminho[0];
        }

    } while (fazProximaPermutacao(caminho, caminho + tamanho));

    // retorna o melhor caminho, umas vez que o menor custo foi alterado por meio do ponteiro
    return melhorCaminho;
}

int main(){
    int tamanho, menorCusto, **matriz;
    int tempo = time(0);
    clock_t begin, end;

    // cria a matriz de adjascências aleatória e assume o custo como "infinito"
    matriz = inicializaMatriz(&tamanho);
    menorCusto = INT_MAX;

    imprimeMatriz(matriz, tamanho);
    printf("\nEssa é a matriz a ser analisada por força bruta.\n");
    
    // chama a função responsável pelo cálculo e imprime os resultados obtidos
    begin = clock();
    int *melhorCaminho = forcaBrutaCaixeiroViajante(matriz, tamanho, &menorCusto);
    printf("\nO custo mínimo para esse problema do Caixeiro Viajante é de: %i\n", menorCusto);


    // Imprime o caminho de menor custo (o print está dessa forma para melhorar na visualização do caminho)
    printf("Dessa forma, o caminho ideal é:\n");

    for(int i=0; i < tamanho+1; i++){
        if(i==tamanho){
            printf("%i",melhorCaminho[i]);
        }
        else{
            printf("%i-->",melhorCaminho[i]);
        }
    }

    // imprime a quantidade de comparações realizadas, que é da ordem de n!
    printf("\n%i comparações realizadas", COMPARACOES);

    end = clock();
    printf("\nPrograma executado em %.lf ms\n", ((double)end - begin) / ((CLOCKS_PER_SEC / 1000)));

    free(matriz);
    free(melhorCaminho);
    
    return 0;
}
