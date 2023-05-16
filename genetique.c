#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include "grille.h"
#include "genetique.h"
#include "carte.h"

// Plus le score est faible mieux c'est
int score(individu i, float eco, float env){
    grille* g = copie_grille(i);
    relie(g);
    int res_eco = 0;
    int res_env = 0;
    for(int k = 0; k<g->taille; k++){
        for(int j = 0; j<g->taille; j++){
            res_eco += (g->t[k][j].type)*(g->t[k][j].nb_c);
            //fprintf(stderr, "type : %d, nb_c : %d res_eco %d\n", g->t[k][j].type, g->t[k][j].nb_c, res_eco);
            for(int c = 0; c < g->t[k][j].nb_c; c++){
                res_env += (g->t[k][j].c[c].r)*(g->t[k][j].c[c].i)*(g->t[k][j].c[c].i);
            }
        }
    }
    detruire_grille(g);
    return (res_eco*eco + res_env*env);
}

// croise deux individus en prenant 1/4 de l'un et 3/4 de l'autre
individu croisement(individu g1, individu g2){
    individu res = creer_grille(g1->taille);
    generation_carte(res);
    situation_initiale(res);
    for(int k = 0; k<g1->taille; k++){
        for(int j = 0; j<g1->taille; j++){
            if(k < g1->taille/2 && j < g1->taille/2){
                res->t[k][j].x = g1->t[k][j].x;
                res->t[k][j].y = g1->t[k][j].y;
                res->t[k][j].nb_c = g1->t[k][j].nb_c;
                res->t[k][j].type = g1->t[k][j].type;
                res->t[k][j].infra = g1->t[k][j].infra;
                for(int l = 0;l < g1->t[k][j].nb_c;l++){
                    //fprintf(stderr,"6");
                    res->t[k][j].c[l].u = g1->t[k][j].c[l].u;
                    res->t[k][j].c[l].r = g1->t[k][j].c[l].r;
                    res->t[k][j].c[l].i = g1->t[k][j].c[l].i;
                    res->t[k][j].c[l].id = g1->t[k][j].c[l].id;
                }
            }
            else{
                res->t[k][j].x = g2->t[k][j].x;
                res->t[k][j].y = g2->t[k][j].y;
                res->t[k][j].nb_c = g2->t[k][j].nb_c;
                res->t[k][j].type = g2->t[k][j].type;
                res->t[k][j].infra = g2->t[k][j].infra;
                for(int l = 0;l < g2->t[k][j].nb_c;l++){
                    //fprintf(stderr,"6");
                    res->t[k][j].c[l].u = g2->t[k][j].c[l].u;
                    res->t[k][j].c[l].r = g2->t[k][j].c[l].r;
                    res->t[k][j].c[l].i = g2->t[k][j].c[l].i;
                    res->t[k][j].c[l].id = g2->t[k][j].c[l].id;
                }
            }
        }
    }
    return res;
}

// Applique une mutation a un individu 
/// Une mutation prend k cases contenant un transformateur et les déplaces aléatoirement sur la carte
void mutation(individu g){
    int k = rand() % 5;
    int modif = 0;
    for(int l = 0; l < g->nb_infra; l++){
        if (modif >= k){
            break;
        }
        if(g->infra[l]->infra == GD_TRANSFO || g->infra[l]->infra == PT_TRANSFO){
            modif++;
            int x2 = rand() % g->taille;
            int y2 = rand() % g->taille;
            g->infra[l]->x = x2;
            g->infra[l]->y = y2;
            getCell(x2,y2,g)->infra = g->infra[l]->infra;
            g->infra[l]->infra = VIDE;
        }
    }
}


// Renvoie le meilleur individu de la population (celui qui as le score le plus faible)
individu best(population* p, float eco, float env){
    int min = score(p->t[0], eco, env);
    int indice = 0;
    for(int k = 1; k < p->taille; k++){
        int s = score(p->t[k], eco, env);
        if(s < min){
            min = s;
            indice = k;
        }
    }
    return p->t[indice];
}

// Renvoie le pire individu de la population (celui qui as le score le plus élevé)
individu worst(population* p, float eco, float env){
    int max = score(p->t[0], eco, env);
    int indice = 0;
    for(int k = 1; k < p->taille; k++){
        int s = score(p->t[k], eco, env);
        if(s > max){
            max = s;
            indice = k;
        }
    }
    return p->t[indice];
}

