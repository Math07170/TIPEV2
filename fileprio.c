#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

struct s_noeud{
	int num_sommet;
	int valeur;
};
typedef struct s_noeud noeud;

struct s_fileprio{
	int nb_valeurs;
	//int taille_max;
	noeud* t;
};
typedef struct s_fileprio fileprio;




fileprio creer_fileprio(int taille_max){
	fileprio f;
	f.nb_valeurs = 0;
	//f.taille_max = taille_max;
	f.t = malloc(taille_max * sizeof(noeud));
	if(f.t == NULL){
		printf("Manque de mémoire pour créer la fileprio.\n");
		exit(-1);
	}else{
		return f;
	}
}

bool fileprio_non_vide(fileprio* f){
	return (f -> nb_valeurs != 0);
}

void permute(noeud t[],int i,int j){
	noeud tmp = t[i];
	t[i] = t[j];
	t[j] = tmp;
	return;
}

/* Renvoie l'indice de l'éventuel fils gauche du noeud d'indice i */
int i_fg(int i){
	return 2*(i+1);
}

/* Renvoie l'indice de l'éventuel fils droit du noeud d'indice i */
int i_fd(int i){
	return 2*(i+1)+1;
}

void percole_bas(fileprio f,int i);
void percole_bas(fileprio f,int i){
	int ig = i_fg(i);
	int id = i_fd(i);
	//int n = f.nb_valeurs;
	if(ig < f.nb_valeurs){		// 2 fils
		if(f.t[i].valeur > f.t[ig].valeur || f.t[i].valeur > f.t[id].valeur){
			if (f.t[i].valeur > f.t[ig].valeur){
				permute(f.t,i,ig);
				percole_bas(f,ig);
			}else{
				permute(f.t,i,id);
				percole_bas(f,id);
			}
		}		// else -> percolation terminée
	}else if(ig == f.nb_valeurs - 1){		// 1 seul fils, le dernier noeud du tableau
		if(f.t[i].valeur > f.t[ig].valeur){
			permute(f.t,i,ig);
		}
	}
}

/* Renvoie le NUMÉRO DE SOMMET du sommet avec la valeur la plus faible */
int extraire_fileprio(fileprio* f){
	f -> nb_valeurs -= 1;
	permute(f -> t,0,f -> nb_valeurs);
	int res = f -> t[f -> nb_valeurs].num_sommet;
	percole_bas(*f,0);
	return res;
}




/* Renvoie l'indice du parent du noeud d'indice i */
int i_p(int i){
	return ((i+1)/2)-1;
}

void percole_haut(fileprio f,int i){
	if(i > 0){		// Sinon on est en train de traiter la racine et ça va péter
		int ip = i_p(i);
		if(f.t[i].valeur < f.t[ip].valeur){
			permute(f.t,i,ip);
			percole_haut(f,i);
		}
	}
	return;
}

void inserer_fileprio(fileprio* f,int num_sommet,int valeur){
	noeud nv_noeud;
	nv_noeud.num_sommet = num_sommet;
	nv_noeud.valeur = valeur;
	f -> t[f->nb_valeurs] = nv_noeud;
	percole_haut(*f,f -> nb_valeurs);
	f -> nb_valeurs += 1;
	return;
}

void diminuer_fileprio(fileprio* f,int num_sommet,int nv_val){		// WARNING WARNING O(N)
	int n = f -> nb_valeurs;
	for(int i = 0;i < n;i++){		// On suppose que le même numéro de sommet n'est pas présent 2 fois dans la file de priorité
		if (f -> t[i].num_sommet = num_sommet){
			f -> t[i].valeur = nv_val;
			percole_haut(*f,i);		// ATTENTION
			return;
		}
	}
	return;
}
