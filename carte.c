#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "grille.h"
#include "affichage.h"
#include "carte.h"
//#include "quickimage.h"

const int k0 = 1;
const int k1 = 2;
const int k2 = 5;
const int k3 = 10;
const int k4 = 20;

const int c0 = 0;
const int c1 = 5;
const int c2 = 10;
const int c3 = 20;
const int c4 = 30;


const int P_PLAINE = 32;
const int P_FORET = 25;
const int P_EAU = 0;
const int P_RIVIERE = 0;	// À enlever
const int P_MONTAGNE = 0;
const int P_NEANT = 0;		// Ne pas changer !

int egalites(int A,int B,int C,int D,int T) {
	int res = 0;
	if (A==T) {
		res++;
	}
	if (B==T) {
		res++;
	}
	if (C==T) {
		res++;
	}
	if (D==T) {
		res++;
	}
	return res;
}

int coef(int A,int B,int C,int D,int T,int p) {
	int c = egalites(A,B,C,D,T);
	if (p==0) {
		return 0;
	}
	else if (c==0) {
		return (k0*p + c0);
	}
	else if (c==1) {
		return (k1*p + c1);
	}
	else if (c==2) {
		return (k2*p + c2);
	}
	else if (c==3) {
		return (k3*p + c3);
	}
	else {
		return (k4*p + c4);
	}
}
	


int random_terrain(int A,int B,int C,int D) { // A : Haut, B = Bas, C = Gauche, D = Droite
	
	int Coefplaine = coef(A,B,C,D,PLAINE,P_PLAINE);
	int Coefforet = coef(A,B,C,D,FORET, P_FORET);
	int Coefeau = coef(A,B,C,D,EAU,P_EAU);
	int Coefriviere = coef(A,B,C,D,RIVIERE,P_RIVIERE);
	int Coefmontagne = coef(A,B,C,D,MONTAGNE,P_MONTAGNE);
	int Coefneant = coef(A,B,C,D,NEANT,P_NEANT);
	
	int somme = Coefplaine + Coefforet + Coefeau + Coefriviere + Coefmontagne + Coefneant;

	int p1 = Coefplaine;
	int p2 = p1 + Coefforet;
	int p3 = p2 + Coefeau;
	int p4 = p3 + Coefriviere;
	int p5 = p4 + Coefmontagne;

	//printf("somme : %d\n", somme);
	//printf("coef : %d\n", Coefplaine);
	
	int res = rand() % somme;
	
	//printf("res 1 : %d\n", res);
	
	if (0 <= res && res < p1) {
		res = PLAINE;
	} else if (p1 <= res && res < p2) {
		res = FORET;
	} else if (p2 <= res && res < p3) {
		res = EAU;
	} else if (p3 <= res && res < p4) {
		res = RIVIERE;
	} else if (p4 <= res && res < p5) {
		res = MONTAGNE;
	} else if (p5 <= res && res < somme) {
		res = NEANT;
	} else {
		exit(1);
	}
	//printf("res : %d\n", res);
	return res;
}


void tab_init(int taille, int* t) {
	for (int i=0;i<taille;i++) {
		for (int j=0;j<taille;j++) {
			int k = i*taille + j;
			t[k] = k;
		}
	}
	int a = t[20];
	//printf("20 : %d\n\n", a); //test
}


int affine_terrain(int H, int B, int G, int D, int A) {
	
	if (H==B && (B==G || B==D)) {
		return H;
	}
	else if (G==D && (D==H || D==B)) {
		return G;
	}
	else {
		return A;
	}
}



int random_entre(int min, int max) {
	int m = max - min + 1;
	int res = rand() % max;
	return (res + min);
}


//Pose un terrain t suivant la direction d depuis la case (x,y)
//La fonction diminura de 1 la longeur du terrain à poser
//La fonction augmentera x et y convenablement selon la direction
//Si on est au bord de la grille, la longeur restante à poser passe alors à 0

void poser_terrain(grille* g, int t, int* x, int* y, int dir, int* len) {
	int n = g->taille;
	int d = dir % 8;

	if (*x < 0 || *y < 0 || *x >= n || *y >= n) {
		*len = 0;
	}
	else if (d==0 && *x >= 0) { // Nord
		change_terrain(t,*x,*y,g);
		*x = *x - 1;
	}
	else if (d==1 && *x >= 0 && *y <= (n-1)) { // Nord-Est
		change_terrain(t,*x,*y,g);
		*x = *x - 1;
		*y = *y + 1;
	}
	else if (d==2 && *y <= (n-1)) { // Est
		change_terrain(t,*x,*y,g);
		*y = *y + 1;
	}
	else if (d==3 && *x <= (n-1) && *y <= (n-1)) { // Sud-Est
		change_terrain(t,*x,*y,g);
		*x = *x + 1;
		*y = *y + 1;
	}
	else if (d==4 && *x <= (n-1)) { // Sud
		change_terrain(t,*x,*y,g);
		*x = *x + 1;
	}
	else if (d==5 && *x <= (n-1) && *y >= 0) { // Sud-West
		change_terrain(t,*x,*y,g);
		*x = *x + 1;
		*y = *y - 1;
	}
	else if (d==6 && *y >= 0) { // West
		change_terrain(t,*x,*y,g);
		*y = *y - 1;
	}
	else if (d==7 && *x >= 0 && *y >= 0) { // Nord-West
		change_terrain(t,*x,*y,g);
		*x = *x - 1;
		*y = *y - 1;
	}
	else {
		*len = 0;
	}
	*len = *len - 1;
}

