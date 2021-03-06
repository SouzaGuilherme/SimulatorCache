// Library.c the functions of library.h the simulator Cache
#include<stdio.h>
#include<string.h>
#include<math.h>
#include <stdlib.h>
#include "library.h"

int conversion( int val ){
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF ); 
    return (val << 16) | ((val >> 16) & 0xFFFF);
};

int* readEnd(cacheConfig *AcessCache){
	FILE *arq;     
	int numLinha;  
	size_t result; 
	int *p;
	int numberEnd = 0;
	
	arq = fopen(AcessCache->nameArq,"r");
	if (!arq) {
		printf("Erro na abertura do arquivo original.dat.\n");
		exit(1);
	}

	for (int i = 1;!feof(arq); ++i) {
		numLinha = 0;
		
		result = fread(&numLinha, 4, 1, arq); 
		numberEnd++;
	}
	AcessCache->operations = numberEnd-1;
	fclose(arq);

	arq = fopen(AcessCache->nameArq,"r");
	if (!arq) {
		printf("Erro na abertura do arquivo original.dat.\n");
		exit(1);
	}

	int vetorEnd[numberEnd];
	for (int i = 1;!feof(arq); ++i) {
		numLinha = 0;
		
		result = fread(&numLinha, 4, 1, arq); 
		if (result) {
		
			numLinha = conversion(numLinha);
			vetorEnd[i] = numLinha;
		}
	}

	fclose(arq);
	p = &vetorEnd[0];
	return p;
};

void startCache(cacheConfig *AcessCache){
	int i=0, j=0;
	AcessCache->cache = (long unsigned int**) malloc((AcessCache->numberSets/AcessCache->associativity)*sizeof(long unsigned int*));
	if (AcessCache->cache == NULL){
		printf("Erro ao alocar Cache\n");
		exit(1);
	}
	
	for (i = 0; i < (AcessCache->numberSets/AcessCache->associativity); i++){ //Percorre as linhas do Vetor de Ponteiros
       AcessCache->cache[i] = (long unsigned int*) malloc((AcessCache->associativity+1) * sizeof(long unsigned int)); //Aloca um Vetor de Inteiros para cada posição do Vetor de Ponteiros.
       for (j = 0; j < (AcessCache->associativity+1); j++){ //Percorre o Vetor de Inteiros atual.
            AcessCache->cache[i][j] = -1; //Inicializa com -1.
       }
  	}		
};


void searchEnd(cacheConfig *AcessCache){
	// Acesso o primeiro endereco
	long unsigned int index=0, i=0;
	while(i < AcessCache->operations){
		int flagEncontrouEnd = 0;
		// Verifico se é totalmente associativa e arrumo o index
		if (AcessCache->numberSets == AcessCache->associativity){
			index = 0;
		}else{
			index = indexMod(AcessCache->vetEnd[i], AcessCache->numberSets/AcessCache->associativity);
		}
		// Verifico o Bit de Verificacao
		if (AcessCache->cache[index][0] == 1){
			// Significa que ha um dado aqui dentro
			for (int j = 1; j <= AcessCache->associativity; ++j){
				// Irei verificar em todas posicoes ao lado do bit de verificacao
				if (AcessCache->cache[index][j] == AcessCache->vetEnd[i]){
					AcessCache->hit++;
					flagEncontrouEnd = 1;
					break;
				}
			}
			if (flagEncontrouEnd == 0){
				AcessCache->miss++;
				// Tratar qual o miss
				if (AcessCache->numberSets == AcessCache->associativity){
					// Verifico se a ultima posicao ainda contem o valor de preenchimento da matriz
					if (AcessCache->cache[index][AcessCache->associativity] != -1){
						// Miss de capacidade
						AcessCache->missCapacidade++;
					}else{
						AcessCache->missCompulsorio++;
					}
				}else{
					// Miss de Conflito
					AcessCache->missConflito++;
				}
				// Escrever na cache o endereco que nao existe		
				writeCache(AcessCache, index, AcessCache->vetEnd[i]);
			}
		}else{
			AcessCache->miss++;
			// Miss Compulsorio
			AcessCache->missCompulsorio++;
			// Escrever na cache o endereco que nao existe
			writeCache(AcessCache, index, AcessCache->vetEnd[i]);
		}
		++i;
	}
};


long unsigned int indexMod(long unsigned int value, long unsigned int mod){
	long unsigned int auxMod = value%mod;
	return auxMod;
};


void writeCache(cacheConfig *AcessCache, long unsigned int index, int endValue){
	int flagInsertPosicionNull = 0;
	// Seto o bit de verificacao para 1
	AcessCache->cache[index][0] = 1;
	if (AcessCache->numberSets == AcessCache->associativity){
		// Cache Totalmente associativa
		for (int j = 1; j <= AcessCache->associativity; ++j){
			if (AcessCache->cache[index][j] == -1){
				// Achou local disponivel e armazenou
				AcessCache->cache[index][j] = endValue;
				flagInsertPosicionNull = 1;
				break;
			}
		}

		if (flagInsertPosicionNull == 0){
			// Não tem lugar disponivel, aplica-se Rand
			int positionRand = 0;
			while(positionRand == 0){
				positionRand = (rand()%(AcessCache->associativity));
			}
			AcessCache->cache[index][positionRand] = endValue;
		}

	}else if (AcessCache->associativity == 1){
		// Como e MP nao preciso percorrer as posicoes ao lado
		AcessCache->cache[index][1] = endValue;

	}else{
		// Qualquer outra associatividade
		for (int k = 1; k <= AcessCache->associativity; ++k){
			if (AcessCache->cache[index][k] == -1){
				AcessCache->cache[index][k] = endValue;
				flagInsertPosicionNull = 1;
				break;
			}
		}

		if (flagInsertPosicionNull == 0){
			// Não tem lugar disponivel, aplica-se Rand
			int positionRand = 0;
			while(positionRand == 0){
				positionRand = (rand()%(AcessCache->associativity));
			}
			AcessCache->cache[index][positionRand] = endValue;
		}
	}
};


void printResults(cacheConfig *AcessCache){
	system("reset");
	printf("\nSimulador NOME DO SIMULADOR\n\n");
	printf("--- Configuracoes da Cache ---\n");
	printf("Numero de Conjuntos:             %lu\n", AcessCache->numberSets);
	printf("Tamanho do Bloco:                %lu\n",AcessCache->blockSize);
	printf("Associatividade:                 %lu\n",AcessCache->associativity);
	printf("Tamanho da Cache:                %lub\n\n", AcessCache->sizeCache);
	printf("--- Funcoes executadas pelo simulador ---\n");
	printf("Arquivo de Enderecos:            %s\n", AcessCache->nameArq);
	printf("Numero de Operacoes:             %lu\n\n", AcessCache->operations);
	printf("--- Resultados Obtidos ---\n");
	printf("Numero de HIT:                   %lu\n", AcessCache->hit);
	printf("Numero de MISS:                  %lu\n", AcessCache->miss);
	printf("Numero de Miss Compulsorio:      %lu\n", AcessCache->missCompulsorio);
	printf("Numero de Miss Conflito:         %lu\n", AcessCache->missConflito);
	printf("Numero de Miss capacidade:       %lu\n\n", AcessCache->missCapacidade);
	
	printf("--- Fim do Simulador ---\n\n\n");
}