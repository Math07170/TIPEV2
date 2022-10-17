#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

const int VIDE = 0;
const int USINE = 1;
const int MAISON = 2;
const int BARNABE = 3;


struct s_grille {
    int taille;
    int** t;
};

typedef struct s_grille grille;


grille creer_grille(int n) {
    grille g;
    g.taille = n;
    g.t = malloc(sizeof(int*) * n);
    for(int k = 0; k < n; k++){
        g.t[k] = malloc(sizeof(int) * n);
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            g.t[i][j] = VIDE;
        }
    }
    return g;
}

void ajoute(int x, int i, int j, grille* g) {
    int n = g->taille;
    assert(i>=0 && j>=0 && i<n && j<n && g->t[i][j] == VIDE);
    g->t[i][j] = x;
}

void supprime(int i, int j, grille* g) {
    int n = g->taille;
    assert(i>=0 && j>=0 && i<n && j<n);
    g->t[i][j] = VIDE;
}

void remplace(int x, int i, int j, grille* g) {
    int n = g->taille;
    assert(i>=0 && j>=0 && i<n && j<n);
    g->t[i][j] = x;
}

bool est(int x, int i, int j, grille* g) {
    int n = g->taille;
    assert(i>=0 && j>=0 && i<n && j<n);
    return (g->t[i][j] == x);
}

void randomize(int element, int nb_element, grille* g) {
    int counter = 0;
    int n = g->taille;
    while (counter < nb_element) {
        int i = rand() % n;
        int j = rand() % n;
        if (est(VIDE, i, j, g)) {
            g->t[i][j] = element;
            counter++;
        }
    }
    return;
}


void affiche(grille* g) {
    int n = g->taille;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            int e = g->t[i][j];
            if (e != VIDE) {
                printf("i : %d, j : %d, élément : %d\n", i,j,e);
            }
        }
    }



}


int heuristique(int xa, int ya, int xb, int yb) {
    int x = xa - xb;
    int y = ya - yb;
    if (x < 0) {
        x = -x;
    }
    if (y < 0) {
        y = -y;
    }
    return x + y;
}


void astar(grille* g) {

    
    
    𝑓 𝑖𝑙𝑒𝑝𝑟𝑖𝑜 ← nouvelle_fileprio(); (c'est une file de priorité min)
𝑝 ← nouveau_tableau(𝑛, −1);
𝑐 ← nouveau_tableau(𝑛, 𝐵𝑙𝑎𝑛𝑐);
Lycée Claude Fauriel Informatique MPI(*)
7 / 8
𝑔 ← nouveau_tableau(𝑛, +∞);
𝑓 ← nouveau_tableau(𝑛, +∞);
𝑐[𝑥𝑑 ] ← 𝐺𝑟𝑖𝑠;
𝑔[𝑥𝑑 ] ← 0;
𝑓 [𝑥𝑑 ] ← 𝑔[𝑥𝑑 ] + ℎ(𝑥𝑑 );
inserer((𝑥𝑑 , 𝑓 [𝑥𝑑 ]), 𝑓 𝑖𝑙𝑒𝑝𝑟𝑖𝑜);
Boucle principale :
Tant que non_vide( 𝑓 𝑖𝑙𝑒𝑝𝑟𝑖𝑜) ∧ 𝑐[𝑥𝑡 ] ≠ 𝑁𝑜𝑖𝑟 Faire
𝑥 ← extraire( 𝑓 𝑖𝑙𝑒𝑝𝑟𝑖𝑜);
𝑐[𝑥] ← 𝑁𝑜𝑖𝑟;
𝑣 ← voisins(𝑥);
Pour chaque 𝑦 ∈ 𝑣 Faire
Si 𝑐[𝑦] = 𝐵𝑙𝑎𝑛𝑐 Alors
𝑐[𝑦] ← 𝐺𝑟𝑖𝑠;
𝑝[𝑦] ← 𝑥;
𝑔[𝑦] ← 𝑔[𝑥] + 𝑝(𝑥, 𝑦);
𝑓 [𝑦] ← 𝑔[𝑦] + ℎ(𝑦);
inserer((𝑦, 𝑓 [𝑦]), 𝑓 𝑖𝑙𝑒𝑝𝑟𝑖𝑜);
SinonSi 𝑐[𝑦] = 𝐺𝑟𝑖𝑠 ∧ 𝑔[𝑥] + 𝑝(𝑥, 𝑦) < 𝑔[𝑦] Alors
𝑝[𝑦] ← 𝑥;
𝑔[𝑦] ← 𝑔[𝑥] + 𝑝(𝑥, 𝑦);
𝑓 [𝑦] ← 𝑔[𝑦] + ℎ(𝑦);
diminuer((𝑦, 𝑓 [𝑦]), 𝑓 𝑖𝑙𝑒𝑝𝑟𝑖𝑜);
Fin Si
Fin Pour
Fin Tant que
S
}



int main(){
    srand(time(NULL)); 

    int n = 100;
    grille g = creer_grille(100);
    randomize(USINE, 200, &g);
    randomize(MAISON, 200, &g);
    randomize(BARNABE, 200, &g);
    affiche(&g);
    


    return 0;
}