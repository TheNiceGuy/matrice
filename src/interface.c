#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "interface.h"
#include "widget/panel.h"
#include "widget/grid.h"
#include "widget/list.h"
#include "widget/label.h"
#include "matrice.h"
#include "buffer.h"
#include "misc.h"
#include "vector.h"
#include "config.h"

struct interface* interface_new() {
    struct interface* ui;

    /* On alloue la mémoire. */
    ui = malloc(sizeof(struct interface));
    if(ui == NULL)
        return NULL;

    /* On obtient la taille du terminale. */
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ui->taille);

    /* TODO: vérifier la taille */

    ui->name = vector_new(LIST_SIZE);
    ui->matrice = vector_new(LIST_SIZE);

    /* On crée le panel définition. */
    ui->def_buf_name = buffer_new(10, 10);
    ui->def = npanel_new(3, VERTICAL, 1, 1);
    ui->def_p_name = npanel_new(2, HORIZONTAL, 0, 0);
    ui->def_p_dim = npanel_new(2, HORIZONTAL, 0, 0);
    ui->def_la_name = nlabel_new("Name: ", 0, 0);
    ui->def_la_name_str = nlabel_new(ui->def_buf_name->data, 0, 0);
    ui->def_la_dim = nlabel_new("Dimensions: ", 0, 0);
    ui->def_g_dim = ngrid_new_ptr(2, 1, (int[]){UI_MAT_X, UI_MAT_Y}, FALSE, "%1d", 0, 0);
    ui->def_g_mat = ngrid_new(UI_MAT_X, UI_MAT_Y, TRUE, "%3d", 0, 0);

nlabel_change_string(ui->def_la_dim, "TOP FUCKING KEK");

    /* On lie les élèments du panel définition. */
    npanel_set_child(ui->def_p_name, (struct widget*)ui->def_la_name, 0);
    npanel_set_child(ui->def_p_name, (struct widget*)ui->def_la_name_str, 1);
    npanel_set_child(ui->def_p_dim, (struct widget*)ui->def_la_dim, 0);
    npanel_set_child(ui->def_p_dim, (struct widget*)ui->def_g_dim, 1);
    npanel_set_child(ui->def, (struct widget*)ui->def_p_name, 0);
    npanel_set_child(ui->def, (struct widget*)ui->def_p_dim, 1);
    npanel_set_child(ui->def, (struct widget*)ui->def_g_mat, 2);

    /* On crée le panel principal. */
    ui->main = npanel_new(3, VERTICAL, 0, 0);
    ui->main_ls_actions = nlist_new((char*[])OPERATION, 6, 0, 0);
    ui->main_la_msg = nlabel_new("asdasdasd", 0, 0);

    /* On lie les élèments du panel principal. */
    npanel_set_child(ui->main, (struct widget*)ui->main_ls_actions, 0);
    npanel_set_child(ui->main, (struct widget*)ui->def, 1);
    npanel_set_child(ui->main, (struct widget*)ui->main_la_msg, 2);
    npanel_resize(ui->main);

    keypad(ui->main_ls_actions->window, TRUE);
    keypad(ui->def_la_name_str->window, TRUE);
    keypad(ui->def_g_dim->window, TRUE);
    curs_set(0);

    return ui;
}

void interface_delete(struct interface* ui) {
    int i;

    for(i = 0; i < ui->name->current; i++) {
        free(vector_get_pointer(ui->name, i));
        matrice_delete(vector_get_pointer(ui->matrice, i));
    }
    vector_delete(ui->name);
    vector_delete(ui->matrice);

    npanel_delete(ui->main);
    buffer_delete(ui->def_buf_name);
    free(ui);
}

void interface_main(struct interface* ui) {
    int key;

    nlist_sel_change(ui->main_ls_actions, 0);

    do {
        npanel_resize(ui->main);
        npanel_draw(ui->main);

        key = wgetch(ui->main_ls_actions->window);
        switch(key) {
            case KEY_UP:
                nlist_sel_dec(ui->main_ls_actions);
                break;
            case KEY_DOWN:
                nlist_sel_inc(ui->main_ls_actions);
                break;
            case 10:
                if(ui->main_ls_actions->sel == ACTION_QUIT)
                    goto QUIT;

                interface_choisir(ui, ui->main_ls_actions->sel);

                break;
        }
    } while(TRUE);

QUIT:
    return;
}

