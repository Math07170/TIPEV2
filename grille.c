#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include "grille.h"
#include "fileprio.h"

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
			g->t[k][l].c = malloc(sizeof(cable)*4);		// Remplacé par les IDs de lignes, mais ça marche en l'état
			if(g->t[k][l].c == NULL){
				printf("Manque de mémoire pour créer la grille\n");
				exit(-1);
			}
		}
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
			g->nb_l = 0;
            g->t[i][j].x = i;
            g->t[i][j].y = j;
            g->t[i][j].nb_c = 0;		// Initialisation
            g->t[i][j].type = NEANT;
            g->t[i][j].infra = VIDE;
        }
    }
    return g;
}

void detruire_grille(grille* g){
	int n = g -> taille;
	for(int i = 0;i < n;i++){
		for(int j = 0;j < n;j++){
				free(g->t[i][j].c);
		 }
		 free(g->t[i]);
	 }
	 free(g->t);
	 free(g);
	return;
}

cell* getCell(int x, int y, grille* g){
    if(x>=g->taille || y>=g->taille || x<0|| y<0) return NULL;
    return (&(g->t[x][y]));
}

bool contient_ligne(cell* c, int id_ligne){
    for(int k = 0; k<c->nb_c; k++){
        if(c->c[k].id == id_ligne) return true;
    }
    return false;
}

/* Ordre : H,B,G,D */
cell** voisins(cell* c, grille* g){
    cell** vois = malloc(4*sizeof(cell*));
    int x = c->x;
    int y = c->y;

    vois[1] =  getCell(x+1, y, g);
    vois[0] =  getCell(x-1, y, g);
    vois[3] =  getCell(x, y+1, g);
    vois[2] =  getCell(x, y-1, g);
    return vois;
}
void deplace(cell* source, cell* destination){

}
void delete_line(int id_line, grille* g){
    int n = g->taille;
    int res = 0;
    for(int x = 0; x<n; x++){
        for(int y = 0; y<n; x++){
            if(contient_ligne(getCell(x, y, g), id_line)){
                (getCell(x, y, g)->nb_c) = (getCell(x, y, g)->nb_c) - 1;
                
            }
        }
    }
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
    //printf("voici son terrain : %d\n", terrain);
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
const int BLANC = 0;
const int GRIS = 1;
const int NOIR = 2;




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

/* Impressionnant de vacuité, pourra être amélioré selon les types de cases... */
int poids(int sx, int sy, grille* g) {
    int n = g->taille;
    cell* c = getCell(sx / n, sx%n, g);
    int res = (c->type)*(c->type);
    return res;
}

/* À mettre dans grille.c ? Ou pas ? */
int* astar(grille* g, cell* depart, cell* final) {	// Situation du tableau voisins à clarifier/régulariser
    
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
    


    for(int i=0;i<n*n;i++) {
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
        
        //if(s / n == final->x && s%n == final->y) break;
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
        cell** vois = voisins(getCell(xs, ys,g), g);
        int voisins[deg];
        int indice = 0;
        for(int k = 0; k<4; k++){
            if(vois[k] != NULL){
                voisins[indice] =  vois[k]->x* n + vois[k]->y;
                indice++;
            }
        }
				// TODO : générer d'une façon ou d'une autre le tableau voisins
        for(int i=0;i<deg;i++) {
            
            int s_v = voisins[i];		// Numéro de sommet du voisin
            
            int xs_v = s_v / n;
            int ys_v = s_v % n;
            if (c[s_v] == BLANC) {
                
                c[s_v] = GRIS;
                
                p[s_v] = s;
                
                d[s_v] = d[s] + poids(s_v,s, g);		// AJOUT ÉVENTUEL D'UN POIDS ICI (poids différent pour chaque case de la grille ?)
                
                f[s_v] = d[s_v] + heuristique(xs,ys,final->x,final->y);
                inserer_fileprio(&file, s_v, f[s_v]);
                
			}else if (c[s_v] == GRIS && d[s] + poids(s_v,s, g) < d[s_v]) {
                p[s_v] = s;
                
                d[s_v] = d[s] + poids(s_v,s, g);
                f[s_v] = d[s_v] + heuristique(final->x,final->y,xs,ys);
                diminuer_fileprio(&file,s_v,f[s_v]);
                
            }
            
        }
        
    }
    cell* ce = final;
    while(ce != depart){
        int k = p[ce->x * n + ce->y];
        cable cable;
        cable.i = 0;
        cable.u = 230;
        cable.r = 50;
        cable.id = g->nb_l;
        ce->c[ce->nb_c] = cable;
        ce->nb_c = ce->nb_c+1;
        fprintf(stderr, "Case : x=%d y=%d", ce->x, ce->y);
        ce = getCell(k / n, k%n, g);
    }
    g->nb_l += 1;
    detruire_fileprio(&file);
    return p;
}