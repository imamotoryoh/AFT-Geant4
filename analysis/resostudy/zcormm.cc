/*
  zcormm.cc
  
  Toshi Gogami, January 4, 2018
*/

void zcormm(){
  // ====== General conditions ====== //
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);

  // ====== Open a ROOT file ======= //
  TFile* f1 = new TFile("10cmfiber_noreso.root");
  TTree* t1 = (TTree*)f1->Get("tree");

  TCut cut1 = "mm<20";
  double hmax = 1500.0;

  // ===== Create a histogram ======== //
  TH2D* h1 = new TH2D("h1","",100,-10.0,10.0,100,-20.0,20.);
  t1->Project("h1","mm:zBeam",cut1);
  h1->GetXaxis()->SetTitle("z (cm)");
  h1->GetYaxis()->SetTitle("-B (MeV)");
  h1->GetXaxis()->SetRangeUser(-7.0,7.0);
  TH1D* h1_ = (TH1D*)h1->ProjectionY("h1_");
  h1_->SetTitle("");
  h1_->SetMaximum(hmax);
  h1_->GetYaxis()->SetTitle("Counts / 0.4 MeV");

  TProfile* pf1 = new TProfile("pf1","",50,-10.0,10.0);
  t1->Project("pf1","mm:zBeam",cut1);
  pf1->SetMarkerColor(2);
  pf1->SetLineColor(2);
  pf1->SetMarkerStyle(25);
  
  TCanvas* c1 = new TCanvas("c1","c1");
  c1->Divide(2,1);
  c1->cd(1);
  h1->Draw("col");
  pf1->Draw("same");
  c1->cd(2);
  h1_->Draw();

  c1->cd(1);
  TF1* func1 = new TF1("func1","[0]+[1]*x",-10.0,10.0);
  func1->SetParameters(0.0,-0.1);
  pf1->Fit("func1","N","",-7.0,7.0);
  func1->SetLineStyle(9);
  func1->Draw("same");
  double p1, p2;
  p1 = func1->GetParameter(0);
  p2 = func1->GetParameter(1);
  char newhname[300], newhname2[500];
  sprintf(newhname,"mm-(%f)-(%f)*zBeam",p1,p2);
  sprintf(newhname2,"%s:zBeam",newhname);
  //cout << newhname << " " << newhname2 << endl;

  c1->cd(2);
  TF1* func2 = new TF1("func2","[0] * exp(-0.5*pow((x-[1])/[2],2.0))",-20.0,20.0);
  func2->SetNpx(1000);
  func2->SetParameters(500.0,0.0,3.0);
  h1_->Fit("func2","N","",-5.0,5.0);
  func2->Draw("same");

  // ===== Create new histogram ====== //
  TH1D* h2_ = (TH1D*)h1_->Clone("h2_");
  t1->Project("h2_",newhname,cut1);
  h2_->SetMaximum(hmax);
  TH2D* h2  = (TH2D*)h1->Clone("h2");
  t1->Project("h2",newhname2,cut1);

  TCanvas* c2 = new TCanvas("c2","c2");
  c2->Divide(2,1);
  c2->cd(1); h2->Draw("col");
  c2->cd(2); h2_->Draw();

  c2->cd(2);
  TF1* func3 = new TF1("func3","[0] * exp(-0.5*pow((x-[1])/[2],2.0))",-20.0,20.0);
  func3->SetNpx(1000);
  func3->SetParameters(760.0,0.0,3.0);
  h2_->Fit("func3","N","",-5.0,5.0);
  func3->Draw("same");

  double w1, w1_er, w2, w2_er;
  double sigma2fwhm = 2.0*sqrt(2.0*log(2.0));
  //cout << sigma2fwhm << endl;
  w1 = func2->GetParameter(2) * sigma2fwhm;
  w2 = func3->GetParameter(2) * sigma2fwhm;
  w1_er = func2->GetParError(2) * sigma2fwhm;
  w2_er = func3->GetParError(2) * sigma2fwhm;
  cout << " Before correction: "
       << w1 << " " << w1_er << " MeV " << endl;
  cout << " After correction:  "
       << w2 << " " << w2_er << " MeV " << endl;

  //c1->Print("bcor_12Xi_4.4MeV.eps","eps");
  //c2->Print("acor_12Xi_3.6MeV.eps","eps");

  
}
