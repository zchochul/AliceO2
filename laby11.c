//sygnal do wycinania ...
double Sygnal(double *x, double *par){
  if(x[0]>=1.01&&x[0]<=1.03){
    //return par[0]*exp(-0.5*((x[0]-par[1])/par[2])*((x[0]-par[1])/par[2]));
    return (0.5*par[0]*par[1]/TMath::Pi()) / TMath::Max(1.e-10, (x[0]-par[2])*(x[0]-par[2])+ .25*par[1]*par[1]);
  }
  else{
    TF1::RejectPoint();//omijamy ...
    return 0;  
  }
}

//sygnal całkowity...
double Sygnal2(double *x, double *par){
   // return par[0]*exp(-0.5*((x[0]-par[1])/par[2])*((x[0]-par[1])/par[2]));
   return (0.5*par[0]*par[1]/TMath::Pi()) / TMath::Max(1.e-10, (x[0]-par[2])*(x[0]-par[2])+ .25*par[1]*par[1]);
}

//tło do wycinania...
double background(double *x, double *par){
 if((x[0]>=0.97 && x[0]<=1.01)|| (x[0]>=1.03 && x[0]<=1.1)){
  return par[0] + par[1]*x[0] + par[2]*x[0]*x[0];
 }
 else{
    TF1::RejectPoint();
    return 0; 
  }
}

//lorentz
Double_t lorentzianPeak(Double_t *x, Double_t *par) {
      return (0.5*par[0]*par[1]/TMath::Pi()) / TMath::Max(1.e-10,(x[0]-par[2])*(x[0]-par[2])+ .25*par[1]*par[1]);
   }

//tło całkowite...
double background2(double *x, double *par){
  return par[0] + par[1]*x[0] + par[2]*x[0]*x[0];
}

//tło +sygnał...
double fitter(double *x, double *par){
	return  par[0] + par[1]*x[0] + par[2]*x[0]*x[0]+ //par[3]*exp(-0.5*((x[0]-par[4])/par[5])*((x[0]-par[4])/par[5]));
   (0.5*par[3]*par[4]/TMath::Pi()) / TMath::Max(1.e-10, (x[0]-par[5])*(x[0]-par[5])+ .25*par[4]*par[4]);
 }

void laby11(){
   TCanvas* can=new TCanvas("can","Fitting",10,10,1000,400);
   can->Divide(2,1);
   can->cd(1);
   TFile* plik=new TFile("nazwa.root");
   TH1D* hist=(TH1D*)plik->Get("same");//otwieranie pliku root...
   TH1D* hist2=(TH1D*)plik->Get("mixed");//otwieranie pliku root...
   
   hist->Sumw2();
   hist->Divide(hist2);
   //hist->Rebin(10);
      hist->GetXaxis()->SetRangeUser(0.99,1.05);
   int nBins=hist->GetEntries();//ile binów...
   TF1* function1=new TF1("function1",background,0,2,3);
   hist->Draw();//data histogram
   hist->SetTitle("Fitting");
   hist->Fit(function1,"N");//fitowanie tła
   double par1=function1->GetParameter(0);
   double par2=function1->GetParameter(1);
   double par3=function1->GetParameter(2);
   cout<<"par1ssjsjsj "<<par1<<endl;

   TF1* kwadratowa=new TF1("kwadratowa",background2,0,3,3);//samo tło
   kwadratowa->SetParameters(par1,par2,par3);
   kwadratowa->Draw("same");


   TH1D *his2=(TH1D*)hist->Clone("signal");//signal histpgram 
   for(int i=1;i<=nBins;i++){
   
     his2->SetBinContent(i,(int)(hist->GetBinContent(i)-kwadratowa->Eval(hist->GetBinCenter(i))));
     
   }

   // his2->Draw("same");
   // sygnal dopasowanie z wycięciem ...
   TF1* sygnal=new TF1("sygnalz",Sygnal,0,3,3);
   sygnal->SetParameters(5,1.02,0.01); //wspolczynnik skalowania, srednia, sigma
   for(int iii=0;iii<nBins;iii++)
   	if(his2->GetBinContent(iii+1)<0)
   		his2->SetBinContent(iii+1,0);
   		//his2->Draw();
   his2->Fit(sygnal,"N");
   double par4=sygnal->GetParameter(0);
   double par5=sygnal->GetParameter(1);
   double par6=sygnal->GetParameter(2);
   //cout<<"scaling coefficient= "<< par4<<endl;
    cout<<"mean= "<< par6<<endl;
    cout<<"sigma= "<< abs(par5)<<endl;
 
   //sygnal dopasowanie ostateczne...
   TF1* sygnal2=new TF1("sygnal2",Sygnal2,0,3,3);
   sygnal2->SetParameters(par4,par5,par6);
   sygnal2->SetLineColor(kBlue);
   
   sygnal2->Draw("same");
   
    double sigma2_1 = par6-2*abs(par5);
    double sigma2_2 = par6+2*abs(par5);
    double sigma3_1 = par6-3*abs(par5);
    double sigma3_2 = par6+3*abs(par5);
    cout << "sigma3_1 = " << sigma3_1<< endl;
    cout << "sigma2_1 = " << sigma2_1<< endl;
    cout << "mean= " << par6 << endl;
    cout << "sigma2_2 = " << sigma2_2<< endl;
    cout << "sigma3_2 = " << sigma3_2<< endl;

      TLine* l21 = new TLine((double)sigma2_1,0,sigma2_1,400);
            l21->SetLineColor(kPink+6);
            l21->SetLineStyle(2);
            l21->SetLineWidth(2);
      TLine* l22 = new TLine((double)sigma2_2,0,sigma2_2,400);
            l22->SetLineColor(kPink+6);
            l22->SetLineStyle(2);
            l22->SetLineWidth(2);   
      TLine* l31 = new TLine((double)sigma3_1,0,sigma3_1,400);
            l31->SetLineColor(kPink+8);
            l31->SetLineStyle(2);
            l31->SetLineWidth(2);
      TLine* l32 = new TLine((double)sigma3_2,0,sigma3_2,400);
            l32->SetLineColor(kPink+8);
            l32->SetLineStyle(2);
            l32->SetLineWidth(2);       
      l21->Draw("same");
      l22->Draw("same");
      l31->Draw("same");
      l32->Draw("same");
   //to i to...
   TF1* fitdata=new TF1("fitdata",fitter,0,25,6);
   fitdata->SetParameters(par1,par2,par3,par4,par5,par6);
   fitdata->SetLineColor(kBlack);
   fitdata->Draw("same");
   double num = sygnal2->Integral((double)sigma2_1,(double)sigma2_2);
      double den = fitdata->Integral((double)sigma2_1,(double)sigma2_2);
      cout<<sigma2_2<<" "<<sigma2_1<<endl;
      float purity = num/den;
      cout<<"purity = "<<purity<<endl;
  
   //napis------------------------------------
   
   TLatex *jj=new TLatex(.15, .8,   Form("#frac{S}{S+B} = %.2f for 2#sigma", purity));
   jj->SetNDC();
   jj->Draw();
   TLatex *j=new TLatex(.15, .7,   Form("#sigma= %.4f", par5));
   j->SetNDC();
   //j->Draw();
    //---------------------koniec napisu
   can->cd(2);
   his2->Draw();
   sygnal2->Draw("same");
}
