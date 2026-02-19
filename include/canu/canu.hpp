/* Copyright (C) 2026 Rafał Lalik
   SPDX-License-Identifier: GPL-3.0-or-later
   Authors: Rafał Lalik [committer] */

#ifndef RL_CANVAS_UNIFIER_HPP
#define RL_CANVAS_UNIFIER_HPP

#include <string>

#include <TCanvas.h>
#include <TColor.h>
#include <TH1.h>
#include <THStack.h>
#include <TLatex.h>
#include <TStyle.h>

#include <optional>
#include <set>

namespace cu
{

constexpr std::array<Color_t, 6> color_p6 = {kP6Blue, kP6Yellow, kP6Red, kP6Grape, kP6Gray, kP6Violet};
constexpr std::array<Color_t, 8> color_p8 = {kP8Blue, kP8Orange, kP8Red, kP8Pink, kP8Green, kP8Cyan, kP8Azure, kP8Gray};
constexpr std::array<Color_t, 10> color_p10 = {kP10Blue,  kP10Yellow, kP10Red,   kP10Gray, kP10Violet,
                                               kP10Brown, kP10Orange, kP10Green, kP10Ash,  kP10Cyan};

constexpr auto pt2px(auto value_pt) { return static_cast<UInt_t>(value_pt * 4 / 3); }

constexpr auto px2pt(auto value_px) { return static_cast<double>(value_px) * 3 / 4; }

/**
 * @brief Resize canvas to true sizes specified by TCanvas arguments.
 *
 * @param can canvas to change size
 * @param override_width new width to set
 * @param override_height new height to set, if left uninitialzied or empty `{}`, then aspect ration
 * of the canvas will be preserved
 * @param override_scale scale all dimensions, equal to 1 if left uninitialzied
 */
inline auto set_canvas_size(TCanvas* can, UInt_t override_width, std::optional<UInt_t> override_height = {},
                            std::optional<UInt_t> override_scale = {}) -> UInt_t
{
    const auto winw = can->GetWw();
    const auto winh = can->GetWh();

    const auto new_width = override_width * override_scale.value_or(1);
    const auto ratio = new_width / static_cast<double>(winw);
    const auto new_height = override_height.value_or(static_cast<UInt_t>(winh * ratio));

    // BUG in ROOT with SetWindowSize() for batch mode? Here is workaround
    if (can->IsBatch() && !can->IsWeb()) { can->SetCanvasSize(new_width, new_height); }
    else
    {
        const auto width = can->GetWindowWidth();
        const auto height = can->GetWindowHeight();

        can->SetWindowSize(new_width + (width - winw), new_height + (height - winh));
    }

    std::printf("Resize canvas %s : %dx%d to %dx%d\n", can->GetName(), winw, winh, new_width, new_height);

    return new_height;
}

/**
 * @brief Resize canvas to true sizes specified by TCanvas arguments.
 *
 * Mast be called only once on each canvas. Calling SetWindowSize() on a canvas will not change the
 * constructor call's width,height but only Www and Wh. Thus anu further subsequent call of this
 * function will resize the canvas to the original size.
 *
 * @param can canvas to be fixed
 */
inline auto fix_canvas_size(TCanvas* can)
{
    const auto winw = can->GetWw();
    const auto winh = can->GetWh();

    const auto width = can->GetWindowWidth();
    const auto height = can->GetWindowHeight();

    // BUG in ROOT with SetWindowSize() for batch mode? Here is workaround
    if (can->IsBatch() && !can->IsWeb()) { can->SetCanvasSize(width, height); }
    else
    {
        can->SetWindowSize(width + (width - winw), height + (height - winh));
    }

    std::printf("Fixing canvas size from %dx%d to %dx%d\n", winw, winh, can->GetWw(), can->GetWh());
}

/**
 * @brief Set fonts for all canvases.
 *
 * @param size given in points, should be same as the paper font size
 * @param font find type, should be x3 of ROOT fonts, otherwise result will be broken.
 */
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters,*-magic-numbers)
constexpr auto set_global_fonts(UInt_t size, Style_t font = 43)
{
    // static_assert(font % 10 == 3, "Font must of of x3 family of ROOT fonts."); FIXME make it
    // working

    gStyle->SetLabelFont(font, "xyz");
    gStyle->SetLabelSize(size, "xyz");
    gStyle->SetTitleFont(font, "xyz");
    gStyle->SetTitleSize(size, "xyz");
}

