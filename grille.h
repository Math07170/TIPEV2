#ifndef GRILLE_H
#define GRILLE_H

extern const int VIDE;
extern const int USINE;
extern const int GD_VILLE;
extern const int PT_VILLE;
extern const int VILLAGE;
extern const int CENTRALE;
extern const int GD_TRANSFO;
extern const int PT_TRANSFO;


extern const int PLAINE;
extern const int FORET;
extern const int EAU;
extern const int RIVIERE;
extern const int MONTAGNE;

struct s_cable{
	int id;
	float u;
	float i;
	float r;
};
typedef struct s_cable cable;

struct s_cell{ 
	int nb_c;
    cable* c;
    int type;
    int infra;
};
typedef struct s_cell cell;

struct s_grille{
    int taille;
    cell** t;
};
typedef struct s_grille grille;

grille creer_grille(int n);
void randomize_infra(int element, int nb_element, grille* g);
void randomize_terrain(grille* g);
//void affiche_moche(grille* g);
int heuristique(int xa, int ya, int xb, int yb);
#endif
