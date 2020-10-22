//Compile: make all
//Run: ./count
    // N
    // N chars


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define T 8 // Numero de Threads

int main(int argc,char **argv){
    int i, j, k, n, localmaior, maior;
    char *vec, c;
    int *count;

    count = (int*)calloc(256, sizeof(int));

    scanf("%d\n", &n);

    vec = (char*)malloc(sizeof(char)*n+1);

    fgets(vec, n+1, stdin);

    localmaior = 0;

    #pragma omp parallel for num_threads(T) private(i) shared(vec, count)
    // Preenche os vetores 
    for(i = 0; i < n; i++){
        count[(int)vec[i]] += 1;
    }

    #pragma omp parallel for num_threads(T) private(j) shared(count) reduction(max:localmaior)
    for(j = 0; j < 256; j++){
        if(count[j] > localmaior){
            localmaior = count[j];
        }
    }

    #pragma omp parallel for num_threads(T) private(k) shared(count, localmaior)
    for(k = 0; k < 256; k++){
        if(count[k] == localmaior){
            printf("%c ", k);
        }
    }

    printf("\n");

    free(vec);
    free(count);

    return 0;
}