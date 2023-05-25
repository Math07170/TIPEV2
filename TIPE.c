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



// ----------------------------------------------------- //







// ----------------------------------------------------- //


// TODO :

// chercher un element
// plus proche element d'un sommet x
// Relier deux voisins avec un certain element (ex : câbles)

// Trouver ou placer les éléments (ex : transformateurs)
// Fonction d'optimisation (càd ?)




int main(){
    srand(time(NULL)); 
    /*population* pop = creer_population(100);
    fprintf(stderr, "Population créée\n");
    int n = 100;

    for(int x = 0; x< 3; x++){
        pop = next_generation(pop, 0.5, 0.5);
        
        fprintf(stderr, "Generation %d, Score moyen : %f, taille mémoire : %ld \n", x, moyenne(pop, 0.5, 0.5), sizeof(grille));
    }*/

    population_v2* pop = creer_population_v2(200);
    int gen = 50;
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
    /*randomize_terrain(&g);
    randomize_infra(USINE, 4, &g);
    randomize_infra(GD_VILLE, 3, &g);
    randomize_infra(PT_VILLE, 5, &g);
    randomize_infra(VILLAGE, 15, &g);
    randomize_infra(CENTRALE, 1, &g);
    randomize_infra(GD_TRANSFO, 5, &g);
    randomize_infra(PT_TRANSFO, 12, &g);*/	// NE PAS SUPPRIMER
    
    //////terrain_infra_test8(g);		// TEST, penser à effacer les preuves
    //////affiche_moche(&g);
    affichePPM(g,true,"grille.ppm");	// Le booléen correspond à la présence du quadrillage
    //init_ncurses();	// REQUIS avec ncurses
    //astar(g, getCell(0,0,g), getCell(50,50,g));
    //affiche(g);		// Ancienne implémentation avec ncurses
	/*
	 * 
	// TESTS FILEPRIO
    fileprio f = creer_fileprio(n*n);
    inserer_fileprio(&f, 5, 5);
    // print_tableau(f);
    inserer_fileprio(&f, 1, 1);
    // print_tableau(f);
    inserer_fileprio(&f, 4, 4);
    // print_tableau(f);
    inserer_fileprio(&f, 3, 3);
    // print_tableau(f);
    inserer_fileprio(&f, 2, 2);
    // print_tableau(f);
    fprintf(stderr, "%d \n", extraire_fileprio(&f));
    fprintf(stderr, "%d \n", extraire_fileprio(&f));
    fprintf(stderr, "%d \n", extraire_fileprio(&f));
    inserer_fileprio(&f, 0, 0);
    inserer_fileprio(&f, 2, 2);
    diminuer_fileprio(&f,5,-1);
    diminuer_fileprio(&f,4,0);
    diminuer_fileprio(&f,2,-2);
    while(fileprio_non_vide(&f)){
        fprintf(stderr, "%d \n", extraire_fileprio(&f));
        // print_tableau(f);
        //extraire_fileprio(&f);
    }
    // ORDRE ATTENDU (valeurs) : 1 2 3 -2 -1 0 0 4 5
    // ORDRE ATTENDU (sommets) : 1 2 3 2 5 0<->4
    detruire_fileprio(&f);
    // TEST OK
	*/
	
	//sleep(60);		// Bien pratique avec ncurses
	//endwin();			// REQUIS avec ncurses pour ne pas détruire le terminal
	
	detruire_grille(g);
		
    return 0;
}
