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
	
	
	return;
}

void dessine_case(cell c){
	int couleur = c.infra+c.type;
	char ch;
	if(c.infra == VIDE) ch = ' ';
	else if(c.infra == USINE) ch = 'U';
	else if(c.infra == GD_VILLE) ch = 'G';
	else if(c.infra == PT_VILLE) ch = 'P';
	else if(c.infra == VILLAGE) ch = 'V';
	else if(c.infra == CENTRALE) ch = 'C';
	else if(c.infra == GD_TRANSFO) ch = 'T';
	else /*if(c.infra == PT_TRANSFO)*/ ch = 't';
	
	attron(COLOR_PAIR(couleur));
	for(int i = 0;i < 3;i++){
		for(int j = 0;j < 6;j++){
			mvaddch(3*c.x+i,6*c.y+j,' ');
		}
	}
	mvaddch(3*c.x+1,6*c.y+3,ch);
	attroff(COLOR_PAIR(couleur));
	
	
	
	return;
}

void affiche(grille* g){
	int n = g->taille;
	erase();
	
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			dessine_case(g->t[i][j]);
		}
	}
	refresh();
	return;
}

