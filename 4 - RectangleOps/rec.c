// compilar: make all
// executar: make run

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define T 8 // Numero de Threads

/* Função de alocação de matriz */
double **allocm(int l, int c) { 
    int i;
	double **mat = NULL; 

	mat = (double**)malloc(sizeof(double*)*l); //matriz recebe as linhas

	for(i = 0; i < l; i++){ // percorre as linhas da matriz
		mat[i] = (double*)malloc(sizeof(double)*c); //cada linha recebe c colunas
	}

	return mat;
}

/* Função de liberação de memória */
void freem(double **mat, int l, int c){
	int i;
	for(i = 0; i < l; i++){
		free(mat[i]);
	}	
	free(mat);

}

/* Função que calcula o perimetro de um retangulo dado base e altura */
int perimetro(int b, int h){
    return 2*(b+h);
}

/* Função que calcula a area de um retangulo dado base e altura */
int area(int b, int h){
    return b*h;
}

/* Função que calcula a diagonal de um retangulo dado base e altura */
double diag(int b, int h){
    return sqrt((b*b) + (h*h));
}

int main(int argc,char **argv){
    int n, i, j;
    int *base = NULL, *alt = NULL;
    double** res = NULL;

    scanf("%d", &n);

    // Alocação dos vetores base a altura
    base = (int*)malloc(sizeof(int)*n); 
    alt = (int*)malloc(sizeof(int)*n); 

    // Alocação da matriz de resultado
    res = allocm(n, 3);

    for(i = 0; i < n; i++){
        scanf("%d", &base[i]);
    }

    for(i = 0; i < n; i++){
        scanf("%d", &alt[i]);
    }
      
    /* Parte paralelizada */
	#pragma omp parallel num_threads(T), shared(base, alt, res) private(i)
	{
        for(i = 0; i < n; i++){
            #pragma omp single
            {
                #pragma omp task
                {res[i][0] = perimetro(base[i], alt[i]);}
                #pragma omp task
                {res[i][1] = area(base[i], alt[i]);}
                #pragma omp task
                {res[i][2] = diag(base[i], alt[i]);}
            }
        } // Fim do for
    } // Fim da região paralela
    
    // Impressão da matriz de resultados
    for(i = 0; i < n; i++){
        printf("base[%d]=%d, alt[%d]=%d, per[%d]=%d, area[%d]=%d, diag[%d]=%.2lf\n", i, base[i], i, alt[i], i, (int)res[i][0], i, (int)res[i][1], i, res[i][2]);
    }
    
    free(base);
    free(alt);

    freem(res, n, 3);

   return 0;
}