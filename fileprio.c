#include <stdbool.h>
#include <stdlib.h>

struct s_noeud = {
	int num_sommet;
	int valeur;
};
typedef struct s_noeud noeud;

struct s_fileprio = {
	int nb_valeurs;
	//int taille_max;
	int* t;
}
typedef struct s_fileprio fileprio;




fileprio creer_fileprio(int taille_max){
	fileprio f;
	f.nb_valeurs = 0;
	//f.taille_max = taille_max;
	f.t = malloc(taille_max * sizeof(noeud));
	return f;
}

bool fileprio_non_vide(fileprio* f){
	return (f -> nb_valeurs != 0);
}

void permute(int t[],int i,int j){
	int tmp = t[i];
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
		if(f.t[i] > f.t[ig] || f.t[i] > f.t[id]){
			if (f.t[i] > f.t[ig]){
				permute(f.t,i,ig);
				percole_bas(f.t,ig);
			}else{
				permute(f.t,i,id);
				percole_bas(f.t,id);
			}
		}		// else -> percolation terminée
	}else if(ig == f.nb_valeurs - 1){		// 1 seul fils, le dernier noeud du tableau
		if(f.t[i] > f.t[ig]){
			permute(f.t,i,ig);
		}
	}
}

/* Renvoie le NUMÉRO DE SOMMET du sommet avec la valeur la plus faible */
int extraire_fileprio(fileprio* f){
	f -> nb_valeurs -= 1;
	permute(f -> t,0,nb_valeurs);
	res = f -> (t[nb_valeurs]).num_sommet;
	percole_bas(&f,0);
	return res;
}




/* Renvoie l'indice du parent du noeud d'indice i */
int i_p(int i){
	return ((i+1)/2)-1;
}

void percole_haut(fileprio f,int i){
	if(i > 0){		// Sinon on est en train de traiter la racine et ça va péter
		int ip = i_p(i);
		if(f.t[i] < f.t[ip]){
			permute(f.t,i,ip);
			percole_haut(f,i);
		}
	}
	return;
}

void inserer_fileprio(fileprio* f,int num_sommet,int distance){
	noeud nv_noeud;
	nv_noeud.num_sommet = num_sommet;
	nv_noeud.valeur = valeur;
	t[f->nb_valeurs] = nv_noeud;
	percole_haut(&f,f -> nb_valeurs);
	f -> nb_valeurs += 1;
	return;
}

