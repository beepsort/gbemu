#include <gtest/gtest.h>

int give0() {
    return 0;
}

TEST(give0Test, give0Is0) {
    EXPECT_EQ(give0(), 0);
    EXPECT_NE(give0(), 1);
}
