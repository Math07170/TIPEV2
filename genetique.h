#ifndef GENETIQUE_H
#define GENETIQUE_H

typedef grille* individu;

struct s_population {
    int* score;
    int taille;
    individu* t;
};
typedef struct s_population population;
int score(individu i, float eco, float env);
float moyenne(population* p, float eco, float env);
void mutation(individu i);
individu croisement(individu i1, individu i2);
int* tri_rapide(population* p, float eco, float env);
population* next_generation(population* p, float eco, float env);
population* creer_population(int n);

#endif // GENETIQUE_H