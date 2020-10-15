//Compile: make all
//Run: ./est_desc < in.txt ou make run (roda caso de teste padrao)

//This program reads and return statistics of a matrix, each one in a row

//Luca Porto - 9778943

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define T 8 // Numero de Threads

/* Função de alocação de matriz */
double **allocm(int l, int c) { //função de alocação de matriz
    int i;
	double **mat = NULL; 

	mat = (double**)malloc(sizeof(double*)*l); //matriz recebe as linhas

	for(i = 0; i < l; i++){ // percorre as linhas da matriz
		mat[i] = (double*)malloc(sizeof(double)*c); //cada linha recebe c colunas
	}

	return mat;
}

/* Função de preenchimento da matriz */
double **fillm(double **mat, int l, int c) {
	int i, j;

	for(i = 0; i < l; i++) { //percorre as linhas
		for(j = 0; j < c; j++){//percorre as colunas
			scanf("%lf", &(mat[i][j])); //recebe valores de linha e coluna
		}
	}
	return mat;
}

void printMat(double **res, int l, int c){
	int i, j;

	for(i = 0; i < l; i++){
		for(j = 0; j < c; j++){
			printf("%.1lf ", res[i][j]);// percorre linhas e colunas e vai imprimindo
		}
		printf("\n");
	}
}

/* Função de liberação de memória */
void freem(double **mat, int l, int c){
	int i;
	for(i = 0; i < l; i++){
		free(mat[i]);
	}	
	free(mat);

}

/* Funcao que calcula a media aritmética das colunas */
double* mediaArit(double **mat, double ***res, int l, int c){
    int i, j; 
    double sum = 0, mean = 0;
	
	double *vec = (double*)malloc(sizeof(double)*c);

    for(j = 0; j < c; j++){
        for(i = 0; i < l; i++){
            sum += mat[i][j]; // Soma todos os valores da coluna
        }

        mean = sum/l; // Divide pelo número de itens

        (*res)[0][j] = mean; // Armazena na matriz de resultados
		vec[j] = mean; // Armazena no vetor de resultados para uso na variância

        mean = 0;
        sum = 0;
    }

	return vec;
} 

/* Funcao que calcula a media harmônica das colunas */
void mediaHarm(double **mat, double ***res, int l, int c){
	int i, j;
	double sum = 0, mean = 0;

	for(j = 0; j < c; j++){
        for(i = 0; i < l; i++){
			sum += 1/mat[i][j]; // Somatorio do inverso dos itens
		}
		mean = l/sum; // Calculo da razao entre o numero de itens e a soma
		(*res)[1][j] = mean; // Armazena na matriz de resultados

		mean = 0;
		sum = 0;
	}
}

/* Funcao que calcula a variância das colunas */
double* variancia(double **mat, double ***res, double *mean, int l, int c){
	int i, j, m, n;
	double sum = 0, var = 0;
	
	double *vec = (double*)malloc(sizeof(double)*c);

	for(j = 0; j < c; j++){
		for(i = 0; i < l; i++){
			var += pow((mat[i][j]-mean[j]), 2); // Calculo da variancia
		}
		(*res)[4][j] = var/(l-1); // Armazena na matriz de resultados
		vec[j] = var/(l-1); // Armazena no vetor de resultados para uso no desvio padrao

		var = 0;
	}

	return vec;
}

/* Funcao que calcula o desvio padrão das colunas */
double* desvioP(double ***res, double *var, int c){
	int i;

	double *vec = (double*)malloc(sizeof(double)*c);

	for(i = 0; i < c; i++){
		vec[i] = sqrt(var[i]); // Armazena calculo do desvio padrao no vetor para uso no coeficiente de variacao
		(*res)[5][i] = vec[i]; // Armazena na matriz de resultados
	}

	return vec;
}

/* Funcao que calcula o coeficiente de variacão das colunas */
void coefVar(double ***res, double *d, double *m, int c){
	int i;

	for(i = 0; i < c; i++){	
		(*res)[6][i] = d[i]/m[i]; // Calcula coeficiente e armazena na matriz de resultados
	}
}

