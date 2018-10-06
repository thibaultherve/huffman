#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <math.h>
#include "noeud.h"
#include "huffman.h"


char* chaine;
char* codage[NB_CARACTERE];

void compression(char* f) {
	int occurrences[NB_CARACTERE] = {0};

	FILE* fichier = fopen(f, "r");

	if (fichier == NULL) {
		fprintf(stderr, "Erreur d'ouverture du fichier en lecture");
		return;
	}
	
	printf("Ouverture du fichier réussi !\n");

	char* texte = malloc(sizeof(char));
	char c;

	int i=0;
	while((c = fgetc(fichier)) != EOF) {
		if (c > 255 || c<0) {
			fprintf(stderr, "Caracteres incorrects dans le fichier");
			return;
		}
		texte = realloc(texte, i+1*sizeof(char));
		texte[i] = c;
		i++;
	}
	i++;
	texte = realloc(texte, i*sizeof(char));
	texte[i] = '\0';

	


	int taille_texte = (int)strlen(texte);

	for (int i=0; i<taille_texte; i++) {
		for (int j=0; j<=NB_CARACTERE; j++) {
			if (texte[i] == j) {
				occurrences[j]++;
			}
		}
	}

	
	//comptage des noeuds
	int nb_noeuds = 0;
	for (int i=0; i<NB_CARACTERE; i++) {
		if (occurrences[i]>0) {
			printf("occurrences[%c] = %d\n", i, occurrences[i]);
			nb_noeuds++;
		}
	}

	//création tableau de noeuds
	nd* tab_noeuds = (nd*) malloc(nb_noeuds*sizeof(nd));

	//création des premiers noeuds (feuilles)
	for (int i=0, j=0; i<NB_CARACTERE; i++) {
		if (occurrences[i]>0) {
			tab_noeuds[j] = creer_noeud(NULL, NULL, i, occurrences[i], TYPE_FEUILLE);
			j++;
		}

	}
	int numero_noeud=48;
	while (nb_noeuds > 1) {
		tab_noeuds = trier_tab_noeuds(tab_noeuds, nb_noeuds);

		//printf ("Noeuds restants (dans l'ordre) :\n");
		/*for (int i = 0; i < nb_noeuds; i++)
		{
			printf("[%c]:%d\n", tab_noeuds[i]->lettre, tab_noeuds[i]->occurrence);
		}
		printf("\n");
		*/


		if (tab_noeuds[0]->type == TYPE_FEUILLE && tab_noeuds[1]->type == TYPE_NOEUD) {
			//printf("noeud[0] feuille, noeud[1] noeud\n");
			//printf("Creation du noeud d'occurrence %d :\nson fils gauche est %c:%d\nson fils droit est %c:%d\n", tab_noeuds[0]->occurrence + tab_noeuds[1]->occurrence, tab_noeuds[1]->lettre, tab_noeuds[1]->occurrence, tab_noeuds[0]->lettre, tab_noeuds[0]->occurrence);
			tab_noeuds[0] = creer_noeud(tab_noeuds[1], tab_noeuds[0], numero_noeud, tab_noeuds[0]->occurrence + tab_noeuds[1]->occurrence, TYPE_NOEUD);
			
		}
		else if (tab_noeuds[0]->type == TYPE_NOEUD && tab_noeuds[1]->type == TYPE_NOEUD) {
			//printf("Creation du noeud d'occurrence %d :\nson fils gauche est %c:%d\nson fils droit est %c:%d\n", tab_noeuds[0]->occurrence + tab_noeuds[1]->occurrence, tab_noeuds[1]->lettre, tab_noeuds[1]->occurrence, tab_noeuds[0]->lettre, tab_noeuds[0]->occurrence);
			tab_noeuds[0] = creer_noeud(tab_noeuds[1], tab_noeuds[0], numero_noeud, tab_noeuds[0]->occurrence + tab_noeuds[1]->occurrence, TYPE_NOEUD);
		
		}
		else {
			//printf("Creation du noeud d'occurrence %d :\nson fils gauche est %c:%d\nson fils droit est %c:%d\n", tab_noeuds[0]->occurrence + tab_noeuds[1]->occurrence, tab_noeuds[0]->lettre, tab_noeuds[0]->occurrence, tab_noeuds[1]->lettre, tab_noeuds[1]->occurrence);
			tab_noeuds[0] = creer_noeud(tab_noeuds[0], tab_noeuds[1], numero_noeud, tab_noeuds[0]->occurrence + tab_noeuds[1]->occurrence, TYPE_NOEUD);
			
		}
		nb_noeuds--;



		for(int i=1; i<nb_noeuds; i++) {
			tab_noeuds[i] = tab_noeuds[i+1];
		}
		numero_noeud++;
	}

	nd racine = tab_noeuds[0];

	char* sequence_compressee = malloc(0);
	int taille_sequence_compressee = 0;
	nd n = racine;
	//printf("La racine est [%c]:%d\n", n->lettre, n->occurrence);
	fc_codage(n, -1, 0);

	for (int i = 0; i<(int)strlen(texte); i++) {

		taille_sequence_compressee += strlen(codage[texte[i]]);
		//printf("taille_sequence_compressee = %d\n", taille_sequence_compressee);
		sequence_compressee = realloc(sequence_compressee, taille_sequence_compressee*sizeof(char));

		sequence_compressee = strcat(sequence_compressee, codage[texte[i]]);
		//printf("codage[%c] = %s\n", texte[i], codage[texte[i]]);
		//printf("sequence_compressee = %s\n", sequence_compressee);
		
	}	

	fclose(fichier);

	fichier = fopen("code_compresse.bin", "wb");

	if(fichier == NULL) {
		fprintf(stderr, "Erreur d'ouverture du fichier en ecriture\n");
		return;
	}

	printf("sequence_compressee = %s\n", sequence_compressee);

	

	char* octet_a_ecrire = malloc(9*sizeof(char));

	octet_a_ecrire[0] = '\0';
	printf("taille %lu\n", strlen(octet_a_ecrire));
	for(int i=0; i< (int) strlen(sequence_compressee); i=i+8) {
		octet_a_ecrire = strncpy(octet_a_ecrire, &sequence_compressee[i], 8);
		octet_a_ecrire[8] = '\0';
		
		

		if (strlen(octet_a_ecrire) < 9) {
			for (int i = (int) strlen(octet_a_ecrire); i < 9; i++)
			{
				octet_a_ecrire[i] = '0';
			}
			octet_a_ecrire[8] = '\0';
		}
		printf("taille %lu\n", strlen(octet_a_ecrire));
		printf("octet_a_ecrire = %s\n", octet_a_ecrire);
		unsigned long int mavaleur = strtoll(octet_a_ecrire, NULL, 2);
		fwrite(&mavaleur, sizeof(char), 1, fichier);
	}
	

	fclose(fichier);

	
}

