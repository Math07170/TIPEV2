#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

const int VIDE = 0;		// noir
const int USINE = 1;	// noir
const int GD_VILLE = 2;	// noir
const int PT_VILLE = 3;	// noir
const int VILLAGE = 4;	// noir
const int CENTRALE = 5;	// rouge
const int GD_TRANSFO = 6;	// rouge
const int PT_TRANSFO = 7;	// rouge

const int PLAINE = 0;	// yellow
const int FORET = 10;	// green
const int EAU = 20;		// blue
const int RIVIERE = 30;	// cyan
const int MONTAGNE = 40;	// white

// Il faut ajouter des constantes pour les câbles !

struct s_cable{
	int id;
	float u;
	float i;
	float r;
};
typedef struct s_cable cable;

struct s_cell{ 
	int nb_c;
    cable* c;
    int type;
    int infra;
};
typedef struct s_cell cell;

struct s_grille{
    int taille;
    cell** t;
};
typedef struct s_grille grille;



grille creer_grille(int n) {
    grille g;
    g.taille = n;
    g.t = malloc(sizeof(cell*) * n);
    if(g.t == NULL){
		printf("Manque de mémoire pour créer la grille\n");
		exit(-1);
	}
    for(int k = 0; k < n; k++){
        g.t[k] = malloc(sizeof(cell) * n);
        if(g.t[k] == NULL){
			printf("Manque de mémoire pour créer la grille\n");
			exit(-1);
		}
		for(int l = 0;l < n;l++){
			g.t[k][l].c = malloc(sizeof(cable)*4);		// NOMBRE MAX DE CABLES : 4 (car 4 directions)
			if(g.t[k][l].c == NULL){
				printf("Manque de mémoire pour créer la grille\n");
				exit(-1);
			}
		}
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            g.t[i][j].nb_c = 0;		// Initialisation
            g.t[i][j].type = PLAINE;
            g.t[i][j].infra = VIDE;
        }
    }
    return g;
}


void ajoute(int infrastructure, int i, int j, grille* g) {
    int n = g->taille;
    assert(i>=0 && j>=0 && i<n && j<n && g->t[i][j].infra == VIDE);
    g->t[i][j].infra = infrastructure;
}


void supprime(int i, int j, grille* g) {
    int n = g->taille;
    assert(i>=0 && j>=0 && i<n && j<n);
    g->t[i][j].infra = VIDE;
}


void remplace(int infrastructure, int i, int j, grille* g) {
    int n = g->taille;
    assert(i>=0 && j>=0 && i<n && j<n);
    g->t[i][j].infra = infrastructure;
}


/* Renvoie true si 'g[i][j]' == x */
bool est(int infrastructure, int i, int j, grille* g){
    int n = g->taille;
    assert(i>=0 && j>=0 && i<n && j<n);
    return (g->t[i][j].infra == infrastructure);
}

void change_terrain(int terrain, int i, int j, grille* g){
    int n = g->taille;
    assert(i>=0 && j>=0 && i<n && j<n);
    g->t[i][j].type = terrain;
}

void randomize_infra(int element, int nb_element, grille* g){
    int counter = 0;
    int n = g->taille;
    while (counter < nb_element) {
        int i = rand() % n;
        int j = rand() % n;
        if (est(VIDE, i, j, g)) {		// AUSSI VÉRIFIER QUE LA CASE EST "CONSTRUCTIBLE"
            g->t[i][j].infra = element;
            counter++;
        }
    }
    return;
}

void randomize_terrain(grille* g){		// TEMPORAIRE !!!
	int n = g->taille;
    for(int i = 0; i < n;i++){
		for(int j = 0 ; j < n;j++){
			int t = 10 * (rand() % 5);
			g->t[i][j].type = t;
		}
	}
    return;
}


/* N'est plus utilisé */
/*void affiche_moche(grille* g) {
    int n = g->taille;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            int e = g->t[i][j];
            if (e != VIDE) {
                printf("i : %d, j : %d, élément : %d\n", i,j,e);
            }
        }
    }
}*/


int heuristique(int xa, int ya, int xb, int yb) {
    int x = xa - xb;
    int y = ya - yb;
    if (x < 0) {
        x = -x;
    }
    if (y < 0) {
        y = -y;
    }
    return x + y;
}