void interface_choisir(struct interface* ui, int index) {
    switch(index) {
        case ACTION_DEFINE:
            nlist_sel_change(ui->main_ls_actions, -1);
            interface_def_name(ui);
            interface_grid_dim(ui, ui->def_g_dim);
            interface_grid(ui, ui->def_g_mat);

            vector_push_pointer(ui->name, buffer_get_string(ui->def_buf_name));
            vector_push_pointer(ui->matrice, matrice_copy(ui->def_g_mat->st_matrice));

            nlist_sel_change(ui->main_ls_actions, 0);
            break;
        case ACTION_ADD:

            break;
        case ACTION_MUL:

            break;
        case ACTION_TRANS:

            break;
        case ACTION_DET:

            break;
        case ACTION_QUIT:
            break;
    }
}

void interface_def_name(struct interface* ui) {
    int key;

    do {
        npanel_resize(ui->main);
        npanel_draw(ui->main);

        key = wgetch(ui->def_la_name_str->window);
        switch(key) {
            case KEY_BACKSPACE:
            case KEY_DC:
            case 127:
                buffer_backspace(ui->def_buf_name);
                nlabel_change_string(ui->def_la_name_str, ui->def_buf_name->data);
                break;
            case '_':
            case '0' ... '9':
            case 'a' ... 'z':
            case 'A' ... 'Z':
                buffer_append_char(ui->def_buf_name, key);
                nlabel_change_string(ui->def_la_name_str, ui->def_buf_name->data);
                break;
            case KEY_ENTER:
            case 10:
                return;
        }
    } while(TRUE);
}

void interface_grid(struct interface* ui, struct ngrid* st_ngrid) {
    int key;

    keypad(st_ngrid->window, TRUE);
    ngrid_sel_change(st_ngrid, 0, 0);

    do {
        npanel_resize(ui->main);
        npanel_draw(ui->main);

        key = wgetch(st_ngrid->window);
        switch(key) {
            case KEY_UP:
                ngrid_sel_dec_y(st_ngrid);
                break;
            case KEY_DOWN:
                ngrid_sel_inc_y(st_ngrid);
                break;
            case KEY_LEFT:
                ngrid_sel_dec_x(st_ngrid);
                break;
            case KEY_RIGHT:
                ngrid_sel_inc_x(st_ngrid);
                break;
            case '-':
            case '0' ... '9':
                ngrid_cat(st_ngrid, key);
                break;
            case KEY_ENTER:
            case 10:
                ngrid_sel_change(st_ngrid,-1,-1);
                return;
        }
    } while(TRUE);
}

void interface_grid_dim(struct interface* ui, struct ngrid* st_ngrid) {
    int key;

    keypad(st_ngrid->window, TRUE);
    ngrid_sel_change(st_ngrid, 0, 0);

    do {
        npanel_resize(ui->main);
        npanel_draw(ui->main);

        key = wgetch(st_ngrid->window);
        switch(key) {
            case KEY_UP:
                ngrid_sel_dec_y(st_ngrid);
                break;
            case KEY_DOWN:
                ngrid_sel_inc_y(st_ngrid);
                break;
            case KEY_LEFT:
                ngrid_sel_dec_x(st_ngrid);
                break;
            case KEY_RIGHT:
                ngrid_sel_inc_x(st_ngrid);
                break;
            case '0' ... '9':
                ngrid_cat(st_ngrid, key);
                if(ngrid_get(st_ngrid) == 0)
                    ngrid_set(st_ngrid, 1);

                ngrid_change(ui->def_g_mat, ui->def_g_dim->st_matrice->data[0],
                                            ui->def_g_dim->st_matrice->data[1]);
                break;
            case KEY_ENTER:
            case 10:
                ngrid_sel_change(st_ngrid,-1,-1);
                return;
        }
    } while(TRUE);
}

#if 0

int interface_choisir() {
    int boucle = TRUE;
    int input, opts;
    char** op = OPERATION;

    do {
        /* On efface les fenêtre. */
        werase(operation);
        werase(statut);

        /* On écrit les fenêtres. */
        wprint_center(statut, "Please choose your action.");
        for(opts = 0; opts < OPERATION_NB; opts++) {
            if(choix == opts) {
                wattron(operation, A_STANDOUT);
                wprint_center(operation, op[opts]);
                wattroff(operation, A_STANDOUT);
            } else {
                wprint_center(operation, op[opts]);
            }
        }

        /* On actualise le terminal. */
        wrefresh(statut);

        /* On attent pour l'entrée de l'utilisateur. */
        input = wgetch(operation);
        switch(input) {
            case KEY_UP:
                if(choix > 0)
                    choix--;
                break;
            case KEY_DOWN:
                if(choix < OPERATION_NB-1)
                    choix++;
                break;
            case 10:
                boucle = FALSE;
                break;
        }
    } while(boucle);

    return choix;
}

