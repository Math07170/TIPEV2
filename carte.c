#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "grille.h"
#include "affichage.h"
#include "carte.h"

const int c0 = 1;
const int c1 = 10;
const int c2 = 20;
const int c3 = 50;
const int c4 = 100;

const int P_PLAINE = 20;
const int P_FORET = 20;
const int P_EAU = 0;
const int P_RIVIERE = 0;	// À enlever
const int P_MONTAGNE = 16;
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

int coef(int A,int B,int C,int D,int T) {
	int c = egalites(A,B,C,D,T);
	if (c==0) {
		return c0;
	}
	else if (c==1) {
		return c1;
	}
	else if (c==2) {
		return c2;
	}
	else if (c==3) {
		return c3;
	}
	else {
		return c4;
	}
}
	


int random_terrain(int A,int B,int C,int D) {
	
	int Coefplaine = P_PLAINE*coef(A,B,C,D,PLAINE);
	int Coefforet = P_FORET*coef(A,B,C,D,FORET);
	int Coefeau = P_EAU*coef(A,B,C,D,EAU);
	int Coefriviere = P_RIVIERE*coef(A,B,C,D,RIVIERE);
	int Coefmontagne = P_MONTAGNE*coef(A,B,C,D,MONTAGNE);
	int Coefneant = P_NEANT*coef(A,B,C,D,NEANT);
	
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
	printf("20 : %d\n\n", a);
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

		printf("ran : %d\nlen : %d\nx : %d\ni : %d\nj : %d\n\n", r,len_actuel,x,i,j);

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
	}
	
	//deuxième passage
	/*
	int NB = 100;
	
	for (int k1=0;k1<NB;k1++) {
		int i = rand() % n;
		int j = rand() % n;
		
		int ter[4];
		
		cell* c = getCell(i,j,g);
		cell** tc = voisins(c, g);
		for (int k=0;k<4;k++) {
			if (tc[0] == NULL) {
				ter[i] = NEANT;
			}
			else {
				ter[i] = tc[0]->type;
			}		
		}
		int terrain = random_terrain(ter[1], ter[2], ter[3], ter[4]);
		change_terrain(terrain,i,j,g);
		
		ti[i] = ti[max];
		tj[j] = tj[max];
		max--;
	}	
	*/
	return g;
}	
	
