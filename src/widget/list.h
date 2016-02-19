#ifndef LIST_INCLUDE_H
#define LIST_INCLUDE_H

#include "widget.h"

struct nlist {
    struct widget;
    char** options;
    int size;
    int sel;
};

struct nlist* nlist_new(char** options, int size, int pad_x, int pad_y);
void nlist_delete(struct nlist* st_nlist);
void nlist_resize(struct nlist* st_nlist);
void nlist_move(struct nlist* st_nlist, int x, int y);
void nlist_draw(struct nlist* st_nlist);
void nlist_sel_change(struct nlist* st_nlist, int sel);
int nlist_sel_inc(struct nlist* st_nlist);
int nlist_sel_dec(struct nlist* st_nlist);

#endif
