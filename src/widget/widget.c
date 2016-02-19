#include <ncurses.h>
#include "widget.h"
#include "config.h"

int widget_resize(struct widget* st_widget) {
    int i;

    /* On recrée les enfants. */
    if(st_widget->child != NULL) {
        for(i = 0; i < st_widget->childs; i++) {
            if(st_widget->child[i] == NULL)
                continue;

            (*st_widget->child[i]->func_resize)(st_widget->child[i]);
        }
    }

    /* On s'assure que la fenêtre doit être recréer. */
    if(!st_widget->resize)
        return FALSE;

        wclear(st_widget->window);
        wrefresh(st_widget->window);

    /* On libère l'ancienne fenêtre. */
    //if(st_widget->window != NULL) {
//        delwin(st_widget->window);
//        st_widget->window = NULL;
    //}

    /* On marque la fenêtre comme recréer. */
    st_widget->resize = FALSE;

    /* On marque la fenêtre à dessinner. */
    widget_draw_set(st_widget);
    return TRUE;
}

void widget_resize_set(struct widget* st_widget) {

    /* On marque la fenêtre mère comme à recréer. */
    if(st_widget->parent != NULL)
        widget_resize_set(st_widget->parent);

    /* On marque cette fenêtre comme à recréer. */
    st_widget->resize = TRUE;
}

int widget_draw(struct widget* st_widget) {
    int i;

    /* On dessine les enfants. */
    if(st_widget->child != NULL) {
        for(i = 0; i < st_widget->childs; i++) {
            if(st_widget->child[i] == NULL)
                continue;

            (*st_widget->child[i]->func_draw)(st_widget->child[i]);
        }
    }

    /* On s'assure que la fenêtre doit être redessinée. */
    if(!st_widget->draw)
        return FALSE;

    /* On efface la fenêtre. */
    werase(st_widget->window);

    /* On dessine une boite s'il y a un pad. */
    if(st_widget->pad_x > 0 && st_widget->pad_y > 0)
        box(st_widget->window, 0, 0);

    /* On marque la fenêtre comme étant redessinée. */
    st_widget->draw = FALSE;

    return TRUE;
}

void widget_draw_set(struct widget* st_widget) {
    int i;

    /* On marque la fenêtre à redessiner. */
    st_widget->draw = TRUE;

    /* On marque les enfants comme à redessiner. */
    if(st_widget->child != NULL)
        for(i = 0; i < st_widget->childs; i++)
            widget_draw_set(st_widget->child[i]);
}
