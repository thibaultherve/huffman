#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "noeud.h"
#include "huffman.h"

int main(int argc, char** argv) {	

	if (argc == 1 || argc>4) {
		fprintf(stderr, "Usage : %s [COMPRESSION TYPE] [INPUT FILE] [OUTPUT FILE]\nCOMPRESSION TYPE : -c COMPRESSION, -d DECOMPRESSION\n", argv[0]);
		return EXIT_FAILURE;
	}

	if(argc == 2 ) {
		if (strcmp(argv[1], "--help")==0) {
			printf("Huffman HELP\nCompresser un fichier : %s -c [INPUT FILE] [OUTPUT FILE]\n", argv[0]);
			printf("Decompresser un fichier : %s -d [INPUT FILE] [OUTPUT FILE]\n", argv[0]);
		}
		else {
			fprintf(stderr, "Usage : %s [COMPRESSION TYPE] [INPUT FILE] [OUTPUT FILE]\nCOMPRESSION TYPE : -c COMPRESSION, -d DECOMPRESSION\n", argv[0]);
			return EXIT_FAILURE;
		}		
	}


	if(argc==4 && strcmp(argv[1], "-c")==0) {
		printf("COMPRESSION...\n\n");
		char* sequence = compression(argv[2], argv[3]);
		if (sequence == NULL) {
			return EXIT_FAILURE;
		}
		else {
			printf("%s\n\n", sequence);
			printf("COMPRESSION TERMINEE\n");
		}
		free(sequence);
	}
	
	if (argc==4 && strcmp(argv[1], "-d")==0) {
		printf("DECOMPRESSION...\n\n");
		char* sequence = decompression(argv[2], argv[3]);
		if (sequence == NULL) {
			return EXIT_FAILURE;
		}
		else {
			printf("%s\n\n", sequence);
			printf("DECOMPRESSION TERMINEE\n");
		}
		free(sequence);
	}
	
	return EXIT_SUCCESS;
}
