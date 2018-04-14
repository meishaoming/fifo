#include <gtest/gtest.h>

#include "ringbuffer.h"

TEST(RingBufferTest, TestBufferInit)
{
    char buf[1024];

    ringbuffer rb;
    EXPECT_NE(0, ringbuffer_init(&rb, NULL, 0));
    EXPECT_NE(0, ringbuffer_init(&rb, NULL, 1));

    EXPECT_EQ(0, ringbuffer_init(&rb, buf, 1));
    EXPECT_EQ(0, ringbuffer_init(&rb, buf, 2));
    EXPECT_EQ(0, ringbuffer_init(&rb, buf, 4));
    EXPECT_EQ(0, ringbuffer_init(&rb, buf, 8));
    EXPECT_EQ(0, ringbuffer_init(&rb, buf, 16));
    EXPECT_EQ(0, ringbuffer_init(&rb, buf, 32));
    EXPECT_EQ(0, ringbuffer_init(&rb, buf, 64));
    EXPECT_EQ(0, ringbuffer_init(&rb, buf, 128));
    EXPECT_EQ(0, ringbuffer_init(&rb, buf, 256));
    EXPECT_EQ(0, ringbuffer_init(&rb, buf, 512));
    EXPECT_EQ(0, ringbuffer_init(&rb, buf, 1024));

    EXPECT_NE(0, ringbuffer_init(&rb, buf, 3));
    EXPECT_NE(0, ringbuffer_init(&rb, buf, 5));
    EXPECT_NE(0, ringbuffer_init(&rb, buf, 48));
    EXPECT_NE(0, ringbuffer_init(&rb, buf, 96));
    EXPECT_NE(0, ringbuffer_init(&rb, buf, 1023));
}

TEST(RingBufferTest, TestBufferLength)
{
    ringbuffer rb;
    char buffer[1024];
    ringbuffer_init(&rb, buffer, sizeof(buffer));

    unsigned int size = 0;
    EXPECT_EQ(size, ringbuffer_length(&rb));

    ringbuffer_push(&rb, 1);
    ++size;
    EXPECT_EQ(size, ringbuffer_length(&rb));

    ringbuffer_push(&rb, 'a');
    ++size;
    EXPECT_EQ(size, ringbuffer_length(&rb));

    char ch;
    ringbuffer_pop(&rb, &ch);
    --size;
    EXPECT_EQ(size, ringbuffer_length(&rb));

    ringbuffer_pop(&rb, &ch);
    --size;
    EXPECT_EQ(size, ringbuffer_length(&rb));
}

TEST(RingBufferTest, TestBufferInputNotExceedsPushMaxPopMax)
{
    ringbuffer rb;
    char buffer[1024];
    ringbuffer_init(&rb, buffer, sizeof(buffer));

    char data = 0;

    for (int i = 0; i < sizeof(buffer); i++) {
        data = 0xAA + i;
        ringbuffer_push(&rb, data);
        EXPECT_EQ(i + 1, ringbuffer_length(&rb));
    }

    for (int i = 0; i < sizeof(buffer); i++) {
        ringbuffer_pop(&rb, &data);
        EXPECT_EQ((char)(0xAA + i), data);
        EXPECT_EQ(sizeof(buffer) - i - 1, ringbuffer_length(&rb));
    }
}

TEST(RingBufferTest, TestBufferInputNotExceedsPush2Pop1)
{
    ringbuffer rb;
    char buffer[1024];
    ringbuffer_init(&rb, buffer, sizeof(buffer));

    const int num_of_elem_push = (2 * sizeof(buffer)) - 2;
    char push_data = 0;
    char pop_data = 0;

    for (int i = 0; i < num_of_elem_push; i += 2) {
        push_data = 0xAA + i;
        ringbuffer_push(&rb, push_data);
        push_data++;
        ringbuffer_push(&rb, push_data);

        EXPECT_EQ(i / 2 + 2, ringbuffer_length(&rb));

        EXPECT_EQ(0, ringbuffer_pop(&rb, &pop_data));
        EXPECT_EQ((char)(0xAA + i / 2), pop_data);
        EXPECT_EQ(i / 2 + 1, ringbuffer_length(&rb));
    }

}

