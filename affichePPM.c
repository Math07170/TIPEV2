#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include "grille.h"
#include "affichePPM.h"

const int TAILLE_TUILE = 25;
/* Si cette valeur est autre chose que 25, l'affichage des "lettres" des infrastructures ne sera pas centré (mais restera grosso modo dans la tuile)
 * Rien d'autre n'est hardcodé */
 
pixel lire_pixel(image* img,int i,int j){
	return img -> pixels[i+img->hauteur*j];
}

void dessine_pixel(image* img,int i,int j,pixel p){
	int indice = i+img->hauteur*j;
	img -> pixels[indice] = p;
	return;
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

/* Prend en argument une image et en fait un fichier PPM */
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

/* Renvoie la couleur d'une ligne en fonction de sa tension (3 possibilités) */
pixel couleur_ligne(cell* c,int id_l){
	pixel res;
	int tension = tension_ligne(c,id_l);
	if(tension == 400000){		// Haute tension
		res.rouge = 64;
		res.vert = 64;
		res.bleu = 64;
	}else if(tension == 30000){		// Moyenne tension
		res.rouge = 128;
		res.vert = 128;
		res.bleu = 128;
	}else{		// 230		// Basse tension
		res.rouge = 192;
		res.vert = 192;
		res.bleu = 192;
	}
	return res;
}

/* Dessine toutes les lignes électriques sur une tuile */
void dessine_lignes(cell* c,image* img,grille* g){
	cell** v = voisins(c, g);
	// Indices des coupures entre les zones d'affichage des différentes lignes
	int indice_0 = TAILLE_TUILE * 8 / 25;
	int indice_1 = TAILLE_TUILE * 11 / 25;
	int indice_2 = TAILLE_TUILE * 14 / 25;
	int indice_3 = TAILLE_TUILE * 17 / 25;
	
	for(int id_l = 0;id_l < g->nb_l;id_l++){
		int tension = tension_ligne(c,id_l);
		pixel ligne;
		int indice_min;
		int indice_max;
		if(tension == 400000){		// Haute tension
			ligne.rouge = 64;
			ligne.vert = 64;
			ligne.bleu = 64;
			indice_min = indice_0;
			indice_max = indice_1;
		}else if(tension == 30000){		// Moyenne tension
			ligne.rouge = 128;
			ligne.vert = 128;
			ligne.bleu = 128;
			indice_min = indice_1;
			indice_max = indice_2;
		}else if(tension == 230){		// Basse tension
			ligne.rouge = 192;
			ligne.vert = 192;
			ligne.bleu = 192;
			indice_min = indice_2;
			indice_max = indice_3;
		}else{		// NE DEVRAIT JAMAIS S'AFFICHER
			ligne.rouge = 255;
			ligne.vert = 96;
			ligne.bleu = 240;
			indice_min = indice_0;
			indice_max = indice_3;
		}
		if(v[0] != NULL && contient_ligne(v[0],id_l) && contient_ligne(c,id_l)){		// Affiche en haut
			for(int i = 0;i < (TAILLE_TUILE * 2 / 5) - 2;i++){
				for(int j = indice_min;j < indice_max;j++){
					dessine_pixel(img,(c->x)*TAILLE_TUILE+i,(c->y)*TAILLE_TUILE+j,ligne);
				}
			}
		}
		if(v[1] != NULL && contient_ligne(v[1],id_l) && contient_ligne(c,id_l)){		// Affiche en bas
			for(int i = TAILLE_TUILE - 1;i > TAILLE_TUILE - TAILLE_TUILE * 2 / 5 + 1;i--){
				for(int j = indice_min;j < indice_max;j++){
					dessine_pixel(img,(c->x)*TAILLE_TUILE+i,(c->y)*TAILLE_TUILE+j,ligne);
				}
			}
		}
		if(v[2] != NULL && contient_ligne(v[2],id_l)  && contient_ligne(c,id_l)){		// Affiche à gauche
			for(int j = 0;j < (TAILLE_TUILE * 2 / 5) - 2;j++){
				for(int i = indice_min;i < indice_max;i++){
					dessine_pixel(img,(c->x)*TAILLE_TUILE+i,(c->y)*TAILLE_TUILE+j,ligne);
				}
			}
		}
		if(v[3] != NULL && contient_ligne(v[3],id_l) && contient_ligne(c,id_l)){		// Affiche à droite
			for(int j = TAILLE_TUILE - 1;j > TAILLE_TUILE - TAILLE_TUILE * 2 / 5 + 1;j--){
				for(int i = indice_min;i < indice_max;i++){
					dessine_pixel(img,(c->x)*TAILLE_TUILE+i,(c->y)*TAILLE_TUILE+j,ligne);
				}
			}
		}
	}
	return;
}

/* Ajoute dans img la "lettre" correspondant à l'infrastructure de c */
void dessine_infra_lettre(cell* c,image* img){
	pixel noir;			// Couleur pour les consommateurs
	pixel rouge;		// Couleur pour les transformateurs et centrales
	noir.rouge = 0;
	noir.vert = 0;
	noir.bleu = 0;
	rouge.rouge = 255;
	rouge.vert = 0;
	rouge.bleu = 0;
	// Coordonnées du pixel en haut à gauche du "centre" de la cellule :
	int x0 = (c -> x)*TAILLE_TUILE + TAILLE_TUILE * 2 / 5;
	int y0 = (c -> y)*TAILLE_TUILE + TAILLE_TUILE * 2 / 5;
	// Pour TAILLE_TUILE = 25, le "centre" de la cellule va jusqu'à x0+4 et y0+4
	switch(c -> infra){
		case 1:		// USINE, U noir	// Si on met "USINE", GCC n'est pas content...
			dessine_pixel(img,x0,y0,noir);
			dessine_pixel(img,x0+1,y0,noir);
			dessine_pixel(img,x0+2,y0,noir);
			dessine_pixel(img,x0+3,y0,noir);
			dessine_pixel(img,x0+4,y0,noir);
			dessine_pixel(img,x0+4,y0+1,noir);
			dessine_pixel(img,x0+4,y0+2,noir);
			dessine_pixel(img,x0+4,y0+3,noir);
			dessine_pixel(img,x0+4,y0+4,noir);
			dessine_pixel(img,x0+4,y0+4,noir);
			dessine_pixel(img,x0+3,y0+4,noir);
			dessine_pixel(img,x0+2,y0+4,noir);
			dessine_pixel(img,x0+1,y0+4,noir);
			dessine_pixel(img,x0,y0+4,noir);
			break;
		case 2:		// GD_VILLE, G noir
			dessine_pixel(img,x0,y0+4,noir);
			dessine_pixel(img,x0,y0+3,noir);
			dessine_pixel(img,x0,y0+2,noir);
			dessine_pixel(img,x0,y0+1,noir);
			dessine_pixel(img,x0+1,y0+1,noir);
			dessine_pixel(img,x0+1,y0,noir);
			dessine_pixel(img,x0+2,y0,noir);
			dessine_pixel(img,x0+3,y0,noir);
			dessine_pixel(img,x0+3,y0+1,noir);
			dessine_pixel(img,x0+4,y0+1,noir);
			dessine_pixel(img,x0+4,y0+2,noir);
			dessine_pixel(img,x0+4,y0+3,noir);
			dessine_pixel(img,x0+4,y0+4,noir);
			dessine_pixel(img,x0+3,y0+4,noir);
			dessine_pixel(img,x0+2,y0+4,noir);
			dessine_pixel(img,x0+2,y0+3,noir);
			break;
		case 3:		// PT_VILLE, P noir
			dessine_pixel(img,x0,y0,noir);
			dessine_pixel(img,x0+1,y0,noir);
			dessine_pixel(img,x0+2,y0,noir);
			dessine_pixel(img,x0+3,y0,noir);
			dessine_pixel(img,x0+4,y0,noir);
			dessine_pixel(img,x0,y0+1,noir);
			dessine_pixel(img,x0,y0+2,noir);
			dessine_pixel(img,x0+1,y0+3,noir);
			dessine_pixel(img,x0+2,y0+2,noir);
			dessine_pixel(img,x0+2,y0+1,noir);
			break;
		case 4:		// VILLAGE, V noir
			dessine_pixel(img,x0,y0,noir);
			dessine_pixel(img,x0+1,y0,noir);
			dessine_pixel(img,x0+2,y0,noir);
			dessine_pixel(img,x0+2,y0+1,noir);
			dessine_pixel(img,x0+3,y0+1,noir);
			dessine_pixel(img,x0+4,y0+1,noir);
			dessine_pixel(img,x0+4,y0+2,noir);
			dessine_pixel(img,x0+4,y0+3,noir);
			dessine_pixel(img,x0+3,y0+3,noir);
			dessine_pixel(img,x0+2,y0+3,noir);
			dessine_pixel(img,x0+2,y0+4,noir);
			dessine_pixel(img,x0+1,y0+4,noir);
			dessine_pixel(img,x0,y0+4,noir);
			break;
		case 5:		// CENTRALE, C rouge
			dessine_pixel(img,x0,y0+4,rouge);
			dessine_pixel(img,x0,y0+3,rouge);
			dessine_pixel(img,x0,y0+2,rouge);
			dessine_pixel(img,x0,y0+1,rouge);
			dessine_pixel(img,x0+1,y0+1,rouge);
			dessine_pixel(img,x0+1,y0,rouge);
			dessine_pixel(img,x0+2,y0,rouge);
			dessine_pixel(img,x0+3,y0,rouge);
			dessine_pixel(img,x0+3,y0+1,rouge);
			dessine_pixel(img,x0+4,y0+1,rouge);
			dessine_pixel(img,x0+4,y0+2,rouge);
			dessine_pixel(img,x0+4,y0+3,rouge);
			dessine_pixel(img,x0+4,y0+4,rouge);
			break;
		case 6:		// GD_TRANSFO, T rouge
			dessine_pixel(img,x0,y0,rouge);
			dessine_pixel(img,x0,y0+1,rouge);
			dessine_pixel(img,x0,y0+2,rouge);
			dessine_pixel(img,x0,y0+3,rouge);
			dessine_pixel(img,x0,y0+4,rouge);
			dessine_pixel(img,x0+1,y0+2,rouge);
			dessine_pixel(img,x0+2,y0+2,rouge);
			dessine_pixel(img,x0+3,y0+2,rouge);
			dessine_pixel(img,x0+4,y0+2,rouge);
			break;
		case 7:		// PT_TRANSFO, t rouge
			dessine_pixel(img,x0,y0+2,rouge);
			dessine_pixel(img,x0+1,y0+2,rouge);
			dessine_pixel(img,x0+2,y0+2,rouge);
			dessine_pixel(img,x0+3,y0+2,rouge);
			dessine_pixel(img,x0+4,y0+3,rouge);
			dessine_pixel(img,x0+4,y0+3,rouge);
			dessine_pixel(img,x0+4,y0+4,rouge);
			dessine_pixel(img,x0+1,y0+3,rouge);
			dessine_pixel(img,x0+1,y0+4,rouge);
			dessine_pixel(img,x0+1,y0+1,rouge);
			break;
		default:	// Pas d'infrastructure, rien à faire
			{}
	}
	return;
}

/* Modifie l'image img pour qu'elle prenne en compte la cellule c */
void dessine_cell(cell* c,image* img,grille* g,bool quadrillage){
	pixel terrain;		// Couleur associée au type de terrain
	pixel bordure;		// Bordure noire entre les tuiles
	bordure.rouge = 0;
	bordure.vert = 0;
	bordure.bleu = 0;
	switch(c -> type){		// Note : couleurs choisies vraiment à l'arrache
		case 10:	// PLAINE	// Si on met "PLAINE", on obtient l'erreur "l'étiquette du case ne se réduit pas en une constante entière", malgré l'inclusion de grille.h...
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
	dessine_lignes(c,img,g);
	if(quadrillage) for(int k = 0;k < TAILLE_TUILE;k++){		// Sépare les cases par du noir, si demandé
		dessine_pixel(img,(c->x)*TAILLE_TUILE+(TAILLE_TUILE-1),(c->y)*TAILLE_TUILE+k,bordure);
		dessine_pixel(img,(c->x)*TAILLE_TUILE+k,(c->y)*TAILLE_TUILE+(TAILLE_TUILE-1),bordure);
	}
	dessine_infra_lettre(c,img);
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
