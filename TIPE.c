#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include "grille.h"
#include "fileprio.h"
#include "affichage.h"

struct s_sommet{
    int x;
    int y;
    int type;
};
typedef struct s_sommet sommet;

/* Impressionnant de vacuité */
int poids(int sx, int sy) {
    return 1;
}

/* À mettre dans grille.c ? Ou pas ? */
void astar(grille* g, int** voisins, sommet* depart, sommet* final) {		// Situation du tableau voisins à clarifier/régulariser

    int n = g->taille;
    int DMAX = n+n+1;
    fileprio file = creer_fileprio(n*n);

    int* p = malloc(sizeof(int) * n*n);
    int* c = malloc(sizeof(int) * n*n);
    int* d = malloc(sizeof(int) * n*n);
    int* f = malloc(sizeof(int) * n*n);
    
    if(p == NULL || c == NULL || d == NULL || f == NULL){
		printf("Manque de mémoire pour A*\n");
	}

    int s_d = depart->x * n + depart->y;
    int s_f = final->x * n + final->y;


    for(int i=0;i<n;i++) {
        p[i] = -1;
        c[i] = BLANC;
        d[i] = DMAX;
        f[i] = DMAX;
    }

    c[s_d] = GRIS;
    d[s_d] = 0;
    f[s_d] = heuristique(depart->x,depart->y, final->x,final->y);

    inserer_fileprio(&file, s_d, f[s_d]);

    while(fileprio_non_vide(&file) && c[s_f] != NOIR) {
        int s = extraire_fileprio(&file);		// Numéro du sommet courant
        c[s] = NOIR;

        int xs = s / n;
        int ys = s % n;

        int deg;		// nombre de voisins
        if(s == 0 || s == n-1 || s == n*n-n || s == n*n-1){		// la case est dans un angle -> seulement 2 voisins
            deg = 2;
        }else if (xs == 0 || ys == 0 || xs == n-1 || ys == n-1){	// la case est en bord de tableau -> seulement 3 voisins
            deg = 3;
        }else{			// cas général -> 4 voisins
            deg = 4;
        }
				// TODO : générer d'une façon ou d'une autre le tableau voisins
        for(int i=0;i<deg;i++) {
            int s_v = voisins[s][i];		// Numéro de sommet du voisin
            int xs_v = s_v / n;
            int ys_v = s_v % n;
            if (c[s_v] == BLANC) {

                c[s_v] = GRIS;
                p[s_v] = s;
                d[s_v] = d[s] + 1;		// AJOUT ÉVENTUEL D'UN POIDS ICI (poids différent pour chaque case de la grille ?)
                f[s_v] = d[s_v] + heuristique(xs,ys,xs_v,ys_v);

                inserer_fileprio(&file, s_v, f[s_v]);
			}else if (c[s_v] == GRIS && d[s] + poids(s_v,s) < d[s_v]) {
                p[s_v] = s;
                d[s_v] = d[s] + poids(s_v,s);
                f[s_v] = d[s_v] + heuristique(xs_v,ys_v,xs,ys);
                diminuer_fileprio(&file,s_v,f[s_v]);
            }
        }
    }
}



// ----------------------------------------------------- //







// ----------------------------------------------------- //


// TODO :

// chercher un element
// plus proche element d'un sommet x
// Relier deux voisins avec un certain element (ex : câbles)

// Trouver ou placer les éléments (ex : transformateurs)
// Fonction d'optimisation (càd ?)



int main(){
    srand(time(NULL)); 

    int n = 10;
    grille g = creer_grille(n);
    randomize(USINE, 5, &g);
    randomize(MAISON, 15, &g);
    randomize(BARNABE, 10, &g);
    //affiche_moche(&g);
    init_ncurses();
    affiche(&g);

    return 0;
}
