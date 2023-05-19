#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include "grille.h"

const int TAILLE_TUILE = 16;		// Pas encore utilisé

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

pixel lire_pixel(image* img,int i,int j){
	return img -> pixels[i+img->hauteur*j];
}		// ATTENTION, il faudrait vérifier qu'il s'agit bien de "hauteur" et non de "largeur", ça semble être le cas mais c'est suspect...

void dessine_pixel(image* img,int i,int j,pixel p){
	int indice = i+img->hauteur*j;
	img -> pixels[indice] = p;
	return;
}		// Même remarque que lire_pixel

int rouge(pixel p){
	return p.rouge;
}

int vert(pixel p){
	return p.vert;
}

int bleu(pixel p){
	return p.bleu;
}

/* Prend en argument une image et en fait un fichier PPM */
/* TODO : donner en argument le nom du fichier... */
void ecritPPM(image* img,char* nom_fichier){
	FILE* fichier = fopen(nom_fichier,"w+");
	fprintf(fichier,"P3\n%d %d\n255\n",img->largeur,img->hauteur);
	//fprintf(stdout,"P3\n%d %d\n255\n",img->largeur,img->hauteur);
	for(int i = 0;i < img->largeur;i++){	// Pixels, vers la droite
		for(int j = 0;j < img->hauteur;j++){	// Pixels, vers le bas
			pixel p = lire_pixel(img,i,j);
			fprintf(fichier,"%d %d %d\n",rouge(p),vert(p),bleu(p));
			//fprintf(stdout,"%d %d %d\n",rouge(p),vert(p),bleu(p));
		}
	}
	fclose(fichier);
	return;
}

/*void test1(){
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
	free(img.pixels);
	return;
}

void test2(){
	pixel pxl1;
	pixel pxl2;
	pixel pxl3;
	pixel pxl4;
	pxl1.rouge = 0;
	pxl1.vert = 0;
	pxl1.bleu = 0;
	pxl2.rouge =255;
	pxl2.vert = 255;
	pxl2.bleu = 255;
	pxl3.rouge = 0;
	pxl3.vert = 0;
	pxl3.bleu = 0;
	pxl4.rouge = 0;
	pxl4.vert = 0;
	pxl4.bleu = 0;
	
	image img;
	img.hauteur = 2;
	img.largeur = 2;
	img.pixels = malloc(2*2*sizeof(pixel));
	img.pixels[0] = pxl1;
	img.pixels[1] = pxl2;
	img.pixels[2] = pxl3;
	img.pixels[3] = pxl4;
	
	ecritPPM(&img);
	free(img.pixels);
	return;
}*/

/* Dessine une (seule !) ligne électrique sur une tuile
 * Peut nécessiter jusqu'à 4 appels pour une seule cellule, avec des indice_ligne différents */
void dessine_ligne(cell* c,image* img,grille* g,int indice_ligne){
	pixel ligne;		// Pixel servant à dessiner les lignes électriques
	ligne.rouge = 128;
	ligne.vert = 128;
	ligne.bleu = 128;
	int id_l = c->c[indice_ligne].id;		// On suppose que la ligne est là-dedans, À AMÉLIORER
	cell** v = voisins(c, g);
	if(v[0] != NULL && contient_ligne(v[0],id_l)){		// Affiche en haut
		for(int i = 0;i < TAILLE_TUILE * 2 / 5;i++){
			for(int j = TAILLE_TUILE * 2 / 5;j < TAILLE_TUILE - (TAILLE_TUILE * 2 / 5);j++){
				dessine_pixel(img,(c->x)*TAILLE_TUILE+i,(c->y)*TAILLE_TUILE+j,ligne);
			}
		}
	}
	if(v[1] != NULL && contient_ligne(v[1],id_l)){		// Affiche en bas
		//mvaddch(3*c.x+2,6*c.y+2,ACS_CKBOARD);		// Copier-coller, pas d'inquiétude à avoir
		//mvaddch(3*c.x+2,6*c.y+3,ACS_CKBOARD);
	}
	if(v[2] != NULL && contient_ligne(v[2],id_l)){		// Affiche à gauche
		//mvaddch(3*c.x+1,6*c.y,ACS_CKBOARD);
		//mvaddch(3*c.x+1,6*c.y+1,ACS_CKBOARD);
	}
	if(v[3] != NULL && contient_ligne(v[3],id_l)){		// Affiche à droite
		//mvaddch(3*c.x+1,6*c.y+4,ACS_CKBOARD);
		//mvaddch(3*c.x+1,6*c.y+5,ACS_CKBOARD);
	}
	free(v);		// Requis, voir fonction voisins
	return;
}

