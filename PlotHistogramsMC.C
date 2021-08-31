void PlotHistogramsMC(const char *infile)
{

  TFile *ifile = new TFile(infile);

  
  const int particleTypes=3;
  
  TCanvas* c1 = new TCanvas("c1","Efficiencies",10,10,2*600,400);
  c1->Divide(2,1);

  ifile->cd("efficiency-pid");

  //use "true" if you want to save efficiency histogram to file
  bool SaveFile = false; //if you want to *plot* QA histograms, put "false"!
  TFile *corrFile;
  
  TH1F* hRecoEff[particleTypes];
  TH1F* hKineEff[particleTypes];

  c1->cd(1);  
  gPad->SetBottomMargin(0.15);
  gPad->SetLeftMargin(0.15);
  const char* type;
  for(int i=0;i<particleTypes;i++)
    {
      if(i==0) type="Pi";
      else if(i==1) type="Ka";
      else if (i==2) type="Pr";

      hRecoEff[i] = (TH1F*) (gDirectory->Get(Form("pt%s",type)))->Clone();
      hKineEff[i] = (TH1F*) (gDirectory->Get(Form("pt%sMC",type)))->Clone();

      hRecoEff[i]->Sumw2(); hKineEff[i]->Sumw2();
      //hRecoEff[i]->Rebin(2);
      //hKineEff[i]->Rebin(2);
      hRecoEff[i]->Divide(hKineEff[i]);

      
      //*****Plots*********
      if(!strcmp(type,"Pi")){
	hRecoEff[i]->SetMarkerColor(kRed);
      }
      else if(!strcmp(type,"Ka"))
	{
	  hRecoEff[i]->SetMarkerColor(kBlue);
	}
      else if (!strcmp(type,"Pr"))
	{
	  hRecoEff[i]->SetMarkerColor(kMagenta);
	}
      hRecoEff[i]->SetMarkerStyle(20);
      hRecoEff[i]->SetMarkerSize(0.7);
      hRecoEff[i]->SetTitle(Form("Efficiency %s",type));
      hRecoEff[i]->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
      hRecoEff[i]->GetYaxis()->SetTitle(Form("Efficiency"));
      hRecoEff[i]->GetXaxis()->SetLabelSize(0.05);
      hRecoEff[i]->GetYaxis()->SetLabelSize(0.04);
      hRecoEff[i]->GetXaxis()->SetTitleSize(0.05);
      hRecoEff[i]->GetYaxis()->SetTitleSize(0.04);
      hRecoEff[i]->GetYaxis()->SetTitleOffset(1.25);
      //hRecoEff[i]->GetXaxis()->SetNdivisions(505);
      hRecoEff[i]->GetYaxis()->SetNdivisions(505);
      hRecoEff[i]->GetXaxis()->SetRangeUser(0,5);
      hRecoEff[i]->SetName(Form("Eff%s",type));
      if(i==0) hRecoEff[i]->Draw("P");
      else hRecoEff[i]->Draw("Psame");

      hRecoEff[i]->Draw("Csame");

    }
  
  //****Saving******
  if(SaveFile){
    corrFile = new TFile();
    TString outfile = "eff.corr.Pythia.LHC18g4.root";
    corrFile->Open(outfile.Data(),"RECREATE");
    for(int i=0;i<particleTypes;i++)
      {
      hRecoEff[i]->Write();
      }
    corrFile->Close();
    return;
  }


  //********** Single Particle Purity ***************
  TH2F* hPDG[particleTypes];
  TH1F* hPurity[particleTypes];
  int PDG;
 
  c1->cd(2);  
  gPad->SetBottomMargin(0.15);
  gPad->SetLeftMargin(0.15);

  for(int i=0;i<particleTypes;i++)
    {
      if(i==0){ type="Pi"; PDG=211;}
      else if(i==1){ type="Ka"; PDG=321;}
      else if (i==2) {type="Pr"; PDG=2212;}

      hPDG[i] = (TH2F*)gDirectory->Get(Form("PDG%s",type))->Clone();
      //hPDG[i]->Sumw2();
      hPDG[i]->Rebin2D(10,1);
      int interval = hPDG[i]->GetYaxis()->FindBin(PDG);
      hPurity[i] = (TH1F*)(hPDG[i])->ProjectionX("_py",interval-1,interval+1)->Clone();
      //hPurity[i]->Sumw2();
      hPurity[i]->Divide((TH1F*)(hPDG[i])->ProjectionX("_py",4000,8001));

      if(!strcmp(type,"Pi")){
	hPurity[i]->SetMarkerColor(kRed);
	hPurity[i]->SetLineColor(kRed);
      }
      else if(!strcmp(type,"Ka"))
	{
	  hPurity[i]->SetMarkerColor(kBlue);
	hPurity[i]->SetLineColor(kBlue);
	}
      else if (!strcmp(type,"Pr"))
	{
	  hPurity[i]->SetMarkerColor(kMagenta);
	hPurity[i]->SetLineColor(kMagenta);
	}
      hPurity[i]->SetMarkerStyle(20);
      hPurity[i]->SetMarkerSize(1.);

      gStyle->SetOptStat(0);
      hPurity[i]->SetTitle(Form("Purity"));
      hPurity[i]->GetXaxis()->SetTitle("#it{p}_{T} (GeV/#it{c})");
      hPurity[i]->GetYaxis()->SetTitle(Form("Purity"));
      hPurity[i]->GetXaxis()->SetLabelSize(0.05);
      hPurity[i]->GetYaxis()->SetLabelSize(0.05);
      hPurity[i]->GetXaxis()->SetTitleSize(0.05);
      hPurity[i]->GetYaxis()->SetTitleSize(0.04);
      hPurity[i]->GetYaxis()->SetNdivisions(505);
      hPurity[i]->GetXaxis()->SetRangeUser(0,5);
      hPurity[i]->GetYaxis()->SetRangeUser(0,1.1);
      if(i==0) hPurity[i]->Draw("P");
      else  hPurity[i]->Draw("Psame");
    }

  //*****Legend*********
  TLegend *leg = new TLegend (0.7,0.6,0.9,0.85);
  leg->SetFillColor(kWhite);
  leg->SetBorderSize(2);
  leg->SetTextSize(0.05);
  leg->SetNColumns(2);
  leg->AddEntry(hPurity[0],"#pi^{+}","p");
  leg->AddEntry(hPurity[1],"K^{+}","p");
  leg->AddEntry(hPurity[2],"p","p");
  leg->Draw();


  //********** Pt ***************
  TCanvas* c2 = new TCanvas("c2","Transverse momentum",10,10,3*300,2*400);
  c2->Divide(3,2);

  TH1F* Pt[3];
  gPad->SetLeftMargin(0.15);
  for(int i=0;i<particleTypes;i++)
    {
      if(i==0) type="Pi";
      else if(i==1) type="Ka";
      else if (i==2) type="Pr";

      Pt[i] = (TH1F*) (gDirectory->Get(Form("pt%s",type)))->Clone();
      c2->cd(i+1);
      gPad->SetLogy();
      if(!strcmp(type,"Pi")){
	Pt[i]->SetMarkerColor(kRed);
	Pt[i]->SetLineColor(kRed);
      }
      else if(!strcmp(type,"Ka"))
	{
	  Pt[i]->SetMarkerColor(kBlue);
	  Pt[i]->SetLineColor(kBlue);
	}
      else if (!strcmp(type,"Pr"))
	{
	  Pt[i]->SetMarkerColor(kMagenta);
	  Pt[i]->SetLineColor(kMagenta);
	}
      Pt[i]->GetXaxis()->SetRangeUser(0,5);
      Pt[i]->SetLineWidth(2);
      Pt[i]->DrawCopy();

      //Efficiency correction
      c2->cd(i+4);
      Pt[i]->SetTitle(Form("pt %s from MC truth",type));
      gPad->SetLogy();
      Pt[i]->Divide(hRecoEff[i]);
      Pt[i]->DrawCopy();
      hKineEff[i]->DrawCopy("same");
      // for(int bin=1; bin<Pt[i]->GetNbinsX(); bin++)
      // 	{
      // 	  Pt[i]->
      // 	}
    }

}
