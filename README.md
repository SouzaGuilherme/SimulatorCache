# SimulatorCache
Cache simulator developed for computer architecture discipline 2.

## Enunciado
[Enunciado](https://ava.ufpel.edu.br/pre/mod/resource/view.php?id=118857)

## Relatorio
Foi desenvolvido 3 arquivos os quais são essenciais para a execução do trabalho aqui apresentado:
1. SimulatorCache.c
2. library.c
3. library.h

Um executável do simulador também foi adicionado para que não haja necessidade de recompilar o código novamente:
* SimulatorCacheExecutavel

**Execucao do simulador**

Para executar o simulador basta por linha de comando passar a seguinte instrução:

> ./SimulatorCacheExecutavel

Dessa forma será  executado o simulador no modo default onde ele contém as seguintes configurações:

* Numero de conjuntos    = 1024.
* Tamanho do bloco     	 = 4.
* Associatividade    	 = 1.
* Arquivo de leitura 	 = arqBinario1.dat.

O simulador permite a configuração da cache da forma que o usuário bem desejar, bastando passar por linha de comando as seguintes instruções

> ./SimulatorCacheExecutavel <Numero_de_Conjuntos> <Tamanho_do_Bloco> <Associatividade> <Nome_do_Arquivo>

*O nome do arquivo deve-se incluir o formato desejado, como no exemplo abaixo:*

> ./SimulatorCacheExecutavel 2048 4 2 arquivoBinario2.dat.

*Com a falta de algum dos argumento o simulador não executará*.

O simulador conta com 5 arquivos binários diferentes em endereços e quantidade para que possa ser usado, dando ainda a possibilidade de o usuário por conta própria inserir quantos mais desejados.

1. arqBinario1.dat.
2. arqBinario2.dat.
3. arqBinario3.dat.
4. arqBinario4.dat.
5. arqBinario5.dat.

_Sendo uma relacao quantitativa de arqBinario1.dat < arqBinario2.dat < ... < arqBinario5.dat_.

**Descricao do funcionamento do simulador**

O simulador possui uma estrutura que contém todos os dados os quais será preciso para sua execução. A cache foi pensada como uma matriz NxM onde N = Numeros_de_conjuntos e M = Associatividade + 1.
Tal matriz trabalha com inteiros, para maior facilidade no tratamento dos endereços, pois sabe-se que utilizando a função _mod_ em um inteiro decimal, ela nos retornara a oposição a qual o endereço devera ocupar. Dessa forma quando um endereço é solicitado basta chamar a função mod como dito anteriormente, e verificar se o endereço se encontra nessa posição na cache (a matriz explicada acima) caso o endereço esteja lá, um hit é contabilizado, podendo prosseguir para as próximas operações. porém se o endereço não estiver lá, um miss será contabilizado e tratado entre os 3 tipos de misses: Compulsório, conflito e capacidade. Logo após tal tratamento, será efetuado a escrita do endereço na cache na posição a qual ela devera ocupar, a posição será dada de acordo com a configuração da cache.
Pois se a cache for totalmente associativa, pode-se escrever em qualquer posição dentro do bloco até sua capacidade ser preenchida, quando a capacidade máxima é atingida, o endereço passa a ser escrito em uma posição randômica (politica de substituição randômica).
Se a cache for mapeamento direto, será necessário somente escrever na posição dada pela função _mod_, ou seja, caso haja um endereço lá dentro, não será preciso calcular uma nova posição para ser escrita, basta somente realizar a substituição do endereço.
Caso seja entre os dois extremos (mapeamento direto e totalmente associativo), verifico se na posição retornada pela função _mod_ há espaço dentro do bloco para escrita, se houver, escrevo em tal posição livre, caso não tenha, realizo a função randômica que me dará uma posição aleatória dentro do bloco para a escrita.

**Descricao dos codigos disponiveis**

**simulatorCache.c**

O arquivo simulatorCache.c contém a função main do programa somente, onde realiza os seguintes passos:

1. Recebe por parâmetro as configurações desejadas pelo usuário
ou seta o as configurações default da cache.
2. Realiza a chamada da função que realiza a leitura dos arquivos (será falado posteriormente).
3. Realiza a chamada da função que realiza a procura do endereço na cache (será falado posteriormente).
4. Realiza a chamada da função que realiza a impressão dos resultados obtidos ao executar o simular. 

**library.h**

O arquivo library.h contém os headers das nossas funções além de ser o arquivo o qual as explica, para maior facilidade tais comentários serão aqui discorridos ao usuário.

> typedef struct cacheComplete cacheConfig

Contém todos os dados relevantes sobre a cache, como:

* As configurações passadas pelo usuário.
* Resultados obtidos a partir da execução do simulador.

> void startCache(cacheConfig \*AcessCache).

Função encarregada de alocar a cache, com as configurações default ou passadas pelo usuário, criando um matriz de numero_de_conjunto x associatividade +1, onde será possível procurar e escrever os endereços passados pelos arquivos binários.

>void searchEnd(cacheConfig \*AcessCache).

Função encarregada de realizar a procura de um determinado endereço lido do arquivo binário. Sendo também a função com o papel mais importante, pois nela será tratado:

* Hit na cache.
* Miss na cache.
* Tipos de misses gerado.
* Chama a função a qual desempenhada a verificação de qual linha determinado endereço deve ocupar.
* Chama a função a qual desempenha a escrita de determinado endereço na cache caso ele venha a não ser encontrado.

Como dito é a função mais importante do código, pois ela que rege o fluxo e comportamento do programa. 

>long unsigned int indexMod(long unsigned int value, long unsigned int mod).

Função encarregada de realizar a operação **_mod_**, entre o endereço e as n_entradas que a cache possui. 

>void writeCache(cacheConfig \*AcessCache, long unsigned int index, int endValue).

Função encarregada de realizar a escrita de determinado endereço o qual não foi encontrado pela função _searchEnd()_. Seu papel é realizar a atualização do bit de verificao da cache para 1 (de forma a indicar que há um endereço nessa entrada da cache), e escrever o dado na posição correta.
Quando se tem um maior nível de associatividade como em alguns casos que podem ser testado no simulador, esta função se encarrega de encontrar o lugar adequado no bloco e na cache e quando necessário substituir algum dado existente na cache, realiza também a escolha _randomica_ da posição a qual o endereço passara a ocupar. 

> void printResults(cacheConfig \*AcessCache).

Função encarregada de apenas exibir na tela as configurações da cache, os dados os quais o simulador fez uso para sua execução e os resultados obtidos após o termino da mesma.

>int conversion(int val).

Função encarregada de realizar a conversão do endereço lido do arquivo, tal ação se fez necessário pelos arquivos se encontrarem em _big endian_, dessa forma os endereços passão a ser tratados como inteiros decimais.

>int* readEnd(cacheConfig \*AcessCache).

Função encarregado de realizar a leitura dos arquivos binários, chamando a função _conversion()_ para tratamentos dos dados, após armazenando-os em um vetor para que possa ser utilizado como inteiros decimais durante a execução do simulador.