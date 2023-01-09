#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <ncurses.h>
#include "grille.h"
#include "fileprio.h"


/* Initialise ncurses */
void init_ncurses() {
	initscr();
	noecho();
	nodelay(stdscr, TRUE);
	cbreak();
	curs_set(0);
	start_color();
	init_pair(VIDE,COLOR_WHITE,COLOR_BLACK);
	init_pair(USINE,COLOR_CYAN,COLOR_BLACK);
	init_pair(MAISON,COLOR_RED,COLOR_BLACK);
	init_pair(BARNABE,COLOR_YELLOW,COLOR_BLACK);		// Couleurs allouées au pif, c'est temporaire
}


void dessine_case(int x, int y, int couleur, char c) {
	attron(COLOR_PAIR(couleur));
	mvaddch(x,2*y,c);
	//mvaddch(x,1+2*y,c);		//	C'est plus joli si on l'enlève, mais ça sera dégueu en rajoutant les couleurs de fond...
	attroff(COLOR_PAIR(couleur));
}

void affiche(grille* g) {
	int n = g->taille;
	erase();
	
	for(int i=0;i<n;i++) {
		for(int j=0;j<n;j++) {
			int c = g->t[i][j]; // à terme, c sera un cell et non un int
			if (c == VIDE) { dessine_case(i,j,VIDE,'V'); }
			else if (c == USINE) { dessine_case(i,j,USINE,'U'); }
			else if (c == MAISON) { dessine_case(i,j,MAISON,'M'); }
			else if (c == BARNABE) { dessine_case(i,j,BARNABE,'B'); }
		}
	}
	refresh();
}
