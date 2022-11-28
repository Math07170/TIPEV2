#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// https://graphviz.org/Gallery/undirected/grid.html

int affiche_grille (grille g*) {
	
	int n = g->taille;
	
	FILE *fp; //suprimmer tout avant d'ouvrir le fichier
	if ((fp = fopen ("graphique.dot", "w+")) == NULL) {
		printf("Erreur lors de l'écriture de 'graĥique.dot' !");
		exit(-2);
	}
	fputs("graph grid\n{\n\tfontname=\"Helvetica,Arial,sans-serif\"\n\tnode [fontname=\"Helvetica,Arial,sans-serif\"]\n\tedge [fontname=\"Helvetica,Arial,sans-serif\"]\n\tlayout=dot\n\tlabel=\"grid\"\n\tlabelloc = \"t\"\n\tnode [shape=plaintext]\n\t", fp);
	
	//FPUTS LES LIAISONS
	
	fputs("edge [weight=1000 style=dashed color=dimgrey]", fp);
	
	for(int i=0;i<n;i++) {
		for(int j=0;j<n;j++) {
			fputs("F", fp);
		}
		fputs("\t"
	
	fclose(fp);
	return 0;
}
