#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

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
	//int a = t[20];
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
	int res = rand() % m;
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

grille* generation_carte(int n) {
	grille* g = creer_grille(n);

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

	free(t);
	return g;
}	
	
/////////////
/////////////
/////////////

//Transposition carte réelle --> grille

//{Rouge,Vert,Bleu}

struct s_pixel2{
	int rouge;
	int vert;
	int bleu;
};
typedef struct s_pixel2 pixel2;
struct s_image2{
	int hauteur;
	int largeur;
	pixel2* pixels;
};
typedef struct s_image2 image2;

pixel2 lire_pixel2(image2* img,int i,int j){
	return img -> pixels[img->largeur*i+j];
}

int rouge2(pixel2 p){
	return p.rouge;
}

int vert2(pixel2 p){
	return p.vert;
}

int bleu2(pixel2 p){
	return p.bleu;
}

int plage(pixel2 p) {

	int point[3] = {rouge2(p),vert2(p),bleu2(p)};
	int indice = 0;
	int dmin = 100000;

	int COULEUR[8][3] = {{255,0,0},{0,255,0},{0,0,255},{255,255,0},{255,0,255},{0,255,255},{255,255,255},{0,0,0}};

	for(int i=0;i<8;i++) {

		int r = (point[0]-COULEUR[i][0])*(point[0]-COULEUR[i][0]);
		int v 
		= (point[1]-COULEUR[i][1])*(point[1]-COULEUR[i][1]);
		int b = (point[2]-COULEUR[i][2])*(point[2]-COULEUR[i][2]);
		int d = sqrt(r + v + b);

		if(d<dmin) {
			indice = i;
			dmin = d;
		}
	}
	// rouge : 0, vert : 1, bleu : 2, jaune : 3, violet : 4, cyan : 5, blanc : 6, noir : 7

	if ((indice == 3) || (indice == 7) || (indice == 4)) { // jaune, blanc, noir
		return PLAINE;
	}
	else if ((indice == 1)) { // vert
		return FORET;
	}
	else if ((indice == 2) || (indice == 5)) { // bleu, cyan
		return EAU;
	}
	else { // rouge
		return MONTAGNE;
	}
}


grille* convertir(image2* I) {
	assert(I->hauteur == I->largeur);
	grille* g = creer_grille(I->hauteur);
	int n = g->taille;
	for (int i=0;i<n;i++) {
		for (int j=0;j<n;j++) {
			
			pixel2 p = lire_pixel2(I,i,j);
			g->t[i][j].type = plage(p);
		}
	}
	return g;
}


grille* recuperer_image() {
	FILE* fichier = fopen("test2.ppm","r");

	int largeur;
	int taille;

	fscanf(fichier, "P3 %d %d 255", &largeur, &taille);
	assert((largeur == taille) && (largeur > 0) && (largeur <= 1000));

	image2* Image = malloc(sizeof(image2));
	Image->pixels = malloc(taille*taille*sizeof(pixel2));
	Image->hauteur = taille;
	Image->largeur = taille;

	for(int i=0;i<taille;i++) {
		for(int j=0;j<taille;j++) {
			int rouge;
			int vert;
			int bleu;
			fscanf(fichier, "%d %d %d", &rouge, &vert, &bleu);
			Image->pixels[taille*i+j].rouge = rouge;
			Image->pixels[taille*i+j].vert = vert;
			Image->pixels[taille*i+j].bleu = bleu;
		}
	}
	grille* g = convertir(Image);
	free(Image->pixels);
	free(Image);
	//deuxième_passage(g,10000);
	return g;
}