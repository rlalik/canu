{
    auto can = new TCanvas("can2", "can2", 800, 200);
    can->SetFillColor(kGray);

    can->Divide(3, 1, 0, 0);

    auto h1 = new TH1I("h1", "h1;x_{XYZ}^{Q} [Xyz];counts^{2} [a.u.]", 100, -10, 10);
    auto h2 = new TH1I("h2", "h2;x_{XYZ}^{Q} [Xyz];counts^{2} [a.u.]", 100, -10, 10);
    auto h3 = new TH1I("h3", "h3;x_{XYZ}^{Q} [Xyz];counts^{2} [a.u.]", 100, -10, 10);

    h1->FillRandom("gaus", 10000);
    h2->FillRandom("gaus", 10000);
    h3->FillRandom("gaus", 10000);

    can->cd(1);
    // gPad->SetFillColor(38);
    h1->Draw();

    can->cd(2);
    // gPad->SetFillColor(38);
    h2->Draw();

    can->cd(3);
    // gPad->SetFillColor(38);
    h3->Draw();

    can->SaveAs("papers/generic/img2.C");
    can->SaveAs("papers/generic/img2.pdf");
}
