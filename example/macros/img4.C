{
    auto can = new TCanvas("can4", "can4", 800, 400);
    can->SetFillColor(kGray);

    auto h = new TH1I("h", "h;x_{XYZ}^{Q} [Xyz];counts^{2} [a.u.]", 100, -10, 10);

    h->FillRandom("gaus", 10000);

    can->cd();
    h->Draw();

    can->SaveAs("papers/generic/img4.C");
    can->SaveAs("papers/generic/img4.pdf");
}
