#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
	// Méfiance

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



grille creer_grille(int n) {
    grille g;
    g.taille = n;
    g.t = malloc(sizeof(int*) * n);
    if(g.t == NULL){
		printf("Manque de mémoire pour créer la grille\n");
		exit(-1);
	}
    for(int k = 0; k < n; k++){
        g.t[k] = malloc(sizeof(int) * n);
        if(g.t[k] == NULL){
		printf("Manque de mémoire pour créer la grille\n");
		exit(-1);
	}
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


void affiche_moche(grille* g) {
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
