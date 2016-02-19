#ifndef LABEL_INCLUDE_H
#define LABEL_INCLUDE_H

#include <ncurses.h>
#include "widget.h"

struct nlabel {
    struct widget;
    char* string;
};

struct nlabel* nlabel_new(char* string, int pad_x, int pad_y);
void nlabel_delete(struct nlabel* st_nlabel);
void nlabel_resize(struct nlabel* st_nlabel);
void nlabel_move(struct nlabel* st_nlabel, int x, int y);
void nlabel_draw(struct nlabel* st_nlabel);
void nlabel_change_string(struct nlabel* st_nlabel, char* string);

#endif
