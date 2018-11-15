// main simulator cache
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "library.h"

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
		strncpy(AcessCache.nameArq, "arqBinario1.dat", 20);
		// Inicio a Cache com as configuracoes passadas
		startCache(&AcessCache);

	}
	// Seto todos os dados em Zero
	AcessCache.hit = 0;
	AcessCache.miss = 0;	
	AcessCache.missCompulsorio = 0;	
	AcessCache.missConflito = 0;	
	AcessCache.missCapacidade = 0;
	AcessCache.sizeCache = AcessCache.numberSets*AcessCache.blockSize*AcessCache.associativity;	
	AcessCache.timeMed = 0;
	//Le os enderecos passados o os converte para inteiros
	AcessCache.vetEnd = readEnd(&AcessCache);
	AcessCache.timeHit = 1.0;
	AcessCache.fault = 20.0;
	
	// Pesquiso o Endereco
	searchEnd(&AcessCache);
	// Calculo do tempo medio
	AcessCache.timeMed = ((float)AcessCache.miss/(float)AcessCache.operations);
	AcessCache.timeMed = 0.52f*20.0f;
	
	// printo o resultado
	printResults(&AcessCache); 

	return 0;	
};