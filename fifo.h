#ifndef _USER_FIFO_H_
#define _USER_FIFO_H_

struct __fifo {
    unsigned int read_pos;
    unsigned int write_pos;
    unsigned int mask;
    char *buf;
};

typedef struct __fifo fifo_t;

#define DEFINE_FIFO(fifo, size) \
char __fifo_##buf[((size < 2) || (size & (size - 1))) ? -1 : size]; \
struct __fifo fifo = \
{ \
    .read_pos = 0, \
    .write_pos = 0, \
    .mask = size - 1, \
    .buf = __fifo_##buf, \
}

#ifdef __cplusplus
extern "C" {
#endif

unsigned int fifo_size(fifo_t *fifo);
unsigned int fifo_len(fifo_t *fifo);
unsigned int fifo_avail(fifo_t *fifo);

int fifo_is_full(fifo_t *fifo);
int fifo_is_empty(fifo_t *fifo);

void fifo_reset(fifo_t *fifo);

int fifo_put(fifo_t *fifo, char ch);
int fifo_get(fifo_t *fifo, char *ch);
int fifo_peek(fifo_t *fifo, char *ch);

#ifdef __cplusplus
}
#endif

#endif // #ifndef _USER_FIFO_H_
