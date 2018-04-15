#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ringbuffer {
    unsigned int read_pos;
    unsigned int write_pos;
    char *buf;
    unsigned int buf_size;
} ringbuffer;

int ringbuffer_init(ringbuffer *ringbuf, char *buf, unsigned int buf_size);

int ringbuffer_put(ringbuffer *ringbuf, char ch);

int ringbuffer_get(ringbuffer *ringbuf, char *r_val);

int ringbuffer_peek(ringbuffer *ringbuf, char *r_val);

void ringbuffer_reset(ringbuffer *ringbuf);

int ringbuffer_is_full(ringbuffer *ringbuf);

int ringbuffer_is_empty(ringbuffer *ringbuf);

unsigned int ringbuffer_length(ringbuffer *ringbuf);

unsigned int ringbuffer_space_left(ringbuffer *ringbuf);

#ifdef __cplusplus
}
#endif

#endif // #ifndef _RINGBUFFER_H_