//pose un cours d'eau avec pour sommet initial (xi,yi) et de longueur maximum len_max

void eau_passage(grille* g, int x, int y) {

	int n = g->taille;

	int ptourner = 10;

	int len = n*n;
	int direction = rand() % 8; // nord = 0 , est = 2, .... mod 8

	while (len > 0 && !est_terrain(EAU,x,y,g)) {
		
		int r = rand() % 100;
		if (r > ptourner) {
			poser_terrain(g, EAU, &x, &y, direction, &len);
		}
		else {
			int rd = rand() % 2;
			if (rd == 1) {
				direction += 1;
			}
			else if (direction == 1) {
				direction = 8;
			}
			else {
				direction -= 1;
			}
			poser_terrain(g, EAU, &x, &y, direction, &len);
		}
	}
}

//Pose une montagne de taille aléatoire sur la carte

void montagne_passage(grille* g) {
	int n = g->taille;

	int x = rand() % n;
	int y = rand() % n;

	int lignes = x + random_entre(6,8);
	int i = x;
		
	while (i < lignes && i < n) {

		int j1 = y;
		int j2 = y;

		int len1 = random_entre(3,4);
		int len2 = random_entre(3,4);


		while(j1 >= 0 && j1 < n && len1 > 0) {	
			poser_terrain(g,MONTAGNE,&i,&j1,2,&len1);
		}
		while(j2 < n && len2 > 0 && j2 >= 0) {	
			poser_terrain(g,MONTAGNE,&i,&j2,6,&len2);
		}
		i++;
	}
	int r = rand() % 100;
	if (r < 60) {
		eau_passage(g,x,y);
	}
}


void deuxième_passage(grille* g, int NB) {

	int n = g->taille;
	
	for (int k1=0;k1<NB;k1++) {
		int i = rand() % n;
		int j = rand() % n;

		int ter[4];
		
		cell* c = getCell(i,j,g);
		cell** v_c = voisins(c, g);
		
		for (int k=0;k<4;k++) {
			if (v_c[k] == NULL) {
				ter[k] = NEANT;
			}
			else {
				ter[k] = v_c[k]->type;
			}
		}
		
		int terrain = affine_terrain(ter[0], ter[1], ter[2], ter[3], c->type);
		change_terrain(terrain,i,j,g);
		free(v_c);
	}	



}

grille* generation_carte(grille* g) {
	int n = g->taille;

	int* t = malloc(n*n*sizeof(int));
	tab_init(n,t);
	
	//premier passage

	int len_actuel = n*n;

	while (len_actuel > 0) {
		int r = rand() % len_actuel;
		int x = t[r];
		int i = x / n;
		int j = x % n;

		//printf("ran : %d\nlen : %d\nx : %d\ni : %d\nj : %d\n\n", r,len_actuel,x,i,j); //test

		int ter[4];
		
		cell* c = getCell(i,j,g);
		cell** v_c = voisins(c, g);
		
		for (int k=0;k<4;k++) {
			if (v_c[k] == NULL) {
				ter[k] = NEANT;
			}
			else {
				ter[k] = v_c[k]->type;
			}
		}
		
		int terrain = random_terrain(ter[0], ter[1], ter[2], ter[3]);
		change_terrain(terrain,i,j,g);
		
		t[r] = t[len_actuel-1];
		len_actuel--;
		free(v_c);
	}
	
	deuxième_passage(g,100000);

	int nb_montagnes = n / 40;
	for(int b=0;b<nb_montagnes;b++) {
		montagne_passage(g);
	}

	return g;
}	
	
/////////////
/////////////
/////////////

//Transposition carte réelle --> grille

/*int plage(Pixel p) {

	float red = (float) (p.r + 1);
	float blue = (float) (p.b + 1);
	float green = (float) (p.g + 1);

	float somme = red + blue + green;

	float r = somme / red;
	float g = somme / green;
	float b = somme / blue;

	fprintf(stderr, "somme : %f\n", somme);
	fprintf(stderr, "somme bizzre: %f\n", green);
	fprintf(stderr, "cette chose : %f\n", g);
	fprintf(stderr, "voila : %f\n\n", r);

	if (r <= 2.0) { // rouge
		return FORET;
	}
	else if (g <= 2.0) { // vert
		return PLAINE;
	}
	else if (b <= 2.0) { // bleu
		return EAU;
	}
	else {
		return MONTAGNE;
	}
}*/



/*grille* convertir(Image* I) {
	assert(I->h == I->w);
	grille* g = creer_grille(I->h);
	int n = g->taille;
	for (int i=0;i<n;i++) {
		for (int j=0;j<n;j++) {
			int x = n*j;
			g->t[i][j].type = plage(I->dat[x]);
		}
	}
	return g;
}*/