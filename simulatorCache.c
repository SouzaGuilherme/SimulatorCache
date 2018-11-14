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
	int missCompulsorio;
	int missConflito;
	int missCapacidade;
	char nameArq[20];
}cacheConfig;

void startCache(cacheConfig *AcessCache);
void searchEnd(cacheConfig *AcessCache);
int indexMod(int value, int mod);
void writeCache(cacheConfig *AcessCache, int index, int endValue);
void printResults(cacheConfig *AcessCache);

int main(int argc, char* argv[]){
	// recebo os parametros por meio de linha de comando
	cacheConfig AcessCache;
	if (argv[1] != NULL){
		printf("ENTROU\n");
		AcessCache.numberSets = atoi(argv[1]);
		AcessCache.blockSize = atoi(argv[2]);
		AcessCache.associativity = atoi(argv[3]);
		// char nameArq[20] = "";
		strncpy(AcessCache.nameArq, argv[4], 20);



		// testing
		// printf("%d\n", AcessCache.numberSets);
		// printf("%d\n", AcessCache.blockSize);
		// printf("%d\n", AcessCache.associativity);
		// printf("%s\n", nameArq);
		
		// Inicio a Cache com as configuracoes passadas
		startCache(&AcessCache);

		// testing
		// printf("%d\n", AcessCache.numberSets);
		// printf("%d\n", AcessCache.blockSize);
		// printf("%d\n", AcessCache.associativity);
		// printf("%s\n", nameArq);
		
	}else{
		// testing
		// printf("NOOOOO\n");
		// Inicio a Cache com o Default
		AcessCache.numberSets = 1024;
		AcessCache.blockSize = 4;
		AcessCache.associativity = 1;
		// testing
		// printf("%d\n", AcessCache.numberSets);
		// printf("%d\n", AcessCache.blockSize);
		// printf("%d\n", AcessCache.associativity);
		// Testing
		// printf("CHEG\n");
		// Inicio a Cache com as configuracoes passadas
		startCache(&AcessCache);

	}
	// Seto todos os dados em Zero
	AcessCache.hit = 0;
	AcessCache.miss = 0;	
	AcessCache.missCompulsorio = 0;	
	AcessCache.missConflito = 0;	
	AcessCache.missCapacidade = 0;	
		

	// Pesquiso o Endereco
	searchEnd(&AcessCache);

	// printo o resultado
	printResults(&AcessCache); 



	// // Possiveis prints
	// printf("MISS: %d\n", AcessCache.miss);
	// printf("HIT: %d\n", AcessCache.hit);
	// printf("MISS Compulsorio: %d\n", AcessCache.missCompulsorio);
	// printf("MISS Conflito: %d\n", AcessCache.missConflito);
	// printf("MISS capacidade: %d\n", AcessCache.missCapacidade);
	
};

void startCache(cacheConfig *AcessCache){
	if (AcessCache->numberSets == AcessCache->associativity){
		// Logo a cache e totalmente associativa
		AcessCache->cache = (int**) malloc(sizeof(int*));
		if (AcessCache->cache == NULL){
	 		printf("Erro ao alocar Cache\n");
	 		exit(1);
		}
		for (int i = 0; i < AcessCache->numberSets+1; ++i)
	 		AcessCache->cache[i] = (int*) malloc(sizeof(int));
	}else{
		AcessCache->cache = (int**) malloc(((AcessCache->numberSets)/AcessCache->associativity)*sizeof(int*));
		if (AcessCache->cache == NULL){
		 	printf("Erro ao alocar Cache\n");
		 	exit(1);
		}
		
		for (int i = 0; i < AcessCache->numberSets+1; ++i)
		 	AcessCache->cache[i] = (int*) malloc(sizeof(int));

	}
	// Mexer aqui pois gerara vazamento de memoria;

	for (int i = 0; i < AcessCache->numberSets/AcessCache->associativity; ++i)
		AcessCache->cache[i][0] = 0;
	
	printf("ALOCOU TUDO\n");
};

