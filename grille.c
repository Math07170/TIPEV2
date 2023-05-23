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

const int NB_USINE = 30;
const int NB_GD_VILLE = 15;
const int NB_PT_VILLE = 50;
const int NB_VILLAGE = 70;
const int NB_CENTRALE = 10;

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
    g->nb_infra = 0;
    g->infra = malloc(sizeof(cell*) * n*n);
    // initialisation tu tableau infra 
    for(int k = 0; k < n*n; k++){
        g->infra[k] = NULL;
        if(g->infra == NULL){
            printf("Manque de mémoire alerte pour créer la grille\n");
        }
    }
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
			(g->t[k][l]).c = malloc(sizeof(cable)*1000);		// Remplacé par les IDs de lignes, mais ça marche en l'état
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
void copie_case(cell* depart, cell* arrive){
    arrive->x = depart->x;
    arrive->y = depart->y;
    arrive->nb_c = depart->nb_c;
    arrive->type = depart->type;
    arrive->infra = depart->infra;
    for(int i = 0;i < depart->nb_c;i++){
        arrive->c[i].u = depart->c[i].u;
        arrive->c[i].r = depart->c[i].r;
        arrive->c[i].i = depart->c[i].i;
        arrive->c[i].id = depart->c[i].id;
    }
}
grille* copie_grille(grille* g){
    //fprintf(stderr, "%d \n ", g->t[99][99].nb_c);
    int n = g->taille;
    grille* copie = creer_grille(n);
    copie->nb_infra = g->nb_infra;
    copie->nb_l = g->nb_l;
    
    for(int i = 0;i < n;i++){
        for(int j = 0;j < n;j++){
            //fprintf(stderr,"i = %d, j = %d\n",i,j);
            copie->t[i][j].x = g->t[i][j].x;
            //fprintf(stderr,"1");
            copie->t[i][j].y = g->t[i][j].y;
            //fprintf(stderr,"2");
            copie->t[i][j].nb_c = g->t[i][j].nb_c;
            //fprintf(stderr,"3");
            copie->t[i][j].type = g->t[i][j].type;
            //fprintf(stderr,"4");
            copie->t[i][j].infra = g->t[i][j].infra;
            //fprintf(stderr,"5");
            //fprintf(stderr,"nb_c = %d\n",copie->t[i][j].nb_c);
            for(int k = 0;k < g->t[i][j].nb_c;k++){
                //fprintf(stderr,"6");
                copie->t[i][j].c[k].u = g->t[i][j].c[k].u;
                copie->t[i][j].c[k].r = g->t[i][j].c[k].r;
                copie->t[i][j].c[k].i = g->t[i][j].c[k].i;
                copie->t[i][j].c[k].id = g->t[i][j].c[k].id;
            }
            
        }
    }
    for(int i = 0;i < g->nb_infra;i++){
        cell* tes = g->infra[i];
        copie->infra[i] = getCell(g->infra[i]->x,g->infra[i]->y,copie);
    }
    return copie;
}
void detruire_grille(grille* g){
	int n = g -> taille;
	for(int i = 0;i < n;i++){
		for(int j = 0;j < n;j++){
				free(g->t[i][j].c);
		 }
		 free(g->t[i]);
	}
    free(g->infra);
	free(g->t);
	free(g);
}

cell* getCell(int x, int y, grille* g){
    if(x>=g->taille || y>=g->taille || x<0|| y<0){
        //fprintf(stderr, "Erreur : cellule x=%d y=%d hors de la grille\n", x, y);
        return NULL;
    }
    //fprintf(stderr, "getCell : %d %d\n", g->t[x][y].x, g->t[x][y].y);
    return (&(g->t[x][y]));
}

bool contient_ligne(cell* c, int id_ligne){
    for(int k = 0; k<c->nb_c; k++){
        if(c->c[k].id == id_ligne) return true;
    }
    return false;
}

/* Ordre : H,B,G,D
 * IL EST REQUIS DE LIBÉRER LE TABLEAU RENVOYÉ PAR CETTE FONCTION APRÈS UTILISATION !!! */
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

