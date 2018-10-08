#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "noeud.h"
#include "huffman.h"

int main(int argc, char** argv) {	

	if (argc == 1 || argc>3) {
		fprintf(stderr, "Usage : %s [compression type] [file]\n", argv[0]);
		return EXIT_FAILURE;
	}

	if(argc == 2 ) {
		if (strcmp(argv[1], "--help")==0) {
			printf("Huffman help\nCompresser un fichier : %s -c [file]\n", argv[0]);
			printf("Decompresser un fichier : %s -d [file]\n", argv[0]);
		}
		else {
			fprintf(stderr, "Usage : %s [compression type] [file]\n", argv[0]);
			return EXIT_FAILURE;
		}		
	}


	if(argc==3 && strcmp(argv[1], "-c")==0)
		compression("a_lire");
	
	if (argc==3 && strcmp(argv[1], "-d")==0)
		decompression("code_compresse.bin");
	
	return EXIT_SUCCESS;
}
