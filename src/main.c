#include <stdio.h>
#include "matrice.h"

#define TEST_MATRIX_33 {0, 1, 2, \
                        3, 4, 5, \
                        6, 7, 8}
#define TEST_MATRIX_43 {0, 1, 2, 3, \
                        4, 5, 6, 7, \
                        8, 9,10, 11}
#define TEST_MATRIX_56 {0, 1, 2, 3, 4 , \
                        5, 6, 7, 8, 9 , \
                        10,11,12,13,14, \
                        15,16,17,18,19, \
                        20,21,22,23,24, \
                        25,26,27,28,29}

int main() {
    struct matrice* matrice_A = matrice_new_ptr(3, 3, (int[])TEST_MATRIX_33);
    struct matrice* matrice_B = matrice_new_ptr(3, 3, (int[])TEST_MATRIX_33);
    struct matrice* matrice_C = matrice_new_ptr(4, 3, (int[])TEST_MATRIX_43);
    struct matrice* matrice_D = matrice_new_ptr(5, 6, (int[])TEST_MATRIX_56);
    struct matrice* matrice_T = matrice_transposition(matrice_C);
    struct matrice* matrice_ADD = matrice_add(matrice_A, matrice_B);
    struct matrice* matrice_MUL = matrice_mul(matrice_C, matrice_T);
    struct matrice* matrice_RM = matrice_remove_row_col(matrice_D, 0, 0);

    matrice_print(matrice_A); printf("\n");
    matrice_print(matrice_B); printf("\n");
    matrice_print(matrice_C); printf("\n");
    matrice_print(matrice_D); printf("\n");
    matrice_print(matrice_T); printf("\n");
    matrice_print(matrice_ADD); printf("\n");
    matrice_print(matrice_MUL); printf("\n");
    matrice_print(matrice_RM); printf("\n");
    printf("%d\n", matrice_determinant(matrice_A));

    matrice_delete(matrice_A);
    matrice_delete(matrice_B);
    matrice_delete(matrice_C);
    matrice_delete(matrice_D);
    matrice_delete(matrice_T);
    matrice_delete(matrice_ADD);
    matrice_delete(matrice_MUL);
    matrice_delete(matrice_RM);

    return 0;
}
