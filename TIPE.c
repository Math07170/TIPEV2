#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include "fileprio.h"


const int VIDE = 0;
const int USINE = 1;
const int MAISON = 2;
const int BARNABE = 3;

const int BLANC = 0;
const int GRIS = 1;
const int NOIR = 2;



struct s_grille{
    int taille;
    int** t;
};

typedef struct s_grille grille;

struct s_sommet{
    int x;
    int y;
};

typedef struct s_sommet sommet;



grille creer_grille(int n) {
    grille g;
    g.taille = n;
    g.t = malloc(sizeof(int*) * n);
    for(int k = 0; k < n; k++){
        g.t[k] = malloc(sizeof(int) * n);
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            g.t[i][j] = VIDE;		// Initialisation
        }
    }
    return g;
}

void ajoute(int x, int i, int j, grille* g) {
    int n = g->taille;
    assert(i>=0 && j>=0 && i<n && j<n && g->t[i][j] == VIDE);
    g->t[i][j] = x;
}

void supprime(int i, int j, grille* g) {
    int n = g->taille;
    assert(i>=0 && j>=0 && i<n && j<n);
    g->t[i][j] = VIDE;
}

void remplace(int x, int i, int j, grille* g) {
    int n = g->taille;
    assert(i>=0 && j>=0 && i<n && j<n);
    g->t[i][j] = x;
}

/* Renvoie true si 'g[i][j]' == x */
bool est(int x, int i, int j, grille* g) {
    int n = g->taille;
    assert(i>=0 && j>=0 && i<n && j<n);
    return (g->t[i][j] == x);
}

void randomize(int element, int nb_element, grille* g) {
    int counter = 0;
    int n = g->taille;
    while (counter < nb_element) {
        int i = rand() % n;
        int j = rand() % n;
        if (est(VIDE, i, j, g)) {
            g->t[i][j] = element;
            counter++;
        }
    }
    return;
}


void affiche(grille* g) {
    int n = g->taille;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            int e = g->t[i][j];
            if (e != VIDE) {
                printf("i : %d, j : %d, élément : %d\n", i,j,e);
            }
        }
    }



}


int heuristique(int xa, int ya, int xb, int yb) {
    int x = xa - xb;
    int y = ya - yb;
    if (x < 0) {
        x = -x;
    }
    if (y < 0) {
        y = -y;
    }
    return x + y;		// SUS
}

int poids(int sx, int sy) {
    return 1;
}

void astar(grille* g, int** voisins, sommet* depart, sommet* final) {		// Situation du tableau voisins à clarifier/régulariser

    int n = g->taille;
    int DMAX = n+n+1;
    fileprio file = creer_fileprio(n*n);

    int* p = malloc(sizeof(int) * n*n);
    int* c = malloc(sizeof(int) * n*n);
    int* d = malloc(sizeof(int) * n*n);
    int* f = malloc(sizeof(int) * n*n);

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
        int s = extraire_fileprio(&file);
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
            int s_v = voisins[s][i];
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
                diminuer_fileprio(&file,s_v,f[s_v]);		// Flemme de comprendre ce qu'est "y"
            }
        }
    }
}



// ----------------------------------------------------- //







// ----------------------------------------------------- //




// chercher un element
// plus proche element d'un sommet x
// Relier deux voisins avec un certain element (ex : cables)

// Trouver ou placer les elements (ex : transformateurs)
// Fonction d'optimisation



int main(){
    srand(time(NULL)); 

    int n = 100;
    grille g = creer_grille(100);
    randomize(USINE, 200, &g);
    randomize(MAISON, 200, &g);
    randomize(BARNABE, 200, &g);
    affiche(&g);
    


    return 0;
}
