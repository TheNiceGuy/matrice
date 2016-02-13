#include <stdlib.h>
#include <string.h>
#include "grid.h"
#include "misc.h"
#include "config.h"

struct ngrid* ngrid_new(struct matrice* st_matrice, char* format, int pad_x, int pad_y) {
    struct ngrid* st_ngrid;

    /* On alloue la mémoire. */
    st_ngrid = malloc(sizeof(struct ngrid));
    if(st_ngrid == NULL)
        return NULL;

    /* On configure la structure. */
    st_ngrid->window = NULL;
    st_ngrid->parent = NULL;
    st_ngrid->func_delete = (void (*)(struct widget* st_widget))&ngrid_delete;
    st_ngrid->func_resize = (void (*)(struct widget* st_widget))&ngrid_resize;
    st_ngrid->func_move = (void (*)(struct widget* st_widget, int y, int x))&ngrid_move;
    st_ngrid->func_draw = (void (*)(struct widget* st_widget))&ngrid_draw;
    st_ngrid->pad_x = pad_x;
    st_ngrid->pad_y = pad_y;
    st_ngrid->format = format;
    st_ngrid->st_matrice = st_matrice;
    st_ngrid->sel_x = 0;
    st_ngrid->sel_y = 0;

    return st_ngrid;
}

void ngrid_delete(struct ngrid* st_ngrid) {
    /* On libère la fenêtre. */
    delwin(st_ngrid->window);

    /* On libère la mémoire. */
    free(st_ngrid);
}

void ngrid_resize(struct ngrid* st_ngrid) {
    char buffer[BUFFER_SIZE];
    int size;
    int w ,h;

    /* On libère l'ancienne fenêtre. */
    if(st_ngrid->window != NULL)
        delwin(st_ngrid->window);

    /* On obtient la taille d'un nombre selon le format. */
    snprintf(buffer, BUFFER_SIZE, st_ngrid->format, 0);
    size = strlen(buffer)+1;

    /* On calcule la taille requise pour la matrice. */
    w = 2*st_ngrid->pad_x+(st_ngrid->st_matrice->x)*size;
    h = 2*st_ngrid->pad_y+(st_ngrid->st_matrice->y);

    /* On crée la fenêtre. */
    st_ngrid->window = newwin(h, w-1, 0, 0);
}

void ngrid_move(struct ngrid* st_ngrid, int x, int y) {
    /* On bouge la fenêtre. */
    mvwin(st_ngrid->window, y, x);
}

#define d_matrice st_ngrid->st_matrice
void ngrid_draw(struct ngrid* st_ngrid) {
    int i, j;

    /* On efface la fenêtre. */
    werase(st_ngrid->window);

    /* On rajoute le pad en y. */
    for(j = 0; j < st_ngrid->pad_y; j++)
        wprintw(st_ngrid->window, "\n");

    /* On écrit la matrice. */
    for(j = 0; j < d_matrice->y; j++) {
        /* On rajoute le pad en x. */
        for(i = 0; i < st_ngrid->pad_x; i++)
            wprintw(st_ngrid->window, " ");

        /* On écrit la ligne. */
        for(i = 0; i < d_matrice->x; i++) {
            if(st_ngrid->sel_x == i && st_ngrid->sel_y == j)
                wattron(st_ngrid->window, A_STANDOUT);

            wprintw(st_ngrid->window, st_ngrid->format, d_matrice->data[i+j*d_matrice->x]);
            wattroff(st_ngrid->window, A_STANDOUT);

            if(i < d_matrice->x-1)
                wprintw(st_ngrid->window, " ");
        }
        wnewline(st_ngrid->window);
    }

    /* On actualise la fenêtre. */
    touchwin(st_ngrid->window);
    wrefresh(st_ngrid->window);
}
#undef d_matrice

void ngrid_change_matrice(struct ngrid* st_ngrid, struct matrice* st_matrice) {
    /* On libère la fenêtre. */
    delwin(st_ngrid->window);

    /* On configure la structure. */
    st_ngrid->st_matrice = st_matrice;

    /* On recrée la fenêtre. */
    ngrid_resize(st_ngrid);

    /* On actualise la fenêtre mère. */
    if(st_ngrid->parent != NULL)
        (*st_ngrid->parent->func_resize)(st_ngrid->parent);
}

void ngrid_change_selection(struct ngrid* st_ngrid, int sel_x, int sel_y) {
    /* On configure la structure. */
    st_ngrid->sel_x = sel_x;
    st_ngrid->sel_y = sel_y;
}
