#ifndef WIDGET_INCLUDE_H
#define WIDGET_INCLUDE_H

#include <ncurses.h>

struct widget {
    WINDOW* window;
    struct widget* parent;
    struct widget** child;
    int childs;
    void (*func_delete)(struct widget* st_widget);
    void (*func_resize)(struct widget* st_widget);
    void (*func_move)(struct widget* st_widget, int x, int y);
    void (*func_draw)(struct widget* st_widget);
    int resize;
    int draw;
    int pad_x;
    int pad_y;
};

int widget_resize(struct widget* st_widget);
void widget_resize_set(struct widget* st_widget);
int widget_draw(struct widget* st_widget);
void widget_draw_set(struct widget* st_widget);

#endif
