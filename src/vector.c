#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "config.h"

struct vector* vector_new(int32_t size) {
    struct vector* st_vec;

    /* On alloue la mémoire à la structure. */
    st_vec = malloc(sizeof(struct vector));
    if(st_vec == NULL)
        return NULL;

    /* On configure la structure. */
    st_vec->size = size;
    st_vec->current = 0;
    st_vec->data = malloc(size*sizeof(void*));
    if(st_vec->data == NULL)
        return NULL;

    return st_vec;
}

void vector_delete(struct vector* st_vec) {
    /* On libère la mémoire. */
    free(st_vec->data);
    free(st_vec);
}

int vector_increase(struct vector* st_vec, int32_t size) {
    /* On alloue la mémoire de la structure. */
    void* newlocation = malloc(size*sizeof(void*));
    if(newlocation == NULL)
        return FALSE;

    /* On copie les pointeurs dans la nouvelle mémoire. */
    memcpy(newlocation, st_vec->data, st_vec->size*sizeof(void*));

    /* On libère l'ancien pointeur. */
    free(st_vec->data);

    /* On configure the structure. */
    st_vec->size = size;
    st_vec->data = newlocation;

    return TRUE;
}

int vector_push_pointer(struct vector* st_vec, void* pointer) {
    /* On augmente la taille du vector au besoins. */
    if(st_vec->current >= st_vec->size)
        if(vector_increase(st_vec, 2*st_vec->size) == FALSE)
            return FALSE;

    /* On copie le pointeur dans la mémoire. */
    memcpy(st_vec->data+st_vec->current*sizeof(void*), &pointer, sizeof(void*));
    st_vec->current++;

    return TRUE;
}

void* vector_get_pointer(struct vector* st_vec, int32_t pos) {
    return *((void**)(st_vec->data+pos*sizeof(void*)));
}
