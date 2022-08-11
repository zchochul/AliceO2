//sygnal do wycinania ...
double Sygnal(double *x, double *par){
  if(x[0]>=1.01&&x[0]<=1.03){
    return par[0]*exp(-0.5*((x[0]-par[1])/par[2])*((x[0]-par[1])/par[2]));
  }
  else{
    TF1::RejectPoint();//omijamy ...
    return 0;  
  }
}

//sygnal całkowity...
double Sygnal2(double *x, double *par){
    return par[0]*exp(-0.5*((x[0]-par[1])/par[2])*((x[0]-par[1])/par[2]));
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

//tło całkowite...
double background2(double *x, double *par){
  return par[0] + par[1]*x[0] + par[2]*x[0]*x[0];
}

//tło +sygnał...
double fitter(double *x, double *par){
	return  par[0] + par[1]*x[0] + par[2]*x[0]*x[0]+ par[3]*exp(-0.5*((x[0]-par[4])/par[5])*((x[0]-par[4])/par[5]));
 }

void laby11(){
   TCanvas* can=new TCanvas("can","zadanie11",10,10,1000,400);
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
   hist->SetTitle("Hiperfitowanie");
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
   cout<<"scaling coefficient= "<< par4<<endl;
    cout<<"mean= "<< par5<<endl;
    cout<<"sigma= "<< abs(par6)<<endl;
 
   //sygnal dopasowanie ostateczne...
   TF1* sygnal2=new TF1("sygnal2",Sygnal2,0,3,3);
   sygnal2->SetParameters(par4,par5,par6);
   sygnal2->SetLineColor(kBlue);
   
   sygnal2->Draw("same");
   
    double sigma2_1 = par5-2*abs(par6);
    double sigma2_2 = par5+2*abs(par6);
        TLine* l = new TLine((double)sigma2_1,0,sigma2_1,400);
      l->SetLineColor(kPink+6);
            l->SetLineStyle(2);
            l->SetLineWidth(2);
           TLine* l2 = new TLine((double)sigma2_2,0,sigma2_2,400);
      l2->SetLineColor(kPink+6);
                  l2->SetLineStyle(2);
            l2->SetLineWidth(2);         
      l->Draw("same");
      l2->Draw("same");
   //to i to...
   TF1* fitdata=new TF1("fitdata",fitter,0,25,6);
   fitdata->SetParameters(par1,par2,par3,par4,par5,par6);
   fitdata->SetLineColor(kBlack);
   fitdata->Draw("same");
   double num = sygnal2->Integral((double)sigma2_1,(double)sigma2_2);
      double den = fitdata->Integral((double)sigma2_1,(double)sigma2_2);
      cout<<sigma2_2<<" "<<sigma2_1<<endl;
      cout<<"purity = "<<num/den<<endl;
   can->cd(2);
   his2->Draw();
   sygnal2->Draw("same");
}
