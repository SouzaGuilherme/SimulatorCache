// Library.c the functions of library.h the simulator Cache

#include<stdio.h>
#include<string.h>
#include<math.h>

int main()
{
    char bin[100];
    unsigned long dec = 0;
    int i = 0;
    int s;
    fgets( bin, sizeof(bin), stdin);
    s = strlen( bin );
    while( s-- ) {
        if( bin[s] == '0' || bin[s] == '1' ) {
            dec = dec + pow(2, i++) * (bin[s] - '0');
        }
    };
    printf("\nDecimal Equivalent of Binary Number: \t %u\n", dec);
    return 0;
}