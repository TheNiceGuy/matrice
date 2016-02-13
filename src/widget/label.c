#include <stdlib.h>
#include <string.h>
#include "label.h"
#include "misc.h"
#include "config.h"

struct nlabel* nlabel_new(char* string, int pad_x, int pad_y) {
    struct nlabel* st_nlabel;

    /* On alloue la mémoire. */
    st_nlabel = malloc(sizeof(struct nlabel));
    if(st_nlabel == NULL)
        return NULL;

    /* On configure la structure. */
    st_nlabel->window = NULL;
    st_nlabel->parent = NULL;
    st_nlabel->func_delete = (void (*)(struct widget* st_widget))&nlabel_delete;
    st_nlabel->func_resize = (void (*)(struct widget* st_widget))&nlabel_resize;
    st_nlabel->func_move = (void (*)(struct widget* st_widget, int y, int x))&nlabel_move;
    st_nlabel->func_draw = (void (*)(struct widget* st_widget))&nlabel_draw;
    st_nlabel->pad_x = pad_x;
    st_nlabel->pad_y = pad_y;
    st_nlabel->string = string;

    return st_nlabel;
}

void nlabel_delete(struct nlabel* st_nlabel) {
    /* On libère la fenêtre. */
    delwin(st_nlabel->window);

    /* On libère la mémoire. */
    free(st_nlabel);
}

void nlabel_resize(struct nlabel* st_nlabel) {
    int w;
    int h;

    /* On libère l'ancienne fenêtre. */
    if(st_nlabel->window != NULL)
        delwin(st_nlabel->window);

    /* On calcul la taille de la fenêtre. */
    w = 2*st_nlabel->pad_x+strlen(st_nlabel->string);
    h = 2*st_nlabel->pad_y+1;

    /* On crée la fenêtre. */
    st_nlabel->window = newwin(h, w, 0, 0);
}

void nlabel_move(struct nlabel* st_nlabel, int x, int y) {
    /* On bouge la fenêtre. */
    mvwin(st_nlabel->window, y, x);
}

void nlabel_draw(struct nlabel* st_nlabel) {
    int j;

    /* On efface la fenêtre. */
    werase(st_nlabel->window);

    /* On rajoute le pad en y. */
    for(j = 0; j < st_nlabel->pad_y; j++)
        wprintw(st_nlabel->window, "\n");

    wcenter(st_nlabel->window, st_nlabel->string);
    wprintw(st_nlabel->window, "%s", st_nlabel->string);

    /* On actualise la fenêtre. */
    touchwin(st_nlabel->window);
    wrefresh(st_nlabel->window);
}
