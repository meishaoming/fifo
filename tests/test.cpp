#include <gtest/gtest.h>

#include "fifo.h"

TEST(FifoTest, TestBufferInit)
{
    DEFINE_FIFO(g_fifo, 1024);
}

TEST(FifoTest, TestBufferLength)
{
    DEFINE_FIFO(fifo, 1024);

    unsigned int size = 0;
    EXPECT_EQ(size, fifo_len(&fifo));

    fifo_put(&fifo, 1);
    ++size;
    EXPECT_EQ(size, fifo_len(&fifo));

    fifo_put(&fifo, 'a');
    ++size;
    EXPECT_EQ(size, fifo_len(&fifo));

    char ch;
    fifo_get(&fifo, &ch);
    --size;
    EXPECT_EQ(size, fifo_len(&fifo));

    fifo_get(&fifo, &ch);
    --size;
    EXPECT_EQ(size, fifo_len(&fifo));
}

TEST(FifoTest, TestBufferInputNotExceedsPushMaxPopMax)
{
    DEFINE_FIFO(fifo, 1024);
    const int size = 1024;

    char data = 0;

    for (int i = 0; i < size; i++) {
        data = 0xAA + i;
        fifo_put(&fifo, data);
        EXPECT_EQ(i + 1, fifo_len(&fifo));
    }

    for (int i = 0; i < size; i++) {
        fifo_get(&fifo, &data);
        EXPECT_EQ((char)(0xAA + i), data);
        EXPECT_EQ(size - i - 1, fifo_len(&fifo));
    }
}

TEST(FifoTest, TestBufferInputNotExceedsPush2Pop1)
{
    DEFINE_FIFO(fifo, 1024);
    const int size = 1024;

    const int num_of_elem_put = (2 * size) - 2;
    char put_data = 0;
    char get_data = 0;

    for (int i = 0; i < num_of_elem_put; i += 2) {
        put_data = 0xAA + i;
        fifo_put(&fifo, put_data);
        put_data++;
        fifo_put(&fifo, put_data);

        EXPECT_EQ(i / 2 + 2, fifo_len(&fifo));

        EXPECT_EQ(0, fifo_get(&fifo, &get_data));
        EXPECT_EQ((char)(0xAA + i / 2), get_data);
        EXPECT_EQ(i / 2 + 1, fifo_len(&fifo));
    }

}

TEST(FifoTest, TestBufferInputExceedsPushMaxPlus1PopMax)
{
    DEFINE_FIFO(fifo, 1024);
    const int size = 1024;

    const int num_of_elem_put = size + 1;
    char data;

    for (int i = 0; i < num_of_elem_put; i++) {
        data = 0xAA + i;
        fifo_put(&fifo, data);
        if (i < size) {
            EXPECT_EQ(i + 1, fifo_len(&fifo));
        } else {
            EXPECT_EQ(size, fifo_len(&fifo));
        }
    }

    for (int i = 0; i < (size - 1); i++) {
        EXPECT_EQ(0, fifo_get(&fifo, &data));
        EXPECT_EQ((char)(0xAA + i), data);
        EXPECT_EQ(size - i - 1, fifo_len(&fifo));
    }

    EXPECT_EQ(0, fifo_get(&fifo, &data));
    EXPECT_EQ((char)(0xAA + size - 1), data);
    EXPECT_EQ(0, fifo_len(&fifo));
}

TEST(FifoTest, TestBufferInputExceeds)
{
    DEFINE_FIFO(fifo, 64);
    const int size = 64;

    int num_of_elem_put = 2 * size;
    char put_data = 0;
    char get_data = 0;

    for (int i = 0; i < num_of_elem_put; i += 2) {
        put_data = 0xAA + i;
        fifo_put(&fifo, put_data);
        put_data++;
        fifo_put(&fifo, put_data);

        if ((i / 2 + 1) < size) {
            EXPECT_EQ(i / 2 + 2, fifo_len(&fifo));
        } else {
            EXPECT_EQ(size, fifo_len(&fifo));
        }

        EXPECT_EQ(0, fifo_get(&fifo, &get_data));

        if ((i / 2 + 1) < size) {
            EXPECT_EQ(i / 2 + 1, fifo_len(&fifo));
        } else {
            EXPECT_EQ(size - 1, fifo_len(&fifo));
        }

        EXPECT_EQ((char)(0xAA + i / 2), get_data);
    }

    EXPECT_EQ((num_of_elem_put / 2 - 1), fifo_len(&fifo));

    for (int i = 0; i < (num_of_elem_put / 2 - 1); i++) {
        EXPECT_EQ(0, fifo_get(&fifo, &get_data));
        EXPECT_EQ((char)(0xAA + (num_of_elem_put / 2) + i), get_data);
    }
}

