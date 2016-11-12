#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#define SIZE 1600   
/* tamanho do vetor */
int main ( int argc, char **argv ){
	int id,tam;
	int occurrences;   /* n�mero de ocorr�ncias do n�mero alvo */
	int target;           /* n�mero a ser buscado */
	int i;                   /* iterador */
	int *vec,*calc;      /* vetor no qual o n�mero ser� buscado */
	int randValue;
	MPI_Init(&argc,&argv); // inicializa
	/* quem sou? */
	MPI_Comm_rank(MPI_COMM_WORLD,&id);
	/* quantos são? */
	MPI_Comm_size(MPI_COMM_WORLD,&tam);
	occurrences = 0;
	if (id == 0){
		vec=(int *) malloc(SIZE*sizeof(int));
		//printf("Digite o vetor a ser analisado separando cada elemento em uma linha diferente: ");
		for(i=0; i<SIZE; i++){
			scanf("%d", &vec[i]);   /* leitura do vetor */
		}
		printf("Digite o valor a ser buscado:\n ");
		scanf("%d", &target);   /* leitura do n�mero a ser buscado */
	    
		
	}
	MPI_Bcast(&target, 1, MPI_INT, 0,MPI_COMM_WORLD);
	calc=(int *) malloc((SIZE/tam)*sizeof(int));
	MPI_Scatter(vec, SIZE/tam, MPI_INT, calc,SIZE/tam ,MPI_INT, 0, MPI_COMM_WORLD);
	for (i = 0; i < SIZE/tam; i++)
	{
		if(calc[i] == target) 
			calc[i] = 1;
		else
			calc[i] = 0;
	}
	MPI_Gather(calc, SIZE/tam, MPI_INT, vec, SIZE/tam,MPI_INT, 0, MPI_COMM_WORLD); //It's a little weird i know =.=
	
	if (id == 0)
	{
		for (i = 0; i < SIZE; i++){	
			if(vec[i] == 1) 
				occurrences++;
		}
	
		printf("\nFinalizado\n\nNumero de vezes em que %d aparece no vetor e: %d\n",target, occurrences);
	}
	MPI_Finalize();

	return 0;
}