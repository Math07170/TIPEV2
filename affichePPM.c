#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <ncurses.h>
#include "grille.h"
#include "fileprio.h"

int TAILLE_TUILE = 2;

struct s_pixel{
	int rouge;
	int vert;
	int bleu;
};
typedef struct s_pixel pixel;
//typedef pixel* tuile;	// De taille 16*16 !!!
struct s_image{
	int hauteur;	// Comptée en tuiles de 16*16
	int largeur;	// Idem
	//tuile* tuile;
	pixel* pixels;
};
typedef struct s_image image;

pixel lire_pixel(image* img,int i,int j){
	return img -> pixels[img->largeur*i+j];
}

int rouge(pixel p){
	return p.rouge;
}

int vert(pixel p){
	return p.vert;
}

int bleu(pixel p){
	return p.bleu;
}

void ecritPPM(image* img){
	FILE* fichier = fopen("test.ppm","w+");
	fprintf(fichier,"P3\n%d %d\n255\n",img->largeur*TAILLE_TUILE,img->hauteur*TAILLE_TUILE);
	fprintf(stdout,"P3\n%d %d\n255\n",img->largeur*TAILLE_TUILE,img->hauteur*TAILLE_TUILE);
	for(int i = 0;i < img->hauteur;i++){
		for(int j = 0;j < img->largeur;j++){
			pixel p = lire_pixel(img,i,j);
			fprintf(stdout,"%d %d %d\n",rouge(p),vert(p),bleu(p));
			for(int k = 0;k < TAILLE_TUILE;k++){
				for(int l = 0;l < TAILLE_TUILE;l++){
					fprintf(fichier,"%d %d %d\n",rouge(p),vert(p),bleu(p));
				}
			}
		}
	}
	fclose(fichier);
	return;
}

int main(){
	pixel pxl1;
	pixel pxl2;
	pixel pxl3;
	pixel pxl4;
	pixel pxl5;
	pixel pxl6;
	pixel pxl7;
	pixel pxl8;
	pxl1.rouge = 255;
	pxl1.vert = 0;
	pxl1.bleu = 0;
	
	pxl2.rouge =0;
	pxl2.vert = 255;
	pxl2.bleu = 0;
	
	pxl3.rouge = 0;
	pxl3.vert = 0;
	pxl3.bleu = 255;
	
	pxl4.rouge = 0;
	pxl4.vert = 255;
	pxl4.bleu = 255;
	
	pxl5.rouge =255;
	pxl5.vert = 0;
	pxl5.bleu = 255;
	
	pxl6.rouge = 255;
	pxl6.vert = 255;
	pxl6.bleu = 0;
	
	pxl7.rouge =0;
	pxl7.vert = 0;
	pxl7.bleu = 0;
	
	pxl8.rouge = 255;
	pxl8.vert = 255;
	pxl8.bleu = 255;
	image img;
	img.hauteur = 4;
	img.largeur = 2;
	img.pixels = malloc(8*sizeof(pixel));
	img.pixels[0] = pxl1;
	img.pixels[1] = pxl2;
	img.pixels[2] = pxl3;
	img.pixels[3] = pxl4;
	img.pixels[4] = pxl5;
	img.pixels[5] = pxl6;
	img.pixels[6] = pxl7;
	img.pixels[7] = pxl8;
	ecritPPM(&img);
	// free !!!
	return 0;
}

/*void affichePPM(grille* g){
	int n = g->taille;
	erase();
	
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			dessine_case(g->t[i][j],g);
		}
	}
	refresh();
	return;
}*/
