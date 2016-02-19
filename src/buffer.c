#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "config.h"
#include "buffer.h"
#include "misc.h"

struct buffer* buffer_new(int32_t size, int32_t max) {
    struct buffer* st_buffer;

    /* On alloue la mémoire. */
    st_buffer = malloc(sizeof(struct buffer));
    if(st_buffer == NULL)
        return NULL;

    /* On alloue la mémoire pour le buffer. */
    st_buffer->data = calloc(size, sizeof(char));
    if(st_buffer == NULL)
        return NULL;

    /* On configure la structure. */
    st_buffer->data[0] = '\0';
    //st_buffer->data[1] = '\0';
    st_buffer->size = size;
    st_buffer->set = 0;
    st_buffer->max = max;

    return st_buffer;
}

void buffer_delete(struct buffer* st_buffer) {
    /* On libère la mémoire. */
    free(st_buffer->data);
    free(st_buffer);
}


int buffer_increase(struct buffer* st_buffer, int32_t size) {
    char* newlocation;

    /* On alloue la mémoire pour le nouveau buffer. */
    newlocation = malloc(size);
    if(newlocation == NULL)
        return FALSE;

    /* On copie la mémoire dans le nouveau buffer. */
    memcpy(newlocation, st_buffer->data, st_buffer->size);

    /* On libère la mémoire de l'ancien buffer. */
    free(st_buffer->data);

    /* On configure la structure. */
    st_buffer->data = newlocation;
    st_buffer->size = size;

    return TRUE;
}

int buffer_append_char(struct buffer* st_buffer, char c) {
    int ret;

    /* On regarde si on a atteint la limite. */
    if(st_buffer->max != 0 && st_buffer->max <= st_buffer->set)
        return FALSE;

    /* On augmente le buffer si nécéssaire. */
    if(st_buffer->set >= st_buffer->size-1) {
        buffer_increase(st_buffer, 2*st_buffer->size);
        ret = TRUE;
    } else {
        ret = FALSE;
    }

    /*On ajoute le caractère. */
    st_buffer->data[st_buffer->set] = c;
    st_buffer->data[st_buffer->set+1] = '\0';
    st_buffer->set++;

    return ret;
}

void buffer_backspace(struct buffer* st_buffer) {
    if(st_buffer->set == 0) {
        /* On vide le buffer si on est retourné au début. */
        st_buffer->data[st_buffer->set] = '\0';
    } else {
        /* On enlève un caractère. */
        st_buffer->set--;
        st_buffer->data[st_buffer->set] = '\0';
    }
}

char* buffer_get_string(struct buffer* st_buffer) {
    char* string;

    /* On alloue la mémoire pour le string. */
    string = malloc(strlen(st_buffer->data));
    if(string == NULL)
        return NULL;

    /* On copie le string. */
    memcpy(string, st_buffer->data, strlen(st_buffer->data));

    return string;
}

void buffer_dump_ASCII(struct buffer* st_buffer) {
    int32_t i;

    /* On écrit chaque caractères du buffer. */
    for(i = 0; i < st_buffer->set; i++) {
        if(st_buffer->data[i] == '\n')
            printf("[\\n]");
        else if(st_buffer->data[i] == '\0')
            printf("[\\0]");
        else
            printf("[%c]", st_buffer->data[i]);
    }

    printf("\n");
}
