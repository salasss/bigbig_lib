#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct
{
    int signe;            // 0 nega,1 posi
    int k;                // taille du block == taille des tab ducoup
    unsigned short *bloc; // tableau qui stock les val

} bigbig;

void afficher_bigbig(bigbig num);

// qst2) fonc qui cree un nombre sur p bits et init a 0 avec p multiple de 16
bigbig create_bigbig(int s, int taille)
{
    assert(taille % 16 == 0);
    bigbig n;
    n.signe = s;
    n.k = taille / 16;
    n.bloc = malloc(n.k * sizeof(unsigned short));
    assert(n.bloc != NULL);
    for (int i = 0; i < n.k; i++)
    {
        n.bloc[i] = 0;
    }
    return n;
}

int max_big(int a, int b)
{
    return (a >= b) ? a : b;
}
int meme_signature(int a, int b)
{
    return (a != b) ? 0 : 1;
}

void nettoyer_zero(bigbig *c)
{
    //1er version :
    while (c->k > 1 && c->bloc[c->k - 1] == 0)
    {
        c->k--;
    };
    //2nd version pour bien fiare les chose faut reallouer ce qu'on a besoin sans prendre des cases e nplus pour rien
    unsigned short *tmp = realloc(c->bloc,c->k * sizeof(unsigned short));
    if (tmp != NULL) {
        c->bloc = tmp;
    } 
}
// qst3) une fonction qui effecture l'addition de 2 nombre de cette representation et qui le dans dans un 3 eme parametre
bigbig addtion_bigbig(bigbig a, bigbig b, bigbig *c)
{
    // on trouve la taille du nv element
    int taille_addition = max_big(a.k, b.k) + 1;
    c->signe = a.signe;
    c->k = taille_addition;
    c->bloc = malloc(c->k * sizeof(unsigned short));
    assert(c->bloc != NULL);
    unsigned int retenue = 0;
    for (int i = 0; i < c->k; i++)
    {
        unsigned short a_val = (i < a.k) ? a.bloc[i] : 0;
        unsigned short b_val = (i < b.k) ? b.bloc[i] : 0;
        unsigned int c_val = a_val + b_val + retenue;
        // ici on prendre la 1er moitier dans 16 bits de la droite
        c->bloc[i] = (unsigned short)(c_val & 0xFFFF);

        // ici faut deplaceer le reste vers la droite
        retenue = (c_val >> 16);
    }

    // SI a la fin on se retrouve avec un 0 a la fin on peut libere une case
    nettoyer_zero(c);

    return *c;
}

// pour la soustraction on doit dabord trouver qui est plus grands valeur absolue
int valeur_absolue(bigbig a, bigbig b)
{
    if (a.k > b.k)
        return 1;
    if (b.k > a.k)
        return -1;

    // 2. Si les tailles sont égales, comparer bloc par bloc
    // On commence par le bloc de poids le plus fort (la fin du tableau)
    for (int i = a.k - 1; i >= 0; i--)
    {
        if (a.bloc[i] > b.bloc[i])
            return 1;
        if (a.bloc[i] < b.bloc[i])
            return -1;
    }

    // 3. Si on sort de la boucle, ils sont parfaitement égaux
    return 0;
}
// on va toujour mettre a plus grnads que b
bigbig soustraction_bigbig(bigbig a, bigbig b, bigbig *c)
{
    // a est tour le grand
    int taille_soustraction = a.k;
    // on init c
    c->k = taille_soustraction;
    c->bloc = malloc(c->k * sizeof(unsigned short));
    assert(c->bloc != NULL);

    int emprunt = 0; // de la soustraction
    for (int i = 0; i < c->k; i++)
    {
        unsigned short val_a = (i < a.k) ? a.bloc[i] : 0;
        unsigned short val_b = (i < b.k) ? b.bloc[i] : 0;
        int val_c = val_a - val_b - emprunt;

        if (val_c < 0)
        {
            val_c += 65536;
            emprunt = 1;
        }
        else
        {
            emprunt = 0;
        }

        c->bloc[i] = (unsigned short)val_c;
    }
    nettoyer_zero(c);
    return *c;
}

bigbig add_soustration_bigbig(bigbig a, bigbig b, bigbig *c)
{
    int mme_signature = meme_signature(a.signe, b.signe);
    if (mme_signature == 1)
    {
        addtion_bigbig(a, b, c);
        c->signe = a.signe; // le signe des deux
        nettoyer_zero(c);
        return *c;
    }
    int test = valeur_absolue(a, b);
    if (test == 1)
    {
        soustraction_bigbig(a, b, c);
        c->signe = a.signe;
        nettoyer_zero(c);
        return *c;
    }
    else
    {
        soustraction_bigbig(b, a, c);
        c->signe = b.signe;
        nettoyer_zero(c);
        return *c;
    }
}
void redimentionner(bigbig *D, int S_taille)
{
    if (D->k == S_taille)
        return;
    unsigned short *temp = realloc(D->bloc, S_taille * sizeof(unsigned short));
    assert(temp != NULL);
    if (D->k < S_taille)
    {
        for (int i = D->k; i < S_taille; i++)
        {
            D->bloc[i] = 0;
        }
    }
}

