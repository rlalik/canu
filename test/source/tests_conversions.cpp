#include <gtest/gtest.h>

#include "canu/canu.hpp"

TEST(TestsConversions, px2pt) { ASSERT_EQ(cu::px2pt(10), 7.5); }

TEST(TestsConversions, pt2px) { ASSERT_EQ(cu::pt2px(7.5), 10); }
