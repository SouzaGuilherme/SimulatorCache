// Library.c the functions of library.h the simulator Cache

#include<stdio.h>
#include<string.h>
#include<math.h>

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
}

int main()
{
	int cont=0;

	char url[]="binary.txt",
		info[32];
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
}
