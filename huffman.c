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
	

	char* texte = malloc(sizeof(char));
	char c;

	int i=0;
	while((c = fgetc(fichier)) != EOF) {
		texte = realloc(texte, i+1*sizeof(char));
		texte[i] = c;
		i++;
	}

	texte = realloc(texte, i+1*sizeof(char));
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
	int numero_noeud=255;
	while (nb_noeuds > 1) {
		tab_noeuds = trier_tab_noeuds(tab_noeuds, nb_noeuds);

		if (tab_noeuds[0]->type == TYPE_FEUILLE && tab_noeuds[1]->type == TYPE_NOEUD) {
			tab_noeuds[0] = creer_noeud(tab_noeuds[1], tab_noeuds[0], numero_noeud, tab_noeuds[0]->occurrence + tab_noeuds[1]->occurrence, TYPE_NOEUD);
			
		}
		else if (tab_noeuds[0]->type == TYPE_NOEUD && tab_noeuds[1]->type == TYPE_NOEUD) {
			tab_noeuds[0] = creer_noeud(tab_noeuds[1], tab_noeuds[0], numero_noeud, tab_noeuds[0]->occurrence + tab_noeuds[1]->occurrence, TYPE_NOEUD);
		
		}
		else {
			tab_noeuds[0] = creer_noeud(tab_noeuds[0], tab_noeuds[1], numero_noeud, tab_noeuds[0]->occurrence + tab_noeuds[1]->occurrence, TYPE_NOEUD);
			
		}
		nb_noeuds--;



		for(int i=1; i<nb_noeuds; i++) {
			tab_noeuds[i] = tab_noeuds[i+1];
		}
		numero_noeud++;
	}

	nd racine = tab_noeuds[0];

	char* sequence_compressee = malloc(sizeof(char));
	int taille_sequence_compressee = 0;
	
	fc_codage(racine, -1, 0);

	for (int i = 0; i<(int)strlen(texte); i++) {

		taille_sequence_compressee += strlen(codage[texte[i]]);
		sequence_compressee = realloc(sequence_compressee, taille_sequence_compressee*sizeof(char));

	

		sequence_compressee = strcat(sequence_compressee, codage[texte[i]]);
		
	}
	free(texte);
	fclose(fichier);

	fichier = fopen("code_compresse.bin", "wb");

	if(fichier == NULL) {
		fprintf(stderr, "Erreur d'ouverture du fichier en ecriture\n");
		return;
	}

	printf("COMPRESSION\n%s\n", sequence_compressee);


	for (int i=0; i<NB_CARACTERE; i++) {
		if (occurrences[i]>0) {
			fprintf(fichier, "%c %d|", i, occurrences[i]);
		}
	}

	

	char* octet_a_ecrire = malloc(9*sizeof(char));

	octet_a_ecrire[0] = '\0';	

	//permet d'ignorer les nb_bits_fin du fichier
	int nb_bits_fin = 8 - (strlen(sequence_compressee)%8);

	fprintf(fichier, "\nNBEndBits=%d\n", nb_bits_fin);
	fprintf(fichier, "COMPRESSION\n");
	for(int i=0; i< (int) strlen(sequence_compressee); i=i+8) {
		octet_a_ecrire = strncpy(octet_a_ecrire, &sequence_compressee[i], 8);
		octet_a_ecrire[8] = '\0';

		if (strlen(octet_a_ecrire) < 8) {
			for (int i = (int) strlen(octet_a_ecrire); i < 8; i++) {
				octet_a_ecrire[i] = '0';
			}
			octet_a_ecrire[8] = '\0';
			
		}

		
		unsigned long int mavaleur = strtoll(octet_a_ecrire, NULL, 2);
		
		
		fwrite(&mavaleur, sizeof(char), 1, fichier);
	}

	fclose(fichier);
	
	detruire_tout_noeuds(&racine);
	free(chaine);

	for (int i = 0; i < NB_CARACTERE; i++)
	{
		free(codage[i]);
	}
	free(octet_a_ecrire);
	free(sequence_compressee);
	free(tab_noeuds);
	
	
}

void fc_codage(nd n, int cote, int taille_chaine) {

	if (n == NULL) {
		return;
	}

	if (cote == GAUCHE) {
		taille_chaine++;
		chaine = realloc(chaine, taille_chaine*sizeof(char));
		chaine[taille_chaine-1] = '0';
	}

	if (cote == DROIT) {
		taille_chaine++;
		chaine = realloc(chaine, taille_chaine*sizeof(char));
		chaine[taille_chaine-1] = '1';
	}

	if (n->type == TYPE_FEUILLE) {
		//cas où il n'y a qu'un seul caractère différent
		if (taille_chaine == 0) {
			codage[n->lettre] = realloc(codage[n->lettre], sizeof(char));
			codage[n->lettre][0] = '0';
		}
		else {
			codage[n->lettre] = realloc(codage[n->lettre], taille_chaine*sizeof(char));

			for (int i=0; i<taille_chaine; i++) {		
				codage[n->lettre][i] = chaine[i];
				
			}
			
			
		}
	}

	fc_codage(n->fils_gauche, GAUCHE, taille_chaine);
	fc_codage(n->fils_droit, DROIT, taille_chaine);
}

