// main simulator cache
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <conio.h>
// #include "library.h"

typedef struct cacheComplete{
	int** cache;
	int numberSets;
	int blockSize;
	int associativity;
	int hit;
	int miss;

}cacheConfig;

void startCache(int numberSets, int blockSize, int associativity);

int main(int argc, char* argv[]){
	// recebo os parametros por meio de linha de comando
	cacheConfig *AcessCache;

	if (argv[1] != NULL){
		AcessCache->numberSets = atoi(argv[1]);
		AcessCache->blockSize = atoi(argv[2]);
		AcessCache->associativity = atoi(argv[3]);
		char nameArq[20] = "";
		strncpy(nameArq, argv[4], 20);	

		// Inicio a Cache com as configuracoes passadas
		startCache(AcessCache->numberSets, AcessCache->blockSize, AcessCache->associativity);

		// testing
		printf("%d\n", AcessCache->numberSets);
		printf("%d\n", AcessCache->blockSize);
		printf("%d\n", AcessCache->associativity);
		printf("%s\n", nameArq);
	}else{
		// testing
		printf("NOOOOO\n");
		// Inicio a Cache com o Default
		AcessCache->numberSets = 1024;
		AcessCache->blockSize = 4;
		AcessCache->associativity = 1;
		// Inicio a Cache com as configuracoes passadas
		startCache(AcessCache->numberSets, AcessCache->blockSize, AcessCache->associativity);
	}

};

void startCache(int numberSets, int blockSize, int associativity){
	cacheConfig *AcessCache;
	AcessCache->cache = (int**) malloc(numberSets*sizeof(int*));
	if (AcessCache->cache == NULL){
	 	printf("Erro ao alocar Cache\n");
	 	exit(1);
	}
	
	for (int i = 0; i < numberSets; ++i)
	 	AcessCache->cache[i] = (int*) malloc((associativity)*sizeof(int));
	// Mexer aqui pois gerara vazamento de memoria;
};

void searchEnd(){
	// Acesso o primeiro endereco
	int index, i;
	int vetEnd[5];	// isso vai vim por parametro
	cacheConfig *AcessCache;	// Talvez por parametro ?
	int NUMERO = 5;	// Saberei por parametro ? pois outra funcao ja sabera
	while(vetEnd[i] != NUMERO){
		index = indexMod(vetEnd[i]);	// Criar essa funcao ainda
		// Verifico o Bit de Verificacao
		if (AcessCache->cache[index][0] == 1){
			// Significa que ha um dado aqui dentro
			for (int j = 1; j < AcessCache->associativity; ++j){
				// Irei verificar em todas posicoes ao lado do bit de verificacao
				if (AcessCache->cache[index][j] == vetEnd[i]){
					AcessCache->hit++;
				}else{
					AcessCache->miss++;
					// Tratar qual o miss
					// Provavel miss de Conflito
					// Escrever na cache o endereco que nao existe		
					writeCache(index, i);
				}
			}
		}else{
			AcessCache->miss++;
			// Tratar qual o miss
			// Miss Compulsorio
			// Escrever na cache o endereco que nao existe
			writeCache(index, i);
		}
		i++;
	}
};

void writeCache(int index, int i){
	cacheConfig *AcessCache;
	int vetEnd[5];	// isso vai sair daqui
	// Seto o bit de verificacao para 1
	AcessCache->cache[index][0] = 1;
	if (AcessCache->associativity == 1){
		// Mapeamento Direto
		AcessCache->cache[index][1] = vetEnd[i];
	}else if(AcessCache->associativity >1 && AcessCache->associativity <=4){
		// Associatividade 2 e 4
		for (int i = 0; i < AcessCache->associativity; ++i){
			// Politica de substituicao Randomica
			int j = rand()%(AcessCache->associativity+1);
			AcessCache->cache[index][j] = vetEnd[i];
		}
	}else{
		// Totalemnte Associativa
	}
		
};