/* Renvoie true si 'g[i][j]' == x */
bool est_terrain(int terrain, int i, int j, grille* g){
    int n = g->taille;
    if (i>=0 && j>=0 && i<n && j<n) {
        return (g->t[i][j].type == terrain);
    }
    else {
        return false;
    }
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
int* astar(grille* g, cell* depart, cell* final, int id) {	// Situation du tableau voisins à clarifier/régulariser
    assert(final != NULL && depart != NULL);
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
        int voisins[deg];		// HORREUR !!!
        int indice = 0;
        for(int k = 0; k<4; k++){
            if(vois[k] != NULL){
                voisins[indice] =  vois[k]->x* n + vois[k]->y;
                indice++;
            }
        }
        free(vois);
				// TODO : générer d'une façon ou d'une autre le tableau voisins
        for(int i=0;i<indice;i++) {
            
            int s_v = voisins[i];		// Numéro de sommet du voisin
            
            int xs_v = s_v / n;
            int ys_v = s_v % n;
            cell* v = getCell(xs_v, ys_v,g);
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
        if(ce == NULL){
            fprintf(stderr, "Erreur : x = %d, y=%d\n", final->x, final->y);
            fprintf(stderr, "Erreur : CE est nul\n");
            return NULL;
        }
        
        int k = p[ce->x * n + ce->y];
        //fprintf(stderr, "x = %d, y=%d\n", ce->x, ce->y);
        cable cable;
        if(id == 1){
            cable.i = 2.35;
            cable.u = 230;
            cable.r = 0.06;
            cable.id = g->nb_l;
            ce->c[ce->nb_c] = cable;
            ce->nb_c = ce->nb_c+1;
        }else if(id == 2){
            cable.i = 45;
            cable.u = 30000;
            cable.r = 0.06;
            cable.id = g->nb_l;
            ce->c[ce->nb_c] = cable;
            ce->nb_c = ce->nb_c+1;
        }else if(id == 3){
            cable.i = 450;
            cable.u = 400000;
            cable.r = 0.06;
            cable.id = g->nb_l;
            ce->c[ce->nb_c] = cable;
            ce->nb_c = ce->nb_c+1;
        }else{
            
        }
        //fprintf(stderr, "Case : x=%d y=%d", ce->x, ce->y);
        ce = getCell(k / n, k%n, g);
        //fprintf(stderr, "Case : x=%d y=%d", ce->x, ce->y);
    }
    ce=depart;
    int k = p[ce->x * n + ce->y];
    cable cable;
    if(id == 1){
        cable.i = 2.35;
        cable.u = 230;
        cable.r = 0.06;
        cable.id = g->nb_l;
        ce->c[ce->nb_c] = cable;
        ce->nb_c = ce->nb_c+1;
    }else if(id == 2){
        cable.i = 45;
        cable.u = 30000;
        cable.r = 0.06;
        cable.id = g->nb_l;
        ce->c[ce->nb_c] = cable;
        ce->nb_c = ce->nb_c+1;
    }else if(id == 3){
        cable.i = 450;
        cable.u = 400000;
        cable.r = 0.06;
        cable.id = g->nb_l;
        ce->c[ce->nb_c] = cable;
        ce->nb_c = ce->nb_c+1;
    }else{
            
    }
    g->nb_l += 1;
    detruire_fileprio(&file);
    free(p);
    free(c);
    free(d);
    free(f);
    return NULL;
}
int dist(cell* c1, cell* c2){
    int x = c1->x - c2->x;
    int y = c1->y - c2->y;
    if (x < 0) {
        x = -x;
    }
    if (y < 0) {
        y = -y;
    }
    return x + y;
}

cell** k_plus_proche(grille* g, cell* source, int id_inf, int k, int id_sup){
    cell** top = malloc(k * sizeof(cell*));
    for(int i = 0; i<k; i++){
        top[i] = NULL;
    }
    int i = 0;
    for(int l = 0; l<g->nb_infra; l++){
        cell* actuel = g->infra[l];
        if(actuel->infra < id_inf || actuel->infra > id_sup) continue;
        if(i!=k){
            top[i] = actuel;
            i++;
        }else{
            for(int j = k-1; j>=0; j--){
                if(dist(source, actuel) < dist(source, top[j])){
                    if(j == k-1){
                        top[j] = actuel;
                        continue;
                    }
                    top[j+1] = top[j];
                    top[j] = actuel;
                    
                }else{
                    break;
                }
            }
        }
    }
    return top;

}
cell* barycentre(grille* g, cell* points[], int k){
    int x = 0;
    int y = 0;
    for(int l = 0; l<k; l++){
        if(points[l] == NULL) continue;
        x = x + ((points[l])->x)/k;
        y = y + ((points[l])->y)/k;
        //fprintf(stderr, "Bary : x=%ld, y=%ld \n", x, y);
    }
    x = x;
    y = y;
    return getCell(x, y, g);
}
void situation_initiale_pop(grille** g, int l){
	int n = g[0] -> taille;
	for(int k = 0;k < NB_USINE;k++){
		int i = rand()%n;
		int j = rand()%n;
        for(int m = 0; m<l; m++){
            cell* c = getCell(i,j,g[m]);
            (g[m])->infra[g[m]->nb_infra] = c;
            (g[m])->nb_infra = g[m]->nb_infra + 1;
		    if(c -> infra == VIDE && c -> infra != EAU) c -> infra = USINE;
		    else k-=1;
        }

	}
	for(int k = 0;k < NB_GD_VILLE;k++){
		int i = rand()%n;
		int j = rand()%n;
		for(int m = 0; m<l; m++){
            cell* c = getCell(i,j,g[m]);
            (g[m])->infra[g[m]->nb_infra] = c;
            (g[m])->nb_infra = g[m]->nb_infra + 1;
		    if(c -> infra == VIDE && c -> infra != EAU) c -> infra = GD_VILLE;
		    else k-=1;
        }
	}
	for(int k = 0;k < NB_PT_VILLE;k++){
		int i = rand()%n;
		int j = rand()%n;
		for(int m = 0; m<l; m++){
            cell* c = getCell(i,j,g[m]);
            g[m]->infra[g[m]->nb_infra] = c;
            g[m]->nb_infra = g[m]->nb_infra + 1;
		    if(c -> infra == VIDE && c -> infra != EAU) c -> infra = PT_VILLE;
		    else k-=1;
        }
	}
	for(int k = 0;k < NB_VILLAGE;k++){
		int i = rand()%n;
		int j = rand()%n;
		for(int m = 0; m<l; m++){
            cell* c = getCell(i,j,g[m]);
            g[m]->infra[g[m]->nb_infra] = c;
            g[m]->nb_infra = g[m]->nb_infra + 1;
		    if(c -> infra == VIDE && c -> infra != EAU) c -> infra = VILLAGE;
		    else k-=1;
        }
	}
	for(int k = 0;k < NB_CENTRALE;k++){
		int i = rand()%n;
		int j = rand()%n;
		for(int m = 0; m<l; m++){
            cell* c = getCell(i,j,g[m]);
            g[m]->infra[g[m]->nb_infra] = c;
            g[m]->nb_infra = g[m]->nb_infra + 1;
		    if(c -> infra == VIDE && c -> infra != EAU) c -> infra = CENTRALE;
		    else k-=1;
        }
	}

   
	return;
}
void situation_initiale(grille* g){
	int n = g -> taille;
	for(int k = 0;k < NB_USINE;k++){
		int i = rand()%n;
		int j = rand()%n;
		cell* c = getCell(i,j,g);
        g->infra[g->nb_infra] = c;
        g->nb_infra = g->nb_infra + 1;
		if(c -> infra == VIDE && c -> infra != EAU) c -> infra = USINE;
		else k-=1;
	}
	for(int k = 0;k < NB_GD_VILLE;k++){
		int i = rand()%n;
		int j = rand()%n;
		cell* c = getCell(i,j,g);
        g->infra[g->nb_infra] = c;
        g->nb_infra = g->nb_infra + 1;
		if(c -> infra == VIDE && c -> infra != EAU) c -> infra = GD_VILLE;
		else k-=1;
	}
	for(int k = 0;k < NB_PT_VILLE;k++){
		int i = rand()%n;
		int j = rand()%n;
		cell* c = getCell(i,j,g);
        g->infra[g->nb_infra] = c;
        g->nb_infra = g->nb_infra + 1;
		if(c -> infra == VIDE && c -> infra != EAU) c -> infra = PT_VILLE;
		else k-=1;
	}
	for(int k = 0;k < NB_VILLAGE;k++){
		int i = rand()%n;
		int j = rand()%n;
		cell* c = getCell(i,j,g);
        g->infra[g->nb_infra] = c;
        g->nb_infra = g->nb_infra + 1;
		if(c -> infra == VIDE && c -> infra != EAU) c -> infra = VILLAGE;
		else k-=1;
	}
	for(int k = 0;k < NB_CENTRALE;k++){
		int i = rand()%n;
		int j = rand()%n;
		cell* c = getCell(i,j,g);
        g->infra[g->nb_infra] = c;
        g->nb_infra = g->nb_infra + 1;
		if(c -> infra == VIDE && c -> infra != EAU) c -> infra = CENTRALE;
		else k-=1;
	}

   
	return;
}


//Pose les transformateur de façon aléatoire
void random_transfo(grille* g){
    int n = g -> taille;
    for(int x =0; x<NB_CENTRALE; x++){
        int i = rand()%n;
        int j = rand()%n;
        cell* c = getCell(i,j,g);
        g->infra[g->nb_infra] = c;
        g->nb_infra = g->nb_infra + 1;
        if(c -> infra == VIDE && c -> infra != EAU) c -> infra = CENTRALE;
        else x-=1;
    }
    int NB_petit_transfo = NB_PT_VILLE /5;
    int NB_grand_transfo = (NB_GD_VILLE + NB_USINE + NB_petit_transfo)/5;
    for(int x =0; x<NB_grand_transfo; x++){
        int i = rand()%n;
        int j = rand()%n;
        cell* c = getCell(i,j,g);
        g->infra[g->nb_infra] = c;
        g->nb_infra = g->nb_infra + 1;
        if(c -> infra == VIDE && c -> infra != EAU) c -> infra = GD_TRANSFO;
        else x-=1;
    }
    for(int x =0; x<NB_petit_transfo; x++){
        int i = rand()%n;
        int j = rand()%n;
        cell* c = getCell(i,j,g);
        g->infra[g->nb_infra] = c;
        g->nb_infra = g->nb_infra + 1;
        if(c -> infra == VIDE && c -> infra != EAU) c -> infra = PT_TRANSFO;
        else x-=1;
    }

}
//relie chaque consomateur au transformateur le plus proche.
void relieup(grille* g){
    for(int k = 0; k < g->nb_infra; k++){
        if(g->infra[k]->infra == VILLAGE || g->infra[k]->infra == PT_VILLE){
            cell** destination = k_plus_proche(g, g->infra[k], PT_TRANSFO, 1, PT_TRANSFO);
            cell* c = destination[0];
            if(c == NULL) exit(666);
            astar(g, g->infra[k], c, 1);
            free(destination);
        }else if(g->infra[k]->infra == USINE || g->infra[k]->infra == PT_TRANSFO || g->infra[k]->infra == GD_VILLE){
            cell** destination = k_plus_proche(g, g->infra[k], GD_TRANSFO, 1, GD_TRANSFO);
            cell* c = destination[0];
            if(c == NULL) exit(666);
            astar(g, g->infra[k], c, 2);
            free(destination);
        }else{
            cell** destination = k_plus_proche(g, g->infra[k], CENTRALE, 1, CENTRALE);
            cell* c = destination[0];
            if(c == NULL) exit(666);
            astar(g, g->infra[k], c, 3);
            free(destination);
        }
    }
}


void relie(grille* g) {
     for(int x = 0; x<g->nb_infra; x++){
        //fprintf(stderr, "x=%d", x);
        if(((g->infra[x])->infra > 0 && (g->infra[x])->infra <= 4) && (((g->infra[x])->nb_c) == 0)){
            cell** vois = k_plus_proche(g, g->infra[x], USINE, 5, VILLAGE);
            cell* b = barycentre(g, vois, 5);
            b->infra = PT_TRANSFO;
            g->infra[g->nb_infra] = b;
            g->nb_infra += 1;
            for(int p=0; p<5; p++){
                if(vois[p] == NULL) continue;
                if(vois[p]->nb_c == 0) astar(g, vois[p], b, 1);
            }
            
        }
    }
    for(int x = 0; x<g->nb_infra; x++){
        //fprintf(stderr, "x=%d", x);
        if((g->infra[x])->infra == PT_TRANSFO){
            cell** vois = k_plus_proche(g, g->infra[x], PT_TRANSFO, 10, PT_TRANSFO);
            cell* b = barycentre(g, vois, 5);
            b->infra = GD_TRANSFO;
            g->infra[g->nb_infra] = b;
            g->nb_infra += 1;
            for(int p=0; p<5; p++){
                if(vois[p] == NULL) continue;
                if(vois[p]->nb_c <= 5) astar(g, vois[p], b, 1);
            }
            
        }
    }
    for(int x = 0; x<g->nb_infra; x++){
        //fprintf(stderr, "x=%d", x);
        if((g->infra[x])->infra == GD_TRANSFO){
            cell** vois = k_plus_proche(g, g->infra[x], CENTRALE, 1, CENTRALE);
            if(vois[0] == NULL) continue;
            astar(g, vois[0], g->infra[x], 1);
        }
    }
}
void creation_res(grille* g, int k_vois_cons, int k_vois_transfo){
    return;
}

void export_csv_file(int xl[], int yl[], int n){
    FILE* fichier = NULL;
    fichier = fopen("resultat.csv", "w+");
    if (fichier != NULL)
    {
        for(int i = 0; i<n; i++){
            //fprintf(fichier, "%d;%d\n", xl[i], yl[i]);
        }
        fclose(fichier);
    }
    return;
}