/**
 * @brief Set fonts for a canvas.
 *
 * @param can TCanvas to modify
 * @param size given in points, should be same as the paper font size
 * @param font find type, should be x3 of ROOT fonts, otherwise result will be broken.
 */
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters,*-magic-numbers,misc-no-recursion)
constexpr auto set_canvas_fonts(auto* can, UInt_t size, Style_t font = 43) -> void
{
    // static_assert(font % 10 == 3, "Font must of of x3 family of ROOT fonts."); FIXME make it
    // working

    auto axis_settings = [&](auto* axis) -> void
    {
        axis->SetLabelFont(font);
        axis->SetLabelSize(size);
        axis->SetTitleFont(font);
        axis->SetTitleSize(size);
    };

    for (auto primitive : *can->GetListOfPrimitives())
    {
        if (auto* npad = dynamic_cast<TVirtualPad*>(primitive); npad != nullptr)
        {
            set_canvas_fonts(npad, size, font);
            continue;
        }

        if (auto* hist = dynamic_cast<TH1*>(primitive); hist != nullptr)
        {
            axis_settings(hist->GetXaxis());
            axis_settings(hist->GetYaxis());
            axis_settings(hist->GetZaxis());

            hist->SetTitleFont(font);
            hist->SetTitleSize(size);

            hist->SetTitle("");
        }
    }
}

/**
 * @brief Set fonts for a canvas.
 *
 * @param can TCanvas to modify
 * @param mleft margin left
 * @param mright margin right
 * @param mbot margin bottom
 * @param mtop margin top
 * @param reference_size reference for which all margins are normalized
 */
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters,*-magic-numbers)
constexpr auto set_canvas_margins(TCanvas* can, Float_t mleft, Float_t mright, Float_t mbot, Float_t mtop,
                                  UInt_t reference_size)
{
    const auto can_width = can->GetWw();
    const auto can_height = can->GetWh();

    auto margin_settings = [&](auto* pad)
    {
        const auto width = pad->GetWNDC() * can_width;
        const auto height = pad->GetHNDC() * can_height;

        const auto scale_w = reference_size / width;
        const auto scale_h = reference_size / height;

        pad->SetMargin(mleft * scale_w, mright * scale_w, mbot * scale_h, mtop * scale_h);

        std::printf("Set margins for ref size = %d with canvas %.2fx%.2f -> scale = %.2f %.2f\n"
                    "  %.3f %.3f %.3f %.3f -> %.3f %.3f %.3f %.3f\n"
                    "  %5.0f %5.0f %5.0f %5.0f -> %5.0f %5.0f %5.0f %5.0f\n",
                    reference_size, width, height, scale_w, scale_h,                                               //
                    mleft, mright, mbot, mtop, mleft * scale_w, mright * scale_w, mbot * scale_h, mtop * scale_h,  //
                    mleft * reference_size, mright * reference_size, mbot * reference_size, mtop * reference_size, //
                    pad->GetLeftMargin() * width, pad->GetRightMargin() * width,                                   //
                    pad->GetBottomMargin() * height, pad->GetTopMargin() * height);
    };

    margin_settings(can);

    for (auto* primitive : *can->GetListOfPrimitives())
    {
        auto* pad = dynamic_cast<TVirtualPad*>(primitive);
        if (pad != nullptr) { margin_settings(pad); }
    }
}

class canu;

