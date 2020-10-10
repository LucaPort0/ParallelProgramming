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
    int localMaior; // Variável que armazenará o maior local de cada thread

    // Definindo lock
    omp_lock_t mylock;
	omp_init_lock(&mylock);

    wtime = omp_get_wtime(); // Inicializando contador de tempo

    maior = 0; // Inicializando um maior global

    // Preenchendo vetor
    #pragma omp parallel num_threads(T) shared(vetor, maior, tam) private(i, ini, end, threadNum, localMaior)
    {

    threadNum = omp_get_thread_num(); // Número da thread

    ini = (tam/T) * threadNum; // Define o inicio de cada bloco em cada thread
     
    if(threadNum == T-1){ // Tratamento para o resto(remainder), colocando-o na última thread
        end = (tam/T)* (threadNum +1) + remainder;    
    }else{
        end = (tam/T)* (threadNum +1);
    }

	// iniciando vetor com nrs "1"
	for(i = ini; i < end; i++){
		vetor[i] = 1;
    }

	vetor[tam/2] = tam; // Inserindo maior valor na metade do vetor
    localMaior = 0;

    ini = (tam/T) * threadNum; // Define o inicio de cada bloco em cada thread
     
    if(threadNum == T-1){ // Tratamento para o resto(remainder), colocando na última thread
        end = (tam/T)* (threadNum +1) + remainder;    
    }else{
        end = (tam/T)* (threadNum +1);
    }

    // Buscando maior valor
	for(i = ini; i < end; i++)
	{
		if(vetor[i] > localMaior)
			localMaior = vetor[i];
    }

    // Lock utilizado para a variavel "maior" receber o valor correto
    omp_set_lock(&mylock);
		if (localMaior > maior)
			maior = localMaior;
	omp_unset_lock(&mylock);

	} // Fim da regiao paralela
    


	/*
	*************************************************************************************
	Não modifique este trecho de código
	*/
   
	wtime = omp_get_wtime() - wtime;

    printf("PAR REDUCTION: Tam=%d, maior=%d, Elapsed wall clock time = %f  \n", tam, maior, wtime); //Imprime o vetor ordenado
    free(vetor); //Desaloca o vetor lido
	
    return 0;
}