/* Funcao que compara valores*/
int cmp(const void * a, const void * b){
	if(*(double*)a > *(double*)b)
		return 1;
	else if(*(double*)a < *(double*)b)
		return -1;
	else 
		return 0;
}

/* Funcao que calcula a mediana das colunas */
void median(double **mat, double ***res, int l, int c){
	int i, j;
	double med;

	double *vec = (double*)malloc(sizeof(double)*l);

	for(j = 0; j < c; j++){
		for(i = 0; i < l; i++){
			vec[i] = mat[i][j]; // Armazena coluna em um vetor
		}

		qsort(&vec[0], l, sizeof(double), cmp); // Ordena o vetor

		// Faz o calculo da mediana e vai pra proxima coluna

		if(l%2 == 0){ // Caso o numero de elementos da linha for par
			(*res)[2][j] = ((vec[(l-1)/2] + vec[((l-1)/2)+1])/2);
		}else{ // Caso seja impar, seleciona o do meio
			(*res)[2][j] = vec[(l-1)/2];
		}
	}

	free(vec);
}

/* Função que calcula a moda das colunas */
void mode(double **mat, double ***res, int l, int c){
	int i, j, k, counter = 1, countMode = 1;
	double mode, check;

	double *vec = (double*)malloc(sizeof(double)*l);

	for(j = 0; j < c; j++){ // Iteracao em cada coluna
		for(i = 0; i < l; i++){
			vec[i] = mat[i][j]; // Armazena coluna em um vetor
		}
		qsort(&vec[0], l, sizeof(double), cmp); // Ordena o vetor

		check = vec[0];
		mode = -1;

		for(k = 1; k < l; k++){ // Iteracao na coluna
			if(vec[k] == check){
				counter++;
			}else if(vec[k] != check && counter > 1){
				if(counter > countMode){ // Se o numero de repeticoes atuais for maior que a geral
					countMode = counter;
					mode = check; // A moda recebe o numero sendo checado
					counter = 1;
					check = vec[k];
				}else if(counter == countMode){ // Caso o numero de repeticoes for igual a moda nao é substituida
					counter = 1;
					check = vec[k];	
				}
			}else{
				check = vec[k];
			}
		}
		countMode = 1;
		counter = 1;

		(*res)[3][j] = mode; // Armazena as modas na matriz de resultados
	}

	free(vec);
}

int main(int argc,char **argv){
    int l, c; //Numero de linhas e colunas
    double **res = NULL, **mat = NULL; // Matrizes resultado e original
	double *m = NULL, *v = NULL, *d = NULL; // Estatisticas
	double wtime;

    scanf("%d %d", &l, &c); // Leitura de dimensões da matriz

	// Alocacao das matrizes original e resultado
    mat = allocm(l, c);
    res = allocm(7, c);

    mat = fillm(mat, l , c); // Preenchimento da matriz

	wtime = omp_get_wtime(); // Inicializando contador de tempo

	/* Parte paralelizada */
	#pragma omp parallel num_threads(T), shared(res)
	{
		#pragma omp single
		{
			#pragma omp task depend(out:m)
			{m = mediaArit(mat, &res, l, c);} // Calculo da média

			#pragma omp task depend(in:m) depend(out:v)
			{v = variancia(mat, &res, m, l, c);} // Calculo da variância

			#pragma omp task depend(in:v) depend(out:d)
			{d = desvioP(&res, v, c);} // Calculo do desvio padrão

			#pragma omp task depend(in:d,m)
			{coefVar(&res, d, m, c);} // Calculo do coeficiente da variacão

			#pragma omp task
			{mediaHarm(mat, &res, l, c);} // Calculo da média harmônica

			#pragma omp task
			{median(mat, &res, l, c); // Calculo da mediana

			#pragma omp task
			{mode(mat, &res, l, c);} // Calculo da mediana
}
		} 
	} //Fim da região paralela

	wtime = omp_get_wtime() - wtime;

	//printf("%lf\n", wtime); // Tempo de execucao(descomentar)
    printMat(res, 7, c);
	printf("\n");

	// Desalocacão de memoria
    freem(mat, l, c);
    freem(res, 7, c);
	free(m);
	free(v);
	free(d);

    return 0;
}