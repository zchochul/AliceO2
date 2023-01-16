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
double limit_eta_lo;
double limit_eta_hi;

void FemtoWorldDetaDphi(const char *infile = "~/Downloads/Hyperloop/51766.root", Int_t n=1,const int Mbins=4){
    TFile *ifile = new TFile(infile);
    TCanvas *c1 = new TCanvas("c1", "Results", 10, 10, 1000, 800);
    limit_eta_lo = -1.4;
    limit_eta_hi = 1.4;
    c1->Divide(3,1);
    //-------------------------------DETA---DPHI---------------------------------------------------------------------
    num = (TH2D*)ifile->Get(Form("femto-world-pair-task-track-phi/SameEvent/relPairDetaDphi"));
    den = (TH2D*)ifile->Get(Form("femto-world-pair-task-track-phi/MixedEvent/relPairDetaDphi"));
    /*scale_den = ((TH2D*)ifile->Get(Form("femto-world-pair-task-track-track/SameEvent/relPairDetaDphi")))->Integral();
    scale_num = ((TH2D*)ifile->Get(Form("femto-world-pair-task-track-track/MixedEvent/relPairDetaDphi")))->Integral();
    num -> Scale(1./scale_num);
    den -> Scale(1./scale_den);*/
    scale_den = den->GetEntries();
    scale_num = num->GetEntries();
    num->GetYaxis()->SetRangeUser(limit_eta_lo,limit_eta_hi);
    //num->Divide(den);
    //num -> Scale(1./(scale_num/scale_den));
    //num->GetYaxis()->SetRangeUser(-1.8,1.8);
    /*c1->cd(1);
    num->SetTitle("#Delta #eta #Delta #varphi");
    num->DrawCopy("surf1");*/

    //-------------------------------------------------------------------------------------------------------------------
    c1 -> cd(1);
    num -> SetTitle("#Delta #eta #Delta #varphi Same Event");
    num->GetYaxis()->SetRangeUser(limit_eta_lo,limit_eta_hi);
    num -> DrawCopy("surf1");
    //--------------
    c1 -> cd(2);
    den -> SetTitle("#Delta #eta #Delta #varphi Mixed Event");
    den->GetYaxis()->SetRangeUser(limit_eta_lo,limit_eta_hi);
    den -> DrawCopy("surf1"); 
    //--------------
    c1->cd(3);
    num->Divide(den);
    num -> Scale(1./(scale_num/scale_den));
    num->SetTitle("#Delta #eta #Delta #varphi");
    num->GetYaxis()->SetRangeUser(limit_eta_lo,limit_eta_hi);
    num->DrawCopy("surf1");
}