TEST(RingBufferTest, TestBufferInputExceedsPushMaxPlus1PopMax)
{
    ringbuffer rb;
    char buffer[1024];
    ringbuffer_init(&rb, buffer, sizeof(buffer));

    const int num_of_elem_push = sizeof(buffer) + 1;
    char data;

    for (int i = 0; i < num_of_elem_push; i++) {
        data = 0xAA + i;
        ringbuffer_push(&rb, data);
        if (i < sizeof(buffer)) {
            EXPECT_EQ(i + 1, ringbuffer_length(&rb));
        } else {
            EXPECT_EQ(sizeof(buffer), ringbuffer_length(&rb));
        }
    }

    for (int i = 0; i < (sizeof(buffer) - 1); i++) {
        EXPECT_EQ(0, ringbuffer_pop(&rb, &data));
        EXPECT_EQ((char)(0xAA + i + 1), data);
        EXPECT_EQ(sizeof(buffer) - i - 1, ringbuffer_length(&rb));
    }

    EXPECT_EQ(0, ringbuffer_pop(&rb, &data));
    EXPECT_EQ((char)(0xAA + num_of_elem_push - 1), data);
    EXPECT_EQ(0, ringbuffer_length(&rb));
}

TEST(RingBufferTest, TestBufferInputExceeds)
{
    ringbuffer rb;
    char buffer[64];
    ringbuffer_init(&rb, buffer, sizeof(buffer));

    int num_of_elem_push = 2 * sizeof(buffer);
    char push_data = 0;
    char pop_data = 0;

    for (int i = 0; i < num_of_elem_push; i += 2) {
        push_data = 0xAA + i;
        ringbuffer_push(&rb, push_data);
        push_data++;
        ringbuffer_push(&rb, push_data);

        if ((i / 2 + 1) < sizeof(buffer)) {
            EXPECT_EQ(i / 2 + 2, ringbuffer_length(&rb));
        } else {
            EXPECT_EQ(sizeof(buffer), ringbuffer_length(&rb));
        }

        EXPECT_EQ(0, ringbuffer_pop(&rb, &pop_data));
        if ((i / 2 + 1) < sizeof(buffer)) {
            EXPECT_EQ(i / 2 + 1, ringbuffer_length(&rb));
        } else {
            EXPECT_EQ(sizeof(buffer) - 1, ringbuffer_length(&rb));
        }

        if (i == (num_of_elem_push - 2)) {
            EXPECT_EQ((char)(0xAA + i / 2 + 1), pop_data);
        } else {
            EXPECT_EQ((char)(0xAA + i / 2), pop_data);
        }
    }

    for (int i = 0; i < (num_of_elem_push / 2 - 1); i++) {
        EXPECT_EQ(0, ringbuffer_pop(&rb, &pop_data));
        EXPECT_EQ((char)(0xAA + num_of_elem_push / 2 + i + 1), pop_data);
    }
}

TEST(RingBufferTest, TestBufferPopEmptyFullSizeAfterCreation)
{
    ringbuffer rb;
    char buffer[1];
    ringbuffer_init(&rb, buffer, sizeof(buffer));

    char data;

    EXPECT_TRUE(ringbuffer_is_empty(&rb));
    EXPECT_NE(0, ringbuffer_pop(&rb, &data));
    EXPECT_NE(0, ringbuffer_peek(&rb, &data));
    EXPECT_FALSE(ringbuffer_is_full(&rb));
    EXPECT_EQ(0, ringbuffer_length(&rb));
}

