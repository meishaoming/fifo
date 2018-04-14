# ringbuffer

This is a c implementation of [ring buffer](https://en.wikipedia.org/wiki/Circular_buffer) for embed system. 

I use it as uart driver rx/tx buffer in my stm32 projects.

## Highligths:

- buffer size must be a power of 2, inspired by kfifo in linux kernel
- push will overwrite the buffer if it's full
- unit test with [googletest](https://github.com/google/googletest), inpired by test in [mbed-os](https://github.com/ARMmbed/mbed-os/blob/master/TESTS/mbedmicro-rtos-mbed/CircularBuffer/main.cpp)
- MIT License