void searchEnd(cacheConfig *AcessCache){
	// Acesso o primeiro endereco
	int index, i=0;
	int flagEncontrouEnd = 0;
	int vetEnd[5]; 
	vetEnd[0] = 1024;	// isso vai vim por parametro
	vetEnd[1] = 10;	// isso vai vim por parametro
	vetEnd[2] = 10;	// isso vai vim por parametro
	vetEnd[3] = 10;	// isso vai vim por parametro
	vetEnd[4] = 2048;	// isso vai vim por parametro
	int NUMERO = 5;	// Saberei por parametro ? pois outra funcao ja sabera
	printf("Porta do while\n");
	while(i != NUMERO){
		printf("Porta do index\n");
		if (AcessCache->numberSets == AcessCache->associativity)
			index = 0;
		else
			index = indexMod(vetEnd[i], AcessCache->numberSets/AcessCache->associativity);	// Criar essa funcao ainda
		printf("Passou index\n");
		// Verifico o Bit de Verificacao
		if (AcessCache->cache[index][0] == 1){
			// Significa que ha um dado aqui dentro
			for (int j = 1; j < AcessCache->associativity+1; ++j){
				// Irei verificar em todas posicoes ao lado do bit de verificacao
				if (AcessCache->cache[index][j] == vetEnd[i]){
					AcessCache->hit++;
					flagEncontrouEnd = 1;
					break;
				}
			}
			if (flagEncontrouEnd == 0){
				AcessCache->miss++;
				// Tratar qual o miss
				if (AcessCache->numberSets == AcessCache->associativity){
					if (AcessCache->cache[index][AcessCache->associativity+1] != NULL)
						// Miss de capacidade
						AcessCache->missCapacidade++;
					else
						AcessCache->missCompulsorio++;
				}else{
					// Miss de Conflito
					AcessCache->missConflito++;
				}
				// Escrever na cache o endereco que nao existe		
				writeCache(AcessCache, index, vetEnd[i]);
			}
		}else{
			AcessCache->miss++;
			// Tratar qual o miss
			// Miss Compulsorio
			AcessCache->missCompulsorio++;
			// Escrever na cache o endereco que nao existe
			writeCache(AcessCache, index, vetEnd[i]);
		}
		++i;
		printf("Passou ++i %d\n", i);
	}
};


int indexMod(int value, int mod){
	return value%mod;
};


void writeCache(cacheConfig *AcessCache, int index, int endValue){
	int flagInsertPosicionNull = 0;
	// Seto o bit de verificacao para 1
	AcessCache->cache[index][0] = 1;
	if (AcessCache->associativity == 1){
		// Mapeamento Direto
		AcessCache->cache[index][1] = endValue;
	}else if(AcessCache->associativity >1 && AcessCache->associativity <=4){
		// Associatividade 2 e 4
		// Enquanto tiver lugar ele insere
		for (int i = 0; i < AcessCache->associativity; ++i){
			if (AcessCache->cache[index][i] == NULL){
				AcessCache->cache[index][i] = endValue;
				flagInsertPosicionNull = 1;
				break;
			}
		}
		// Caso nao tenha lugar ele randoniza uma posicao e insere
		if (flagInsertPosicionNull == 0){
			int j = rand()%(AcessCache->associativity+1);
			AcessCache->cache[index][j] = endValue;
		}
	}else{
		// Totalemnte Associativa
		for (int j = 1; j < AcessCache->associativity; ++j){
			if (AcessCache->cache[0][j] != NULL){
				AcessCache->cache[0][j] = endValue;
				flagInsertPosicionNull = 1;
				break;
			}
		}
		if (flagInsertPosicionNull == 0){
			if (AcessCache->cache[0][AcessCache->associativity] != NULL ){
				// Cache cheia, logo escrevo numa posicao randomica
				int positionRand = rand()%(AcessCache->associativity);
				AcessCache->cache[0][positionRand] = endValue;
			}

		}
	}
};


void printResults(cacheConfig *AcessCache){
	printf("\n\nSimulador NOME DO SIMULADOR\n\n");
	printf("--- Configuracoes da Cache ---\n");
	printf("Numero de Conjuntos:             %d\n", AcessCache->numberSets);
	printf("Tamanho do Bloco:                %d\n",AcessCache->blockSize);
	printf("Associatividade:                 %d\n\n",AcessCache->associativity);
	printf("--- Funcoes executadas pelo simulador ---\n");
	printf("Arquivo de Enderecos:            %s\n", AcessCache->nameArq);
	printf("Numero de Operacoes:             \n\n"/*Tem que por a quantidade gerada aqui*/);
	printf("--- Resultados Obtidos ---\n");
	printf("Numero de HIT:                   %d\n", AcessCache->hit);
	printf("Numero de MISS:                  %d\n", AcessCache->miss);
	printf("Numero de Miss Compulsorio:      %d\n", AcessCache->missCompulsorio);
	printf("Numero de Miss Conflito:         %d\n", AcessCache->missConflito);
	printf("Numero de Miss capacidade:       %d\n\n", AcessCache->missCapacidade);
	printf("Tempo medio de acesso a Cache:\n\n");
	printf("--- Fim do Simulador ---\n\n\n");
}