TEST(RingBufferTest, TestBufferEmpty)
{
    ringbuffer rb;
    char buffer[128];
    ringbuffer_init(&rb, buffer, sizeof(buffer));

    char data = 0;

    for (int i = 0; i < sizeof(buffer); i++) {
        EXPECT_EQ(0, ringbuffer_push(&rb, data));
        EXPECT_FALSE(ringbuffer_is_empty(&rb));
    }

    for (int i = 0; i < sizeof(buffer) - 1; i++) {
        EXPECT_EQ(0, ringbuffer_pop(&rb, &data));
        EXPECT_FALSE(ringbuffer_is_empty(&rb));
    }

    EXPECT_EQ(0, ringbuffer_pop(&rb, &data));
    EXPECT_TRUE(ringbuffer_is_empty(&rb));

    EXPECT_EQ(0, ringbuffer_push(&rb, data));
    EXPECT_FALSE(ringbuffer_is_empty(&rb));
}

TEST(RingBufferTest, TestBufferFull)
{
    ringbuffer rb;
    char buffer[256];
    ringbuffer_init(&rb, buffer, sizeof(buffer));
    char data;

    for (int i = 0; i < (sizeof(buffer) - 1); i++) {
        EXPECT_EQ(0, ringbuffer_push(&rb, data));
        EXPECT_FALSE(ringbuffer_is_full(&rb));
    }

    ringbuffer_push(&rb, data);
    EXPECT_TRUE(ringbuffer_is_full(&rb));

    for (int i = 0; i < (sizeof(buffer) * 2); i++) {
        ringbuffer_push(&rb, data);
        EXPECT_TRUE(ringbuffer_is_full(&rb));
    }

    for (int i = 0; i < sizeof(buffer); i++) {
        EXPECT_EQ(0, ringbuffer_pop(&rb, &data));
        EXPECT_FALSE(ringbuffer_is_full(&rb));
    }
}

TEST(RingBufferTest, TestBufferReset)
{
    ringbuffer rb;
    char buffer[512];
    ringbuffer_init(&rb, buffer, sizeof(buffer));

    char data = 0xAA;

    for (int i = 0; i < sizeof(buffer); i++) {
        EXPECT_EQ(0, ringbuffer_push(&rb, data));
    }

    EXPECT_TRUE(ringbuffer_is_full(&rb));
    EXPECT_FALSE(ringbuffer_is_empty(&rb));

    ringbuffer_reset(&rb);
    EXPECT_FALSE(ringbuffer_is_full(&rb));
    EXPECT_TRUE(ringbuffer_is_empty(&rb));
    EXPECT_NE(0, ringbuffer_pop(&rb, &data));
    EXPECT_EQ(0, ringbuffer_length(&rb));

    for (int i = 0; i < sizeof(buffer); i++) {
        EXPECT_EQ(0, ringbuffer_push(&rb, data));
        data++;
    }

    for (int i = 0; i < sizeof(buffer); i++) {
        EXPECT_EQ(0, ringbuffer_pop(&rb, &data));
        EXPECT_EQ((char)(0xAA + i), data);
    }
}

TEST(RingBufferTest, TestBufferPeekNoPop)
{
    ringbuffer rb;
    char buffer[256];
    ringbuffer_init(&rb, buffer, sizeof(buffer));
    char data = 0;
    char peek_data = 0;
    char value = 0xAA;

    for (int i = 0; i < 3; i++) {
        data = value + i;
        ringbuffer_push(&rb, data);
        ringbuffer_peek(&rb, &peek_data);
        EXPECT_EQ(i + 1, ringbuffer_length(&rb));
    }

    for (int i = 0; i < 1; i++) {
        EXPECT_EQ(0, ringbuffer_peek(&rb, &peek_data));
        EXPECT_EQ(value + i, peek_data);
        EXPECT_EQ(0, ringbuffer_pop(&rb, &data));
        EXPECT_EQ(value + i, data);
        EXPECT_EQ(3 - i - 1, ringbuffer_length(&rb));
    }
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
