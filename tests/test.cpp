#include <gtest/gtest.h>

#include "fifo.h"

TEST(FifoTest, TestBufferInit)
{
    char buf[1024];

    fifo_t fifo;
    EXPECT_NE(0, fifo_init(&fifo, NULL, 0));
    EXPECT_NE(0, fifo_init(&fifo, NULL, 1));

    EXPECT_EQ(0, fifo_init(&fifo, buf, 1));
    EXPECT_EQ(0, fifo_init(&fifo, buf, 2));
    EXPECT_EQ(0, fifo_init(&fifo, buf, 4));
    EXPECT_EQ(0, fifo_init(&fifo, buf, 8));
    EXPECT_EQ(0, fifo_init(&fifo, buf, 16));
    EXPECT_EQ(0, fifo_init(&fifo, buf, 32));
    EXPECT_EQ(0, fifo_init(&fifo, buf, 64));
    EXPECT_EQ(0, fifo_init(&fifo, buf, 128));
    EXPECT_EQ(0, fifo_init(&fifo, buf, 256));
    EXPECT_EQ(0, fifo_init(&fifo, buf, 512));
    EXPECT_EQ(0, fifo_init(&fifo, buf, 1024));

    EXPECT_NE(0, fifo_init(&fifo, buf, 3));
    EXPECT_NE(0, fifo_init(&fifo, buf, 5));
    EXPECT_NE(0, fifo_init(&fifo, buf, 48));
    EXPECT_NE(0, fifo_init(&fifo, buf, 96));
    EXPECT_NE(0, fifo_init(&fifo, buf, 1023));
}

TEST(FifoTest, TestBufferLength)
{
    fifo_t fifo;
    char buffer[1024];
    fifo_init(&fifo, buffer, sizeof(buffer));

    unsigned int size = 0;
    EXPECT_EQ(size, fifo_length(&fifo));

    fifo_push(&fifo, 1);
    ++size;
    EXPECT_EQ(size, fifo_length(&fifo));

    fifo_push(&fifo, 'a');
    ++size;
    EXPECT_EQ(size, fifo_length(&fifo));

    char ch;
    fifo_pop(&fifo, &ch);
    --size;
    EXPECT_EQ(size, fifo_length(&fifo));

    fifo_pop(&fifo, &ch);
    --size;
    EXPECT_EQ(size, fifo_length(&fifo));
}

TEST(FifoTest, TestBufferInputNotExceedsPushMaxPopMax)
{
    fifo_t fifo;
    char buffer[1024];
    fifo_init(&fifo, buffer, sizeof(buffer));

    char data = 0;

    for (int i = 0; i < sizeof(buffer); i++) {
        data = 0xAA + i;
        fifo_push(&fifo, data);
        EXPECT_EQ(i + 1, fifo_length(&fifo));
    }

    for (int i = 0; i < sizeof(buffer); i++) {
        fifo_pop(&fifo, &data);
        EXPECT_EQ((char)(0xAA + i), data);
        EXPECT_EQ(sizeof(buffer) - i - 1, fifo_length(&fifo));
    }
}

TEST(FifoTest, TestBufferInputNotExceedsPush2Pop1)
{
    fifo_t fifo;
    char buffer[1024];
    fifo_init(&fifo, buffer, sizeof(buffer));

    const int num_of_elem_put = (2 * sizeof(buffer)) - 2;
    char put_data = 0;
    char get_data = 0;

    for (int i = 0; i < num_of_elem_put; i += 2) {
        put_data = 0xAA + i;
        fifo_push(&fifo, put_data);
        put_data++;
        fifo_push(&fifo, put_data);

        EXPECT_EQ(i / 2 + 2, fifo_length(&fifo));

        EXPECT_EQ(0, fifo_pop(&fifo, &get_data));
        EXPECT_EQ((char)(0xAA + i / 2), get_data);
        EXPECT_EQ(i / 2 + 1, fifo_length(&fifo));
    }

}

TEST(FifoTest, TestBufferInputExceedsPushMaxPlus1PopMax)
{
    fifo_t fifo;
    char buffer[1024];
    fifo_init(&fifo, buffer, sizeof(buffer));

    const int num_of_elem_put = sizeof(buffer) + 1;
    char data;

    for (int i = 0; i < num_of_elem_put; i++) {
        data = 0xAA + i;
        fifo_push(&fifo, data);
        if (i < sizeof(buffer)) {
            EXPECT_EQ(i + 1, fifo_length(&fifo));
        } else {
            EXPECT_EQ(sizeof(buffer), fifo_length(&fifo));
        }
    }

    for (int i = 0; i < (sizeof(buffer) - 1); i++) {
        EXPECT_EQ(0, fifo_pop(&fifo, &data));
        EXPECT_EQ((char)(0xAA + i + 1), data);
        EXPECT_EQ(sizeof(buffer) - i - 1, fifo_length(&fifo));
    }

    EXPECT_EQ(0, fifo_pop(&fifo, &data));
    EXPECT_EQ((char)(0xAA + num_of_elem_put - 1), data);
    EXPECT_EQ(0, fifo_length(&fifo));
}

