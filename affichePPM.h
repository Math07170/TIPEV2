#ifndef AFFICHEPPM_H
#define AFFICHEPPM_H

// Note : La structure d'image est également utilisée dans carte.c

struct s_pixel{
	int rouge;
	int vert;
	int bleu;
};
typedef struct s_pixel pixel;

struct s_image{
	int largeur;	// i, vers le bas		// Comptée en pixels (pas en tuiles !)
	int hauteur;	// j, vers la droite	// Idem
	pixel* pixels;
};
typedef struct s_image image;

pixel lire_pixel(image* img,int i,int j);
int rouge(pixel p);
int vert(pixel p);
int bleu(pixel p);

void affichePPM(grille* g,bool quadrillage,char* filename);

#endif
 
