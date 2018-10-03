#include <stdlib.h>
#include <stdio.h>

#include <string.h>

#include "noeud.h"



nd creer_noeud(nd fils_gauche, nd fils_droit, char lettre, int occurrence, int type) {

	nd n = (nd) malloc(sizeof(struct noeud));
	n->occurrence = occurrence;
	n->lettre = lettre;	
	n->fils_gauche = fils_gauche;
	n->fils_droit = fils_droit;
	n->type = type;

	
	return n;	
}

//trie selon le nombre d'occurrences
nd* trier_tab_noeuds(nd* n, int nb_noeuds) {
	int occurrence, j;
	char lettre;
	nd noeud_tmp;

	for (int i=0; i<nb_noeuds; i++) {
		occurrence = n[i]->occurrence;
		lettre = n[i]->lettre;
		noeud_tmp = n[i];

		j = i;
		while(j>0 && ((n[j-1]->occurrence > occurrence)  || (n[j-1]->occurrence == occurrence && n[j-1]->lettre > lettre)) ) {
			if(n[j-1]->type == TYPE_NOEUD && n[i]->type == TYPE_NOEUD /*&& n[j-1]->occurrence >= occurrence*/) {
				//printf("correct\n");
			}
			else {
				/*printf("n[%d] = [%c]:%d type = %d\n", j-1, n[j-1]->lettre, n[j-1]->occurrence, n[j-1]->type);
				printf("n[%d] = [%c]:%d type = %d\n", i, n[j-1]->lettre, n[i]->occurrence, n[i]->type);
				printf("incorrect\n");*/
			}

			n[j] = n[j-1];
			j = j-1;
		}
		n[j] = noeud_tmp;
	}
	return n;
}