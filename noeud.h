/* Thibault HERVE / David BAGORY */

#ifndef NOEUD_H
#define NOEUD_H

#define TYPE_NOEUD 0
#define TYPE_FEUILLE 1

struct noeud {
    int occurrence;
    unsigned char lettre;    
    struct noeud* fils_gauche;
    struct noeud* fils_droit;
    int type;
};

typedef struct noeud* nd;

nd creer_noeud(nd fils_gauche, nd fils_droit, char lettre, int occurrence, int type);
nd* trier_tab_noeuds(nd* n, int nb_noeuds);
void setPere(nd fils, nd pere);
void detruire_tout_noeuds(nd* n);

#endif