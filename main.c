#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "noeud.h"
#include "huffman.h"

int main(void) {
	
	char* texte = "Brioche mdr";

	char* code = compression(texte);

	printf("\n%s ->\n%s\n", texte, code);
	
	return EXIT_SUCCESS;
}
