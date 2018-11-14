// main simulator cache
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
// #include <conio.h>
#include "library.h"

long unsigned int* enderecos(long unsigned int* p){
	long unsigned int vetEnd[100];
	for (int i = 0; i < 100; ++i)
		vetEnd[i]=-1;
	for (int i = 0; i < 100; ++i){
		int elev = 1+(rand()%10);
		vetEnd[i] = elev;
		// printf("TA EM -%d\n", j);
		// j++;
		// printf("vetEnd[%d] = %lu\n", j, vetEnd[i]);
	}
	p = &vetEnd[0];
	return p;
}

int main(int argc, char* argv[]){
	// recebo os parametros por meio de linha de comando
	cacheConfig AcessCache;
	if (argv[1] != NULL){
		AcessCache.numberSets = atoi(argv[1]);
		AcessCache.blockSize = atoi(argv[2]);
		AcessCache.associativity = atoi(argv[3]);
		strncpy(AcessCache.nameArq, argv[4], 20);

		// Inicio a cache
		startCache(&AcessCache);		
	}else{
		AcessCache.numberSets = 1024;
		AcessCache.blockSize = 4;
		AcessCache.associativity = 1;
		// Inicio a Cache com as configuracoes passadas
		startCache(&AcessCache);

	}
	// Seto todos os dados em Zero
	AcessCache.hit = 0;
	AcessCache.miss = 0;	
	AcessCache.missCompulsorio = 0;	
	AcessCache.missConflito = 0;	
	AcessCache.missCapacidade = 0;	
		
	// Gero os enderecos

	//Le os enderecos passados o os converte para inteiros
	long unsigned int *p;
	p = enderecos(p);// Nao sei se pode mas receberia o retorno com a quantidade da funcao
	int i=0, cont=0;
	do{
		cont++;
		printf("Vetor[%d] = %lu\n", i, p[i]);
		printf("CONT == %d\n", cont);
		i++;
	}while(i != 100 );

	// Pesquiso o Endereco
	searchEnd(&AcessCache, p, cont);

	// printo o resultado
	printResults(&AcessCache); 

	return 0;	
};