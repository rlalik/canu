#include <gtest/gtest.h>

#include "canu/canu.hpp"

#include <memory>
#include <tuple>

constexpr float margin_left{0.1};
constexpr float margin_right{0.1};
constexpr float margin_bottom{0.1};
constexpr float margin_top{0.1};
constexpr int canvas_base_size{200};
constexpr int canvas_ref_size{2 * canvas_base_size};

namespace
{
auto get_canvas(int fact_x, int fact_y) -> std::tuple<std::unique_ptr<TCanvas>, UInt_t, UInt_t>
{
    auto can = std::make_unique<TCanvas>("can", "can", fact_x * canvas_base_size, fact_y * canvas_base_size);

    cu::fix_canvas_size(can.get());
    cu::set_canvas_margins(can.get(), margin_left, margin_right, margin_bottom, margin_top, canvas_ref_size);

    return std::make_tuple(std::move(can), can->GetWindowWidth(), can->GetWindowHeight());
}

} // namespace

TEST(TestsCanvasMargins, Canvas1x1)
{
    auto [can, can_ref_w, can_ref_h] = get_canvas(1, 1);

    ASSERT_FLOAT_EQ(can->GetLeftMargin() * can_ref_w, margin_left * canvas_ref_size);
    ASSERT_FLOAT_EQ(can->GetRightMargin() * can_ref_w, margin_right * canvas_ref_size);
    ASSERT_FLOAT_EQ(can->GetBottomMargin() * can_ref_h, margin_bottom * canvas_ref_size);
    ASSERT_FLOAT_EQ(can->GetTopMargin() * can_ref_h, margin_top * canvas_ref_size);
}

TEST(TestsCanvasMargins, Canvas2x1)
{
    auto [can, can_ref_w, can_ref_h] = get_canvas(2, 1);

    ASSERT_FLOAT_EQ(can->GetLeftMargin() * can_ref_w, margin_left * canvas_ref_size);
    ASSERT_FLOAT_EQ(can->GetRightMargin() * can_ref_w, margin_right * canvas_ref_size);
    ASSERT_FLOAT_EQ(can->GetBottomMargin() * can_ref_h, margin_bottom * canvas_ref_size);
    ASSERT_FLOAT_EQ(can->GetTopMargin() * can_ref_h, margin_top * canvas_ref_size);
}

TEST(TestsCanvasMargins, Canvas1x2)
{
    auto [can, can_ref_w, can_ref_h] = get_canvas(1, 2);

    ASSERT_FLOAT_EQ(can->GetLeftMargin() * can_ref_w, margin_left * canvas_ref_size);
    ASSERT_FLOAT_EQ(can->GetRightMargin() * can_ref_w, margin_right * canvas_ref_size);
    ASSERT_FLOAT_EQ(can->GetBottomMargin() * can_ref_h, margin_bottom * canvas_ref_size);
    ASSERT_FLOAT_EQ(can->GetTopMargin() * can_ref_h, margin_top * canvas_ref_size);
}

TEST(TestsCanvasMargins, Canvas2x2)
{
    auto [can, can_ref_w, can_ref_h] = get_canvas(2, 2);

    ASSERT_FLOAT_EQ(can->GetLeftMargin() * can_ref_w, margin_left * canvas_ref_size);
    ASSERT_FLOAT_EQ(can->GetRightMargin() * can_ref_w, margin_right * canvas_ref_size);
    ASSERT_FLOAT_EQ(can->GetBottomMargin() * can_ref_h, margin_bottom * canvas_ref_size);
    ASSERT_FLOAT_EQ(can->GetTopMargin() * can_ref_h, margin_top * canvas_ref_size);
}

TEST(TestsCanvasMargins, Canvas3x1)
{
    auto [can, can_ref_w, can_ref_h] = get_canvas(3, 1);

    ASSERT_FLOAT_EQ(can->GetLeftMargin() * can_ref_w, margin_left * canvas_ref_size);
    ASSERT_FLOAT_EQ(can->GetRightMargin() * can_ref_w, margin_right * canvas_ref_size);
    ASSERT_FLOAT_EQ(can->GetBottomMargin() * can_ref_h, margin_bottom * canvas_ref_size);
    ASSERT_FLOAT_EQ(can->GetTopMargin() * can_ref_h, margin_top * canvas_ref_size);
}

TEST(TestsCanvasMargins, Canvas4x3)
{
    auto [can, can_ref_w, can_ref_h] = get_canvas(4, 3);

    ASSERT_FLOAT_EQ(can->GetLeftMargin() * can_ref_w, margin_left * canvas_ref_size);
    ASSERT_FLOAT_EQ(can->GetRightMargin() * can_ref_w, margin_right * canvas_ref_size);
    ASSERT_FLOAT_EQ(can->GetBottomMargin() * can_ref_h, margin_bottom * canvas_ref_size);
    ASSERT_FLOAT_EQ(can->GetTopMargin() * can_ref_h, margin_top * canvas_ref_size);
}
