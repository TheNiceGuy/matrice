#ifndef INTERFACE_INCLUDE_H
#define INTERFACE_INCLUDE_H

#include <sys/ioctl.h>
#include "matrice.h"
#include "buffer.h"
#include "vector.h"

struct interface {
    struct winsize taille;

    struct vector* name;
    struct vector* matrice;

    struct npanel* main;
    struct nlist* main_ls_actions;
    struct nlabel* main_la_msg;

    struct buffer* def_buf_name;
    struct matrice* def_mat_dim;
    struct matrice* def_mat;
    struct npanel* def;
    struct npanel* def_p_name;
    struct npanel* def_p_dim;
    struct nlabel* def_la_name;
    struct nlabel* def_la_name_str;
    struct nlabel* def_la_dim;
    struct ngrid* def_g_mat;
    struct ngrid* def_g_dim;

};

struct interface* interface_new();
void interface_delete(struct interface* interface);
void interface_main(struct interface* interface);
void interface_choisir(struct interface* ui, int index);
void interface_def_name(struct interface* ui);
void interface_grid(struct interface* ui, struct ngrid* st_ngrid);
void interface_grid_dim(struct interface* ui, struct ngrid* st_ngrid);

#endif
