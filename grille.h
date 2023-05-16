#ifndef GRILLE_H
#define GRILLE_H
#include <stdbool.h>

extern const int VIDE;
extern const int USINE;
extern const int GD_VILLE;
extern const int PT_VILLE;
extern const int VILLAGE;
extern const int CENTRALE;
extern const int GD_TRANSFO;
extern const int PT_TRANSFO;

extern const int NEANT;
extern const int PLAINE;
extern const int FORET;
extern const int EAU;
extern const int RIVIERE;
extern const int MONTAGNE;

struct s_cable{
	int id; 	// Identifiant de la ligne
	float u;	// Permet de donner le type de câble
	float i;
	float r;
};
typedef struct s_cable cable;

struct s_cell{ 
	int nb_c;
    cable* c;
    int type;
    int infra;
    int x;
    int y;
};
typedef struct s_cell cell;

struct s_grille{
    int taille;
    int nb_l;
    int nb_infra;
    cell** t;
    cell** infra;
};
typedef struct s_grille grille;

grille* creer_grille(int n);
void detruire_grille(grille* g);
void randomize_infra(int element, int nb_element, grille* g);
void randomize_terrain(grille* g);
//void affiche_moche(grille* g);
bool est_terrain(int terrain, int i, int j, grille* g);
int heuristique(int xa, int ya, int xb, int yb);
cell* getCell(int x, int y, grille* g);
bool contient_ligne(cell* c, int id_cable);
cell** voisins(cell* c, grille* g);
void change_terrain(int terrain, int i, int j, grille* g);
void terrain_infra_test8(grille* g);
int* astar(grille* g, cell* depart, cell* final, int id);
void situation_initiale(grille* g);
void relie(grille* g);
void situation_initiale_pop(grille** gl, int n);
grille* copie_grille(grille* g);
void random_transfo(grille* g);
#endif
