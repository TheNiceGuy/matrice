#ifndef INCLUDE_VECTOR_H
#define INCLUDE_VECTOR_H

struct vector {
    int32_t size;
    int32_t current;
    void* data;
};

struct vector* vector_new(int32_t size);
void vector_delete(struct vector* st_vec);
int vector_increase(struct vector* st_vec, int32_t size);
int vector_push_pointer(struct vector* st_vec, void* pointer);
void* vector_get_pointer(struct vector* st_vec, int32_t pos);

#endif
