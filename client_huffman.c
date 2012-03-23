#include <stdio.h>
#include <stdlib.h>
#include "huffman_coding.h"

int main()
{
	char source[30];
	char destination[30];

	printf("\n------------HUFFMAN CODING-----------\n\n");
	printf("Enter the text file to compress.\n");
	scanf("%s",source);

	printf("Enter the name of the file which is retrieved from the compressed file\n");
	scanf("%s", destination);

	compress(source, destination);

    return 0;
}


