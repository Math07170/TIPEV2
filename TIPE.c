#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include "grille.h"
#include "fileprio.h"
#include "affichage.h"
#include "carte.h"
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

    int n = 200;
    grille* g = creer_grille(n);
    
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
    
    init_ncurses();
    generation_carte(g);
    situation_initiale(g);
    astar(g, getCell(0,0,g), getCell(50,50,g));
    affiche(g);
	/*
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
	sleep(5);		// Hack fumeux TEMPORAIRE pour voir la grille quelques instants
	endwin();		// Arrête proprement ncurses, c'est REQUIS pour ne pas détruire le terminal
	
	detruire_grille(g);
		
    return 0;
}
