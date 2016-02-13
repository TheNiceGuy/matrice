#include <string.h>
#include "misc.h"
#include "config.h"

int int_add(int a, int b, int max) {
    if(a >= 0)
        return (a*10)%max+b;
    else
        return (a*10)%max-b;
}

int concatenate_int(int a, int b) {
    int pow = 10;

    while(b >= pow)
        pow *= 10;

    return a * pow + b;
}

void wprint_center(WINDOW* window, char* string) {
    wcenter(window, string);
    wprintw(window, "%s\n", string);
}

void wcenter(WINDOW* window, char* string) {
    int pos[2];
    int size[2];

    getyx(window, pos[1], pos[0]);
    getmaxyx(window, size[1], size[0]);

    wmove(window, pos[1], (size[0]-strlen(string))/2);
}

void wnewline(WINDOW* window) {
    int x = 0;
    int y = 0;

    getyx(window, y, x);

    if(x == 0)
        return;

    wmove(window, y+1, 0);
}
