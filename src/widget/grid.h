#ifndef GRID_INCLUDE_H
#define GRID_INCLUDE_H

#include <ncurses.h>
#include "widget.h"
#include "matrice.h"

struct ngrid {
    struct widget;
    struct matrice* st_matrice;
    char* format;
    int sel_x;
    int sel_y;
};

struct ngrid* ngrid_new(struct matrice* st_matrice, char* format, int pad_x, int pad_y);
void ngrid_delete(struct ngrid* st_ngrid);
void ngrid_resize(struct ngrid* st_ngrid);
void ngrid_move(struct ngrid* st_ngrid, int x, int y);
void ngrid_draw(struct ngrid* st_ngrid);
void ngrid_change_matrice(struct ngrid* st_ngrid, struct matrice* st_matrice);
void ngrid_change_selection(struct ngrid* st_ngrid, int sel_x, int sel_y);

#endif
