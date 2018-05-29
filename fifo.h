#ifndef _USER_FIFO_H_
#define _USER_FIFO_H_

typedef struct {
    unsigned int read_pos;
    unsigned int write_pos;
    char *buf;
    unsigned int buf_size;
} fifo_t;

#define _FIFO_INITIALIZER(obj, buffer, buffer_size) \
{ \
    .read_pos = 0, \
    .write_pos = 0, \
    .buf = buffer, \
    .buf_size = buffer_size, \
}

#define STATIC_FIFO_DEFINE(name, size) \
    static char _buffer_##name[size]; \
    static struct fifo_t name = \
    _FIFO_INITIALIZER(name, _buffer_##name, size)

#ifdef __cplusplus
extern "C" {
#endif

int fifo_init(fifo_t *fifo_t, char *buf, unsigned int buf_size);

int fifo_push(fifo_t *fifo, char c);

int fifo_pop(fifo_t *fifo, char *c);

int fifo_peek(fifo_t *fifo, char *c);

void fifo_reset(fifo_t *fifo);

int fifo_is_full(fifo_t *fifo);

int fifo_is_empty(fifo_t *fifo);

unsigned int fifo_length(fifo_t *fifo);

unsigned int fifo_space_left(fifo_t *fifo);

#ifdef __cplusplus
}
#endif

#endif // #ifndef _USER_FIFO_H_
