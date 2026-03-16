{
    auto can = new TCanvas("can3", "can3", 800, 200);
    can->SetFillColor(kGray);

    can->Divide(2, 1, 0, 0);

    auto h1 = new TH1I("h1", "h1;x_{XYZ}^{Q} [Xyz];counts^{2} [a.u.]", 100, -10, 10);
    auto h2 = new TH1I("h2", "h2;x_{XYZ}^{Q} [Xyz];counts^{2} [a.u.]", 100, -10, 10);

    h1->FillRandom("gaus", 10000);
    h2->FillRandom("gaus", 10000);

    can->cd(1);
    // gPad->SetFillColor(38);
    h1->Draw();

    can->cd(2);
    // gPad->SetFillColor(38);
    h2->Draw();

    can->SaveAs("papers/generic/img3.C");
    can->SaveAs("papers/generic/img3.pdf");
}
