#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "grid.h"
#include "misc.h"
#include "config.h"

struct ngrid* ngrid_new(int x, int y, int sig, char* format, int pad_x, int pad_y) {
    struct ngrid* st_ngrid;

    /* On alloue la mémoire. */
    st_ngrid = malloc(sizeof(struct ngrid));
    if(st_ngrid == NULL)
        return NULL;

    /* On configure la structure. */
    st_ngrid->window = NULL;
    st_ngrid->parent = NULL;
    st_ngrid->child = NULL;
    st_ngrid->childs = 0;
    st_ngrid->func_delete = (void (*)(struct widget* st_widget))&ngrid_delete;
    st_ngrid->func_resize = (void (*)(struct widget* st_widget))&ngrid_resize;
    st_ngrid->func_move = (void (*)(struct widget* st_widget, int x, int y))&ngrid_move;
    st_ngrid->func_draw = (void (*)(struct widget* st_widget))&ngrid_draw;
    st_ngrid->resize = TRUE;
    st_ngrid->draw = TRUE;
    st_ngrid->pad_x = pad_x;
    st_ngrid->pad_y = pad_y;
    st_ngrid->st_matrice = matrice_new(x, y);
    st_ngrid->sig = sig;
    st_ngrid->format = format;
    st_ngrid->size = format_len(format)+(st_ngrid->sig?1:0);
    st_ngrid->sel_x =-1;
    st_ngrid->sel_y =-1;

    return st_ngrid;
}

struct ngrid* ngrid_new_ptr(int x, int y, int* data, int sig, char* format, int pad_x, int pad_y) {
    struct ngrid* st_ngrid;

    /* On crée la structure. */
    st_ngrid = ngrid_new(x, y, sig, format, pad_x, pad_y);
    if(st_ngrid == NULL)
        return NULL;

    /* On remplace la matrice. */
    matrice_delete(st_ngrid->st_matrice);
    st_ngrid->st_matrice = matrice_new_ptr(x, y, data);

    return st_ngrid;
}

void ngrid_delete(struct ngrid* st_ngrid) {
    /* On libère la fenêtre. */
    delwin(st_ngrid->window);

    /* On libère la mémoire. */
    matrice_delete(st_ngrid->st_matrice);
    free(st_ngrid);
}

void ngrid_resize(struct ngrid* st_ngrid) {
    int w ,h;

    /* On appel la fonction mère. */
    if(!widget_resize((struct widget*)st_ngrid)) return;

    /* On calcule la taille requise pour la matrice. */
    w = 2*st_ngrid->pad_x+(st_ngrid->st_matrice->x)*st_ngrid->size+(st_ngrid->sig?0:2);
    h = 2*st_ngrid->pad_y+(st_ngrid->st_matrice->y);

    /* On libère l'ancienne fenêtre. */
    if(st_ngrid->window != NULL) {
        delwin(st_ngrid->window);
        st_ngrid->window = NULL;
    }

    /* On crée la fenêtre. */
    st_ngrid->window = newwin(h, w-1, 0, 0);
}

void ngrid_move(struct ngrid* st_ngrid, int x, int y) {
    /* On bouge la fenêtre. */
    werase(st_ngrid->window);
    mvwin(st_ngrid->window, y, x);
    wrefresh(st_ngrid->window);
}

