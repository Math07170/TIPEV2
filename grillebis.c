#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#define N 100
struct s_cable{
    int id;
    float intensite;
    float volage;
};
typedef struct s_cable cable;

struct s_infrastructure{
    int id;
};
typedef struct s_infrastructure;

struct s_case {
    int id;
    cable cables[10];
    int id_terrain
};
typedef struct s_case cases;

typedef cases* grille;
