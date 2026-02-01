#include <gtest/gtest.h>

#include "canu/canu.hpp"

TEST(TestsCanvasResize, CanvasResize)
{
    constexpr auto base_w = 400;
    constexpr auto base_h = 400;

    auto can = TCanvas("can", "can", base_w, base_h);

    cu::fix_canvas_size(&can);

    ASSERT_EQ(can.GetWindowWidth(), base_w);
    ASSERT_EQ(can.GetWindowHeight(), base_h);

    ASSERT_EQ(can.GetWw(), can.GetWindowWidth());
    ASSERT_EQ(can.GetWh(), can.GetWindowHeight());

    cu::set_canvas_size(&can, 2 * base_w);

    ASSERT_EQ(can.GetWw(), 2 * base_w);
    ASSERT_EQ(can.GetWh(), 2 * base_h);
}
