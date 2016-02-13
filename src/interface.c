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
#include "misc.h"
#include "config.h"

struct interface {
    struct winsize taille;

    struct npanel* main;
    struct nlist* main_ls_actions;
    struct nlabel* main_la_msg;

    struct npanel* def;
    struct npanel* def_p_name;
    struct npanel* def_p_dim;
    struct nlabel* def_la_name;
    struct nlabel* def_la_name_str;
    struct nlabel* def_la_dim;
    struct matrice* def_mat;
    struct matrice* def_mat_dim;
    struct ngrid* def_g_mat;
    struct ngrid* def_g_dim;

};

struct interface* interface_new() {
    struct interface* ui;

    /* On alloue la mémoire. */
    ui = malloc(sizeof(struct interface));
    if(ui == NULL)
        return NULL;

    /* On obtient la taille du terminale. */
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ui->taille);

    /* TODO: vérifier la taille */

    //ui->main = npanel_new(3, VERTICAL, 0, 0);
    ui->main_ls_actions = nlist_new((char*[])OPERATION, 6, 0, 0);
    //ui->main_la_msg = nlabel_new("", 0, 0);

    ui->def = npanel_new(3, VERTICAL, 0, 0);
    ui->def_p_name = npanel_new(2, HORIZONTAL, 0, 0);
    ui->def_p_dim = npanel_new(2, HORIZONTAL, 0, 0);
    ui->def_la_name = nlabel_new("Name: ", 0, 0);
    ui->def_la_name_str = nlabel_new("default", 0, 0);
    ui->def_la_dim = nlabel_new("Dimensions: ", 0, 0);
    ui->def_mat = matrice_new(UI_NEW_MATRIX_X, UI_NEW_MATRIX_Y);
    ui->def_mat_dim = matrice_new_ptr(2, 1, (int[]){UI_NEW_MATRIX_X, UI_NEW_MATRIX_Y});
    ui->def_g_mat = ngrid_new(ui->def_mat, "%3d", 0, 0);
    ui->def_g_dim = ngrid_new(ui->def_mat_dim, "%1d", 0, 0);

    npanel_set_child(ui->def_p_name, (struct widget*)ui->def_la_name, 0);
    npanel_set_child(ui->def_p_name, (struct widget*)ui->def_la_name_str, 1);
    npanel_resize(ui->def_p_name);

    npanel_set_child(ui->def_p_dim, (struct widget*)ui->def_la_dim, 0);
    npanel_set_child(ui->def_p_dim, (struct widget*)ui->def_g_dim, 1);
    npanel_resize(ui->def_p_dim);

    npanel_set_child(ui->def, (struct widget*)ui->def_p_name, 0);
    npanel_set_child(ui->def, (struct widget*)ui->def_p_dim, 1);
    npanel_set_child(ui->def, (struct widget*)ui->def_g_mat, 2);
    npanel_resize(ui->def);

    printf("%p\n", ui->def->window);
    printf("%p\n", ui->def_p_name->window);
    printf("%p\n", ui->def_p_dim->window);
    printf("%p\n", ui->def_la_name->window);
    printf("%p\n", ui->def_la_name_str->window);
    printf("%p\n", ui->def_la_dim->window);
    printf("%p\n", ui->def_g_mat->window);
    printf("%p\n", ui->def_g_dim->window);

    refresh();

    //nlist_draw();
    npanel_draw(ui->def);


    return ui;

    /* On active les touches pour les fenêtres. */
    //keypad(operation, TRUE);
    //keypad(modification, TRUE);
}

void interface_delete(struct interface* ui) {
    npanel_delete(ui->def);
    matrice_delete(ui->def_mat);
    matrice_delete(ui->def_mat_dim);
    free(ui);
}

void interface_main(struct interface* ui) {
    refresh();
    /*npanel_resize(ui->p_main);
    npanel_draw(ui->p_main);*/

    getch();
}

#if 0
void interface_main() {
    int sel;

    do {
        sel = interface_choisir();
        switch(sel) {
            case ACTION_DEFINE:
                interface_define_matrice();
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
    } while(sel != ACTION_QUIT);
}

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