void fc_codage(nd n, int cote, int taille_chaine) {


	if (n == NULL) {
		return;
	}

	if (cote == GAUCHE) {
		taille_chaine++;
		chaine = realloc(chaine, taille_chaine*sizeof(char));
		chaine[taille_chaine-1] = '0';
		//printf("GAUCHE : chaine[%d] = %c\n", taille_chaine-1, chaine[taille_chaine-1]);
	}

	if (cote == DROIT) {
		taille_chaine++;
		chaine = realloc(chaine, taille_chaine*sizeof(char));
		chaine[taille_chaine-1] = '1';
		//printf("DROITE : chaine[%d] = %c\n", taille_chaine-1, chaine[taille_chaine-1]);
	}

	if (n->type == TYPE_FEUILLE) {
		//cas où il n'y a qu'un seul caractère différent
		if (taille_chaine == 0) {
			codage[n->lettre] = malloc(sizeof(char));
			codage[n->lettre][0] = '0';
		}
		else {

			codage[n->lettre] = malloc(taille_chaine*sizeof(char));

			for (int i=0; i<taille_chaine; i++) {
				//printf("%c", chaine[i]);			
				codage[n->lettre][i] = chaine[i];
				
			}
			
			printf("\ncodage[%c] = %s\n", n->lettre, codage[n->lettre]);
		}
	}

	fc_codage(n->fils_gauche, GAUCHE, taille_chaine);
	fc_codage(n->fils_droit, DROIT, taille_chaine);
}

