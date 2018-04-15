#include "ringbuffer.h"

static inline int is_power_of_2(unsigned long n)
{
    return (n != 0 && ((n & (n - 1)) == 0));
}

int ringbuffer_init(ringbuffer *ringbuf, char *buf, unsigned int buf_size)
{
    if (buf && is_power_of_2(buf_size)) {
        ringbuf->read_pos = 0;
        ringbuf->write_pos = 0;
        ringbuf->buf = buf;
        ringbuf->buf_size = buf_size;
        return 0;
    }
    return -1;
}

int ringbuffer_put(ringbuffer *ringbuf, char ch)
{
    // overwrites the buffer if it's full
    if (ringbuffer_is_full(ringbuf)) {
        ringbuf->read_pos++;
    }
    unsigned int pos = ringbuf->write_pos & (ringbuf->buf_size - 1);
    ringbuf->buf[pos] = ch;
    ringbuf->write_pos++;
    return 0;
}

int ringbuffer_get(ringbuffer *ringbuf, char *r_val)
{
    if (!ringbuffer_is_empty(ringbuf)) {
        unsigned int pos = ringbuf->read_pos & (ringbuf->buf_size - 1);
        *r_val = ringbuf->buf[pos];
        ringbuf->read_pos++;
        return 0;
    }
    return -1;
}

int ringbuffer_peek(ringbuffer *ringbuf, char *r_val)
{
    if (!ringbuffer_is_empty(ringbuf)) {
        unsigned int pos = ringbuf->read_pos & (ringbuf->buf_size - 1);
        *r_val = ringbuf->buf[pos];
        return 0;
    }
    return -1;
}

void ringbuffer_reset(ringbuffer *ringbuf)
{
    ringbuf->read_pos = 0;
    ringbuf->write_pos = 0;
}

int ringbuffer_is_full(ringbuffer *ringbuf)
{
    return ((ringbuf->write_pos - ringbuf->read_pos) == ringbuf->buf_size);
}

int ringbuffer_is_empty(ringbuffer *ringbuf)
{
    return ((ringbuf->write_pos - ringbuf->read_pos) == 0);
}

unsigned int ringbuffer_size(ringbuffer *ringbuf)
{
    return ringbuf->buf_size;
}

unsigned int ringbuffer_length(ringbuffer *ringbuf)
{
    return ringbuf->write_pos - ringbuf->read_pos;
}

unsigned int ringbuffer_space_left(ringbuffer *ringbuf)
{
    return ringbuf->buf_size - (ringbuf->write_pos - ringbuf->read_pos);
}
