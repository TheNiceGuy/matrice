#ifndef PANEL_INCLUDE_H
#define PANEL_INCLUDE_H

#include "widget.h"

enum direction {
    HORIZONTAL,
    VERTICAL
};

struct npanel {
    struct widget;
    enum direction type;
};

struct npanel* npanel_new(int size, enum direction type, int pad_x, int pad_y);
void npanel_delete(struct npanel* st_npanel);
void npanel_resize(struct npanel* st_npanel);
void npanel_move(struct npanel* st_npanel, int x, int y);
void npanel_draw(struct npanel* st_npanel);
void npanel_set_child(struct npanel* st_npanel, struct widget* child, int index);

#endif
