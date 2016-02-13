#include <ncurses.h>
#include <locale.h>
#include "matrice.h"
#include "interface.h"
#include "misc.h"
#include "config.h"

#include "widget/panel.h"
#include "widget/grid.h"
#include "widget/list.h"
#include "widget/label.h"

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
#define TEST_STRING {"x"}

int main() {
    struct interface* ui;

    setlocale(LC_CTYPE, "LANG=en_US.UTF-8");
    initscr();

    ui = interface_new();
    interface_main(ui);
    interface_delete(ui);

    endwin();

    /*(struct matrice* matrice_A = matrice_new_ptr(3, 3, (int[])TEST_MATRIX_33);
    struct matrice* matrice_B = matrice_new_ptr(5, 6, (int[])TEST_MATRIX_56);
    struct matrice* matrice_C = matrice_new_ptr(4, 3, (int[])TEST_MATRIX_43);
    struct npanel* st_panel;
    struct ngrid* st_grid_A;
    struct ngrid* st_grid_B;
    struct ngrid* st_grid_C;
    struct nlist* st_list;
    struct nlabel* st_label;


    initscr();

    st_panel = npanel_new(3, VERTICAL, 2, 0);
    st_grid_A = ngrid_new(matrice_A, "%3d", 0, 0);
    st_grid_B = ngrid_new(matrice_B, "%3d", 0, 0);
    st_grid_C = ngrid_new(matrice_C, "%3d", 0, 0);
    st_list = nlist_new((char*[])OPERATION, 6, 1, 0);
    st_label = nlabel_new("Dimension", 20, 2);

    npanel_set_child(st_panel, (struct widget*)st_grid_A, 0);
    npanel_set_child(st_panel, (struct widget*)st_list, 2);
    npanel_set_child(st_panel, (struct widget*)st_label, 1);
    npanel_resize(st_panel);

    refresh();
    npanel_draw(st_panel);

    getch();
    endwin();

    matrice_delete(matrice_A);
    matrice_delete(matrice_B);
    matrice_delete(matrice_C);
    npanel_delete(st_panel);*/

    return 0;
}

void test() {
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
}
