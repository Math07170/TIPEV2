#ifndef GRILLE_H
#define GRILLE_H

extern const int VIDE;
extern const int USINE;
extern const int MAISON;
extern const int BARNABE;

extern const int PLAINE;
extern const int FORET;
extern const int EAU;
extern const int MONTAGNE;

struct s_cell{ // Pas utilisé actuellement, servira à comme type de t dans la structure s_grille
    int infrastructure;
    int type;
};
typedef struct s_cell cell;

struct s_grille{
    int taille;
    int** t;
};
typedef struct s_grille grille;

grille creer_grille(int n);
void randomize(int element, int nb_element, grille* g);
//void affiche_moche(grille* g);
int heuristique(int xa, int ya, int xb, int yb);
#endif