/* Modifie l'image img pour qu'elle prenne en compte la cellule c */
void dessine_cell(cell* c,image* img,grille* g,bool quadrillage){		// Pour l'instant, seul le type de terrain en pris en compte...
	pixel terrain;		// Couleur associée au type de terrain
	pixel bordure;		// Bordure noire entre les tuiles
	bordure.rouge = 0;
	bordure.vert = 0;
	bordure.bleu = 0;
	switch(c -> type){		// Note : couleurs choisies vraiment à l'arrache
		case 10:	// PLAINE	// Si on met "PLAINE", on obtient "l'étiquette du case ne se réduit pas en une constante entière", malgré l'inclusion de grille.h
			terrain.rouge = 143;
			terrain.vert = 206;
			terrain.bleu = 0;
			break;
		case 20:	// FORET
			terrain.rouge = 38;
			terrain.vert = 72;
			terrain.bleu = 20;
			break;
		case 30:	// EAU
			terrain.rouge = 41;
			terrain.vert = 134;
			terrain.bleu = 204;
			break;
		case 50:	// MONTAGNE
			terrain.rouge = 189;
			terrain.vert = 170;
			terrain.bleu = 145;
			break;
		default:		// Ne doit pas arriver, s'il y a du rose, ce n'est pas normal !!! (y compris RIVIERE)
			terrain.rouge = 255;
			terrain.vert = 96;
			terrain.bleu = 240;
	}
	for(int i = 0;i < TAILLE_TUILE;i++){
		for(int j = 0;j < TAILLE_TUILE;j++){
			dessine_pixel(img,(c->x)*TAILLE_TUILE+i,(c->y)*TAILLE_TUILE+j,terrain);
		}
	}
	if(quadrillage) for(int k = 0;k < TAILLE_TUILE;k++){		// Sépare les cases par du noir, si demandé
		dessine_pixel(img,(c->x)*TAILLE_TUILE+(TAILLE_TUILE-1),(c->y)*TAILLE_TUILE+k,bordure);
		dessine_pixel(img,(c->x)*TAILLE_TUILE+k,(c->y)*TAILLE_TUILE+(TAILLE_TUILE-1),bordure);
	}
	switch(c->nb_c){
		case 1:
			dessine_ligne(c,img,g,0);
			break;
		case 2:
			dessine_ligne(c,img,g,0);
			dessine_ligne(c,img,g,1);
			break;
		case 3:
			dessine_ligne(c,img,g,0);
			dessine_ligne(c,img,g,1);
			dessine_ligne(c,img,g,2);
			break;
		case 4:
			dessine_ligne(c,img,g,0);
			dessine_ligne(c,img,g,1);
			dessine_ligne(c,img,g,2);
			dessine_ligne(c,img,g,3);
			break;
		default:		// 0, ou erreur grossière
			{}			// Pas de ligne à dessinner, RAS
		}
	return;
}

/* Crée l'image représentant une grille, puis en fait un fichier PPM, avec ou sans le quadrillage */
void affichePPM(grille* g,bool quadrillage,char* filename){
	int nt = g->taille;			// Côté de l'image, en tuiles
	int np = nt * TAILLE_TUILE;	// Côté de l'image, en pixels
	image img;
	img.hauteur = np;
	img.largeur = np;	// Grille carrée => image carrée
	img.pixels = malloc(np*np*sizeof(pixel));
	for(int i = 0;i < nt;i++){
		for(int j = 0;j < nt;j++){
			dessine_cell(getCell(i,j,g),&img,g,quadrillage);
		}
	}
	ecritPPM(&img,filename);
	free(img.pixels);
	return;
}
