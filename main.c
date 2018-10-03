#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "noeud.h"
#include "huffman.h"

int main(void) {
	
	

	char* code = compression("a_lire");

	printf("\n\n%s\n", code);
	
	return EXIT_SUCCESS;
}