TEST(FifoTest, TestBufferInputExceeds)
{
    fifo_t fifo;
    char buffer[64];
    fifo_init(&fifo, buffer, sizeof(buffer));

    int num_of_elem_put = 2 * sizeof(buffer);
    char put_data = 0;
    char get_data = 0;

    for (int i = 0; i < num_of_elem_put; i += 2) {
        put_data = 0xAA + i;
        fifo_push(&fifo, put_data);
        put_data++;
        fifo_push(&fifo, put_data);

        if ((i / 2 + 1) < sizeof(buffer)) {
            EXPECT_EQ(i / 2 + 2, fifo_length(&fifo));
        } else {
            EXPECT_EQ(sizeof(buffer), fifo_length(&fifo));
        }

        EXPECT_EQ(0, fifo_pop(&fifo, &get_data));
        if ((i / 2 + 1) < sizeof(buffer)) {
            EXPECT_EQ(i / 2 + 1, fifo_length(&fifo));
        } else {
            EXPECT_EQ(sizeof(buffer) - 1, fifo_length(&fifo));
        }

        if (i == (num_of_elem_put - 2)) {
            EXPECT_EQ((char)(0xAA + i / 2 + 1), get_data);
        } else {
            EXPECT_EQ((char)(0xAA + i / 2), get_data);
        }
    }

    for (int i = 0; i < (num_of_elem_put / 2 - 1); i++) {
        EXPECT_EQ(0, fifo_pop(&fifo, &get_data));
        EXPECT_EQ((char)(0xAA + num_of_elem_put / 2 + i + 1), get_data);
    }
}

TEST(FifoTest, TestBufferPopEmptyFullSizeAfterCreation)
{
    fifo_t fifo;
    char buffer[1];
    fifo_init(&fifo, buffer, sizeof(buffer));

    char data;

    EXPECT_TRUE(fifo_is_empty(&fifo));
    EXPECT_NE(0, fifo_pop(&fifo, &data));
    EXPECT_NE(0, fifo_peek(&fifo, &data));
    EXPECT_FALSE(fifo_is_full(&fifo));
    EXPECT_EQ(0, fifo_length(&fifo));
}

TEST(FifoTest, TestBufferEmpty)
{
    fifo_t fifo;
    char buffer[128];
    fifo_init(&fifo, buffer, sizeof(buffer));

    char data = 0;

    for (int i = 0; i < sizeof(buffer); i++) {
        EXPECT_EQ(0, fifo_push(&fifo, data));
        EXPECT_FALSE(fifo_is_empty(&fifo));
    }

    for (int i = 0; i < sizeof(buffer) - 1; i++) {
        EXPECT_EQ(0, fifo_pop(&fifo, &data));
        EXPECT_FALSE(fifo_is_empty(&fifo));
    }

    EXPECT_EQ(0, fifo_pop(&fifo, &data));
    EXPECT_TRUE(fifo_is_empty(&fifo));

    EXPECT_EQ(0, fifo_push(&fifo, data));
    EXPECT_FALSE(fifo_is_empty(&fifo));
}

TEST(FifoTest, TestBufferFull)
{
    fifo_t fifo;
    char buffer[256];
    fifo_init(&fifo, buffer, sizeof(buffer));
    char data;

    for (int i = 0; i < (sizeof(buffer) - 1); i++) {
        EXPECT_EQ(0, fifo_push(&fifo, data));
        EXPECT_FALSE(fifo_is_full(&fifo));
    }

    fifo_push(&fifo, data);
    EXPECT_TRUE(fifo_is_full(&fifo));

    for (int i = 0; i < (sizeof(buffer) * 2); i++) {
        fifo_push(&fifo, data);
        EXPECT_TRUE(fifo_is_full(&fifo));
    }

    for (int i = 0; i < sizeof(buffer); i++) {
        EXPECT_EQ(0, fifo_pop(&fifo, &data));
        EXPECT_FALSE(fifo_is_full(&fifo));
    }
}

TEST(FifoTest, TestBufferReset)
{
    fifo_t fifo;
    char buffer[512];
    fifo_init(&fifo, buffer, sizeof(buffer));

    char data = 0xAA;

    for (int i = 0; i < sizeof(buffer); i++) {
        EXPECT_EQ(0, fifo_push(&fifo, data));
    }

    EXPECT_TRUE(fifo_is_full(&fifo));
    EXPECT_FALSE(fifo_is_empty(&fifo));

    fifo_reset(&fifo);
    EXPECT_FALSE(fifo_is_full(&fifo));
    EXPECT_TRUE(fifo_is_empty(&fifo));
    EXPECT_NE(0, fifo_pop(&fifo, &data));
    EXPECT_EQ(0, fifo_length(&fifo));

    for (int i = 0; i < sizeof(buffer); i++) {
        EXPECT_EQ(0, fifo_push(&fifo, data));
        data++;
    }

    for (int i = 0; i < sizeof(buffer); i++) {
        EXPECT_EQ(0, fifo_pop(&fifo, &data));
        EXPECT_EQ((char)(0xAA + i), data);
    }
}

TEST(FifoTest, TestBufferPeekNoPop)
{
    fifo_t fifo;
    char buffer[256];
    fifo_init(&fifo, buffer, sizeof(buffer));
    char data = 0;
    char peek_data = 0;
    char value = 0xAA;

    for (int i = 0; i < 3; i++) {
        data = value + i;
        fifo_push(&fifo, data);
        fifo_peek(&fifo, &peek_data);
        EXPECT_EQ(i + 1, fifo_length(&fifo));
    }

    for (int i = 0; i < 1; i++) {
        EXPECT_EQ(0, fifo_peek(&fifo, &peek_data));
        EXPECT_EQ(value + i, peek_data);
        EXPECT_EQ(0, fifo_pop(&fifo, &data));
        EXPECT_EQ(value + i, data);
        EXPECT_EQ(3 - i - 1, fifo_length(&fifo));
    }
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
