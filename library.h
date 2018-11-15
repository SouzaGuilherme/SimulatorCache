// Library.h of simulator cache
#ifndef library_h_
#define library_h_

typedef struct cacheComplete{
	long unsigned int** cache;					// Ponteiro uma matrix
	long unsigned int numberSets;				// Numero de conjuntos
	long unsigned int blockSize;				// Tamanho do bloco
	long unsigned int associativity;			// Associatividade da cache
	char nameArq[20];							// Armazera o nome do .dat
	long unsigned int hit;						// Numero de hits da cache
	long unsigned int miss;						// Numero de misses da cache
	long unsigned int missCompulsorio;			// Numero de misses compulsorios da cache
	long unsigned int missConflito;				// Numero de misses de conflito da cache
	long unsigned int missCapacidade;			// Numero de misses de capacidade da cache
	long unsigned int sizeCache;				// Tamanho total da cache
	long unsigned int operations;				// Numero de operacoes executadas pelo simulador
	int *vetEnd;								// Ponteiro de acesso ao vetor de inteiros que contem os enderecos
	float timeMed;
	float timeHit;
	float fault;
}cacheConfig;


/*	-Recebe como parametro o ponteiro de acesso a estrutura.
	A funcao se encarrega de alocar memoria para cache criando 
	-uma matriz Matriz de (numberSets x associativity+1).
	Todas posicoes matriz[n][0] recebem o valor zero O que 
	-indica que e o bit de validade.
	-O restante da matriz e preenchida com valores -1 para melhor localidade dentro da mesma.
*/ 
void startCache(cacheConfig *AcessCache);


/*	-Recebe como parametro o ponteiro de acesso a estrutura.
	-A funcao se encarrega de procurar os enderecos passados
	todas informacoes necessarias entao dentro da estrutura 
	cacheConfig.
	-Inicialmente chama a funcao indexMod(), para localizar em qual
	linha se encontra o endereco, pelo qual deseja encontrar.
	-Caso o endereco seja encontrado ela indicara o numero de hits.
	-Caso o endereco seja inexistente dentro da cache, ela indeicara 
	os misses e os classificara em Compulsorio, Conflito e Capacidade.
	-Apos classificalos ela chamara a funcao writeCache(), para que
	o endereco possa ser escrito na cache.
	-A funcao nao ha retorno pelo fato de ela preencher todos os dados
	resultantes e necessarios na estrutura.
*/ 
void searchEnd(cacheConfig *AcessCache);


/*	-Recebe como copia.
	@value endereco o qual foi passado
	@mod as N entradas que a cache possui.
	-A funcao se encarrega de realizar a operacao MOD
	de forma: endereco MOD N entradas.
	-A funcao retornara em qual N entrada o endereco 
	se encontra ou devera ser escrito.
*/
long unsigned int indexMod(long unsigned int value, long unsigned int mod);


/*	-Recebe como parametro e por copia, o ponteiro de acesso a estrutura,
	@index (n linha da cache) onde devera ser escrito o endereco
	@endValue o endereco (dado) a ser escrito
	-A funcao se encarrega de escrever o endereco na posicao ideal,
	fazendo a subtituicao caso seja necessario e atualizando o bit
	de validade para 1.
*/
void writeCache(cacheConfig *AcessCache, long unsigned int index, int endValue);


/*	-Recebe como parametro ponteiro de acesso a estrutura.
	-A funcao se encarrega de printas todas informacoes e
	resultados obtidos com a execucao do simulador.
*/
void printResults(cacheConfig *AcessCache);


/*	-Funcao recebe como copia.
	@val valor de endereco obtido da leitura do arquivo.
	-A funcao se encarregara de converter tal endereco
	lido do arquivo binario para um valor decimal.
	-A funcao retornara o valor em decimal.
*/
int conversion(int val);


/*	-Funcao recebe como parametro o ponteiro de acesso a estrutura.
	-A funcao se encarregara da abertura e leitura do arquivo.dat.
	-Ira chamar a funcao conversion() para converter os valores lidos
	e armazenalos em um vetor de inteiros.
	-A Funcao retornara um ponteiro para vetor de inteiros.
*/
int* readEnd(cacheConfig *AcessCache);

// void memoryFree(cacheConfig *AcessCache);

#endif