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

/* Pour l'instant, n'affiche que la 1ère ligne passant dans chaque case ! */
void dessine_case(cell c,grille* g){
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
	
	if(c.nb_c != 0){		// Affiche UNE ligne, s'il y a existence
		int id_l = c.c[0].id;		// On suppose que la ligne est là-dedans
		cell** v = voisins(&c, g);
		
		if(v[0] != NULL && contient_ligne(v[0],id_l)){
			mvaddch(3*c.x,6*c.y+3,ACS_CKBOARD);
			mvaddch(3*c.x,6*c.y+4,ACS_CKBOARD);
		}
		if(v[1] != NULL && contient_ligne(v[1],id_l)){
			mvaddch(3*c.x+2,6*c.y+3,ACS_CKBOARD);
			mvaddch(3*c.x+2,6*c.y+4,ACS_CKBOARD);
		}
		if(v[2] != NULL && contient_ligne(v[2],id_l)){
			mvaddch(3*c.x+1,6*c.y,ACS_CKBOARD);
			mvaddch(3*c.x+1,6*c.y+1,ACS_CKBOARD);
		}
		if(v[3] != NULL && contient_ligne(v[3],id_l)){
			mvaddch(3*c.x+1,6*c.y+4,ACS_CKBOARD);
			mvaddch(3*c.x+1,6*c.y+5,ACS_CKBOARD);
		}
	}
	
	return;
}

void affiche(grille* g){
	int n = g->taille;
	erase();
	
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			dessine_case(g->t[i][j],g);
		}
	}
	refresh();
	return;
}

