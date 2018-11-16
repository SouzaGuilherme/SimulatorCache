# SimulatorCache
Cache simulator developed for computer architecture discipline 2.

## Enunciado
[Flex Grid]()

## Relatorio
Foi desenvolvido 3 arquivos os quais sao essenciais para a execucao do trabalho aqui apresentado:
1. SimulatorCache.c
2. library.c
3. library.h

Um executavel do simulador tambem foi adicionado para que nao haja necessidade de recompilar o codigo novamente:
* SimulatorCacheExecutavel

**Execucao do simulador**

Para executar o limulador basta por linha de comando passar a seguinte instrucao

> ./SimulatorCacheExecutavel

Dessa forma sera executado o simulador no modo default onde ele contem as seguintes configuracoes:

* Numero de conjuntos    = 1024.
* Tamanho do bloco     	 = 4.
* Associatividade    	 = 1.
* Arquivo de leitura 	 = arqBinario1.dat.

O simulador permite a configuracao da cache da forma que o usuario bem desejar, bastando passar por linha de comando as seguintes instrucoes

> ./SimulatorCacheExecutavel <Numero_de_Conjuntos> <Tamanho_do_Bloco> <Associatividade> <Nome_do_Arquivo>

*O nome do arquivo deve-se incluir o formato desejado, como no exemplo abaixo:*

> ./SimulatorCacheExecutavel 2048 4 2 arquivoBinario2.dat.

*Com a falta de alguns dos argumentos o simulador nao executara*.

O simulador conta com 5 aquivos binarios diferentes em enderecos e quantidade para que possa ser usado, dando ainda a possibilidade de o usuario por conta propria inserir quantos mais desejados.

1. arqBinario1.dat.
2. arqBinario2.dat.
3. arqBinario3.dat.
4. arqBinario4.dat.
5. arqBinario5.dat.

_Sendo uma relacao quantitativa de arqBinario1.dat < arqBinario2.dat < ... < arqBinario5.dat_

**Descricao do funcionamento do simulador**

O simulador contem uma estrutura a qual contem todos os dados os quais sera preciso para sua execucao. A cache foi pensada como uma matriz NxM onde N = Numeros_de_conjuntos e M = Associatividade + 1.
Tal matriz trabalha com inteiros, para maior facilidade no tratamento dos enderecos, pois sabe-se que utilizando a funcao _mod_ em um inteiro decimal, ela nos retornara a posicao a qual o endereco devera ocupar. Dessa forma quando um endereco e solicitado basta chamar a funcao mod como dito anteriormente, e verificar se o endereco se encontra nessa posicao na cache (a matriz explicada acima) caso o endereco esteja la, um hit e contabilizado, podendo proceguir para as proximas operacoes. Porem se o endereco nao estiver la, um miss sera contabilizado e tratado entre os 3 tipos de misses: Compulsorio, conflito e capacidade. Logo apos tal tratamento sera efetuado a escrita do endereco na cache na posicao a qual ela devera ocupar, a posicao sera dada de acordo com a configuracao da cache.
Pois se a cache for totalmente associativa, pode-se escrever em qualquer posicao dentro do bloco ate sua capacidade ser preenchida, quando a capacidade maxima e atingida, o endereco passa a ser escrita em uma posicao randomica (politica de substituicao randomica).
Se a cache for mapeamento direto, sera necessario somente escrever na posicao dada pela funcao _mod_, caso haja um endereco la dentro nao ha necessidade de tomar alguma providencia, pois necessita somente realizar a substituicao do endereco.
Caso seja entre os dois estremos (mapeamento direto e totalmente associativo), verifico de na posicao retornado pela funcao _mod_ ha espaco dentro do bloco para escrita, se houver, escrevo em tal posicao livre, caso nao tenha, realizo a funcao randomica que me dara uma posicao aleatoria dentro do bloco.

**Descricao dos codigos disponiveis**

**simulatorCache.c**

O arquivo simulatorCache.c contem a funcao main do programa somente, onde realiza os seguintes passos:

1. Recebe por parametro as configuracoes desejada pelo usuario 
ou seta o as configuracoes default da cache.
2. Realiza a chamada da funcao que realiza a leitura dos aquivos (sera falado posteriormente).
3. Realiza a chamada da funcao que realiza a procura do endereco na cache (sera falado posteriormente).
4. Realiza a chamada da funcao que realiza a impressao dos resultados obtidos ao excutar o simular.

**library.h**

O arquivo library.h contem os headers das nossas funcoes alem de ser o arquivo o qual as explica, para maior facilidade tais comentario serao aqui descorridos ao usuario.

> typedef struct cacheComplete cacheConfig

contem todos os dados relevantes sobre a cache, como:

* As configuracoes passadas pelo usuario.
* Resultados obtidos apartir da execucao do simulador.

> void startCache(cacheConfig \*AcessCache).

Funcao encarregada de alocar nossa cache, com as configuracoes default ou passadas pelo usuario, criando um matriz de numero_de_conjunto x associatividade, onde sera possivel procurar e escrever os enderecos passado pelos arquivos binarios.

>void searchEnd(cacheConfig \*AcessCache).

Funcao encarregada de realizar a procura de um determinado endereco ligo do arquivo binario. Sendo tambem a funcao com o papel mais importante, pois nela sera tratado:

* Hit na cache.
* Miss na cache.
* Tipos de misses gerado.
* chama a funcao a qual desempenhada a verificacao de qual linha determinado endereco deve ocupar.
* Chama a funcao a qual desempenha a escrita de determinado endereco na cache caso ele venha a nao ser encontrado.

Como dito e a funcao mais importante do codigo, pois ela que regis o fluxo e comportamento do programa.

>long unsigned int indexMod(long unsigned int value, long unsigned int mod).

Funcao encarregada de realizar a oepracao **_mod_**, entre o endereco e as n_entradas que a cache possui.

>void writeCache(cacheConfig \*AcessCache, long unsigned int index, int endValue).

Funcao encarregada de realizar a escrita de determinado endereco o qual nao foi encontrado pela funcao _searchEn()_. Seu papel e realizar a atualizacao do bit de verificao da cache para 1 (de forma a indicar que ha um endereco nessa entrada da cache), e escrever o dado na posicao correta.
Quando se tempo um maior nivel de associatividade como em alguns casos que podem ser testado no simulador, esta funcao se encarrega de encontrar o lugar adequado no bloco e na cache e quando necessario substituir algum dados existente na cache, realiza tambem a escolha _randomica_ da posicao a qual o endereco passara a ocupar.

> void printResults(cacheConfig \*AcessCache).

Funcao encarregada de apenas exibir na tela as configuracoes da cache, os dados os quais o simulador fez uso para sua execucao e os resultados obtidos apos o termino da mesma.

>int conversion(int val).

Funcao encarregada de realizar a conversao do endereco lido do arquivo, tal acao se fez necessario pelos arquivos se encontrarem em _big endian_, dessa forma os enderecos passao a ser tratados como inteiros decimais.

>int* readEnd(cacheConfig \*AcessCache).

Funcao encarregado de realizar a leitura dos arquivos binarios, chamando a funcao _conversion()_ para tratamentos dos dados, apos armazenando-os em um vetor para que possa ser utlizado como inteiros decimais durante a execucao do simulador.

