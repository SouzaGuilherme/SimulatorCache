// Library.h of simulator cache
#ifndef library_h_
#define library_h_

typedef struct cacheComplete{
	long unsigned int** cache;
	long unsigned int numberSets;
	long unsigned int blockSize;
	long unsigned int associativity;
	long unsigned int hit;
	long unsigned int miss;
	long unsigned int missCompulsorio;
	long unsigned int missConflito;
	long unsigned int missCapacidade;
	char nameArq[20];
}cacheConfig;

void startCache(cacheConfig *AcessCache);

void searchEnd(cacheConfig *AcessCache, long unsigned int *vetEnd, int cont);

long unsigned int indexMod(long unsigned int value, long unsigned int mod);

void writeCache(cacheConfig *AcessCache, long unsigned int index, long unsigned int endValue);

void printResults(cacheConfig *AcessCache);

int conversion (char bin[]);

int* openReadArq();

#endif