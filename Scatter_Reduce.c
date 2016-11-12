#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
/* função que decide se n é primo, sendo n maior do que 1
retorna 1 caso o número seja primo, e 0 caso não seja */
int isprime(int n)
{
	int i, squareroot;
	if (n > 1) {
		squareroot = (int) sqrt((double) n);
		for (i = 3; i <= squareroot; i = i + 2) {
			if ((n % i) == 0) {
				return 0;
			}
		}
		return 1;
	} 
	else {
		return 0;
	}
}

int main(int argc, char **argv)
{
	int *n, i; /* numero a ser testado */
	int temp = 0;
	int size, *parc;
	int pc=0; /* prime counter */
	int limit; /* maior número a ser testado */
	int id, thr;
	int sizePthr;
	int b = 0;
	int mod;
	MPI_Status status;

	MPI_Init(&argc,&argv); // inicializa
	/* quem sou? */
	MPI_Comm_rank(MPI_COMM_WORLD,&id);
	/* quantos são? */
	MPI_Comm_size(MPI_COMM_WORLD,&thr);

	if(argc < 2) {
		printf("Digite o numero a ser testado\n");
		return 0;
	} else {
		sscanf(argv[1], "%d", &limit);
	}
	size = (limit+1)/2;
	sizePthr = size/(thr);
	mod = size%thr;
	if(id == 0){
		n=(int*) malloc(size*sizeof(int));
		for(i = 1; i <= limit; i = i + 2){
			n[b]=i;
			b++;
		}
		printf("%d\n",mod );

	}
	parc = (int*) malloc((sizePthr)*sizeof(int));
	MPI_Scatter(n, sizePthr, MPI_INT, parc, sizePthr, MPI_INT, 0, MPI_COMM_WORLD);
	//MPI_Gather(n, size/thr, MPI_INT, n, size/thr, MPI_INT, 0, MPI_COMM_WORLD);
	//....

	for (i = 0; i < sizePthr; i++)
	{
		temp +=isprime(parc[i]);
	}
	MPI_Reduce(&temp, &pc, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	

	if(id == 0){
		//In the case, the division wasn't to all
		for (i = size-mod; i < b; i++)
		{
			pc +=isprime(n[i]);
		}
		printf("\nFinalizado.\nTotal de primos encontrados %d\n", pc);
	}
	
	MPI_Finalize();

	return 0;
}