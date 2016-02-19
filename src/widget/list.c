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
    st_nlist->child = NULL;
    st_nlist->childs = 0;
    st_nlist->func_delete = (void (*)(struct widget* st_widget))&nlist_delete;
    st_nlist->func_resize = (void (*)(struct widget* st_widget))&nlist_resize;
    st_nlist->func_move = (void (*)(struct widget* st_widget, int x, int y))&nlist_move;
    st_nlist->func_draw = (void (*)(struct widget* st_widget))&nlist_draw;
    st_nlist->resize = TRUE;
    st_nlist->draw = TRUE;
    st_nlist->pad_x = pad_x;
    st_nlist->pad_y = pad_y;
    st_nlist->size = size;
    st_nlist->sel =-1;

    return st_nlist;
}

void nlist_delete(struct nlist* st_nlist) {
    /* On libère la fenêtre. */
    delwin(st_nlist->window);

    /* On libère la mémoire. */
    free(st_nlist->options);
    free(st_nlist);
}

void nlist_resize(struct nlist* st_nlist) {
    int len;
    int w;
    int h;
    int i;

    /* On appel la fonction mère. */
    if(!widget_resize((struct widget*)st_nlist)) return;

    /* On trouve la largeur maximale. */
    for(w = i = 0; i < st_nlist->size; i++) {
        len = strlen(st_nlist->options[i]);

        if(len > w)
        w = len;
    }

    w = 2*st_nlist->pad_x+w;
    h = 2*st_nlist->pad_y+st_nlist->size;

    /* On libère l'ancienne fenêtre. */
    if(st_nlist->window != NULL) {
        delwin(st_nlist->window);
        st_nlist->window = NULL;
    }

    /* On crée la fenêtre. */
    st_nlist->window = newwin(h, w, st_nlist->pad_y, st_nlist->pad_x);
}

void nlist_move(struct nlist* st_nlist, int x, int y) {
    /* On bouge la fenêtre. */
    werase(st_nlist->window);
    mvwin(st_nlist->window, y, x);
    wrefresh(st_nlist->window);
}

void nlist_draw(struct nlist* st_nlist) {
    int j;

    /* On appel la fonction mère. */
    if(!widget_draw((struct widget*)st_nlist)) return;

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
void nlist_sel_change(struct nlist* st_nlist, int sel) {
    /* On marque la fenêtre à desinner. */
    widget_draw_set((struct widget*)st_nlist);

    /* On change la sélection. */
    st_nlist->sel = sel;
}

int nlist_sel_inc(struct nlist* st_nlist) {
    /* On s'assure de ne pas sortir des limites. */
    if(st_nlist->sel >= st_nlist->size-1)
        return FALSE;

    /* On marque la fenêtre à desinner. */
    widget_draw_set((struct widget*)st_nlist);

    /* On augmente la sélection. */
    st_nlist->sel++;
    return TRUE;
}

int nlist_sel_dec(struct nlist* st_nlist) {
    /* On s'assure de ne pas sortir des limites. */
    if(st_nlist->sel <= 0)
        return FALSE;

    /* On marque la fenêtre à desinner. */
    widget_draw_set((struct widget*)st_nlist);

    /* On diminue la sélection. */
    st_nlist->sel--;
    return TRUE;
}
