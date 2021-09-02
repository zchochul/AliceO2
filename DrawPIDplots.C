void DrawPIDplots(const char *infile)
{

 TFile *ifile = new TFile(infile);
  ifile->cd("pid-monitors");

  //********** Single Particle Purity ***************
 gStyle->SetOptFit(0001);
 gStyle->SetOptStat(0000); //Statistics?
 gStyle->SetCanvasColor(0); //Colors - white	
 gStyle->SetPadColor(0);	
 gStyle->SetStatColor(0);
// improved palette
  const Int_t NRGBs = 5;
  const Int_t NCont = 50;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);


  TH2F* pidTPCdEdx[4]; //pidTPCdEdxAll
  TH2F* pidTOFTime[4];
  TH2F* pidTPCTOFNSigma[4];

  TH2F* pidTOFNSigma[4];
  TH2F* pidTPCNSigma[4];
  const char* type;
  for(int i=0;i<3;i++)
    {
      if(i==0) {type="Pi"; int PDG = 211;}
      else if(i==1) {type="Ka"; int PDG = 321;}
      else if (i==2) {type="Pr"; int PDG = 2212;}
      else if(i==3) {type="All"; int PDG = 0;}

      cout<<Form("TpcVsMomentum%s",type)<<endl;
      pidTPCdEdx[i] = (TH2F*)gDirectory->Get(Form("TpcVsMomentum%s",type))->Clone();
      pidTPCdEdx[i]->Sumw2();
      
      pidTOFTime[i] = (TH2F*)gDirectory->Get(Form("TofVsMomentum%s",type))->Clone();
      pidTOFTime[i]->Sumw2();

      /*
      pidTPCTOFNSigma[i] = (TH2F*)gDirectory->Get(Form("pidTPCTOFNSigma%s",type))->Clone();
      pidTPCTOFNSigma[i]->Sumw2();

      pidTOFNSigma[i] = (TH2F*)gDirectory->Get(Form("pidTOFNSigma%s",type))->Clone();
      pidTOFNSigma[i]->Sumw2();

      pidTPCNSigma[i] = (TH2F*)gDirectory->Get(Form("pidTPCNSigma%s",type))->Clone();
      pidTPCNSigma[i]->Sumw2();
      */

      //****************Plots customization***************8
      pidTPCdEdx[i]->SetTitle("TPC signal vs p_{T}");
      pidTPCdEdx[i]->GetXaxis()->SetTitle("p_{T} (GeV/#it{c})");
      pidTPCdEdx[i]->GetYaxis()->SetTitle("dE/dx");
      
      pidTPCdEdx[i]->GetYaxis()->SetTitleOffset(1.2);
      pidTPCdEdx[i]->GetYaxis()->SetTitleSize(0.05);
      pidTPCdEdx[i]->GetYaxis()->SetLabelSize(0.04);
      pidTPCdEdx[i]->GetXaxis()->SetTitleSize(0.05);
      pidTPCdEdx[i]->GetXaxis()->SetLabelSize(0.04);

      pidTOFTime[i]->SetTitle("TOF signal vs p_{T}");
      pidTOFTime[i]->GetXaxis()->SetTitle("p_{T} (GeV/#it{c})");
      pidTOFTime[i]->GetYaxis()->SetTitle("TOF time");
      pidTOFTime[i]->GetYaxis()->SetTitleOffset(1.2);
      pidTOFTime[i]->GetYaxis()->SetTitleSize(0.05);
      pidTOFTime[i]->GetYaxis()->SetLabelSize(0.04);
      pidTOFTime[i]->GetXaxis()->SetTitleSize(0.05);
      pidTOFTime[i]->GetXaxis()->SetLabelSize(0.04);
      
      /*

      pidTPCTOFNSigma[i]->GetXaxis()->SetTitle("N#sigma_{TPC}");
      pidTPCTOFNSigma[i]->GetYaxis()->SetTitle("N#sigma_{TPC}");
      pidTPCTOFNSigma[i]->GetYaxis()->SetTitleOffset(1.2);
      pidTPCTOFNSigma[i]->GetYaxis()->SetTitleSize(0.05);
      pidTPCTOFNSigma[i]->GetYaxis()->SetLabelSize(0.04);
      pidTPCTOFNSigma[i]->GetXaxis()->SetTitleSize(0.05);
      pidTPCTOFNSigma[i]->GetXaxis()->SetLabelSize(0.04);

      pidTPCNSigma[i]->GetXaxis()->SetTitle("p_{T} (GeV/#it{c})");
      pidTPCNSigma[i]->GetYaxis()->SetTitle("N#sigma_{TPC}");
      pidTPCNSigma[i]->GetYaxis()->SetTitleOffset(1.2);
      pidTPCNSigma[i]->GetYaxis()->SetTitleSize(0.05);
      pidTPCNSigma[i]->GetYaxis()->SetLabelSize(0.04);
      pidTPCNSigma[i]->GetXaxis()->SetTitleSize(0.05);
      pidTPCNSigma[i]->GetXaxis()->SetLabelSize(0.04);

      pidTOFNSigma[i]->GetXaxis()->SetTitle("p_{T} (GeV/#it{c})");
      pidTOFNSigma[i]->GetYaxis()->SetTitle("N#sigma_{TOF}");
      pidTOFNSigma[i]->GetYaxis()->SetTitleOffset(1.2);
      pidTOFNSigma[i]->GetYaxis()->SetTitleSize(0.05);
      pidTOFNSigma[i]->GetYaxis()->SetLabelSize(0.04);
      pidTOFNSigma[i]->GetXaxis()->SetTitleSize(0.05);
      pidTOFNSigma[i]->GetXaxis()->SetLabelSize(0.04);
      */
    }

 //************ dE/dx, TOF time, TPC & TOF Nsigma vs momentum.
  
  TCanvas* can[4];
  can[0]= new TCanvas("c0","PID plots Pion",10,10,1000,400);
  can[0]->Divide(2);
  can[1]= new TCanvas("c1","PID plots Kaon",10,10,1000,400);
  can[1]->Divide(2);
  can[2]= new TCanvas("c2","PID plots Proton",10,10,1000,400);
  can[2]->Divide(2);
  // can[3]= new TCanvas("c3","PID plots All",10,10,2000,400);
  // can[3]->Divide(5);
  
  for(int i=0;i<3;i++)
    {
      can[i]->cd(1);
      gPad->SetLeftMargin(0.15);
      gPad->SetBottomMargin(0.15);
      gPad->SetRightMargin(0.15);
      pidTPCdEdx[i]->Draw("colz");
 
      can[i]->cd(2);
      gPad->SetLeftMargin(0.15);
      gPad->SetBottomMargin(0.15);
      pidTOFTime[i]->Draw("colz");
      /*
      can[i]->cd(3);
      gPad->SetLeftMargin(0.15);
      gPad->SetBottomMargin(0.15);
      pidTPCTOFNSigma[i]->Draw("colz");

      can[i]->cd(4);
      gPad->SetLeftMargin(0.15);
      gPad->SetBottomMargin(0.15);
      pidTPCNSigma[i]->Draw("colz");

      can[i]->cd(5);
      gPad->SetLeftMargin(0.15);
      gPad->SetBottomMargin(0.15);
      pidTOFNSigma[i]->Draw("colz");
      */
      }


}
