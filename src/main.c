#include <stdio.h>
#include <stdlib.h>
#include "matrice.h"
#include "config.h"

#define TEST_MATRIX_33 { 1, 4, 0, \
                         3,-1, 3, \
                         2, 0, 0}
#define TEST_MATRIX_43 {0, 1, 2, 3, \
                        4, 5, 6, 7, \
                        8, 9,10, 11}
#define TEST_MATRIX_56 {0, 1, 2, 3, 4 , \
                        5, 6, 7, 8, 9 , \
                        10,11,12,13,14, \
                        15,16,17,18,19, \
                        20,21,22,23,24, \
                        25,26,27,28,29}

char* err;

int get_int(char* string, int min, int max) {
    int input;

    do {
        printf("%s [%d, %d] : ", string, min, max);
        scanf("%d", &input);
    } while(input < min || input > max);

    return input;
}

void get_enter() {
    char key;

    printf("\nAppuyer sur <entré> pour continuer...");

    while ((key = getchar()) != '\n' && key != EOF);
    getchar();
}

struct matrice* get_matrice(int mx, int my) {
    int i, x, y;
    struct matrice* st_matrice;

    x = get_int("Veuillez entrer la dimension en X", 1, mx);
    y = get_int("Veuillez entrer la dimension en Y", 1, my);

    st_matrice = matrice_new(x, y);
    for(i = 0; i < st_matrice->x*st_matrice->y; i++) {
        clear();
        matrice_print(st_matrice, i);
        st_matrice->data[i] = get_int("Veuillez entrer la valeur de la case", -99, 99);
    }

    return st_matrice;
}

void menu_add() {
    struct matrice* matrice_A;
    struct matrice* matrice_B;
    struct matrice* matrice_S;

    clear();
    printf("Veuillez configurer la première matrice.\n");
    matrice_A = get_matrice(9, 9);

    clear();
    printf("Veuillez configurer la deuxième matrice.\n");
    matrice_B = get_matrice(9, 9);

    matrice_S = matrice_add(matrice_A, matrice_B);

    clear();
    printf("Première matrice :\n");
    matrice_print(matrice_A, -1);
    printf("\nDeuxième matrice :\n");
    matrice_print(matrice_B, -1);
    printf("\nLa matrice résultante de la somme :\n");
    if(matrice_S != NULL)
        matrice_print(matrice_S, -1);
    else
        printf("ERREUR : %s\n", err);

    get_enter();
    if(matrice_A != NULL) matrice_delete(matrice_A);
    if(matrice_B != NULL) matrice_delete(matrice_B);
    if(matrice_S != NULL) matrice_delete(matrice_S);
}

void menu_mul() {
    struct matrice* matrice_A;
    struct matrice* matrice_B;
    struct matrice* matrice_P;

    clear();
    printf("Veuillez configurer la première matrice.\n");
    matrice_A = get_matrice(9, 9);

    clear();
    printf("Veuillez configurer la deuxième matrice.\n");
    matrice_B = get_matrice(9, 9);

    matrice_P = matrice_mul(matrice_A, matrice_B);

    clear();
    printf("Première matrice :\n");
    matrice_print(matrice_A, -1);
    printf("\nDeuxième matrice :\n");
    matrice_print(matrice_B, -1);
    printf("\nLa matrice résultante du produit :\n");
    if(matrice_P != NULL)
        matrice_print(matrice_P, -1);
    else
        printf("ERREUR : %s\n", err);

    get_enter();
    if(matrice_A != NULL) matrice_delete(matrice_A);
    if(matrice_B != NULL) matrice_delete(matrice_B);
    if(matrice_P != NULL) matrice_delete(matrice_P);
}

void menu_trans() {
    struct matrice* matrice_A;
    struct matrice* matrice_T;

    clear();
    printf("Veuillez configurer la matrice.\n");
    matrice_A = get_matrice(9, 9);

    matrice_T = matrice_transposition(matrice_A);

    clear();
    printf("Matrice initiale :\n");
    matrice_print(matrice_A, -1);
    printf("\nLa matrice résultante de la Transposition :\n");
    if(matrice_T != NULL)
        matrice_print(matrice_T, -1);
    else
        printf("ERREUR : %s\n", err);

    get_enter();
    if(matrice_A != NULL) matrice_delete(matrice_A);
    if(matrice_T != NULL) matrice_delete(matrice_T);
}

void menu_det() {
    struct matrice* matrice_A;
    int det;

    clear();
    printf("Veuillez configurer la matrice.\n");
    matrice_A = get_matrice(5, 5);

    det = matrice_determinant(matrice_A);

    clear();
    printf("Matrice initiale :\n");
    matrice_print(matrice_A, -1);
    printf("\nLe déterminant de la matrice : ");
    if(err == NULL)
        printf("%d\n", det);
    else
        printf("\nERREUR : %s\n", err);

    get_enter();
    if(matrice_A != NULL) matrice_delete(matrice_A);
}

void menu_cramer() {
    struct matrice* matrice_A;
    struct matrice* matrice_B;
    struct matrice* matrice_S;

    clear();
    printf("Veuillez configurer la matrice des coefficients.\n");
    matrice_A = get_matrice(9, 9);

    clear();
    printf("Veuillez configurer la matrice des constantes.\n");
    matrice_B = get_matrice(9, 9);

    matrice_S = matrice_solve_cramer(matrice_A, matrice_B);

    clear();
    printf("Matrice des coefficients :\n");
    matrice_print(matrice_A, -1);
    printf("\nMatrice des constantes :\n");
    matrice_print(matrice_B, -1);
    printf("\nMatrice des solutions :\n");
    if(matrice_S != NULL)
        matrice_print(matrice_S, -1);
    else
        printf("ERREUR : %s\n", err);

    get_enter();
    if(matrice_A != NULL) matrice_delete(matrice_A);
    if(matrice_B != NULL) matrice_delete(matrice_B);
    if(matrice_S != NULL) matrice_delete(matrice_S);
}

int main() {
    int exit = FALSE;

    do {
        clear();
        printf("Que voulez-vous effectuer?\n\n");
        printf("[0] Addition de deux matrices\n");
        printf("[1] Multiplication de deux matrices\n");
        printf("[2] Transposition d'une matrice\n");
        printf("[3] Calculer le déterminant d'une matrice\n");
        printf("[4] Résoudre une SEL avec la méthode de Cramer\n");
        printf("[5] Quitter\n");

        switch(get_int("Veuillez faire votre choix", 0, 5)) {
            case 0:
                menu_add();
                break;
            case 1:
                menu_mul();
                break;
            case 2:
                menu_trans();
                break;
            case 3:
                menu_det();
                break;
            case 4:
                menu_cramer();
                break;
            case 5:
                exit = TRUE;
                break;
        }
    } while(!exit);

    return 0;
}
