#ifndef GENETIQUE_H
#define GENETIQUE_H

typedef grille* individu;

struct s_population {
    int* score;
    int taille;
    individu* t;
};
typedef struct s_population population;

struct s_infra {
    int type;
    int x;
    int y;
};
typedef struct s_infra infra;

struct s_individu_v2 {
    infra* t;
    int taille;
};
typedef struct s_individu_v2 individu_v2;

struct s_population_v2 {
    grille* g;
    int* score;
    int taille;
    individu_v2* t;
};
typedef struct s_population_v2 population_v2;

int score(individu i, float eco, float env);
float moyenne(population* p, float eco, float env);
void mutation(individu i);
individu croisement(individu i1, individu i2);
int* tri_rapide(population* p, float eco, float env);
population* next_generation(population* p, float eco, float env);
population* creer_population(int n);
void free_population(population* p);
individu best(population* p, float eco, float env);
population_v2* creer_population_v2(int n);
population_v2* next_generation_v2(population_v2* pop);
float moyenne_v2(population_v2* p);
void free_population_v2(population_v2* p);
grille* best_v2(population_v2* p, float eco, float env);
#endif // GENETIQUE_H