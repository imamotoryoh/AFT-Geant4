/*
  mmreso.cc
  
  Toshi Gogami, January 4, 2018
*/

void mmreso(){
  // ====== General conditions ====== //
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);

  // ====== Open a ROOT file ======= //
  //TFile* f1 = new TFile("10cmfiber_noreso.root");
  TFile* f1 = new TFile("10cmfiber_reso.root");
  TTree* t1 = (TTree*)f1->Get("tree");

  TCut cut1 = "mm<20";
  double hmax = 1650.0;

  // ===== Create a histogram ======== //
  double xmin = -100.0, xmax = 100.0;
  int xbin = 800;
  TH1D* h1 = new TH1D("h1","",xbin,xmin,xmax);
  h1->GetXaxis()->SetTitle("-B (MeV)");
  h1->GetYaxis()->SetTitle("Counts / 250 keV");
  h1->SetMaximum(hmax);
  h1->SetFillStyle(1001);
  h1->SetFillColor(9);
  TH1D* h2 = (TH1D*)h1->Clone("h2");
  //h1->GetXaxis()->SetRangeUser(-7.0,7.0);
  t1->Project("h1","mm_cor+2.0",cut1);
  t1->Project("h2","mm_cor_purekk+2.0",cut1);
  h2->SetMaximum(hmax);

  TCanvas* c1 = new TCanvas("c1","c1");
  gPad->SetLogy(0);
  h1->Draw();

  TF1* func1 = new TF1("func1","[0]*exp(-0.5*pow((x-[1])/[2],2.0))",-10.,10.);
  func1->SetParameters(500,0.0,1.);
  func1->SetNpx(2000);
  //func1->SetLineStyle(7);
  func1->SetLineColor(2);
  func1->SetLineWidth(1);
  func1->SetLineStyle(7);
  h1->Fit("func1","N","",-0.5,5.);
  func1->Draw("same");

  TCanvas* c2 = new TCanvas("c2","c2");
  gPad->SetLogy(0);
  h2->Draw();

  TF1* func2 = (TF1*)func1->Clone("func2");
  h2->Fit("func2","N","",-5.0,5.0);
  func2->Draw("same");

  double w1, w1_er, w2, w2_er;
  double sigma2fwhm = 2.0*sqrt(2.0*log(2.0));
  //cout << sigma2fwhm << endl;
  w1 = func1->GetParameter(2) * sigma2fwhm;
  w2 = func2->GetParameter(2) * sigma2fwhm;
  w1_er = func1->GetParError(2) * sigma2fwhm;
  w2_er = func2->GetParError(2) * sigma2fwhm;
  cout << endl;
  cout << " " << f1->GetName() << endl;
  cout << " With decay:     "
       << w1 << " " << w1_er << " MeV " << endl;
  cout << " Without decay:  "
       << w2 << " " << w2_er << " MeV " << endl;

  // ---- The number of events which are out of three sigma --- //
  double th = 0.0;
  th = func1->GetParameter(1) - 3.0*w1/sigma2fwhm;
  double dbin = (xmax-xmin)/(double)xbin;
  int ith = (th - xmin)/dbin + 1;
  //cout << " " << th  << " " << dbin << " " << ith << endl;
  int nall, nless;
  nall  = h1->Integral(0,xbin);
  nless = h1->Integral(0,ith);
  cout << " N_{<3sigma}/Nall = " << nless << "/" << nall << " = "
       << (double)nless/(double)nall << endl;

  // === Print ==== //
  //c1->Print("mm_cor_noreso.eps","eps");
  //c2->Print("mm_cor_purekk_noreso.eps","eps");
  
}


/*
  Processing mmreso.cc...
 FCN=35.9734 FROM MIGRAD    STATUS=CONVERGED     105 CALLS         106 TOTAL
                     EDM=3.26537e-09    STRATEGY= 1      ERROR MATRIX ACCURATE 
  EXT PARAMETER                                   STEP         FIRST   
  NO.   NAME      VALUE            ERROR          SIZE      DERIVATIVE 
   1  p0           4.41875e+02   2.49301e+01   2.80524e-02  -8.49286e-06
   2  p1          -5.44055e-01   1.30826e-01   6.37824e-05  -3.10441e-03
   3  p2           1.02250e+00   5.80332e-02   3.84175e-05  -4.10488e-03
 FCN=150.654 FROM MIGRAD    STATUS=CONVERGED      77 CALLS          78 TOTAL
                     EDM=6.67447e-09    STRATEGY= 1  ERROR MATRIX UNCERTAINTY   2.5 per cent
  EXT PARAMETER                                   STEP         FIRST   
  NO.   NAME      VALUE            ERROR          SIZE      DERIVATIVE 
   1  p0           1.49002e+03   1.61688e+01  -2.32836e-02   1.02069e-05
   2  p1          -2.31813e-01   7.80053e-03   7.01889e-06  -8.70658e-03
   3  p2           8.98300e-01   5.90537e-03   1.59885e-06   8.38727e-03

 10cmfiber_noreso.root
 With decay:     2.40779 0.136658 MeV 
 Without decay:  2.11533 0.0139061 MeV 
 N_{<3sigma}/Nall = 8669/13690 = 0.633236
 Info in <TCanvas::Print>: eps file mm_cor_noreso.eps has been created
 Info in <TCanvas::Print>: eps file mm_cor_purekk_noreso.eps has been created
 
  ----------------------------------------------------------------------
  
  Processing mmreso.cc...
 FCN=29.7651 FROM MIGRAD    STATUS=CONVERGED      99 CALLS         100 TOTAL
                     EDM=6.32225e-07    STRATEGY= 1      ERROR MATRIX ACCURATE 
  EXT PARAMETER                                   STEP         FIRST   
  NO.   NAME      VALUE            ERROR          SIZE      DERIVATIVE 
   1  p0           4.25861e+02   2.49294e+01   2.46551e-02  -8.44092e-05
   2  p1          -5.75933e-01   1.41285e-01   6.10566e-05  -7.72871e-03
   3  p2           1.08468e+00   6.21468e-02   3.68687e-05   1.20508e-02
 FCN=144.123 FROM MIGRAD    STATUS=CONVERGED      79 CALLS          80 TOTAL
                     EDM=2.24652e-11    STRATEGY= 1  ERROR MATRIX UNCERTAINTY   1.9 per cent
  EXT PARAMETER                                   STEP         FIRST   
  NO.   NAME      VALUE            ERROR          SIZE      DERIVATIVE 
   1  p0           1.41776e+03   1.54240e+01  -1.45523e-02  -3.01767e-07
   2  p1          -2.29369e-01   8.20685e-03   5.88511e-06  -8.61471e-04
   3  p2           9.44753e-01   6.26137e-03   2.34815e-06  -9.92680e-04

 10cmfiber_reso.root
 With decay:     2.55423 0.146344 MeV 
 Without decay:  2.22472 0.0147444 MeV 
 N_{<3sigma}/Nall = 8494/13690 = 0.620453
 Info in <TCanvas::Print>: eps file mm_cor_reso.eps has been created
 Info in <TCanvas::Print>: eps file mm_cor_purekk_reso.eps has been created
*/



