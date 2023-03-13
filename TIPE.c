#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include "grille.h"
#include "fileprio.h"
#include "affichage.h"
#include "carte.h"
#include <ncurses.h>
#include <unistd.h>

const int BLANC = 0;
const int GRIS = 1;
const int NOIR = 2;

struct s_sommet{
    int x;
    int y;
    int type;
};
typedef struct s_sommet sommet;

/* Impressionnant de vacuité, pourra être amélioré selon les types de cases... */
int poids(int sx, int sy) {
    return 1;
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
    fprintf(stderr, "Test %d  \n", final->y);
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
        if(s / n == final->x && s%n == final->y) break;
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
        for(int k = 0; k<deg; k++){
            if(vois[k] != NULL){
                voisins[k] = vois[k]->x * n + vois[k]->y;
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
    for(int k = 0; k < n*n; k++){
        
        if(p[k] != -1){
            
            cell* c = getCell(p[k] / n, p[k]%n, g);
            if(c->x == final->x && c->y == final->y){
            
            }
            //fprintf(stderr,"x=%d, y=%d", p[k] / n, p[k]%n);
            
            cable cable;
            cable.i = 0;
            cable.u = 230;
            cable.r = 50;
            cable.id = g->nb_l;
            g->nb_l += 1;
            c->c[c->nb_c] = cable;
            c->nb_c = c->nb_c+1;
            
        }
        
    }

    return p;
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

    int n = 100;
    grille* g = creer_grille(n);
    
    /*randomize_terrain(&g);
    randomize_infra(USINE, 4, &g);
    randomize_infra(GD_VILLE, 3, &g);
    randomize_infra(PT_VILLE, 5, &g);
    randomize_infra(VILLAGE, 15, &g);
    randomize_infra(CENTRALE, 1, &g);
    randomize_infra(GD_TRANSFO, 5, &g);
    randomize_infra(PT_TRANSFO, 12, &g);*/	// NE PAS SUPPRIMER
    //////terrain_infra_test8(g);		// TEST, penser à effacer les preuves
    //////affiche_moche(&g);
    
    //init_ncurses();
    //generation_carte(g);

    //astar(g, getCell(10,10,g), getCell(50,50,g));
	//affiche(g);
	
	// TESTS FILEPRIO
    fileprio f = creer_fileprio(n*n);
    inserer_fileprio(&f, 5, 5);
    // print_tableau(f);
    inserer_fileprio(&f, 1, 1);
    // print_tableau(f);
    inserer_fileprio(&f, 4, 4);
    // print_tableau(f);
    inserer_fileprio(&f, 3, 3);
    // print_tableau(f);
    inserer_fileprio(&f, 2, 2);
    // print_tableau(f);
    fprintf(stderr, "%d \n", extraire_fileprio(&f));
    fprintf(stderr, "%d \n", extraire_fileprio(&f));
    fprintf(stderr, "%d \n", extraire_fileprio(&f));
    inserer_fileprio(&f, 0, 0);
    inserer_fileprio(&f, 2, 2);
    diminuer_fileprio(&f,5,-1);
    diminuer_fileprio(&f,4,0);
    diminuer_fileprio(&f,2,-2);
    while(fileprio_non_vide(&f)){
        fprintf(stderr, "%d \n", extraire_fileprio(&f));
        // print_tableau(f);
        //extraire_fileprio(&f);
    }
    // ORDRE ATTENDU (valeurs) : 1 2 3 -2 -1 0 0 4 5
    // ORDRE ATTENDU (sommets) : 1 2 3 2 5 0<->4
    detruire_fileprio(&f);
    // TEST OK
	
	//sleep(5);		// Hack fumeux TEMPORAIRE pour voir la grille quelques instants
	//endwin();		// Arrête proprement ncurses, c'est REQUIS pour ne pas détruire le terminal
	
	detruire_grille(g);
		
    return 0;
}