class axis_properties
{
    std::optional<Int_t> m_max_digits{};
    std::optional<Int_t> m_ndivisions{};
    std::optional<Float_t> m_toffset{}; // title offsets
    std::optional<Float_t> m_loffset{}; // label offsets

    friend class canu;

public:
    // clang-format off
    auto set_max_digits(Int_t max_digits) -> axis_properties& { m_max_digits = max_digits; return *this; }
    auto set_ndivisions(Int_t ndivisions) -> axis_properties& { m_ndivisions = ndivisions; return *this; }
    auto set_title_offset(Float_t offset) -> axis_properties& { m_toffset = offset; return *this; }
    auto set_label_offset(Float_t offset) -> axis_properties& { m_loffset = offset; return *this; }
    // clang-format on
};

class canu
{
public:
    /**
     * @brief Initialize canvas uniform class
     *
     * @param page_width page width in pt
     * @param column_width column_width in pt
     * @param font_size font size in pt, should be the font size of the paper
     */
    // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
    canu(UInt_t page_width, UInt_t column_width, UInt_t font_size)
        : m_page_width{page_width}, m_column_width{column_width}, m_font_size{font_size}
    {
    }

    auto get_page_width() const { return m_page_width; }
    auto get_column_width() const { return m_column_width; }

    // clang-format off
    auto set_details_scale(UInt_t scale) -> canu& { m_details_scale = scale; return *this; }
    auto set_remove_title(bool remove) -> canu& { m_remove_title = remove; return *this; }
    // clang-format on

    // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
    auto set_margins(std::optional<Float_t> margin_left, std::optional<Float_t> margin_right,
                     std::optional<Float_t> margin_bottom, std::optional<Float_t> margin_top)
    {
        if (margin_left.has_value()) m_margin_left = margin_left.value();
        if (margin_right.has_value()) m_margin_right = margin_right.value();
        if (margin_bottom.has_value()) m_margin_bottom = margin_bottom.value();
        if (margin_top.has_value()) m_margin_top = margin_top.value();

        return *this;
    }

    auto x_prop() -> axis_properties& { return m_axis_x; }
    auto y_prop() -> axis_properties& { return m_axis_y; }
    auto z_prop() -> axis_properties& { return m_axis_z; }

    auto make_page_wide(TCanvas* can, std::optional<UInt_t> height = {})
    {
        single_canvas_action(can);
        m_page_height = set_canvas_size(can, m_page_width, height, m_details_scale);
        set_canvas_fonts(can);
        set_canvas_margins(can, m_margin_left, m_margin_right, m_margin_bottom, m_margin_top,
                           m_page_width * m_details_scale);
    }

    auto make_column_wide(TCanvas* can, std::optional<UInt_t> height = {})
    {
        single_canvas_action(can);
        m_page_height = set_canvas_size(can, m_column_width, height, m_details_scale);
        set_canvas_fonts(can);
        set_canvas_margins(can, m_margin_left, m_margin_right, m_margin_bottom, m_margin_top,
                           m_page_width * m_details_scale);
    }

private:
    auto single_canvas_action(TCanvas* can) -> void
    {
        if (!m_canvas_tracker.contains(can))
        {
            m_canvas_tracker.insert(can);
            fix_canvas_size(can);
        }
    }

