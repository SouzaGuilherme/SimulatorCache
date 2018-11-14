// Library.c the functions of library.h the simulator Cache

#include<stdio.h>
#include<string.h>
#include<math.h>
#include <stdlib.h>
#include "library.h"

int conversion (char bin[]){
	unsigned long dec = 0;
   	int i = 0;
    int s;
    //fgets( bin, sizeof(bin), stdin);
    s = strlen( bin ); 
    while( s-- ) {
        if( bin[s] == '0' || bin[s] == '1' ) {
            dec = dec + pow(2, i++) * (bin[s] - '0');
        }
    };
   	// printf("\nDecimal Equivalent of Binary Number: \t %u\n", dec);
    return dec;
};

int* openReadArq(){
	int cont=0;
	char url[]="arqBinario2.dat",info[32];
	FILE *arq;
	
	arq = fopen(url, "r");
	if(arq == NULL)
		printf("Erro, nao foi possivel abrir o arquivo\n");
	else{
		while( (fgets(info, sizeof(info), arq))!=NULL ){
			cont++;
		}		
	}
	fclose(arq);
	
	int vetIndex[cont];	
	arq = fopen(url, "r");
	if(arq == NULL)
		printf("Erro, nao foi possivel abrir o arquivo\n");
	else{
		int i = 0;
		while( (fgets(info, sizeof(info), arq))!=NULL ){
			vetIndex[i] = conversion(info);
			i++;
			
		}
	}
	fclose(arq);

	for(int i=0 ; i < cont; i++){
		printf("\n %d binary e igual a: %d ",i+1,vetIndex[i]);
	}

	return 0;
};


void startCache(cacheConfig *AcessCache){
	if (AcessCache->numberSets == AcessCache->associativity){
		// Logo a cache e totalmente associativa
		AcessCache->cache = (long unsigned int**) malloc(sizeof(long unsigned int*));
		if (AcessCache->cache == NULL){
	 		printf("Erro ao alocar Cache\n");
	 		exit(1);
		}
		for (long unsigned int i = 0; i < AcessCache->numberSets+1; ++i)
	 		AcessCache->cache[i] = (long unsigned int*) malloc(sizeof(long unsigned int));
	}else{
		AcessCache->cache = (long unsigned int**) malloc(((AcessCache->numberSets)/AcessCache->associativity)*sizeof(long unsigned int*));
		if (AcessCache->cache == NULL){
		 	printf("Erro ao alocar Cache\n");
		 	exit(1);
		}
		
		for (long unsigned int i = 0; i < AcessCache->numberSets+1; ++i)
		 	AcessCache->cache[i] = (long unsigned int*) malloc(sizeof(long unsigned int));

	}
	// Mexer aqui pois ira gerar vazamento de memoria;

	for (long unsigned int i = 0; i < AcessCache->numberSets/AcessCache->associativity; ++i)
		AcessCache->cache[i][0] = 0;

	for (long unsigned int i = 1; i <= AcessCache->numberSets/AcessCache->associativity; ++i){
		for (long unsigned int j = 1; i <= AcessCache->associativity; ++i){
			AcessCache->cache[i][j] = -1;
		}
	}	
};


void searchEnd(cacheConfig *AcessCache, long unsigned int *vetEnd, int cont){
	// Acesso o primeiro endereco
	int index, i=0;
	int flagEncontrouEnd = 0;
	int NUMERO = cont;	// Saberei por parametro ? pois outra funcao ja sabera
	while(i <= NUMERO){
		if (AcessCache->numberSets == AcessCache->associativity)
			index = 0;
		else
			index = indexMod(vetEnd[i], AcessCache->numberSets/AcessCache->associativity);	// Criar essa funcao ainda
		// Verifico o Bit de Verificacao
		if (AcessCache->cache[index][0] == 1){
			// Significa que ha um dado aqui dentro
			for (int j = 1; j <= AcessCache->associativity; ++j){
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
					if (AcessCache->cache[index][AcessCache->associativity+1] != -1)
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
	}
};


long unsigned int indexMod(long unsigned int value, long unsigned int mod){
	return value%mod;
};


void writeCache(cacheConfig *AcessCache, long unsigned int index, long unsigned int endValue){
	int flagInsertPosicionNull = 0;
	// Seto o bit de verificacao para 1
	AcessCache->cache[index][0] = 1;
	if (AcessCache->associativity == 1){
		// Mapeamento Direto
		AcessCache->cache[index][1] = endValue;
	}else if(AcessCache->associativity >1 && AcessCache->associativity <=4){
		// Associatividade 2 e 4
		// Enquanto tiver lugar ele insere
		for (long unsigned int i = 0; i < AcessCache->associativity; ++i){
			if (AcessCache->cache[index][i] == -1){
				AcessCache->cache[index][i] = endValue;
				flagInsertPosicionNull = 1;
				break;
			}
		}
		// Caso nao tenha lugar ele randoniza uma posicao e insere
		if (flagInsertPosicionNull == 0){
			int j = rand()%(AcessCache->associativity);
			AcessCache->cache[index][j+1] = endValue;
		}
	}else{
		// Totalmente Associativa
		for (int j = 1; j <= AcessCache->associativity; ++j){
			if (AcessCache->cache[0][j] != -1){
				AcessCache->cache[0][j] = endValue;
				flagInsertPosicionNull = 1;
				break;
			}
		}
		if (flagInsertPosicionNull == 0){
			if (AcessCache->cache[0][AcessCache->associativity] != -1 ){
				// Cache cheia, logo escrevo numa posicao randomica
				long int positionRand = rand()%(AcessCache->associativity);
				AcessCache->cache[0][positionRand] = endValue;
			}

		}
	}
};


void printResults(cacheConfig *AcessCache){
	printf("\n\nSimulador NOME DO SIMULADOR\n\n");
	printf("--- Configuracoes da Cache ---\n");
	printf("Numero de Conjuntos:             %lu\n", AcessCache->numberSets);
	printf("Tamanho do Bloco:                %lu\n",AcessCache->blockSize);
	printf("Associatividade:                 %lu\n\n",AcessCache->associativity);
	printf("--- Funcoes executadas pelo simulador ---\n");
	printf("Arquivo de Enderecos:            %s\n", AcessCache->nameArq);
	printf("Numero de Operacoes:             \n\n"/*Tem que por a quantidade gerada aqui*/);
	printf("--- Resultados Obtidos ---\n");
	printf("Numero de HIT:                   %lu\n", AcessCache->hit);
	printf("Numero de MISS:                  %lu\n", AcessCache->miss);
	printf("Numero de Miss Compulsorio:      %lu\n", AcessCache->missCompulsorio);
	printf("Numero de Miss Conflito:         %lu\n", AcessCache->missConflito);
	printf("Numero de Miss capacidade:       %lu\n\n", AcessCache->missCapacidade);
	printf("Tempo medio de acesso a Cache:\n\n");
	printf("--- Fim do Simulador ---\n\n\n");
}