void copier(bigbig *D, bigbig *S)
{
    int i;
    if (D->k < S->k)
        redimentionner(D, S->k);
    D->k = S->k;
    D->signe = S->signe;
    for (int i = 0; i < S->k; i++)
    {
        D->bloc[i] = S->bloc[i];
    }
}

// on attaque la qst4 : la multiplication
bigbig multiplication(bigbig a, bigbig b, bigbig *c)
{
    c->k = a.k + b.k;
    c->bloc = calloc(c->k, sizeof(unsigned short));
    c->signe = (a.signe == b.signe) ? 1 : 0;
    assert(c->bloc != NULL);

    for (int i = 0; i < a.k; i++)
    {
        unsigned int carry = 0;
        for (int j = 0; j < b.k; j++)
        {
            int idx = i + j;
            unsigned int prod = (unsigned int)a.bloc[i] * (unsigned int)b.bloc[j];
            unsigned int temp = (unsigned int)c->bloc[idx] + prod + carry;
            c->bloc[idx] = (unsigned short)(temp & 0xFFFF);
            carry = temp >> 16;
        }

        // la il faut que rajojute la retenu restant
        int idx = i + b.k;
        while (carry != 0 && idx < c->k)
        {
            unsigned int temp = (unsigned int)c->bloc[idx] + carry;
            c->bloc[idx] = (unsigned short)(temp & 0xFFFF);
            carry = temp >> 16;
            idx++;
        }
    }

    // if faut mainnte verifier si le derinier bloc est vide ou pas sinon ou lenleve

    nettoyer_zero(c);

    return *c;
}

void test_unit()
{
    // Exemple1 : 65535 + 1 (0xFFFF + 0x0001)
    bigbig a2, b2, c2;
    a2.k = 1;
    a2.signe = 1;
    a2.bloc = malloc(sizeof(unsigned short));
    a2.bloc[0] = 0xFFFF; // 65535

    b2.k = 1;
    b2.signe = 1;
    b2.bloc = malloc(sizeof(unsigned short));
    b2.bloc[0] = 0x0001;

    addtion_bigbig(a2, b2, &c2);

    assert(c2.bloc[0] == 0x0000);
    assert(c2.bloc[1] == 0x0001);
    assert(c2.k == 2);
    assert(c2.signe == 1);
    printf("addition : OK\n");

    // Exemple2 : +645 + -2002
    bigbig a, b, c;

    a = create_bigbig(1, 16); // signe 1 (posi)
    a.bloc[0] = 0x0285;       // 645

    b = create_bigbig(0, 16); // signe 0 (nega)
    b.bloc[0] = 0x07D2;       // 2002

    add_soustration_bigbig(a, b, &c);
    assert(c.bloc[0] == 0x054D);
    assert(c.k == 1);
    assert(c.signe == 0);
    printf("soustraction normal : OK\n");
    // Exemple2 : +0 + -2002
    bigbig a0, b0, c0;

    a0 = create_bigbig(1, 16); // signe 1 (posi)
    a0.bloc[0] = 0x0000;

    b0 = create_bigbig(0, 16); // signe 0 (nega)
    b0.bloc[0] = 0x07D2;       // 2002

    add_soustration_bigbig(a0, b0, &c0);
    assert(c0.bloc[0] == 0x07D2);
    assert(c0.k == 1);
    assert(c0.signe == 0);
    printf("soustraction avec 0 : OK \n");
    // Exemple2 : +0 + -2002
    bigbig ax, bx, cx;

    ax = create_bigbig(1, 16); // signe 1 (posi)
    ax.bloc[0] = 0x07D2;

    bx = create_bigbig(0, 16); // signe 0 (nega)
    bx.bloc[0] = 0x07D2;       // 2002

    add_soustration_bigbig(ax, bx, &cx);
    assert(cx.bloc[0] == 0x000);
    assert(cx.k == 1);
    assert(cx.signe == 0);
    printf("soustraction qui donne 0 : OK \n");

    // Exemple2 : +2002 * 2002
    bigbig axx, bxx, cxx;

    // Cas : 2002 * 0
    axx = create_bigbig(1, 16);
    axx.bloc[0] = 0x07D2;

    bxx = create_bigbig(1, 16); //
    bxx.bloc[0] = 0x07D2;

    multiplication(axx, bxx, &cxx);

    // Vérifications
    assert(cxx.bloc[0] == 0x2844); // Le résultat doit être 0
    assert(cxx.bloc[1] == 0x003D);
    assert(cxx.k == 2); // Taille 1

    printf("Multiplication 2002*2002 : OK \n");

    bigbig aauc;
    aauc = create_bigbig(1, 16);
    aauc.bloc[0] = 0x0011;
    multiplication(aauc, aauc, &aauc);
    assert(aauc.bloc[0] == 0x0121);
    assert(aauc.k == 1);
    printf("Multiplication au carre : OK \n");
}

int main()
{
    test_unit();

    return 0;
}
void afficher_bigbig(bigbig num)
{
    printf("Signe: %d, k: %d, Valeur: ", num.signe, num.k);
    for (int i = num.k - 1; i >= 0; i--)
    {
        printf("%04X ", num.bloc[i]);
    }
    printf("\n");
}
