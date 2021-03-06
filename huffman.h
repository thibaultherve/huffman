/* Thibault HERVE / David BAGORY */

#ifndef HUFFMAN_H
#define HUFFMAN_H

#define NB_CARACTERE 256

#define GAUCHE 1
#define DROIT 0

char* compression(char* fin, char* fout);
void fc_codage(nd n, int cote, int taille_chaine);

char* decompression(char* fin, char* fout);
char* toBinary(unsigned char val);
#endif