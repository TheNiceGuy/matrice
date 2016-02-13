#ifndef INTERFACE_INCLUDE_H
#define INTERFACE_INCLUDE_H

#include "matrice.h"

struct interface* interface_new();
void interface_delete(struct interface* interface);
void interface_main(struct interface* interface);
int interface_choisir();
void interface_define_matrice();

#endif