TEST(FifoTest, TestBufferPopEmptyFullSizeAfterCreation)
{
    DEFINE_FIFO(fifo, 1024);
    const int size = 1024;

    char data;

    EXPECT_TRUE(fifo_is_empty(&fifo));
    EXPECT_NE(0, fifo_get(&fifo, &data));
    EXPECT_NE(0, fifo_peek(&fifo, &data));
    EXPECT_FALSE(fifo_is_full(&fifo));
    EXPECT_EQ(0, fifo_len(&fifo));
}

TEST(FifoTest, TestBufferEmpty)
{
    DEFINE_FIFO(fifo, 128);
    const int size = 128;

    char data = 0;

    for (int i = 0; i < size; i++) {
        EXPECT_EQ(0, fifo_put(&fifo, data));
        EXPECT_FALSE(fifo_is_empty(&fifo));
    }

    for (int i = 0; i < size - 1; i++) {
        EXPECT_EQ(0, fifo_get(&fifo, &data));
        EXPECT_FALSE(fifo_is_empty(&fifo));
    }

    EXPECT_EQ(0, fifo_get(&fifo, &data));
    EXPECT_TRUE(fifo_is_empty(&fifo));

    EXPECT_EQ(0, fifo_put(&fifo, data));
    EXPECT_FALSE(fifo_is_empty(&fifo));
}

TEST(FifoTest, TestBufferFull)
{
    DEFINE_FIFO(fifo, 256);
    const int size = 256;

    char data;

    for (int i = 0; i < (size - 1); i++) {
        EXPECT_EQ(0, fifo_put(&fifo, data));
        EXPECT_FALSE(fifo_is_full(&fifo));
    }

    fifo_put(&fifo, data);
    EXPECT_TRUE(fifo_is_full(&fifo));

    for (int i = 0; i < (size * 2); i++) {
        fifo_put(&fifo, data);
        EXPECT_TRUE(fifo_is_full(&fifo));
    }

    for (int i = 0; i < size; i++) {
        EXPECT_EQ(0, fifo_get(&fifo, &data));
        EXPECT_FALSE(fifo_is_full(&fifo));
    }
}

TEST(FifoTest, TestBufferReset)
{
    DEFINE_FIFO(fifo, 512);
    const int size = 512;

    char data = 0xAA;

    for (int i = 0; i < size; i++) {
        EXPECT_EQ(0, fifo_put(&fifo, data));
    }

    EXPECT_TRUE(fifo_is_full(&fifo));
    EXPECT_FALSE(fifo_is_empty(&fifo));

    fifo_reset(&fifo);
    EXPECT_FALSE(fifo_is_full(&fifo));
    EXPECT_TRUE(fifo_is_empty(&fifo));
    EXPECT_NE(0, fifo_get(&fifo, &data));
    EXPECT_EQ(0, fifo_len(&fifo));

    for (int i = 0; i < size; i++) {
        EXPECT_EQ(0, fifo_put(&fifo, data));
        data++;
    }

    for (int i = 0; i < size; i++) {
        EXPECT_EQ(0, fifo_get(&fifo, &data));
        EXPECT_EQ((char)(0xAA + i), data);
    }
}

TEST(FifoTest, TestBufferPeekNoPop)
{
    DEFINE_FIFO(fifo, 256);
    const int size = 256;

    char data = 0;
    char peek_data = 0;
    char value = 0xAA;

    for (int i = 0; i < 3; i++) {
        data = value + i;
        fifo_put(&fifo, data);
        fifo_peek(&fifo, &peek_data);
        EXPECT_EQ(i + 1, fifo_len(&fifo));
    }

    for (int i = 0; i < 1; i++) {
        EXPECT_EQ(0, fifo_peek(&fifo, &peek_data));
        EXPECT_EQ(value + i, peek_data);
        EXPECT_EQ(0, fifo_get(&fifo, &data));
        EXPECT_EQ(value + i, data);
        EXPECT_EQ(3 - i - 1, fifo_len(&fifo));
    }
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
