{
    auto can = new TCanvas("can1", "can1", 800, 200);
    can->SetFillColor(kGray);

    auto h = new TH1I("h1", "h1;x_{XYZ}^{Q} [Xyz];counts^{2} [a.u.]", 100, -10, 10);

    h->FillRandom("gaus", 10000);

    can->cd();
    h->Draw();

    can->SaveAs("papers/generic/img1.C");
    can->SaveAs("papers/generic/img1.pdf");
}
