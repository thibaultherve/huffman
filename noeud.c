/* Thibault HERVE / David BAGORY */

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
			n[j] = n[j-1];
			j = j-1;
		}
		n[j] = noeud_tmp;
	}
	for (int i = 0; i < nb_noeuds; ++i)
	{
		printf("noeuds [%c]:%d\n", n[i]->lettre, n[i]->occurrence);
	}
	printf("\n");
	
	return n;
}

void detruire_tout_noeuds(nd* n) {
	if ((*n) != NULL) {
		
		if (((*n)->fils_gauche) != NULL) {
			detruire_tout_noeuds(&((*n)->fils_gauche));
		}

		if (((*n)->fils_droit) != NULL) {
			detruire_tout_noeuds(&((*n)->fils_droit));
		}

		free((*n));
		(*n) = NULL;

		
	}
}