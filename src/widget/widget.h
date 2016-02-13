#ifndef WIDGET_INCLUDE_H
#define WIDGET_INCLUDE_H

#include <ncurses.h>

struct widget {
    WINDOW* window;
    struct widget* parent;
    void (*func_delete)(struct widget* st_widget);
    void (*func_resize)(struct widget* st_widget);
    void (*func_move)(struct widget* st_widget, int x, int y);
    void (*func_draw)(struct widget* st_widget);
    int pad_x;
    int pad_y;
};

#endif
