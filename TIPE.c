#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include "grille.h"
#include "fileprio.h"
#include "affichage.h"
#include "affichePPM.h"
#include "carte.h"
#include "genetique.h"
#include <ncurses.h>
#include <unistd.h>

int main(){
    srand(time(NULL));
    
    // ANCIEN ALGORITHME
	/*grille* g = generation_carte(100);
    situation_initiale(g);
    relie(g);*/
    
    // PPREMIER ALGORITHME GÉNÉTIQUE
    /*population* pop = creer_population(100);
    fprintf(stderr, "Population créée\n");
    int n = 100;

    for(int x = 0; x< 3; x++){
        pop = next_generation(pop, 0.5, 0.5);
        
        fprintf(stderr, "Generation %d, Score moyen : %f, taille mémoire : %ld \n", x, moyenne(pop, 0.5, 0.5), sizeof(grille));
    }*/
	
	// ALGORITHME GÉNÉTIQUE V2
    grille* gr = generation_carte(100);
    //grille* gr = recuperer_image();
    population_v2* pop = creer_population_v2(20, gr);
    int gen = 20;
    double* score = malloc(gen*sizeof(double));

    
    fprintf(stderr, "Population créée\n");
    for(int x =0; x < gen; x++){
        pop = next_generation_v2(pop);
        score[x] = moyenne_v2(pop);
        fprintf(stderr,"Genération %d : Moyenne %lf \n", x, score[x]);
    }
    grille* g = best_v2(pop, 0.5, 0.5);
    free_population_v2(pop);
    export_csv_file(score, gen);
    free(score);
    
    // ANCIEN AFFICHAGE
    /*init_ncurses();	// REQUIS avec ncurses
    affiche(g);
    sleep(30);
    endwin();*/
	
    affichePPM(g,true,"grille.ppm");
    
	detruire_grille(g);
	
    return 0;
}
