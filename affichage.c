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
	
	for(int i = 0;i < 8;i++){
		
		init_pair(VIDE+PLAINE,COLOR_BLACK,COLOR_YELLOW);
		init_pair(VIDE+FORET,COLOR_BLACK,COLOR_GREEN);
		init_pair(VIDE+EAU,COLOR_BLACK,COLOR_BLUE);
		init_pair(VIDE+RIVIERE,COLOR_BLACK,COLOR_CYAN);
		init_pair(VIDE+MONTAGNE,COLOR_BLACK,COLOR_WHITE);
		
		init_pair(USINE+PLAINE,COLOR_BLACK,COLOR_YELLOW);
		init_pair(USINE+FORET,COLOR_BLACK,COLOR_GREEN);
		init_pair(USINE+EAU,COLOR_BLACK,COLOR_BLUE);
		init_pair(USINE+RIVIERE,COLOR_BLACK,COLOR_CYAN);
		init_pair(USINE+MONTAGNE,COLOR_BLACK,COLOR_WHITE);
		
		init_pair(GD_VILLE+PLAINE,COLOR_BLACK,COLOR_YELLOW);
		init_pair(GD_VILLE+FORET,COLOR_BLACK,COLOR_GREEN);
		init_pair(GD_VILLE+EAU,COLOR_BLACK,COLOR_BLUE);
		init_pair(GD_VILLE+RIVIERE,COLOR_BLACK,COLOR_CYAN);
		init_pair(GD_VILLE+MONTAGNE,COLOR_BLACK,COLOR_WHITE);
		
		init_pair(PT_VILLE+PLAINE,COLOR_BLACK,COLOR_YELLOW);
		init_pair(PT_VILLE+FORET,COLOR_BLACK,COLOR_GREEN);
		init_pair(PT_VILLE+EAU,COLOR_BLACK,COLOR_BLUE);
		init_pair(PT_VILLE+RIVIERE,COLOR_BLACK,COLOR_CYAN);
		init_pair(PT_VILLE+MONTAGNE,COLOR_BLACK,COLOR_WHITE);
		
		init_pair(VILLAGE+PLAINE,COLOR_BLACK,COLOR_YELLOW);
		init_pair(VILLAGE+FORET,COLOR_BLACK,COLOR_GREEN);
		init_pair(VILLAGE+EAU,COLOR_BLACK,COLOR_BLUE);
		init_pair(VILLAGE+RIVIERE,COLOR_BLACK,COLOR_CYAN);
		init_pair(VILLAGE+MONTAGNE,COLOR_BLACK,COLOR_WHITE);
		
		init_pair(CENTRALE+PLAINE,COLOR_RED,COLOR_YELLOW);
		init_pair(CENTRALE+FORET,COLOR_RED,COLOR_GREEN);
		init_pair(CENTRALE+EAU,COLOR_RED,COLOR_BLUE);
		init_pair(CENTRALE+RIVIERE,COLOR_RED,COLOR_CYAN);
		init_pair(CENTRALE+MONTAGNE,COLOR_RED,COLOR_WHITE);
		
		init_pair(GD_TRANSFO+PLAINE,COLOR_RED,COLOR_YELLOW);
		init_pair(GD_TRANSFO+FORET,COLOR_RED,COLOR_GREEN);
		init_pair(GD_TRANSFO+EAU,COLOR_RED,COLOR_BLUE);
		init_pair(GD_TRANSFO+RIVIERE,COLOR_RED,COLOR_CYAN);
		init_pair(GD_TRANSFO+MONTAGNE,COLOR_RED,COLOR_WHITE);
		
		init_pair(PT_TRANSFO+PLAINE,COLOR_RED,COLOR_YELLOW);
		init_pair(PT_TRANSFO+FORET,COLOR_RED,COLOR_GREEN);
		init_pair(PT_TRANSFO+EAU,COLOR_RED,COLOR_BLUE);
		init_pair(PT_TRANSFO+RIVIERE,COLOR_RED,COLOR_CYAN);
		init_pair(PT_TRANSFO+MONTAGNE,COLOR_RED,COLOR_WHITE);
		
		//init_pair(USINE,COLOR_CYAN,COLOR_BLACK);
		//init_pair(MAISON,COLOR_RED,COLOR_BLACK);
		//init_pair(BARNABE,COLOR_YELLOW,COLOR_BLACK);		// Couleurs allouées au pif, c'est temporaire
	}
	return;
}


void dessine_case(int x, int y, int couleur, char c) {
	attron(COLOR_PAIR(couleur));
	for(int i = 0;i < 3;i++){
		for(int j = 0;j < 6;j++){
			mvaddch(3*x+i,6*y+j,' ');
		}
	}
	mvaddch(3*x+1,6*y+3,c);
	attroff(COLOR_PAIR(couleur));
}

void affiche(grille* g) {
	int n = g->taille;
	erase();
	
	for(int i=0;i<n;i++) {
		for(int j=0;j<n;j++) {
			int infra = g->t[i][j].infra;
			int type = g->t[i][j].type;
			
			if (infra == VIDE) dessine_case(i,j,VIDE+type,' ');
			else if (infra == USINE) dessine_case(i,j,USINE+type,'U');
			else if (infra == GD_VILLE) dessine_case(i,j,USINE+type,'G');
			else if (infra == PT_VILLE) dessine_case(i,j,USINE+type,'P');
			else if (infra == VILLAGE) dessine_case(i,j,USINE+type,'V');
			else if (infra == CENTRALE) dessine_case(i,j,USINE+type,'C');
			else if (infra == GD_TRANSFO) dessine_case(i,j,USINE+type,'T');
			else if (infra == PT_TRANSFO) dessine_case(i,j,USINE+type,'t');									
		}
	}
	refresh();
	return;
}