void decompression(char* f) {
	FILE* fichier = fopen(f, "rb");

	char chaine[12];
	int occurrences[NB_CARACTERE] = {0};
	unsigned char c;
	int nb;

	while(fscanf(fichier, "%c %d|", &c, &nb) == 2) {
		occurrences[c] = nb;
	}
	int nb_bits_fin = 0;

	fscanf(fichier, "NBEndBits=%d", &nb_bits_fin);
	fread(&chaine, sizeof(char), 13, fichier);	

	if (strcmp(chaine, "\nCOMPRESSION\n") != 0) {
		fprintf(stderr, "Le contenu du fichier n'est pas au bon format\n");
		return;
	}

	//comptage des noeuds
	int nb_noeuds = 0;
	for (int i=0; i<NB_CARACTERE; i++) {
		if (occurrences[i]>0) {
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
	int numero_noeud=255;
	while (nb_noeuds > 1) {
		tab_noeuds = trier_tab_noeuds(tab_noeuds, nb_noeuds);

		if (tab_noeuds[0]->type == TYPE_FEUILLE && tab_noeuds[1]->type == TYPE_NOEUD) {
			tab_noeuds[0] = creer_noeud(tab_noeuds[1], tab_noeuds[0], numero_noeud, tab_noeuds[0]->occurrence + tab_noeuds[1]->occurrence, TYPE_NOEUD);
			
		}
		else if (tab_noeuds[0]->type == TYPE_NOEUD && tab_noeuds[1]->type == TYPE_NOEUD) {
			tab_noeuds[0] = creer_noeud(tab_noeuds[1], tab_noeuds[0], numero_noeud, tab_noeuds[0]->occurrence + tab_noeuds[1]->occurrence, TYPE_NOEUD);
		
		}
		else {
			tab_noeuds[0] = creer_noeud(tab_noeuds[0], tab_noeuds[1], numero_noeud, tab_noeuds[0]->occurrence + tab_noeuds[1]->occurrence, TYPE_NOEUD);
			
		}
		nb_noeuds--;



		for(int i=1; i<nb_noeuds; i++) {
			tab_noeuds[i] = tab_noeuds[i+1];
		}
		numero_noeud++;
	}

	nd racine = tab_noeuds[0];
	fc_codage(racine, -1, 0);

	unsigned char valeur;
	
	char* sequence_compressee = malloc(sizeof(char));
	int compt =1;


	
	while(fread(&valeur, sizeof(unsigned char), 1, fichier) == 1) {
		sequence_compressee = realloc(sequence_compressee, 1+(compt*8)*sizeof(char));

		sequence_compressee = strcat(sequence_compressee, toBinary(valeur));
		compt++;
	}

	char* buffer = malloc(sizeof(char));

	int taille_sequence_DEcompressee = 0;
	char* sequence_DEcompressee = malloc(taille_sequence_DEcompressee*sizeof(char));



	//parcours de notre sequence compresse
	for (int i = 0, k=0; i < (int) strlen(sequence_compressee) - nb_bits_fin; i++) {

		int nb_resultat = 0;
		char save_cara;

		if (k>7)
			k=0;

		buffer = realloc(buffer, (k+2)*sizeof(char));
		buffer[k] = sequence_compressee[i];
		buffer[k+1] = '\0';



		for(int j=0; j<NB_CARACTERE; j++) {
			if (codage[j] != NULL) {
				if (strncmp(codage[j], buffer, i+1) == 0) {
					nb_resultat++;
					save_cara = j;
				}
			}			
		}

		if(nb_resultat == 1){

			buffer = realloc(buffer, sizeof(char));
			buffer[0] = '\0';
			k=0;

			taille_sequence_DEcompressee++;
			sequence_DEcompressee = realloc(sequence_DEcompressee, taille_sequence_DEcompressee*sizeof(char));
			sequence_DEcompressee[taille_sequence_DEcompressee-1] = save_cara;
		}
		else {
			k++;
		}

		nb_resultat = 0;
	}

	sequence_DEcompressee = realloc(sequence_DEcompressee, (taille_sequence_DEcompressee+1)*sizeof(char));
	sequence_DEcompressee[taille_sequence_DEcompressee] = '\0';
	printf("DECOMPRESSION\n%s\n", sequence_DEcompressee);

}


char* toBinary(unsigned char val) {
   int c, d, i;
   char* binaire_str;
   
   i = 0;
   binaire_str = (char*)malloc(8+1);
   
   
   for (c = 7 ; c >= 0 ; c--)
   {
      d = val >> c;
     
      if (d & 1)
         *(binaire_str+i) = 1 + '0';
      else
         *(binaire_str+i) = 0 + '0';
     
      i++;
   }
   *(binaire_str+i) = '\0';
   
   return binaire_str;
}