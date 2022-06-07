double fun(double *x, double *param)
{
	return param[0]*TMath::Exp(-param[1]*x[0])+param[2]*TMath::Exp(-param[3]*x[0]);
}

double chi2(double *x, double *param) {
	if (x[0]>0) {
		return TMath::Power(x[0],param[0]*0.5-1)*TMath::Exp(-x[0]*0.5)/TMath::Gamma(param[0]*0.5)/TMath::Power(2,param[0]/2);
	}
	return 0;
}
double dystChi2(double *x, double *param) {
	if (x[0]>0) {
		TF1 *fun = new TF1("fun",chi2,0,20,1);
		fun->SetParameter(0,param[0]);
		return fun->Integral(0,x[0]);
	}
	return 0;
}

bool testChi2(double T, double alpha, int ndf, double &result)
{
    TF1 *kw = new TF1(" ", dystChi2, 0, 20, 1);
    kw -> SetParameter(0,ndf);
    result = kw -> GetX(1.-alpha);
    if(T < result) {
		return true;
	} 
	else {
		return false;
	}
}

int stojek_2_2() {
	ifstream ifile;
	int xmin=0,xmax=2,nparams=4;
	TF1 *fun = new TF1("fun",fun,xmin,xmax,nparams);
	
	double tj[21],yj[21],sigmaj[21];
		
	ifile.open("dane_sw.txt");
	double t,val,sigma;
	int count=0;
	
	TCanvas *c1 = new TCanvas("c1","Ex2",10,10,700,700);
	
	while(ifile>>t>>val)
	{
		sigma=TMath::Sqrt(val);
		tj[count]=t;
		yj[count]=val;
		sigmaj[count]=sigma;
		count++;
	}
	ifile.close();
	
	TGraphErrors *gr = new TGraphErrors(count,&tj[0],&yj[0],0,&sigmaj[0]);
    gr -> SetMarkerColor(kBlack); 
    gr -> SetMarkerStyle(kFullCircle);
    gr -> SetTitle("Wykres; t; y(t)");
    gr -> Fit(fun);
    gr -> Draw("AP");
    fun -> Draw("same");
    
    double nk, npk, T;
	for (int i=0; i<21; i++) {
		nk=yj[i];
		npk=fun->Eval(tj[i]);
		T+=(nk-npk)*(nk-npk)/npk;
	}
	cout<<"T= "<<T<<endl;
	
	int ndf = count-1-4;
	double result;
	if (testChi2(T, 0.01, ndf, result)) {
		cout<<"Nie ma podstaw do odrzucenia hipotezy na poziomie istotności 0.01"<<endl;
		cout<<"Krytyczna "<<result<<endl;
	}
	else {
		cout<<"Są podstawy do odrzucenia hipotezy"<<endl;
	}
	
	return 0;
}
