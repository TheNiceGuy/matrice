#ifndef MATRICE_INCLUDE_H
#define MATRICE_INCLUDE_H

struct matrice {
    int x;
    int y;
    float* data;
};

struct matrice* matrice_new(int x, int y);
struct matrice* matrice_new_ptr(int x, int y, int* data);
void matrice_delete(struct matrice* st_matrice);
struct matrice* matrice_add(struct matrice* matrice_A, struct matrice* matrice_B);
struct matrice* matrice_mul(struct matrice* matrice_A, struct matrice* matrice_B);
struct matrice* matrice_transposition(struct matrice* st_matrice);
struct matrice* matrice_remove_row_col(struct matrice* st_matrice, int x, int y);
struct matrice* matrice_replace_col(struct matrice* st_matrice, struct matrice* col, int index);
struct matrice* matrice_solve_cramer(struct matrice* coef, struct matrice* cons);
int matrice_determinant(struct matrice* st_matrice);
void matrice_print(struct matrice* st_matrice, int focus);


#endif
