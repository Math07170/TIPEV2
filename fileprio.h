#ifndef FILEPRIO_H
#define FILEPRIO_H

#include <stdbool.h>

/* Structure de file de priorité donnant le numéro du sommet associé à la valeur la plus PETITE
 * Implémentation : tas binaire avec un tableau
 * 17/10/22 au soir : pas encore testé, dangereux mais j'ai sommeil */

struct s_noeud = {		// càd une case du tableau
	int num_sommet;
	int valeur;
	//int i_fils_droit;			// a priori non
	//int i_fils_gauche;		// non plus
};
typedef struct s_noeud noeud;

struct s_fileprio = {		// Implémentée par un tas binaire
	int nb_valeurs;
	//int taille_max;		// Pourrait être ajouté pour mettre des assertions...
	int* t;
}
typedef struct s_fileprio fileprio;


fileprio creer_fileprio(int taille_max);		// Toujours mettre n² en argument en attendant l'avis de Mathias...
bool fileprio_non_vide(fileprio* f);
void inserer_fileprio(fileprio* f,int num_sommet,int distance);
int extraire_fileprio(fileprio* f);		// Renvoie un numéro de sommet
// Rajouter une fonction detruire_fileprio ?
// Rajouter une fonction lire_min_fileprio ?


#endif
