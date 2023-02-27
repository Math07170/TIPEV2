#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#include "grille.h"
#include "affichage.h"

const int c0 = 1;
const int c1 = 1;
const int c2 = 1;
const int c3 = 1;
const int c4 = 1;

const int P_PLAINE = 1;
const int P_FORET = 1;
const int P_EAU = 1;
const int P_RIVIERE = 1;
const int P_MONTAGNE = 1;
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
	
	srand(time(NULL));
	
	
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
	
	int res = rand() % somme;
	
	if (0 <= res && res < p1) {
		res = Coefplaine;
	} else if (p1 <= res && res < p2) {
		res = Coefforet;
	} else if (p2 <= res && res < p3) {
		res = Coefeau;
	} else if (p3 <= res && res < p4) {
		res = Coefriviere;
	} else if (p4 <= res && res < p5) {
		res = Coefmontagne;
	} else if (p5 <= res && res < somme) {
		res = Coefneant;
	} else {
		exit(1);
	}
	
	return res;
}


void tab_init(int max, int* t1, int* t2) {
	for (int i=0;i<max;i++) {
		t1[i] = i;
		t2[i] = i;
	}
}

grille* generation_carte() {
	grille* g = creer_grille(100);
	int n = g->taille;
	int max = 100;
	
	int ti[max];
	int tj[max];
	tab_init(max,ti,tj);
	
	//premier passage
	
	for (int k1=0;k1<n;k1++) {
		for (int k2=0;k2<n;k2++) {
			int i = ti[rand() % max];
			int j = tj[rand() % max];
			
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
	}
	
	//deuxième passage
	
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
	
	return g;
}	




int main() {
	
	grille* carte = generation_carte();
	affiche(carte);
	return 0;
 }
	
	
	
