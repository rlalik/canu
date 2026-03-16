#include "canu.hpp"

void _canu_2d_page(const char * macro_name)
{
    gStyle->SetPalette(kBird);

    auto canu = cu::make_nim_paper();
    canu.set_details_scale(2);

    gROOT->ProcessLine(Form(".x %s", macro_name));

    canu.make_page_wide(gPad->GetCanvas());
    canu.format_legend(gPad->GetCanvas());

    TString fname = macro_name;
    fname.ReplaceAll(".C", "");

    gPad->SaveAs(Form("out/%s.pdf", basename(fname.Data())));
    gPad->SaveAs(Form("out/%s.png", basename(fname.Data())));
    gPad->SaveAs(Form("out/%s.C", basename(fname.Data())));
}
