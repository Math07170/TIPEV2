#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include "grille.h"
// Infrastructure
const int VIDE = 0;		// noir
const int USINE = 1;	// noir
const int GD_VILLE = 2;	// noir
const int PT_VILLE = 3;	// noir
const int VILLAGE = 4;	// noir
const int CENTRALE = 5;	// rouge
const int GD_TRANSFO = 6;	// rouge
const int PT_TRANSFO = 7;	// rouge

// Type de terrain
const int NEANT = 0;	// N'a pas à être affiché !!!
const int PLAINE = 10;	// yellow
const int FORET = 20;	// green
const int EAU = 30;		// blue
const int RIVIERE = 40;	// cyan		// NE VA PROBABLEMENT PAS RESTER
const int MONTAGNE = 50;	// white





grille* creer_grille(int n) {
    grille* g = malloc(sizeof(grille));
    g->taille = n;
    g->t = malloc(sizeof(cell*) * n);
    if(g->t == NULL){
		printf("Manque de mémoire pour créer la grille\n");
		exit(-1);
	}
    for(int k = 0; k < n; k++){
        g->t[k] = malloc(sizeof(cell) * n);
        if(g->t[k] == NULL){
			printf("Manque de mémoire pour créer la grille\n");
			exit(-1);
		}
		for(int l = 0;l < n;l++){
			g->t[k][l].c = malloc(sizeof(cable)*4);		// NOMBRE MAX DE CABLES : 4 (car 4 directions)
			if(g->t[k][l].c == NULL){
				printf("Manque de mémoire pour créer la grille\n");
				exit(-1);
			}
		}
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            g->t[i][j].x = i;
            g->t[i][j].y = j;
            g->t[i][j].nb_c = 0;		// Initialisation
            g->t[i][j].type = PLAINE;
            g->t[i][j].infra = VIDE;
        }
    }
    return g;
}
cell* getCell(int x, int y, grille* g){
    if(x>=g->taille || y>=g->taille || x<0|| y<0) return NULL;
    return &(g->t[x][y]);
}
bool contient_ligne(cell* c, int id_cable){
    for(int k = 0; k>c->nb_c; k++){
        if(c->c[k].id == id_cable) return true;
    }
    return false;
    
}
cell** voisins(cell* c, grille* g){
    cell** vois = malloc(4*sizeof(cell*));
    int x = c->x;
    int y = c->y;

    vois[0] =  getCell(x+1, y, g);
    vois[1] =  getCell(x-1, y, g);
    vois[2] =  getCell(x, y+1, g);
    vois[3] =  getCell(x, y-1, g);
}


int get_line_cost(int id_line, grille* g){
    int n = g->taille;
    int res = 0;
    for(int x = 0; x<n; x++){
        for(int y = 0; y<n; x++){
            if(contient_ligne(getCell(x, y, g), id_line)){
                res += getCell(x, y, g)->type;
            }
        }
    }
    return res;
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

void terrain_infra_test8(grille* g){		// À des fins de test, utiliser avec n=8, et faire disparaître les preuves à la fin
	int n = g -> taille;			// 8	!!!
	 for(int i = 0; i < 8;i++){
		for(int j = 0 ; j < 5;j++){
			g->t[i][j].infra = i;
			g->t[i][j].type = (j+1)*10;
		}
		for(int j = 5 ; j < 8;j++){
			g->t[i][j].infra = i;
			g->t[i][j].type = NEANT;
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
