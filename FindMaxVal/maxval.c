// compilar: make all
// executar: make run tam=<tamanho do vetor>

//Luca Porto 9778943

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define T 4 // Numero de Threads

int main(int argc,char **argv){
    double wtime;
    /*
    Não modifique este trecho de código
    *************************************************************************************
    */
	int *vetor, i, maior, tam;

    if ( argc  != 2)
    {
		printf("Wrong arguments. Please use binary <amount_of_elements>\n");
		exit(0);
    } // fim do if

    tam = atoi(argv[1]);

    printf("Amount of vetor=%d\n", tam);
    fflush(0);
	
	vetor=(int*)malloc(tam*sizeof(int)); //Aloca o vetor da dimensão lida

    /************************************************************************************
		Modifique a partir daqui
    */

    int ini, end; // Variáveis de controle para inicio e fim dos blocos em cada thread
    int threadNum; // Número de thread
    int remainder = tam%T; // Porção remanescente(resto) do vetor

	wtime = omp_get_wtime();

    #pragma omp parallel num_threads(T) shared(vetor) private(i, ini, end, threadNum)
    {

    threadNum = omp_get_thread_num(); // Número de thread

    ini = (tam/T) * threadNum; 
     
    if(threadNum == T-1){ // Tratamento para o resto, colocando na última thread
        end = (tam/T)* (threadNum +1) + remainder;    
    }else{
        end = (tam/T)* (threadNum +1);
    }

	// iniciando vetor e fixando o maior valor para validacao
	for(i = ini; i < end; i++)
		vetor[i] = 1;
        int num = vetor[i];
    }

    #pragma omp barrier // Barreira de sincronização

    int j;
    
    for(j = 0; j < tam; j++){
        printf("%d ", vetor[j]);
    }
    
	vetor[tam/2] = tam;
        
	maior = vetor[0];

	#pragma omp parallel for num_threads(T) private(i) reduction (max: maior)

	for(i = 1; i < tam; i++)
	{
		if(vetor[i] > maior)
			maior = vetor[i];
	} // fim do for e da regiao paralela


	/*
	*************************************************************************************
	Não modifique este trecho de código
	*/
   
	wtime = omp_get_wtime() - wtime;

    printf("PAR REDUCTION: Tam=%d, maior=%d, Elapsed wall clock time = %f  \n", tam, maior, wtime); //Imprime o vetor ordenado
    free(vetor); //Desaloca o vetor lido
	
    return 0;
}
