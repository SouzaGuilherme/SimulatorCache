// Library.h of simulator cache
#ifndef cache
#define cache

#include <stdint.h> 
struct status{
	unsigned long hits_l1;
	unsigned long misses_l1
};

struct status *cache(int tamanho_bloco, int tamanho_cache, char *filename);

#endif