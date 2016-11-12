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
	int n; /* numero a ser testado */
	int sendTo;
	int pc=0; /* prime counter */
	int limit; /* maior número a ser testado */
	int id, thr;
	int b;
	int zero = 0;
	MPI_Status status;

	MPI_Init(&argc,&argv); // inicializa
	/* quem sou? */
	MPI_Comm_rank(MPI_COMM_WORLD,&id);
	/* quantos são? */
	MPI_Comm_size(MPI_COMM_WORLD,&thr);
	if(thr > 1){
		if (id==0) {
			sendTo = 1;
			if(argc < 2) {
				printf("Digite o numero a ser testado\n");
				return 0;
			} else {
				sscanf(argv[1], "%d", &limit);
			}
			printf("\nEncontrar numeros primos entre 1 e %d (inclusive).\nCalculando...\n", limit);
			for (n = 1; n <= limit; n = n + 2){  /* testa sequencialmente se um número é primo ou nao */
				MPI_Send(&n,1,MPI_INT,sendTo,100,MPI_COMM_WORLD);
				if (sendTo == thr - 1)
				{
					sendTo = 1;
				}
				else
					sendTo++;

			}
			//Need to find a better way to do this - What this does? unlock useless thrs
			for (n = 1; n < thr; n++){
				
				MPI_Send(&zero,1,MPI_INT,n,100,MPI_COMM_WORLD);
			}

			//
			for (n = 1; n < thr; n++){
				MPI_Recv(&b,1,MPI_INT,n,101,MPI_COMM_WORLD, &status);
				pc = pc + b;
			}
			

			printf("\nFinalizado.\nTotal de primos encontrados %d\n", pc);
		}
		else {
			do
			{
				MPI_Recv(&n,1,MPI_INT,0,100,MPI_COMM_WORLD, &status);
				pc += isprime(n);
			}while(n);
			MPI_Send(&pc,1,MPI_INT,0,101,MPI_COMM_WORLD);
		}
	}
	else{
		if(argc < 2) {
			printf("Digite o numero a ser testado\n");
				return 0;
		} else {
			sscanf(argv[1], "%d", &limit);
		}
		for (n = 1; n <= limit; n = n + 2){  /* testa sequencialmente se um número é primo ou nao */
			pc+= isprime(n);
		}
		printf("\nFinalizado.\nTotal de primos encontrados %d\n", pc);
	}
	MPI_Finalize();

	return 0;
}