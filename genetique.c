#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include "grille.h"
#include "genetique.h"
#include "carte.h"

int score_v2(grille* g, individu_v2* i, float eco, float env){
    grille* copie = copie_grille(g);
    for(int k = 0; k < i->taille; k++){
        cell* c = getCell(i->t[k].x, i->t[k].y, copie);
        c->infra = i->t[k].type;
        copie->infra[copie->nb_infra] = c;
        copie->nb_infra++;
        //fprintf(stderr, "nb_infra : %d x : %d, y : %d, type : %d\n", copie->nb_infra, i->t[k].x, i->t[k].y, i->t[k].type);
    }
    relieup(copie);
    int res_eco = 0;
    int res_env = 0;
    for(int k = 0; k<copie->taille; k++){
        for(int j = 0; j<copie->taille; j++){
            res_eco += (copie->t[k][j].type)*(copie->t[k][j].nb_c);
            //fprintf(stderr, "type : %d, nb_c : %d res_eco %d\n", g->t[k][j].type, g->t[k][j].nb_c, res_eco);
            for(int c = 0; c < copie->t[k][j].nb_c; c++){
                res_env += (((copie->t[k][j].c[c].r)*(copie->t[k][j].c[c].i)*(copie->t[k][j].c[c].i))*3.65*2.4*6*6)*(2.16)/1000;
            }
        }
    }
    return (res_eco*eco + res_env*env);

}
void free_population_v2(population_v2* p){
    for(int k = 0; k < p->taille; k++){
        free(p->t[k].t);
    }
    free(p->t);
    free(p->score);
    free(p);
}
grille* best_v2(population_v2* p, float eco, float env){
    int min = score_v2(p->g, &(p->t[0]), eco, env);
    int indice = 0;
    for(int k = 1; k < p->taille; k++){
        int s = score_v2(p->g, &(p->t[k]), eco, env);
        if(s < min){
            min = s;
            indice = k;
        }
    }
    individu_v2* i = &(p->t[indice]);
    grille* copie = copie_grille(p->g);
    for(int k = 0; k < i->taille; k++){
        cell* c = getCell(i->t[k].x, i->t[k].y, copie);
        c->infra = i->t[k].type;
        copie->infra[copie->nb_infra] = c;
        copie->nb_infra++;
        //fprintf(stderr, "nb_infra : %d x : %d, y : %d, type : %d\n", copie->nb_infra, i->t[k].x, i->t[k].y, i->t[k].type);
    }
    relieup(copie);
    return copie;

}
float moyenne_v2(population_v2* p){
    int res = 0;
    for(int k = 0; k < p->taille; k++){
        res += (p->score[k] == -1) ? score_v2(p->g, &(p->t[k]), 0.5, 0.5) : p->score[k];
    }
    return res/p->taille;
}
// Renvoie la liste des indices des individus de la population triés par score croissant
int* tri_rapide_v2(population_v2* p, float eco, float env){
    int* res = malloc(sizeof(int)*p->taille);
    for(int k = 0; k < p->taille; k++){
        res[k] = k;
    }
    for(int k = 0; k < p->taille; k++){
        if(p->score[res[k]] == -1){
            p->score[res[k]] = score_v2(p->g, &(p->t[res[k]]), eco, env);
        }
        int min = p->score[res[k]];
        int indice = k;
        for(int j = k+1; j < p->taille; j++){
            if(p->score[res[j]] == -1){
                p->score[res[j]] = score_v2(p->g, &(p->t[res[j]]), eco, env);
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

individu_v2 croisement_v2(individu_v2 i1, individu_v2 i2, int n){
    individu_v2 res;
    res.taille = i1.taille + i2.taille;
    infra* temp = malloc(sizeof(infra)*res.taille);
    int taille_f = 0;
    for(int k = 0; k < i1.taille; k++){
        if(i1.t[k].x < n/2 && i1.t[k].y < n/2){
            temp[taille_f].type = i1.t[k].type;
            temp[taille_f].x = i1.t[k].x;
            temp[taille_f].y = i1.t[k].y;
            taille_f++;
        }
    }
    for(int k = 0; k < i2.taille; k++){
        if(!(i2.t[k].x < n/2 && i2.t[k].y < n/2)){
            temp[taille_f].type = i2.t[k].type;
            temp[taille_f].x = i2.t[k].x;
            temp[taille_f].y = i2.t[k].y;
            taille_f++;
        }
    }
    res.taille = taille_f;
    res.t = malloc(sizeof(infra)*res.taille);
    for(int k = 0; k < res.taille; k++){
        res.t[k].type = temp[k].type;
        res.t[k].x = temp[k].x;
        res.t[k].y = temp[k].y;
    }
    free(temp);
    return res;
}

void mutation_v2(individu_v2* i, int n){
    int k = rand() % i->taille;
    int x2 = rand() % n;
    int y2 = rand() % n;
    i->t[k].x = x2;
    i->t[k].y = y2;
}

population_v2* creer_population_v2(int n){
    population_v2* res = malloc(sizeof(population_v2));
    res->taille = n;
    res->t = malloc(sizeof(individu_v2)*n);
    res->score = malloc(sizeof(int)*n);
    for(int k = 0; k<n; k++){
        res->score[k] = -1;
    }
    res->g = creer_grille(100);
    generation_carte(res->g);
    situation_initiale(res->g);

    for(int k = 0; k < n; k++){
        int NB_petit_transfo = NB_PT_VILLE /5;
        int NB_grand_transfo = (NB_GD_VILLE + NB_USINE + NB_petit_transfo)/5;
        res->t[k].t = malloc(sizeof(infra)*(NB_CENTRALE + NB_grand_transfo + NB_petit_transfo));
        res->t[k].taille = NB_CENTRALE + NB_grand_transfo + NB_petit_transfo;
        int taille = res->g->taille;
        for(int x =0; x<NB_CENTRALE; x++){
            int i = rand()%n;
            int j = rand()%n;
            cell* c = getCell(i,j,res->g);
            if(c->infra == VIDE && c->infra != EAU){
                res->t[k].t[x].x = i;
                res->t[k].t[x].y = j;
                res->t[k].t[x].type = CENTRALE;
            }else{
                x-=1;
            }
            
        }
        
        for(int x =NB_CENTRALE; x<NB_CENTRALE+NB_grand_transfo; x++){
            int i = rand()%n;
            int j = rand()%n;
            cell* c = getCell(i,j,res->g);
            if(c->infra == VIDE && c->infra != EAU){
                res->t[k].t[x].x = i;
                res->t[k].t[x].y = j;
                res->t[k].t[x].type = GD_TRANSFO;
            }else{
                x-=1;
            }
            
        }
        for(int x =NB_CENTRALE+NB_grand_transfo; x<NB_CENTRALE+NB_grand_transfo+NB_petit_transfo; x++){
            int i = rand()%n;
            int j = rand()%n;
            cell* c = getCell(i,j,res->g);
            if(c->infra == VIDE && c->infra != EAU){
                res->t[k].t[x].x = i;
                res->t[k].t[x].y = j;
                res->t[k].t[x].type = PT_TRANSFO;
            }else{
                x-=1;
            }
            
        }
        
    }
    
    return res;

}

population_v2* next_generation_v2(population_v2* pop){
    population_v2* res = malloc(sizeof(population_v2));
    res->taille = pop->taille;
    res->t = malloc(sizeof(individu_v2)*pop->taille);
    res->score = malloc(sizeof(int)*pop->taille);
    res->g = pop->g;
    int* t = tri_rapide_v2(pop, 0.5, 0.5);
    for(int k = 0; k < pop->taille/2; k++){
        res->t[k].t = pop->t[t[k]].t;
        res->t[k].taille = pop->t[t[k]].taille;
        res->score[k] = pop->score[t[k]];
        if(rand() % 100 <= 30 ) { 
            res->score[k] = -1;
            mutation_v2(&(res->t[k]), res->g->taille);
        }
        //fprintf(stderr, "k = %d res : %d\n", k, score(res->t[k], eco, env));
    }
    for(int k = pop->taille/2; k < pop->taille; k++){
        //detruire_grille(res->t[k]);
        int n1 = rand() % (pop->taille/2), n2= rand() % (pop->taille/2);
        int i1 = t[n1], i2 = t[n2];
        individu_v2 test1 = pop->t[i1];
        individu_v2 test2 = pop->t[i2];
        
        individu_v2 temp = pop->t[t[k]];
        individu_v2 crois = croisement_v2(test1, test2, res->g->taille);
        res->t[k].t = crois.t;
        res->t[k].taille = crois.taille;

        free(temp.t);
        if(rand() % 100 <= 30 ) mutation_v2(&(res->t[k]), res->g->taille);
        //fprintf(stderr, "k = %d res : %d\n", k, score(res->t[k], eco, env));
        res->score[k] = score_v2(res->g, &(res->t[k]), 0.5, 0.5);
    }
    free(t);
    free(pop->t);
    free(pop->score);
    free(pop);
    //fprintf(stderr, "Moyenne : %d\n", moyenne_v2(*res));
    return res;
}
// Plus le score est faible mieux c'est
int score(individu i, float eco, float env){
    grille* g = copie_grille((grille*)i);
    relieup(g);
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
    for(int i = 0;i < g1->nb_infra;i++){
        if(g1->infra[i]->x < g1->taille/2 && g1->infra[i]->y < g1->taille/2){
            res->infra[res->nb_infra] = getCell(g1->infra[i]->x,g1->infra[i]->y,res);
            cell* test = res->infra[res->nb_infra];
            res->nb_infra += 1;
        }
    }
    for(int i = 0;i < g2->nb_infra;i++){
        if(g2->infra[i]->x >= g2->taille/2 || g2->infra[i]->y >= g2->taille/2){
            res->infra[res->nb_infra] = getCell(g2->infra[i]->x,g2->infra[i]->y,res);
            cell* test = res->infra[res->nb_infra];
            res->nb_infra += 1;
        }
    }
    return res;
}

// Applique une mutation a un individu 
/// Une mutation prend k cases contenant un transformateur et les déplaces aléatoirement sur la carte
void mutation(individu g){
    int k = rand() % g->nb_infra;
    int modif = 0;
    for(int l = 0; l < g->nb_infra; l++){
        if (modif >= k){
            break;
        }
        if(g->infra[l]->infra == GD_TRANSFO || g->infra[l]->infra == PT_TRANSFO){
            modif++;
            int x2 = rand() % g->taille;
            int y2 = rand() % g->taille;
            getCell(x2,y2,g)->infra = g->infra[l]->infra;
            g->infra[l]->infra = VIDE;
            g->infra[l] = getCell(x2,y2,g);
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
        individu i = p->t[k];
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
        if(p->score[k] == -1){
            res += score(p->t[k], eco, env);
        }else{
            res += p->score[k];
        }
        
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
void free_partial_population(population* p, int* t){
    for(int k = p->taille /2; k < p->taille; k++){
        grille* test = p->t[t[k]];
        detruire_grille(test);
        
    }
    free(p->t);
    free(p->score);
    free(p);
}
void free_population(population* p){
    for(int k = 0; k < p->taille; k++){
        grille* g = p->t[k];
        detruire_grille(g);
        int a = 1;
    }
    free(p->t);
    free(p->score);
    free(p);
}
// Utilise la fonction tri_rapide pour générer la population suivante en appliquant une selection naturelle et une chance de mutation de 10% par individu
population* next_generation(population* p, float eco, float env){
    fprintf(stderr, "Pire : %d\n", score(worst(p, eco, env), eco, env));
    population* res = malloc(sizeof(population));
    res->taille = p->taille;
    res->t = malloc(sizeof(individu)*p->taille);
    res->score = malloc(sizeof(int)*p->taille);
    int* t = tri_rapide(p, eco, env);
    fprintf(stderr, "Tri rapide effectué\n");
    for(int k = 0; k < p->taille/2; k++){
        res->t[k] = p->t[t[k]];
        res->score[k] = p->score[t[k]];
        if(rand() % 100 <= 30 ) { 
            res->score[k] = -1;
            mutation(res->t[k]);
        }
        //fprintf(stderr, "k = %d res : %d\n", k, score(res->t[k], eco, env));
    }
    for(int k = p->taille/2; k < p->taille; k++){
        //detruire_grille(res->t[k]);
        int n1 = rand() % (p->taille/2), n2= rand() % (p->taille/2);
        int i1 = t[n1], i2 = t[n2];
        grille* test1 = p->t[i1];
        grille* test2 = p->t[i2];
        
        grille* temp = p->t[t[k]];
        res->t[k] = croisement(test1, test2);
        detruire_grille(temp);
        if(rand() % 100 <= 30 ) mutation(res->t[k]);
        //fprintf(stderr, "k = %d res : %d\n", k, score(res->t[k], eco, env));
        res->score[k] = score(res->t[k], eco, env);
    }
    free(p->score);
    free(p->t);
    free(p);
    free(t);
    
    fprintf(stderr, "Moyenne res : %f\n", moyenne(res, eco, env));
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
        grille* test2 = res->t[k];
        int a = 1;
    }
    random_transfo(res->t[0]);
    return res;
}





