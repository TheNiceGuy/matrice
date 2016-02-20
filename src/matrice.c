#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "matrice.h"
#include "config.h"

extern char* err;

struct matrice* matrice_new(int x, int y) {
    struct matrice* st_matrice;

    /* On s'assure que la matrice n'est pas nulle. */
    if(x < 1 || y < 1)
        return NULL;

    /* On alloue la mémoire à la structure. */
    st_matrice = malloc(sizeof(struct matrice));
    if(st_matrice == NULL) {
        err = "La mémoire n'a pas pu être allouée.";
        return NULL;
    }

    /* On configure la structure. */
    st_matrice->x = x;
    st_matrice->y = y;
    st_matrice->data = calloc(x*y, sizeof(float));

    return st_matrice;
}

struct matrice* matrice_new_ptr(int x, int y, int* data) {
    struct matrice* st_matrice;

    /* On crée la matrice de base. */
    st_matrice = matrice_new(x, y);
    if(st_matrice == NULL)
        return NULL;

    /* On copie les données dans la matrice */
    memcpy(st_matrice->data, data, x*y*sizeof(float));

    return st_matrice;
}

void matrice_delete(struct matrice* st_matrice) {
    /* On libère la mémoire. */
    free(st_matrice->data);
    free(st_matrice);
}

struct matrice* matrice_add(struct matrice* matrice_A, struct matrice* matrice_B) {
    int i, j;
    struct matrice* somme;

    /* On s'assure que les dimensions des matrices sont les mêmes. */
    if((matrice_A->x != matrice_B->x) || (matrice_A->y != matrice_B->y)) {
        err = "Les matrices ne sont pas compatibles.";
        return NULL;
    }

    /* On crée la nouvelle matrice. */
    somme = matrice_new(matrice_A->x, matrice_A->y);
    if(somme == NULL)
        return NULL;

    /* On boucle sur les rangées. */
    for(j = 0; j < matrice_A->y; j++) {
        /* On boucle sur les colonnes. */
        for(i = 0; i < matrice_A->x; i++) {
            somme->data[i+j*somme->x] = matrice_A->data[i+j*matrice_A->x]+
                                        matrice_B->data[i+j*matrice_B->x];
        }
    }

    return somme;
}

struct matrice* matrice_mul(struct matrice* matrice_A, struct matrice* matrice_B) {
    int i, j, k;
    struct matrice* produit;

    /* On s'assure que les matrices sont compatibles. */
    if(matrice_A->x != matrice_B->y) {
        err = "Les matrices ne sont pas compatibles.";
        return NULL;
    }

    /* On crée la nouvelle matrice. */
    produit = matrice_new(matrice_B->x, matrice_A->y);
    if(produit == NULL)
        return NULL;

    /* On boucle sur les rangées. */
    for(j = 0; j < produit->y; j++) {
        /* On boucle sur les colonnes. */
        for(i = 0; i < produit->x; i++) {
            produit->data[i+j*produit->x] = 0;

            /* On boucle pour la somme des multiplications d'une colonne à une rangée. */
            for(k = 0; k < matrice_A->x; k++)
                produit->data[i+j*produit->x] += matrice_A->data[k+j*matrice_A->x]*
                                                 matrice_B->data[i+k*matrice_B->x];
        }
    }

    return produit;
}

struct matrice* matrice_transposition(struct matrice* st_matrice) {
    int i, j;
    struct matrice* resultat;

    /* On crée la nouvelle matrice. */
    resultat = matrice_new(st_matrice->y, st_matrice->x);
    if(resultat == NULL)
        return NULL;

    /* On boucle sur les rangées. */
    for(j = 0; j < resultat->y; j++) {
        /* On boucle sur les colonnes. */
        for(i = 0; i < resultat->x; i++) {
            resultat->data[i+j*resultat->x] = st_matrice->data[j+i*st_matrice->x];
        }
    }

    return resultat;
}

struct matrice* matrice_remove_row_col(struct matrice* st_matrice, int x, int y) {
    int i, j, a, b;
    struct matrice* resultat;

    /* On s'assure que la matrice résultate n'est pas nulle. */
    if((st_matrice->x < 2) || (st_matrice->y < 2)) {
        err = "La matrice résultante est nulle.";
        return NULL;
    }

    /* On crée la nouvelle matrice. */
    resultat = matrice_new(st_matrice->x-1, st_matrice->y-1);
    if(resultat == NULL)
        return NULL;

    /* On boucle sur les rangées. */
    for(b = j = 0; j < st_matrice->y; j++) {
        /* On ignore la rangée spécifiée. */
        if(j == y)
            continue;

        /* On boucle sur les colonnes. */
        for(a = i = 0; i < st_matrice->x; i++) {
            /* On ignore la colonne spécifiée. */
            if(i == x)
                continue;

            /* On copie la valeur dans la nouvelle matrice. */
            resultat->data[a+b*resultat->x] = st_matrice->data[i+j*st_matrice->x]; a++;
        } b++;
    }

