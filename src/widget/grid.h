#ifndef GRID_INCLUDE_H
#define GRID_INCLUDE_H

#include <ncurses.h>
#include "widget.h"
#include "matrice.h"

struct ngrid {
    struct widget;
    struct matrice* st_matrice;
    char* format;
    int sig;
    int size;
    int sel_x;
    int sel_y;
};

struct ngrid* ngrid_new(int x, int y, int sig, char* format, int pad_x, int pad_y);
struct ngrid* ngrid_new_ptr(int x, int y, int* data, int sig, char* format, int pad_x, int pad_y);
void ngrid_delete(struct ngrid* st_ngrid);
void ngrid_resize(struct ngrid* st_ngrid);
void ngrid_move(struct ngrid* st_ngrid, int x, int y);
void ngrid_draw(struct ngrid* st_ngrid);
void ngrid_change(struct ngrid* st_ngrid, int x, int y);
void ngrid_sel_change(struct ngrid* st_ngrid, int sel_x, int sel_y);
int ngrid_sel_inc_x(struct ngrid* st_ngrid);
int ngrid_sel_dec_x(struct ngrid* st_ngrid);
int ngrid_sel_inc_y(struct ngrid* st_ngrid);
int ngrid_sel_dec_y(struct ngrid* st_ngrid);
int ngrid_get(struct ngrid* st_ngrid);
void ngrid_set(struct ngrid* st_ngrid, int val);
void ngrid_cat(struct ngrid* st_ngrid, char c);

#endif
