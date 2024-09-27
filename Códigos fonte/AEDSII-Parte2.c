/*
    3ª Avaliação - Trabalho Prático
    O Problema do Caixeiro Viajante - Vizinho Mais Próximo

    Nomes:  Alexander Neves Barbosa Júnior
            Maurílio Rittershaussen Novaes
            Samuel Viana Santana
    
    Data: 08/08/2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>

// define uma variável global de comparações
int COMPARACOES = 0;

// retorna 1 se as strings forem iguais e 0 caso contrário
int stringsIguais(char *string1, char *string2, int tamanho){
    for(int i=0; i<tamanho; i++){
        if(string1[i]!=string2[i]){
            return 0;
        }
    }
    return 1;
}

// inicializa a matriz de acordo com o arquivo, preservando a natureza da diagonal da matriz
int **criaMatrizDeAdjascencias(FILE *arquivo, int **matriz, int *tamanho, int *tipoMatriz, char *nomeDoCaso){
    char stringAux1[100];
    char stringAux2[100];

    while(fscanf(arquivo, "%s :", stringAux1)!=EOF){

        // acha o nome do caso
        if(stringsIguais(stringAux1, "NAME", 4)){
            fscanf(arquivo, "%s\n", stringAux2);
            strcpy(nomeDoCaso, stringAux2);
        }   

        // acha o tamanho da matriz e aloca ela
        if(stringsIguais(stringAux1, "DIMENSION", 9)){
            fscanf(arquivo, "%i", tamanho);

            matriz = (int**) malloc(*tamanho*sizeof(int*));

            for(int i=0; i<*tamanho; i++){
                matriz[i] = (int*) malloc(*tamanho*sizeof(int));
            }

        }

        // descobre se é matris diagonal superior (1) ou inferior (0)
        if(stringsIguais(stringAux1, "EDGE_WEIGHT_FORMAT", 18)){
            fscanf(arquivo, "%s\n", stringAux2);

            if(stringsIguais(stringAux2,"UPPER_DIAG_ROW",14)){
                *tipoMatriz = 1;
            }
            else{
                *tipoMatriz = 0;
            }
        }

        // começa a colocar os elementos na matriz e transpõe para completar a matriz
        if(stringsIguais(stringAux1, "EDGE_WEIGHT_SECTION", 19)){
            if(*tipoMatriz == 1){
                for(int i=0; i<*tamanho; i++){
                    for(int j=i; j<*tamanho; j++){
                        fscanf(arquivo, "%i", &matriz[i][j]);
                    }
                }

                for(int i=0; i<*tamanho; i++){
                    for(int j=0; j<*tamanho; j++){
                        if(i<j){
                            matriz[j][i]=matriz[i][j];
                        }
                    }
                }
            }
            else{
                for(int j=0; j<*tamanho; j++){
                    for(int i=j; i<*tamanho; i++){
                        fscanf(arquivo, "%i", &matriz[i][j]);
                    }
                }

                for(int i=0; i<*tamanho; i++){
                    for(int j=0; j<*tamanho; j++){
                        if(i>j){
                            matriz[j][i]=matriz[i][j];
                        }
                    }
                }
            }
            break;
        }
    }
    return matriz;
}

/*
    Função responsável pela busca do vizinho mais próximo (i) na linha atual
*/
int acharvizinho(int atual, int n, int visitado[], int ** matriz) {
    int minDist = INT_MAX;
    int maisprox = -1;
    
    for (int i = 0; i < n; i++) {
        if (!visitado[i] && i != atual) {
            int dist = matriz[atual][i];
            if (dist < minDist) {
                minDist = dist;
                maisprox = i;
            }
        }
    }
    
    return maisprox;
}

/*
    Função responsável por criar o caminho do Vizinho Mais Próximo, mantendo o controle por meio do vetorVisitados
*/
void vizinhoMaisProximo(int **matriz, int tamanho, int *caminho, int *custo){
    int *vetorVisitados = (int*) calloc(tamanho, sizeof(int));
    int atual = 0;
    int contador = 1;
    vetorVisitados[atual] = 1;
    
    for (int i = 1; i < tamanho; i++) {
        int maisprox = acharvizinho(atual, tamanho, vetorVisitados, matriz);
        COMPARACOES++;

        vetorVisitados[maisprox] = 1;
        *custo+=matriz[i][maisprox];
        caminho[contador] = maisprox;
        contador++;
        atual = maisprox;
    }
    
    caminho[contador++] = caminho[0];
    free(vetorVisitados);

    return;
}

/*
    Para a leitura dos dados de cada caso no Moodle, foi considerado que o arquivo está na mesma pasta que esse fonte.
*/
int main(){
    FILE *arquivo;
    int **matriz;
    int tipoMatriz;
    int tamanho;
    int custo = 0;
    char nomeDoCaso[20];
    clock_t begin, end;
    
    printf("Digite o nome do arquivo (com a extensão): ");
    scanf("%s", nomeDoCaso);
    arquivo = fopen(nomeDoCaso,"r");

    if(!arquivo){
        printf("Arquivo não encontrado");
        return 0;
    }

    // lê o arquivo
    matriz = criaMatrizDeAdjascencias(arquivo, matriz, &tamanho, &tipoMatriz, nomeDoCaso);

    int* caminho = (int*) calloc((tamanho+1), sizeof(int));

    // realiza o algoritmo, marcando o tempo necessário para isso
    begin = clock();
    vizinhoMaisProximo(matriz, tamanho, caminho, &custo);
    end = clock();
    
    // imprime as respostas
    printf("\n--------------O problema em questão é o problema %s--------------\n\n", nomeDoCaso);
    printf("Dessa forma, o caminho ideal é:\n\n");

    for(int i=0; i < tamanho+1; i++){
        if(i==tamanho){
            printf("%i\n\n",caminho[i]);
        }
        else{
            printf("%i-->",caminho[i]);
        }
    }

    printf("O custo total para percorrer esse caminho é de %i\n", custo);
    printf("A quantidade de comparações realizada foi de %i\n", COMPARACOES);
    printf("Programa executado em %.lf ms\n", ((double)end - begin) / ((CLOCKS_PER_SEC / 1000)));
    fclose(arquivo);
    free(matriz);
    free(caminho);

    return 0;
}
