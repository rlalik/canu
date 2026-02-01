#include "canu/canu.hpp"

auto main() -> int
{
    // values for NIM A, two columns paper
    auto canu = cu::canu(522, 252, 12); // NOLINT(*-magic-numbers)

    auto can = TCanvas("can", "can", 400, 400); // NOLINT(*-magic-numbers)

    cu::fix_canvas_size(&can);

    return 0;
}
