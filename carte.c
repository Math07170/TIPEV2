#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

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
const int P_RIVIERE = 0;
const int P_MONTAGNE = 16;
const int P_NEANT = 0; //ne pas changer

int egalite(int A,int B,int C,int D,int T) {
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
	int c = egalite(A,B,C,D,T);
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


void tab_init(int max, int* t1, int* t2) {
	for (int i=0;i<max;i++) {
		t1[i] = i;
		t2[i] = i;
	}
}

grille* generation_carte() {
	srand(time(NULL));
	grille* g = creer_grille(100);
	int n = g->taille;
	int max = 100;
	
	int ti[max];
	int tj[max];
	tab_init(max,ti,tj);
	
	//premier passage
	

	//FAIRE UNE LISTE DE COUPLE

	int NB = 200000;

	while (NB >= 0) {
		int i = ti[rand() % max];
		int j = tj[rand() % max];
		
		int ter[4];
		ter[0] = EAU;
		ter[1] = EAU;
		ter[2] = EAU;
		ter[3] = EAU;
		
		
		cell* c = getCell(i,j,g);
		cell** tc = voisins(c, g);
		cell* cdq = tc[0];
		for (int k=0;k<4;k++) {
			if (tc[k] == NULL) {
				ter[k] = NEANT;
			}
			else {
				ter[k] = tc[k]->type;
			}
		}
		
		int terrain = random_terrain(ter[0], ter[1], ter[2], ter[3]);
		//printf("terrain : %d\n", terrain);
		change_terrain(terrain,i,j,g);
		
		//ti[i] = ti[max-1];
		//tj[j] = tj[max-1];
		NB--;
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
	