    /**
     * @brief Set fonts for a canvas.
     *
     * @param can TCanvas to modify
     */
    // NOLINTNEXTLINE(bugprone-easily-swappable-parameters,*-magic-numbers,misc-no-recursion)
    constexpr auto set_canvas_fonts(auto* pad) -> void
    {
        const auto pad_width = pad->GetWNDC() * pad->GetWw();
        const auto pad_height = pad->GetHNDC() * pad->GetWh();

        std::printf("Pad width x height = %.0f x %.0f\n", pad_width, pad_height);

        auto axis_settings = [&](auto* axis, auto& axis_prop, auto scale) -> void
        {
            axis->SetLabelFont(m_font_kind);
            axis->SetLabelSize(m_font_size * m_details_scale);
            axis->SetTitleFont(m_font_kind);
            axis->SetTitleSize(m_font_size * m_details_scale);

            axis->SetMaxDigits(axis_prop.m_max_digits.value_or(axis->GetMaxDigits()));
            axis->SetNdivisions(axis_prop.m_ndivisions.value_or(axis->GetNdivisions()));
            axis->SetTitleOffset(axis_prop.m_toffset.value_or(axis->GetTitleOffset()));
            axis->SetLabelOffset(axis_prop.m_loffset.value_or(axis->GetLabelOffset()) * scale);

            std::printf("MaxDigits=%d Ndiv=%d to=%.3f lo=%.3f scale=%.3f w=%d h=%d\n", //
                        axis_prop.m_max_digits.value_or(axis->GetMaxDigits()),
                        axis_prop.m_ndivisions.value_or(axis->GetNdivisions()),
                        axis_prop.m_toffset.value_or(axis->GetTitleOffset()),
                        axis_prop.m_loffset.value_or(axis->GetLabelOffset()) * scale, //
                        scale, m_page_width, m_page_height);
        };

        for (auto primitive : *pad->GetListOfPrimitives())
        {
            // if found embedded canvas go into it, otherwise check for histograms
            if (auto* npad = dynamic_cast<TVirtualPad*>(primitive); npad != nullptr)
            {
                set_canvas_fonts(npad);
                continue;
            }

            if (auto* hist = dynamic_cast<TH1*>(primitive); hist != nullptr)
            {
                if (m_remove_title) { hist->SetTitle(""); }

                axis_settings(hist->GetXaxis(), m_axis_x, 1. * m_page_width / pad_height);
                axis_settings(hist->GetYaxis(), m_axis_y, 1. * m_page_width / pad_width);
                axis_settings(hist->GetZaxis(), m_axis_z, 1.);

                hist->SetTitleFont(m_font_kind);
                hist->SetTitleSize(m_font_size * m_details_scale);
            }
            else if (auto* hstack = dynamic_cast<THStack*>(primitive); hstack != nullptr)
            {
                if (m_remove_title) { hstack->SetTitle(""); }

                axis_settings(hstack->GetXaxis(), m_axis_x, 1. * m_page_width / pad_height);
                axis_settings(hstack->GetYaxis(), m_axis_y, 1. * m_page_width / pad_width);
                axis_settings(hstack->GetZaxis(), m_axis_z, 1.);
            }
        }
    }

    UInt_t m_page_width;
    UInt_t m_page_height{0};
    UInt_t m_column_width;

    UInt_t m_font_size;
    Style_t m_font_kind{43}; // NOLINT(*-magic-numbers)

    Float_t m_margin_left{0.05F};   // NOLINT(*-magic-numbers)
    Float_t m_margin_right{0.05F};  // NOLINT(*-magic-numbers)
    Float_t m_margin_bottom{0.05F}; // NOLINT(*-magic-numbers)
    Float_t m_margin_top{0.05F};    // NOLINT(*-magic-numbers)

    UInt_t m_details_scale{1}; // NOLINT(*-magic-numbers)

    axis_properties m_axis_x, m_axis_y, m_axis_z;

    bool m_remove_title{true};

    std::set<TCanvas*> m_canvas_tracker;
};

constexpr auto make_nim_paper()
{
    auto canu = cu::canu(522, 252, 10);             // NOLINT(*-magic-numbers)
    canu.set_margins(0.065F, 0.065F, 0.05F, 0.03F); // NOLINT(*-magic-numbers)

    canu.x_prop().set_max_digits(3).set_label_offset(0.005).set_title_offset(1.0);
    canu.y_prop().set_max_digits(3).set_label_offset(0.005).set_title_offset(1.25);
    canu.z_prop().set_max_digits(3).set_label_offset(0.005);

    return canu;
}

} // namespace cu

#endif
