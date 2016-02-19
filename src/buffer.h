#ifndef BUFFER_INCLUDE_H
#define BUFFER_INCLUDE_H

struct buffer {
    char* data;
    int32_t size;
    int32_t set;
    int32_t max;
};

struct buffer* buffer_new(int32_t size, int32_t max);
void buffer_delete(struct buffer* st_buffer);
int buffer_increase(struct buffer* st_buffer, int32_t size);
int buffer_append_char(struct buffer* st_buffer, char c);
void buffer_backspace(struct buffer* st_buffer);
char* buffer_get_string(struct buffer* st_buffer);
void buffer_dump_ASCII(struct buffer* st_buffer);

#endif