#define DATA new->data
#define X    new->x
#define Y    new->y
#define DIM_X dim->data[0]
#define DIM_Y dim->data[1]
void interface_define_matrice() {
    char string[BUFFER_SIZE];
    char cat[BUFFER_SIZE];
    char nom[BUFFER_SIZE];
    int boucle = TRUE;
    int dim_x = 2;
    int dim_y = 2;
    int sel_x = 0;
    int sel_y = 0;
    int input;
    int i, j;
    struct matrice* dim = matrice_new_ptr(2, 1, (int[]){2, 2});
    struct matrice* new = matrice_new(DIM_X, DIM_Y);

    nom[0] = '\0';
    do {
        /* On efface les fenêtre. */
        werase(modification);
        werase(statut);

        /* On actualise la matrice si elle a changée. */
        if(X != DIM_X || Y != DIM_Y) {
            matrice_delete(new);
            new = matrice_new(DIM_X, DIM_Y);
        }

        /* On aligne les dimensions. */
        snprintf(string, BUFFER_SIZE, "Nom: %s", nom);
        wprint_center(modification, string);

        /* On aligne les dimensions. */
        snprintf(string, BUFFER_SIZE, "Dimensions: %d X %d", DIM_X, DIM_Y);
        wcenter(modification, string);
        wprintw(modification, "Dimensions: ");

        /* On écrit la matrice. */
        wprint_matrice(modification, dim, sel_x, sel_y-1, FALSE, "%d");
        wprint_matrice(modification, new, sel_x, sel_y-2, TRUE, "%3d");

        /* On actualise le terminal. */
        wrefresh(statut);

        /* On attent pour l'entrée de l'utilisateur. */
        input = wgetch(modification);
        switch(input) {
            /* On bouge le curseur vers le haut. */
            case KEY_UP:
                sel_y--;
                break;

            /* On bouge le curseur vers le bas. */
            case KEY_DOWN:
                sel_y++;
                break;

            /* On bouge le curseur vers la gauche. */
            case KEY_LEFT:
                sel_x--;
                break;

            /* On bouge le curseur vers la droite. */
            case KEY_RIGHT:
                sel_x++;
                break;

            /* On gère l'entré d'un nombre. */
            case '0' ... '9':
                if(sel_y == 0) {

                } else if(sel_y == 1) {
                    if(sel_x == 0) {
                        DIM_X = input-'0';
                        if(DIM_X == 0)
                            DIM_X = 1;
                    } else {
                        DIM_Y = input-'0';
                        if(DIM_Y == 0)
                            DIM_Y = 1;
                    }
                } else {
                    DATA[sel_x+(sel_y-2)*X] = int_add(DATA[sel_x+(sel_y-2)*X], input-'0', 100);
                    if(DATA[sel_x+(sel_y-2)*X] > MAX_SIZE)
                        DATA[sel_x+(sel_y-2)*X] = MAX_SIZE;
                }
                break;

            case 'a' ... 'z':
            case 'A' ... 'Z':
                break;

            /* On bascule le négatif dans la matrice. */
            case '-':
                if(sel_y > 1)
                    DATA[sel_x+(sel_y-2)*X] *= -1;
                break;

            /* TODO: fix that */
            case 10:
                boucle = FALSE;
                break;
        }

        /* On ajuste le curseur afin qu'il ne dépasse pas les bornes. */
        if(sel_y  == 0) {
            if(sel_x <= 0) {
                sel_x = 0;
            } else {
                sel_x = 0;
                sel_y++;
            }
        } else if(sel_y  == 1) {
            if(sel_x < 0) {
                sel_x = 0;
                sel_y--;
            } else if(sel_x > 1) {
                sel_x = 0;
                sel_y++;
            }
        } else {
            if(sel_x < 0) {
                if(sel_y == 2) {
                    sel_x = 1;
                    sel_y--;
                } else {
                    sel_x = DIM_X-1;
                    sel_y--;
                }
            } else if(sel_x >= DIM_X) {
                if(sel_y-2 >= DIM_Y-1) {
                    sel_x--;
                } else {
                    sel_x = 0;
                    sel_y++;
                }
            }
        }
    } while(boucle);
}
#endif
