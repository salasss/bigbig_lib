# BigBig : Bibliothèque arithmétique grands nombres en C

Ce projet est une implémentation en C d'une structure de données permettant de manipuler des nombres entiers arbitrairement grands ("Big Int"), dépassant les limites des types standards (`int`, `long long`).
L'approche utilisée découpe les nombres en blocs de 16 bits (`unsigned short`).

## 📋 Structure de données

Les nombres sont représentés par la structure `bigbig` définie comme suit :

```c
typedef struct {
    int signe;            // 0 pour négatif, 1 pour positif
    int k;                // Nombre de blocs
    unsigned short *bloc; // Tableau dynamique (base 2^16)
} bigbig;
```

## ✨ Fonctionnalités implémentées

Le programme gère actuellement :

* **Création** : Allocation dynamique alignée sur des multiples de 16 bits (`create_bigbig`).
* **Comparaison** :
   * `valeur_absolue` : Compare la magnitude de deux grands nombres.
   * `meme_signature` : Vérifie si les signes sont identiques.
* **Arithmétique** :
   * Addition (`addition_bigbig`) : Gestion complète des retenues.
   * Soustraction (`soustraction_bigbig`) : Gestion des emprunts.
   * Opération mixte (`add_soustraction_bigbig`) : Détermine intelligemment s'il faut additionner ou soustraire en fonction des signes (+/-).
   * Multiplication (`multiplication`) : Algorithme classique avec gestion des retenues sur blocs.

## 🚀 Utilisation

### Prérequis

Un compilateur C standard (GCC conseillé).

### Compilation

Compilez le fichier principal (assurez-vous d'être dans le répertoire du projet) :

```bash
gcc main.c -o bigbig
```

(Remplacez `main.c` par le nom réel de votre fichier `.c` si différent)

### Exécution des tests

Le `main` lance actuellement une suite de tests unitaires (`test_unit`) utilisant `assert.h` pour valider le bon fonctionnement des opérations (cas nominaux, nombres négatifs, zéro).

```bash
./bigbig
```

Si aucune erreur ne s'affiche et que vous voyez les messages "OK", c'est que les tests passent.

## 🛠️ Pistes d'amélioration a venir

* [ ] Ajouter une fonction de libération de mémoire (`free_bigbig`) pour éviter les fuites.
* [ ] Améliorer l'affichage (conversion hexadécimal → décimal pour la lecture humaine).
* [ ] Optimiser la multiplication (actuellement en O(n²)).
* [ ] Implémenter la division et le modulo.
* [ ] Ajouter des fonctions d'entrée/sortie pour lire et afficher les nombres en base 10.



## 🤝 Contributions

Les contributions sont les bienvenues ! N'hésitez pas à ouvrir une issue ou une pull request.
