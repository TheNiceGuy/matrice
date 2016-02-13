#ifndef INCLUDE_CONFIG_H
#define INCLUDE_CONFIG_H

#define TRUE  1
#define FALSE 0

#define OPERATION_W taille.ws_col
#define OPERATION_H 8
#define OPERATION_X 0
#define OPERATION_Y 0

#define MODIF_W taille.ws_col
#define MODIF_H taille.ws_row-OPERATION_H-STATUT_H
#define MODIF_X 0
#define MODIF_Y OPERATION_H

#define STATUT_W taille.ws_col
#define STATUT_H 3
#define STATUT_X 0
#define STATUT_Y taille.ws_row-STATUT_H

#define OPERATION_NB 6
#define OPERATION {"Define matrix",                  \
                   "Addition of two matrices",       \
                   "Multiplication of two matrices", \
                   "Transpose matrix",               \
                   "Compute matrix determinant",     \
                   "Quit"}
#define ACTION_DEFINE 0
#define ACTION_ADD    1
#define ACTION_MUL    2
#define ACTION_TRANS  3
#define ACTION_DET    4
#define ACTION_QUIT   5

#define UI_NEW_MATRIX_X 2
#define UI_NEW_MATRIX_Y 2

#define BUFFER_SIZE 50
#define MAX_SIZE 99

#endif
