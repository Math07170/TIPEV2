#ifndef FILEPRIO_H
#define FILEPRIO_H

#include <stdbool.h>

/* Structure de file de priorité donnant le numéro du sommet associé à la valeur la plus PETITE
 * Implémentation : tas binaire avec un tableau
 * 6/3/23 au soir, testé convenablement, semble faire le taf ! (+rajouté une fonction detruire_fileprio) */

struct s_noeud{		// càd une case du tableau
	int num_sommet;
	int valeur;
	//int i_fils_droit;			// a priori non
	//int i_fils_gauche;		// non plus
};
typedef struct s_noeud noeud;

struct s_fileprio{
	int nb_valeurs;
	//int taille_max;		// Pourrait être ajouté pour mettre des assertions...
	noeud* t;
};
typedef struct s_fileprio fileprio;


fileprio creer_fileprio(int taille_max);		// Toujours mettre n² en argument en attendant l'avis de Mathias...
bool fileprio_non_vide(fileprio* f);
void inserer_fileprio(fileprio* f,int num_sommet,int distance);
int extraire_fileprio(fileprio* f);		// Renvoie un numéro de sommet
void diminuer_fileprio(fileprio* f,int num_sommet,int nv_val);
void detruire_fileprio(fileprio* f);
// Rajouter une fonction lire_min_fileprio ?


#endif
