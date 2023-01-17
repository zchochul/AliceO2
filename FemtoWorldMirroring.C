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

void FemtoWorldMirroring(const char *infile = "~/Downloads/Hyperloop/51766.root", Int_t n=1,const int Mbins=4){
    TFile *ifile = new TFile(infile);
    TCanvas *c1 = new TCanvas("c1", "Results", 10, 10, 1000, 800);
    limit_eta_lo = -1.0;
    limit_eta_hi = 1.0;
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
    int xbins1 = num-> GetNbinsX();
    int ybins1 = num-> GetNbinsY();
    //y sym
    for (int x = 0; x <= xbins1; x++){
        for (int y = 1; y<= ybins1/2; y++){
            double val = (num->GetBinContent(x, ybins1/2 +1 + y) + num->GetBinContent(x, ybins1/2 + 1 - y))/2;
            num->SetBinContent(x, ybins1/2 +1 + y, val);
            num->SetBinContent(x, ybins1/2 +1 - y, val);
        }
    }

    // cout <<"bin phi = -1.57 -> "<< num->GetXaxis()->FindBin(-1.57); //0
    // cout <<"bin phi = 0 -> "<< num->GetXaxis()->FindBin(0.0); //3
    // cout <<"bin phi = 1.57 -> "<< num->GetXaxis()->FindBin(1.57); //7
    // cout <<"bin phi = 3.14 -> "<< num->GetXaxis()->FindBin(3.14); //10
    // cout <<"bin phi = 4.71 -> "<< num->GetXaxis()->FindBin(4.71); /15

    //x sym
    for(int y = 0; y <= ybins1; y++){
        for( int x = 1; x <= 2; x++){
            double val = (num->GetBinContent(num->GetXaxis()->FindBin(0.0) + x, y) + num->GetBinContent(num->GetXaxis()->FindBin(0.0) - x, y))/2;
            num->SetBinContent(num->GetXaxis()->FindBin(0.0) + x, y, val);
            num->SetBinContent(num->GetXaxis()->FindBin(0.0) - x, y, val);
        }

        for( int x = 1; x <= 3; x++){
            double val = (num->GetBinContent(num->GetXaxis()->FindBin(3.14) + x, y) + num->GetBinContent(num->GetXaxis()->FindBin(3.14) - x, y))/2;
            num->SetBinContent(num->GetXaxis()->FindBin(3.14) + x, y, val);
            num->SetBinContent(num->GetXaxis()->FindBin(3.14) - x, y, val);
        }
    }
    num -> SetTitle("#Delta #eta #Delta #varphi Same Event");
    num->GetYaxis()->SetRangeUser(limit_eta_lo,limit_eta_hi);
    num -> DrawCopy("surf1");
    //--------------
    c1 -> cd(2);
    int xbins2 = den-> GetNbinsX();
    int ybins2 = den-> GetNbinsY();
    //y sym
    for (int x = 0; x <= xbins2; x++){
        for (int y = 1; y<= ybins2/2; y++){
            double val = (den->GetBinContent(x, ybins2/2 +1 + y) + den->GetBinContent(x, ybins2/2 + 1 - y))/2;
            den->SetBinContent(x, ybins2/2 +1 + y, val);
            den->SetBinContent(x, ybins2/2 +1 - y, val);
        }
    }

    // cout <<"bin phi = -1.57 -> "<< den->GetXaxis()->FindBin(-1.57); //0
    // cout <<"bin phi = 0 -> "<< den->GetXaxis()->FindBin(0.0); //3
    // cout <<"bin phi = 1.57 -> "<< den->GetXaxis()->FindBin(1.57); //7
    // cout <<"bin phi = 3.14 -> "<< den->GetXaxis()->FindBin(3.14); //10
    // cout <<"bin phi = 4.71 -> "<< den->GetXaxis()->FindBin(4.71); /15

    //x sym
    for(int y = 0; y <= ybins2; y++){
        for( int x = 1; x <= 2; x++){
            double val = (den->GetBinContent(den->GetXaxis()->FindBin(0.0) + x, y) + den->GetBinContent(den->GetXaxis()->FindBin(0.0) - x, y))/2;
            den->SetBinContent(den->GetXaxis()->FindBin(0.0) + x, y, val);
            den->SetBinContent(den->GetXaxis()->FindBin(0.0) - x, y, val);
        }

        for( int x = 1; x <= 3; x++){
            double val = (den->GetBinContent(den->GetXaxis()->FindBin(3.14) + x, y) + den->GetBinContent(den->GetXaxis()->FindBin(3.14) - x, y))/2;
            den->SetBinContent(den->GetXaxis()->FindBin(3.14) + x, y, val);
            den->SetBinContent(den->GetXaxis()->FindBin(3.14) - x, y, val);
        }
    }

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
