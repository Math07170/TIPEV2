#ifndef CARTE_H
#define CARTE_H
#include "grille.h"
int egalite(int A,int B,int C,int D,int T);

int coef(int A,int B,int C,int D,int T);

int random_terrain(int A,int B,int C,int D);

void tab_init(int taille, int* t);

grille* generation_carte(grille* g);


#endif
