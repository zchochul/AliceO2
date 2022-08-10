//DETA DPHI
TH2D* num;
TH2D* den;
double scale_num;
double scale_den;
//Invariant mass
TH1D* InvSame;
TH1D* InvMixed;
double scale_invmixed;
double scale_invsame;


void FemtoWorldAnalysisMacro(const char *infile = "~/Downloads/DrugiLongTrain.root", Int_t n=1,const int Mbins=4){
    TFile *ifile = new TFile(infile);
    TCanvas *c1 = new TCanvas("c1", "Okienko", 10, 10, 1000, 800);
    c1->Divide(2,1);

    //-------------------------------DETA---DPHI---------------------------------------------------------------------
    num = (TH2D*)ifile->Get(Form("femto-world-pair-task-track-track/SameEvent/relPairDetaDphi"));
    den = (TH2D*)ifile->Get(Form("femto-world-pair-task-track-track/MixedEvent/relPairDetaDphi"));
    /*scale_den = ((TH2D*)ifile->Get(Form("femto-world-pair-task-track-track/SameEvent/relPairDetaDphi")))->Integral();
    scale_num = ((TH2D*)ifile->Get(Form("femto-world-pair-task-track-track/MixedEvent/relPairDetaDphi")))->Integral();
    num -> Scale(1./scale_num);
    den -> Scale(1./scale_den);*/
    scale_den = den->GetEntries();
    scale_num = num->GetEntries();
    num->Divide(den);
    num -> Scale(1./(scale_num/scale_den));
    //num->GetYaxis()->SetRangeUser(-1.8,1.8);
    //num->GetYaxis()->SetLimits(-1.5,1.5);
    c1->cd(1);
    num->Draw("surf1");
    //-------------------------------------------------------------------------------------------------------------------
    //--------------------------INVARIANT----MASS-----------------------------------------------------------------------
    InvSame = (TH1D*)ifile->Get(Form("femto-world-pair-task-track-track/SameEvent/relPairInvariantMass"));
    InvMixed = (TH1D*)ifile->Get(Form("femto-world-pair-task-track-track/MixedEvent/relPairInvariantMass"));
    scale_invsame = InvSame->Integral(InvSame->FindBin(2),InvSame->FindBin(5));
    scale_invmixed = InvMixed->Integral(InvMixed->FindBin(2),InvMixed->FindBin(5)); 
    InvMixed-> Scale(scale_invsame/scale_invmixed);
    //InvSame->GetXaxis()->SetLimits(-1.5,1.5);
    //InvMixed->GetXaxis()->SetLimits(-1.5,1.5);
    InvSame->GetXaxis()->SetRangeUser(0.8,3);
    InvMixed->GetXaxis()->SetRangeUser(0.8,3);
    InvSame->SetLineColor(kRed);
    InvMixed->SetLineColor(kBlack);
    c1->cd(2);
    InvSame->Draw();
    InvMixed->Draw("Same");
}