#define d_matrice st_ngrid->st_matrice
void ngrid_draw(struct ngrid* st_ngrid) {
    int i, j;

    /* On appel la fonction mère. */
    if(!widget_draw((struct widget*)st_ngrid)) return;

    /* On rajoute le pad en y. */
    wmoverel(st_ngrid->window, 0, st_ngrid->pad_y);

    /* On écrit la matrice. */
    for(j = 0; j < d_matrice->y; j++) {
        /* On rajoute le pad en x. */
        wmoverel(st_ngrid->window, st_ngrid->pad_x, 0);

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

void ngrid_change(struct ngrid* st_ngrid, int x, int y) {
    /* On marque la fenêtre à recréer. */
    widget_resize_set((struct widget*)st_ngrid);

    /* On supprime l'ancienne matrice. */
    matrice_delete(st_ngrid->st_matrice);

    /* On crée la nouvelle matrice. */
    st_ngrid->st_matrice = matrice_new(x, y);

    /* On actualise la fenêtre mère. */
    if(st_ngrid->parent != NULL)
        (*st_ngrid->parent->func_resize)(st_ngrid->parent);

    /* On marque la fenêtre à desinner. */
    widget_resize_set((struct widget*)st_ngrid);
}

void ngrid_sel_change(struct ngrid* st_ngrid, int sel_x, int sel_y) {
    /* On marque la fenêtre à desinner. */
    widget_draw_set((struct widget*)st_ngrid);

    /* On change la sélection. */
    st_ngrid->sel_x = sel_x;
    st_ngrid->sel_y = sel_y;
}

int ngrid_sel_inc_x(struct ngrid* st_ngrid) {
    /* On s'assure de ne pas sortir des limites. */
    if(st_ngrid->sel_x >= st_ngrid->st_matrice->x-1) {
        /* On regarde si on peut changer de ligne. */
        if(st_ngrid->sel_y < st_ngrid->st_matrice->y-1){
            st_ngrid->sel_x = 0;
            st_ngrid->sel_y++;
        } else {
            return FALSE;
        }
    } else {
        st_ngrid->sel_x++;
    }

    /* On marque la fenêtre à desinner. */
    widget_draw_set((struct widget*)st_ngrid);

    return TRUE;
}

int ngrid_sel_dec_x(struct ngrid* st_ngrid) {
    /* On s'assure de ne pas sortir des limites. */
    if(st_ngrid->sel_x <= 0) {
        /* On regarde si on peut changer de ligne. */
        if(st_ngrid->sel_y > 0){
            st_ngrid->sel_x = st_ngrid->st_matrice->x-1;
            st_ngrid->sel_y--;
        } else {
            return FALSE;
        }
    } else {
        /* On diminue la sélection en x. */
        st_ngrid->sel_x--;
    }

    /* On marque la fenêtre à desinner. */
    widget_draw_set((struct widget*)st_ngrid);

    return TRUE;
}

int ngrid_sel_inc_y(struct ngrid* st_ngrid) {
    /* On s'assure de ne pas sortir des limites. */
    if(st_ngrid->sel_y >= st_ngrid->st_matrice->y-1)
        return FALSE;

    /* On marque la fenêtre à desinner. */
    widget_draw_set((struct widget*)st_ngrid);

    /* On diminue la sélection en y. */
    st_ngrid->sel_y++;
    return TRUE;
}

int ngrid_sel_dec_y(struct ngrid* st_ngrid) {
    /* On s'assure de ne pas sortir des limites. */
    if(st_ngrid->sel_y <= 0)
        return FALSE;

    /* On marque la fenêtre à desinner. */
    widget_draw_set((struct widget*)st_ngrid);

    /* On diminue la sélection en y. */
    st_ngrid->sel_y--;
    return TRUE;
}

#define d_matrice st_ngrid->st_matrice
#define d_cell d_matrice->data[st_ngrid->sel_x+st_ngrid->sel_y*d_matrice->x]
int ngrid_get(struct ngrid* st_ngrid) {
    return d_cell;
}

void ngrid_set(struct ngrid* st_ngrid, int val) {
    d_cell = val;
}

void ngrid_cat(struct ngrid* st_ngrid, char c) {
    int power = 10;
    int num;

    /* On marque la fenêtre à dessinner. */
    widget_draw_set((struct widget*)st_ngrid);

    /* On change le signe du nombre si nécéssaire.*/
    if(c == '-') {
        if(st_ngrid->sig)
            d_cell *=  -1;
        return;
    }

    /* On obtient la valeur du caractère. */
    num = c-'0';
    while(num >= power)
        power *= 10;

    /* On ajoute le caractère et on coupe les caractères en trop. */
    if(d_cell < 0)
        d_cell = -((-d_cell*power+num)%(int)pow(10, st_ngrid->size-2));
    else
        d_cell = (d_cell*power+num)%(int)pow(10, st_ngrid->size-(st_ngrid->sig?2:0));

}
#undef d_matrice
#undef d_cell