    return resultat;
}

struct matrice* matrice_replace_col(struct matrice* st_matrice, struct matrice* col, int index) {
    struct matrice* resultat;
    int i, j;

    /* On s'assure que les deux matrices sont compatibles. */
    if(st_matrice->y != col->y) {
        err = "Les matrices ne sont pas compatibles.";
        return NULL;
    }

    /* On crée la nouvelle matrice. */
    resultat = matrice_new(st_matrice->x, st_matrice->y);
    if(resultat == NULL)
        return NULL;

    /* On boucle sur les rangées. */
    for(j = 0; j < st_matrice->y; j++) {
        /* On boucle sur les colonnes. */
        for(i = 0; i < st_matrice->x; i++) {
            /* On copie la colonne. */
            if(i == index)
                resultat->data[i+j*resultat->x] = col->data[j];
            else
                resultat->data[i+j*resultat->x] = st_matrice->data[i+j*st_matrice->x];
        }
    }

    return resultat;
}

struct matrice* matrice_solve_cramer(struct matrice* coef, struct matrice* cons) {
    struct matrice* solution;
    struct matrice* replace;
    int i, j, det, sum;

    /* On s'assure que la matrice des constantes est carrée. */
    if(coef->x != coef->y) {
        err = "La méthode de Cramer ne peut que résoudre des matrices carrées.";
        return NULL;
    }

    /* On s'assure que les deux matrices sont compatibles. */
    if(coef->y != cons->y) {
        err = "Les matrices ne sont pas compatibles.";
        return NULL;
    }

    /* On s'assure que la matrice est une colonne. */
    if(cons->x != 1) {
        err = "La colonne reçue n'est pas une colonne.";
        return NULL;
    }

    /* On s'assure que le déterminant des coefficients n'est pas nulle. */
    det = matrice_determinant(coef);
    if(det == 0) {
        /* On regarde si le système d'équations linéaires est homogène. */
        for(sum = j = 0; j < cons->y; j++)
            sum += cons->data[j];

        /* On détermine le nombre de solutions. */
        if(sum == 0)
            err = "Le SEL admet soit une infinité de solutions ou aucune solution.";
        else
            err = "Le SEL admet une infinité de solutions.";

        return NULL;
    }

    /* On crée la nouvelle matrice. */
    solution = matrice_new(cons->x, cons->y);
    if(solution == NULL)
        return NULL;

    for(i = 0; i < coef->x; i++) {
        /* On remplace la ième colone par la colonne des constantes. */
        replace = matrice_replace_col(coef, cons, i);

        /* On calcule la solution */
        solution->data[i] = (float)matrice_determinant(replace)/(float)det;

        /* On supprime la matrice temporaire. */
        matrice_delete(replace);
    }

    return solution;
}

int matrice_determinant(struct matrice* st_matrice) {
    int a, det;
    struct matrice* sous_matrice;

    /* On reset le message d'erreur. */
    err = NULL;

    /* On s'assure que la matrice est carrée. */
    if(st_matrice->x != st_matrice->y) {
        err = "La matrice n'est pas carré.";
        return 0;
    }

    /* On retourne la valeur de la matrice si sa taille est de 1. */
    if(st_matrice->x == 1)
        return st_matrice->data[0];

    /* TODO: Il est possible d'optimiser cette algorithme énormément en faisant appel
     *       aux propriétés du déterminant d'une matrice. Par contre, l'algorithme
     *       implémenté est très immature. Aucune optimisation n'est esseyée, la
     *       première ligne est toujours utilisée pour calculer le déterminant.
     */
    for(det = a = 0; a < st_matrice->x; a++) {
        /* On enlève la colonne et la ligne reliées à la cellule afin de calculer
         * le sous déterminant de la matrice.
         */
        sous_matrice = matrice_remove_row_col(st_matrice, a, 0);

        /* L'algorithme est recursif, donc on calcule le déterminant de la sous matrice. */
        det += (a%2==0?1:-1)*st_matrice->data[a]*matrice_determinant(sous_matrice);

        /* On libère la mémoire reliée à la sous matrice.*/
        matrice_delete(sous_matrice);
    }

    return det;
}

void matrice_print(struct matrice* st_matrice, int focus) {
    int i, j;

    /* On boucle sur les rangées. */
    for(j = 0; j < st_matrice->y; j++) {
        /* On boucle sur les colonnes. */
        for(i = 0; i < st_matrice->x; i++) {
            if(i+j*st_matrice->x == focus)
                printf("[%2.2f]\t", st_matrice->data[i+j*st_matrice->x]);
            else
                printf("%2.2f\t", st_matrice->data[i+j*st_matrice->x]);
        }
        printf("\n");
    }
}
