#include <stdio.h>          // Inclusion de la bibliothèque standard d'entrée/sortie
#include <stdlib.h>         // Inclusion de la bibliothèque standard pour malloc, free, exit

// qst1) Fonction qui crée un tableau dynamique d'entiers initialisé à 0
int* create_tab(int n){

     int* tab = malloc(n*sizeof(int)); // Alloue dynamiquement un tableau de n entiers
    if (tab == NULL){                  // Vérifie si l'allocation a échoué
        printf("error alloc !");       // Affiche un message d'erreur
        exit(1);                       // Termine le programme en cas d'erreur
    }
    for(int i = 0;i < n; i++){         // Parcourt chaque case du tableau
        tab[i]=0;                      // Initialise chaque case à 0
    }
    return tab;                        // Retourne le pointeur vers le tableau alloué
    }

// qst2) Fonction qui crée un tableau dynamique d'entiers via un pointeur de pointeur
void create_tab1(int n,int ** tab){

    *tab = (int*) malloc(n*sizeof(int)); // Alloue dynamiquement un tableau de n entiers et affecte l'adresse à *tab
    if (*tab == NULL){                   // Vérifie si l'allocation a échoué
        printf("error alloc !");         // Affiche un message d'erreur
        exit(1);                        // Termine le programme en cas d'erreur
    }
    for(int i = 0;i < n; i++){           // Parcourt chaque case du tableau
        (*tab)[i]=0;                     // Initialise chaque case à 0
    }
    printf("\n");                        // Affiche un saut de ligne (inutile ici)
    }

// qst3) fonction qui prends en param un tab alloue dynamiquement et qui modifie la taille du tab plus partieculierement retourne un nouveau tab avec la nouvelle taille
int* resize_tab(int* tab, int old_size, int new_size){
    int* new_tab = malloc(new_size * sizeof(int)); // Alloue un nouveau tableau
    if (new_tab == NULL) {                        // Vérifie si l'allocation a échoué
        printf("error alloc !");                   // Affiche un message d'erreur
        exit(1);                                   // Termine le programme en cas d'erreur
    }
    int min_size = (old_size < new_size) ? old_size : new_size; // Détermine la taille minimale à copier
    for (int i = 0; i < min_size; i++) {               // Copie les éléments de l'ancien tableau vers le nouveau
        new_tab[i] = tab[i];
    }
    for (int i = min_size; i < new_size; i++) {       // Initialise les nouveaux éléments à 0 si le nouveau tableau est plus grand
        new_tab[i] = 0;
    }
    free(tab);                                        // Libère l'ancien tableau
    return new_tab;                                   // Retourne le nouveau tableau
}

// Fonction principale du programme
int main(){
        int taille = 10;                     // Déclare une variable pour la taille du tableau
    int* mon_tab = create_tab(taille);       // Crée un tableau de 10 entiers initialisés à 0
    for(int i = 0; i<taille;i++){            // Parcourt le tableau
    printf("%d",mon_tab[i]);}                // Affiche chaque élément du tableau
	printf("\n"); 
    // free(mon_tab);                           // Libère la mémoire allouée pour mon_tab
///////////////////////////////////////////
    int* mon_autre_tab;                      // Déclare un pointeur pour un autre tableau
    create_tab1(5,&mon_autre_tab);           // Crée un tableau de 5 entiers initialisés à 0 via create_tab1
    for(int i = 0; i<5;i++){                 // Parcourt ce tableau
    printf("%d",mon_autre_tab[i]);}          // Affiche chaque élément du tableau
	printf("\n"); 
    // free(mon_autre_tab);                     // Libère la mémoire allouée pour mon_autre_tab
///////////////////////////////////////////
    int* resized_tab = resize_tab(mon_tab, taille, 15); // Redimensionne mon_tab à une nouvelle taille de 15
	for(int i = 0; i<15;i++){                // Parcourt le tableau redimensionné
	printf("%d",resized_tab[i]);}           // Affiche chaque élément du tableau
    free(resized_tab);                       // Libère la mémoire allouée pour resized_tab
    return 0;                                // Termine le programme
}