// Renvoie la moyenne des scores de la population
float moyenne(population* p, float eco, float env){
    float res = 0;
    for(int k = 0; k < p->taille; k++){
        res += score(p->t[k], eco, env);
    }
    return res/p->taille;
}

// Renvoie la variance des scores de la population
float variance(population* p, float eco, float env){
    float res = 0;
    float m = moyenne(p, eco, env);
    for(int k = 0; k < p->taille; k++){
        res += (score(p->t[k], eco, env) - m)*(score(p->t[k], eco, env) - m);
    }
    return res/p->taille;
}


// Renvoie la médiane des scores de la population
float mediane(population* p, float eco, float env){
    int* t = malloc(sizeof(int)*p->taille);
    for(int k = 0; k < p->taille; k++){
        t[k] = score(p->t[k], eco, env);
    }
    int res = 0;
    for(int k = 0; k < p->taille; k++){
        int min = t[k];
        int indice = k;
        for(int j = k+1; j < p->taille; j++){
            if(t[j] < min){
                min = t[j];
                indice = j;
            }
        }
        int tmp = t[k];
        t[k] = t[indice];
        t[indice] = tmp;
    }
    if(p->taille % 2 == 0){
        res = (t[p->taille/2] + t[p->taille/2 - 1])/2;
    }
    else{
        res = t[p->taille/2];
    }
    free(t);
    return res;
}
// Renvoie la liste des indices des individus de la population triés par score croissant
int* tri_rapide(population* p, float eco, float env){
    int* res = malloc(sizeof(int)*p->taille);
    for(int k = 0; k < p->taille; k++){
        res[k] = k;
    }
    for(int k = 0; k < p->taille; k++){
        if(p->score[res[k]] == -1){
            p->score[res[k]] = score(p->t[res[k]], eco, env);
        }
        int min = p->score[res[k]];
        int indice = k;
        for(int j = k+1; j < p->taille; j++){
            if(p->score[res[j]] == -1){
            p->score[res[j]] = score(p->t[res[j]], eco, env);
            }
            int s = p->score[res[j]];
            if(s < min){
                min = s;
                indice = j;
            }
        }
        int tmp = res[k];
        res[k] = res[indice];
        res[indice] = tmp;
    }
    return res;
}
void free_population(population* p){
    for(int k = 0; k < p->taille; k++){
        detruire_grille(p->t[k]);
    }
    free(p->t);
    free(p->score);
    free(p);
}
// Utilise la fonction tri_rapide pour générer la population suivante en appliquant une selection naturelle et une chance de mutation de 10% par individu
population* next_generation(population* p, float eco, float env){
    fprintf(stderr, "Moyenne : %d\n", score(worst(p, eco, env), eco, env));
    population* res = malloc(sizeof(population));
    res->taille = p->taille;
    res->t = malloc(sizeof(individu)*p->taille);
    res->score = malloc(sizeof(int)*p->taille);
    int* t = tri_rapide(p, eco, env);
    fprintf(stderr, "Tri rapide effectué\n");
    for(int k = 0; k < p->taille/2; k++){
        res->t[k] = p->t[t[k]];
        res->score[k] = p->score[t[k]];
        if(rand() % 100 > 10 ) { 
            res->score[k] = -1;
            mutation(res->t[k]);
        }
        fprintf(stderr, "k = %d res : %d\n", k, score(res->t[k], eco, env));
    }
    for(int k = p->taille/2; k < p->taille; k++){
        //detruire_grille(res->t[k]);
        res->t[k] = croisement(p->t[t[rand() % (p->taille/2)]], p->t[t[rand() % (p->taille/2)]]);
        
        if(rand() % 100 > 10 ) mutation(res->t[k]);
        fprintf(stderr, "k = %d res : %d\n", k, score(res->t[k], eco, env));
        res->score[k] = -1;
    }
    free(t);
    fprintf(stderr, "Moyenne res : %d\n", score(worst(res, eco, env), eco, env));
    return res;
}

population* creer_population(int n){
    population* res = malloc(sizeof(population));
    res->taille = n;
    res->t = malloc(sizeof(individu)*n);
    res->score = malloc(sizeof(int)*n);
    res->score[0] = -1;
    res->t[0] = creer_grille(100);
    generation_carte(res->t[0]);
    situation_initiale(res->t[0]);
    
    for(int k = 1; k < n; k++){
        res->score[k] = -1;
        res->t[k] = copie_grille(res->t[0]);
        random_transfo(res->t[k]);
    }
    random_transfo(res->t[0]);
    return res;
}





