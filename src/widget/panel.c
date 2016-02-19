#include <stdlib.h>
#include "panel.h"
#include "config.h"

struct npanel* npanel_new(int size, enum direction type, int pad_x, int pad_y) {
    struct npanel* st_npanel;

    /* On alloue la mémoire. */
    st_npanel = malloc(sizeof(struct npanel));
    if(st_npanel == NULL)
        return NULL;

    /* On alloue la mémoire pour les pointeurs de widget. */
    st_npanel->child = calloc(size, sizeof(struct widget*));
    if(st_npanel->child == NULL)
        return NULL;

    /* On configure la structure. */
    st_npanel->window = NULL;
    st_npanel->parent = NULL;
    st_npanel->childs = size;
    st_npanel->func_delete = (void (*)(struct widget* st_widget))&npanel_delete;
    st_npanel->func_resize = (void (*)(struct widget* st_widget))&npanel_resize;
    st_npanel->func_move = (void (*)(struct widget* st_widget, int x, int y))&npanel_move;
    st_npanel->func_draw = (void (*)(struct widget* st_widget))&npanel_draw;
    st_npanel->resize = TRUE;
    st_npanel->draw = TRUE;
    st_npanel->pad_x = pad_x;
    st_npanel->pad_y = pad_y;
    st_npanel->type = type;

    return st_npanel;
}

void npanel_delete(struct npanel* st_npanel) {
    int i;

    /* On libère la mémoire des sous widgets. */
    for(i = 0; i < st_npanel->childs; i++) {
        /* On ignore si l'enfant n'est pas définit. */
        if(st_npanel->child[i] == NULL)
            continue;

        (*st_npanel->child[i]->func_delete)(st_npanel->child[i]);
    }

    /* On libère la mémoire du panel. */
    free(st_npanel->child);
    free(st_npanel);
}

void npanel_resize(struct npanel* st_npanel) {
    int i;
    int* x;
    int* y;
    int win_w = 0;
    int win_h = 0;
    int subwin_w;
    int subwin_h;

    /* On appel la fonction mère. */
    if(!widget_resize((struct widget*)st_npanel)) return;

    /* On alloue la mémoire pour la position des fenêtres. */
    x = malloc(st_npanel->childs*sizeof(int));
    y = malloc(st_npanel->childs*sizeof(int));

    /* On calcule la taille requise pour le panel. */
    for(i = 0; i < st_npanel->childs; i++) {
        /* On ignore si l'enfant n'est pas définit. */
        if(st_npanel->child[i] == NULL)
            continue;

        /* On obtient la tailles de la sous fenêtres. */
        getmaxyx(st_npanel->child[i]->window, subwin_h, subwin_w);

        if(st_npanel->type == HORIZONTAL) {
            /* On configure la position de la sous-fenêtre. */
            x[i] = win_w+st_npanel->pad_x;

            /* On additione la largeur de chaque sous-fenêtre. */
            win_w += subwin_w;

            /* On trouve la hauteur nécéssaire. */
            if(win_h < subwin_h)
                win_h = subwin_h;
        } else {
            /* On configure la position de la sous-fenêtre. */
            y[i] = win_h+st_npanel->pad_y;

            /* On additione la hauteur de chaque sous-fenêtre. */
            win_h += subwin_h;

            /* On trouve la largeur nécéssaire. */
            if(win_w < subwin_w)
                win_w = subwin_w;
        }
    }

    win_w += 2*st_npanel->pad_x;
    win_h += 2*st_npanel->pad_y;

    /* On centre les sous fenêtres. */
    for(i = 0; i < st_npanel->childs; i++) {
        /* On ignore si l'enfant n'est pas définit. */
        if(st_npanel->child[i] == NULL)
            continue;

        /* On obtient la tailles des sous fenêtres. */
        getmaxyx(st_npanel->child[i]->window, subwin_h, subwin_w);

        /* On configure la position de la sous-fenêtre. */
        if(st_npanel->type == HORIZONTAL)
            y[i] = (win_h-subwin_h)/2;
        else
            x[i] = (win_w-subwin_w)/2;

        /* On positionne la fenêtre. */
        (*st_npanel->child[i]->func_move)(st_npanel->child[i], x[i], y[i]);
    }

    /* On libère la mémoire. */
    free(x);
    free(y);

    /* TODO: fix that*/
    delwin(st_npanel->window);
    st_npanel->window = NULL;
    if(st_npanel->window == NULL) {
        st_npanel->window = newwin(win_h, win_w, 0, 0);
    } else {
        getmaxyx(st_npanel->window, subwin_h, subwin_w);
        //printf("%d %d %d %d\n", subwin_h, win_h, subwin_w+1, win_w);
        if(subwin_w != win_w || subwin_h != win_h)
            st_npanel->window = newwin(win_h, win_w, 0, 0);
        else
            st_npanel->window = newwin(win_h, win_w, 0, 0);
    }
    //st_npanel->window = newwin(win_h, win_w, 0, 0);
}

void npanel_move(struct npanel* st_npanel, int x, int y) {
    int i;
    int n_x;
    int n_y;
    int d_x;
    int d_y;

    /* On calcule la différentece. */
    getbegyx(st_npanel->window, n_y, n_x);
    d_x = x-n_x;
    d_y = y-n_y;

    mvwin(st_npanel->window, y, x);
    werase(st_npanel->window);
    wrefresh(st_npanel->window);

    for(i = 0; i < st_npanel->childs; i++) {
        /* On bouge les sous fenêtres. */
        getbegyx(st_npanel->child[i]->window, n_y, n_x);
        (*st_npanel->child[i]->func_move)(st_npanel->child[i], n_x+d_x, n_y+d_y);
    }
}

void npanel_draw(struct npanel* st_npanel) {
    st_npanel->draw = FALSE;
    widget_draw((struct widget*)st_npanel);
}

void npanel_set_child(struct npanel* st_npanel, struct widget* child, int index) {
    /* On ignore si l'enfant n'est pas définit. */
    if(child == NULL)
        return;

    /* On marque la fenêtre à recréer. */
    widget_resize_set((struct widget*)st_npanel);

    st_npanel->child[index] = child;
    st_npanel->child[index]->parent = (struct widget*)st_npanel;
}
