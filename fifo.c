#include "fifo.h"

static inline int is_power_of_2(unsigned long n)
{
    return (n != 0 && ((n & (n - 1)) == 0));
}

int fifo_init(fifo_t *fifo, char *buf, unsigned int buf_size)
{
    if (buf && is_power_of_2(buf_size)) {
        fifo->read_pos = 0;
        fifo->write_pos = 0;
        fifo->buf = buf;
        fifo->buf_size = buf_size;
        return 0;
    }
    return -1;
}

int fifo_push(fifo_t *fifo, char c)
{
    // overwrites the buffer if it's full
    if (fifo_is_full(fifo)) {
        fifo->read_pos++;
    }
    unsigned int pos = fifo->write_pos & (fifo->buf_size - 1);
    fifo->buf[pos] = c;
    fifo->write_pos++;
    return 0;
}

int fifo_pop(fifo_t *fifo, char *c)
{
    if (!fifo_is_empty(fifo)) {
        unsigned int pos = fifo->read_pos & (fifo->buf_size - 1);
        *c = fifo->buf[pos];
        fifo->read_pos++;
        return 0;
    }
    return -1;
}

int fifo_peek(fifo_t *fifo, char *c)
{
    if (!fifo_is_empty(fifo)) {
        unsigned int pos = fifo->read_pos & (fifo->buf_size - 1);
        *c = fifo->buf[pos];
        return 0;
    }
    return -1;
}

void fifo_reset(fifo_t *fifo)
{
    fifo->read_pos = 0;
    fifo->write_pos = 0;
}

int fifo_is_full(fifo_t *fifo)
{
    return ((fifo->write_pos - fifo->read_pos) == fifo->buf_size);
}

int fifo_is_empty(fifo_t *fifo)
{
    return ((fifo->write_pos - fifo->read_pos) == 0);
}

unsigned int fifo_size(fifo_t *fifo)
{
    return fifo->buf_size;
}

unsigned int fifo_length(fifo_t *fifo)
{
    return fifo->write_pos - fifo->read_pos;
}

unsigned int fifo_space_left(fifo_t *fifo)
{
    return fifo->buf_size - (fifo->write_pos - fifo->read_pos);
}
