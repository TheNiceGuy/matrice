#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "misc.h"
#include "config.h"

struct nlist* nlist_new(char** options, int size, int pad_x, int pad_y) {
    struct nlist* st_nlist;

    /* On alloue la mémoire. */
    st_nlist = malloc(sizeof(struct nlist));
    if(st_nlist == NULL)
        return NULL;

    /* On alloue la mémoire pour les options. */
    st_nlist->options = malloc(size*sizeof(char**));
    if(st_nlist->options == NULL)
        return NULL;

    /* On copie les options. */
    memcpy(st_nlist->options, options, size*sizeof(char**));

    /* On configure la structure. */
    st_nlist->window = NULL;
    st_nlist->parent = NULL;
    st_nlist->func_delete = (void (*)(struct widget* st_widget))&nlist_delete;
    st_nlist->func_resize = (void (*)(struct widget* st_widget))&nlist_resize;
    st_nlist->func_move = (void (*)(struct widget* st_widget, int y, int x))&nlist_move;
    st_nlist->func_draw = (void (*)(struct widget* st_widget))&nlist_draw;
    st_nlist->pad_x = pad_x;
    st_nlist->pad_y = pad_y;
    st_nlist->size = size;
    st_nlist->sel = 0;

    return st_nlist;
}

void nlist_delete(struct nlist* st_nlist) {
    /* On libère la fenêtre. */
    delwin(st_nlist->window);

    /* On libère la mémoire. */
    free(st_nlist);
}

void nlist_resize(struct nlist* st_nlist) {
    int len;
    int max;
    int i;

    /* On libère l'ancienne fenêtre. */
    if(st_nlist->window != NULL)
        delwin(st_nlist->window);

    /* On trouve la largeur maximale. */
    for(max = i = 0; i < st_nlist->size; i++) {
        len = strlen(st_nlist->options[i]);

        if(len > max)
            max = len+2*st_nlist->pad_x;
    }

    /* On crée la fenêtre. */
    st_nlist->window = newwin(st_nlist->size+2*st_nlist->pad_y, max, 0, 0);
}

void nlist_move(struct nlist* st_nlist, int x, int y) {
    /* On bouge la fenêtre. */
    mvwin(st_nlist->window, y, x);
}

void nlist_draw(struct nlist* st_nlist) {
    int j;

    /* On efface la fenêtre. */
    werase(st_nlist->window);

    /* On rajoute le pad en y. */
    for(j = 0; j < st_nlist->pad_y; j++)
        wprintw(st_nlist->window, "\n");

    for(j = 0; j < st_nlist->size; j++) {

        wcenter(st_nlist->window, st_nlist->options[j]);

        if(st_nlist->sel == j)
            wattron(st_nlist->window, A_STANDOUT);

        wprintw(st_nlist->window, "%s", st_nlist->options[j]);
        wnewline(st_nlist->window);
        wattroff(st_nlist->window, A_STANDOUT);
    }

    /* On actualise la fenêtre. */
    touchwin(st_nlist->window);
    wrefresh(st_nlist->window);
}
