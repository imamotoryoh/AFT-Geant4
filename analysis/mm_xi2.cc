/*
  mm_xi.cc
  Missing mass caculations.
  [[ (K-,K+) reactions ]]
  Original file was "mm.cc"
  
  Toshi Gogami , 11July2014
*/

#include<TMath.h>
#include<TFile.h>
#include<TTree.h>
#include<iostream>
#include "constant.hh"
using namespace std;
extern Double_t CalcMM(double* a, double* b, double matar);
extern Double_t CalcMM2(double* a, double* b, double matar,double, double);

// ~~~~~~~~~~ Momentum Resolutions ~~~~~~~~~~ //
Double_t dp_o_p1 = 1.0e-3; // ~~~~~~~~~~~~~~~ // (Default)
Double_t dp_o_p2 = 6.0e-4; // ~~~~~~~~~~~~~~~ // (Default) S-2S
//Double_t dp_o_p2 = 2.0e-3; // ~~~~~~~~~~~~~~~ // SKS
//Double_t dp_o_p2 = 2.7e-3; // ~~~~~~~~~~~~~~~ // SKSMinus
//Double_t dp_o_p2 = 0.0; // ~~~~~~~~~~~~~~~ // (test)
Double_t dp1 = 0.0 , dp2 = 0.0;// ~~~~~~~~~~~ //
bool momreso=true;
//bool momreso=false;
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

// ~~~~~~~~~~ Angular Resolutions ~~~~~~~~~~ //
//const double dtheta = 2.0e-3;// rad
//const double dphi   = 2.0e-3;// rad
//const double dtheta = 4.2e-3;// rad ( ~ sqrt(3*3 + 3*3); resolution + multi. scat. )
//const double dphi   = 4.2e-3;// rad ( ~ sqrt(3*3 + 3*3); resolution + multi. scat. )
const double dtheta = 4.e-3;// rad 
const double dphi   = 4.e-3;// rad
bool angreso=true;
//bool angreso=false;
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

// ~~~~~~~~~ Energy Loss Corrections ~~~~~~~~~~~~~~~~~~ //
Double_t dpk1 = 5.4e-3;//mean value (10cm CH2) //~~~~~ //
Double_t dpk2 = 5.4e-3;//mean value (10cm CH2) //~~~~~ //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  //

Double_t dpk1_purekk = 10.1e-3;
Double_t dpk2_purekk = 10.1e-3;
Double_t dpk1_fall = 10.1e-3;
Double_t dpk2_fall = 10.1e-3;

TRandom3* dragonrand = new TRandom3();

void mm_xi2(){
  // ============ General conditions ==================
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1111);
  
  // ============ Open ROOT file ======================
  char inputfile[500];
  sprintf(inputfile,"fiber_12Xi-4.root"); // 10cm CH2 target
  TFile* f1 = new TFile(inputfile);
  TTree* t1 = (TTree*)f1->Get("tree");

  // ======= Read variables ==================
  //Double_t evID, trackID;
  Int_t evID, trackID;
  Double_t beamx, beamy, beamz;
  Double_t beamz_recon;
  Double_t beampx, beampy, beampz;
  Double_t beamp;
  Double_t beam_theta;
  Double_t beam_phi;
  Double_t x, y, z, p;
  Double_t px, py, pz;
  Double_t theta, phi;
  Double_t charge;
  Int_t posflag, eleflag, phoflag, proflag;
  Int_t kaoflag, neuflag, pioflag, muoflag;
  Double_t dE_fiberx[1000],  dE_fibery[1000]; 
  Double_t dE_fiberx_kminus[1000],  dE_fibery_kminus[1000];
  Double_t dE_fiberx_kplus[1000],  dE_fibery_kplus[1000];
  Double_t dE_fiberx_xi[1000],  dE_fibery_xi[1000];
  Double_t ent;
  // ======== Target ======
  Double_t mtar = m12C; // 12C(K-,K+)12XiBe
  //Double_t mtar = mp; // p(K-,K+)Xi
  //Double_t mtar = mLi7; // 7Li(K-,K+)7XiH
  //Double_t mtar = m10B; // 10B(K-,K+)10XiLi
  // ======================
  
  Int_t count = 0;
  Double_t mm;
  Double_t km_px, km_py, km_pz, km_p;
  Double_t km_px0, km_py0, km_pz0, km_p0;
  Double_t kp_p0;
  Double_t a[3], b[3], c[3];
  Double_t kp_dE, km_dE;
  Double_t momtrans;
  Double_t enetrans;
  Double_t E1,E2;
  Double_t xi_edep;
  Double_t dE_fiber_kkpath_pure;
  Double_t dE_fiber_kkpath;
  
  ent = t1->GetEntries();
  //ent = 1000;
  t1->SetBranchAddress("eventid" ,  &evID );
  t1->SetBranchAddress("trackID",&trackID);
  t1->SetBranchAddress("xBeam",  &beamx);
  t1->SetBranchAddress("yBeam",  &beamy);
  t1->SetBranchAddress("zBeam",  &beamz);
  t1->SetBranchAddress("zBeamR", &beamz_recon);
  //t1->SetBranchAddress("pxBeam", &beampx);
  //t1->SetBranchAddress("pyBeam", &beampy);
  //t1->SetBranchAddress("pzBeam", &beampz);
  //t1->SetBranchAddress("pBeam",  &beamp );
  t1->SetBranchAddress("thetaBeam", &beam_theta);
  t1->SetBranchAddress("phiBeam", &beam_phi);
  //t1->SetBranchAddress("xVD"  ,  &x );
  //t1->SetBranchAddress("yVD"  ,  &y );
  //t1->SetBranchAddress("zVD"  ,  &z );
  //t1->SetBranchAddress("pxVD" ,  &px);
  //t1->SetBranchAddress("pyVD" ,  &py);
  //t1->SetBranchAddress("pzVD" ,  &pz);
  //t1->SetBranchAddress("pVD"  ,  &p );
  //t1->SetBranchAddress("thetaVD",  &theta );
  //t1->SetBranchAddress("phiVD"  ,  &phi   );
  //t1->SetBranchAddress("charge" , &charge );
  //t1->SetBranchAddress("posflag", &posflag);
  //t1->SetBranchAddress("eleflag", &eleflag);
  //t1->SetBranchAddress("phoflag", &phoflag);
  //t1->SetBranchAddress("proflag", &proflag);
  //t1->SetBranchAddress("kaoflag", &kaoflag);
  //t1->SetBranchAddress("neuflag", &neuflag);
  //t1->SetBranchAddress("pioflag", &pioflag);
  //t1->SetBranchAddress("muoflag", &muoflag);
  t1->SetBranchAddress("km_px",   &km_px);
  t1->SetBranchAddress("km_py",   &km_py);
  t1->SetBranchAddress("km_pz",   &km_pz);
  t1->SetBranchAddress("km_px0",  &km_px0);
  t1->SetBranchAddress("km_py0",  &km_py0);
  t1->SetBranchAddress("km_pz0",  &km_pz0);
  t1->SetBranchAddress("km_p0",   &km_p0);
  t1->SetBranchAddress("kp_px",   &px);
  t1->SetBranchAddress("kp_py",   &py);
  t1->SetBranchAddress("kp_pz",   &pz);
  t1->SetBranchAddress("kp_p",    &p);
  t1->SetBranchAddress("kp_px0",  &beampx);
  t1->SetBranchAddress("kp_py0",  &beampy);
  t1->SetBranchAddress("kp_pz0",  &beampz);
  t1->SetBranchAddress("kp_p0",   &beamp);
  t1->SetBranchAddress("dE_fiberx",     &dE_fiberx);
  t1->SetBranchAddress("dE_fibery",     &dE_fibery);
  t1->SetBranchAddress("dE_fiberx_kminus", &dE_fiberx_kminus);
  t1->SetBranchAddress("dE_fibery_kminus", &dE_fibery_kminus);
  t1->SetBranchAddress("dE_fiberx_kplus",  &dE_fiberx_kplus);
  t1->SetBranchAddress("dE_fibery_kplus",  &dE_fibery_kplus);
  t1->SetBranchAddress("dE_fiberx_xi",     &dE_fiberx_xi);
  t1->SetBranchAddress("dE_fibery_xi",     &dE_fibery_xi);
  //t1->SetBranchAddress("xi_edep",  &xi_edep);  
  
  // ============ Create new ROOT file ===========================
  //string newfilename;
  char newfilename[300];
  //sprintf(newfilename,"ch2_10cm_new2.root");
  //sprintf(newfilename,"ch2_10cm_sks2.root");
  //sprintf(newfilename,"ch2_10cm_sksM.root");
  //sprintf(newfilename,"ch2_10cm_sks_test.root");
  //sprintf(newfilename,"ch2_10cm_xi_sks.root");
  //sprintf(newfilename,"ch2_10cm_xi_s-2s.root");
  //sprintf(newfilename,"newtest_noreso.root");
  //sprintf(newfilename,"newtest_koshikawa.root");
  //sprintf(newfilename,"ttt.root");
  sprintf(newfilename,"10cmfiber_noreso.root");
  //sprintf(newfilename,"10cmfiber_reso.root");
  //sprintf(newfilename,"ch2_5cm_new2.root");
  //sprintf(newfilename,"ch2_5cm_sks2.root");
  //sprintf(newfilename,"ch2_5cm_test.root");
  //sprintf(newfilename,"ch2_5cm_sksM.root");
  //sprintf(newfilename,"ch2_15cm_sks2.root");
  //sprintf(newfilename,"ch2_15cm_new2.root");
  //sprintf(newfilename,"ch2_15cm_sksM.root");
  //sprintf(newfilename,"notarget_new.root"); // 12C(K-,K+)12XiBe
  //sprintf(newfilename,"notarget_C12_new2.root"); // 12C(K-,K+)12XiBe
  //sprintf(newfilename,"notarget_C12_sks2.root"); // 12C(K-,K+)12XiBe
  //sprintf(newfilename,"notarget_1_new2.root"); // p(K-,K+)Xi
  //sprintf(newfilename,"notarget_1_sks2.root"); // p(K-,K+)Xi
  //sprintf(newfilename,"notarget_Li7_new2.root"); // 7Li(K-,K+)7XiH
  //sprintf(newfilename,"notarget_Li7_sks.root"); // 7Li(K-,K+)7XiH
  //sprintf(newfilename,"notarget_Li7_sks2.root"); // 7Li(K-,K+)7XiH
  //sprintf(newfilename,"notarget_B10_new2.root"); // 10Li(K-,K+)10XiLi
  //sprintf(newfilename,"notarget_B10_sks2.root"); // 10Li(K-,K+)10XiLi
  //sprintf(newfilename,"notarget_B10_test.root"); // 10Li(K-,K+)10XiLi
  //TFile* fnew = new TFile("ch2_5cm_new_.root","recreate");
  TFile* fnew = new TFile(newfilename,"recreate");
  TTree* tnew = new TTree("tree","(K-,K+) reaction");
  //TTree* tnew = new TTree("tree","(K-,K+) reaction with 5cm CH2 target (12XiBe)");
  //TTree* tnew = new TTree("tree","(K-,K+) reaction with 10cm CH2 target (12XiBe)");
  //Double_t km_p, km_p0;
  Double_t mm_cor;
  Double_t mm_cor_purekk;
  tnew->Branch("xBeam",  &beamx, "beamx/D");
  tnew->Branch("yBeam",  &beamy, "beamy/D");
  tnew->Branch("zBeam",  &beamz, "beamz/D");
  tnew->Branch("zBeamR", &beamz_recon, "beamz_recon/D");
  tnew->Branch("km_px0", &km_px0, "km_px0/D");
  tnew->Branch("km_py0", &km_py0, "km_py0/D");
  tnew->Branch("km_pz0", &km_pz0, "km_pz0/D");
  tnew->Branch("km_p0",  &km_p0, "km_p0/D");
  tnew->Branch("km_px",  &km_px,  "km_px/D");
  tnew->Branch("km_py",  &km_py,  "km_py/D");
  tnew->Branch("km_pz",  &km_pz,  "km_pz/D");
  tnew->Branch("km_p",   &km_p,   "km_p/D");
  tnew->Branch("kp_px" ,  &px,    "px/D");
  tnew->Branch("kp_py" ,  &py,    "py/D");
  tnew->Branch("kp_pz" ,  &pz,    "pz/D");
  tnew->Branch("kp_p"  ,  &p,     "p/D");
  tnew->Branch("kp_p0" ,  &beamp, "beamp/D");
  tnew->Branch("kp_theta",&theta, "theta/D");
  tnew->Branch("kp_phi"  , &phi,  "phi/D"  );
  tnew->Branch("mm"      , &mm,   "mm/D"   );
  tnew->Branch("mm_cor"   , &mm_cor,   "mm_cor/D"   );
  tnew->Branch("mm_cor_purekk"   , &mm_cor_purekk,   "mm_cor_purekk/D"   );
  tnew->Branch("km_dE"   , &km_dE,"km_dE/D");
  tnew->Branch("kp_dE"   , &kp_dE,"kp_dE/D");
  //tnew->Branch("xi_dE",    &xi_edep,"xi_dE/D");
  tnew->Branch("momtrans", &momtrans,"momtrans/D");
  tnew->Branch("enetrans", &enetrans,"enetrans/D");
  tnew->Branch("dE_fiberx", &dE_fiberx , "dE_fiberx[1000]/D");
  tnew->Branch("dE_fibery", &dE_fibery , "dE_fibery[1000]/D");
  tnew->Branch("dE_fiberx_kminus", &dE_fiberx_kminus , "dE_fiberx_kminus[1000]/D");
  tnew->Branch("dE_fibery_kminus", &dE_fibery_kminus , "dE_fibery_kminus[1000]/D");
  tnew->Branch("dE_fiberx_kplus",  &dE_fiberx_kplus  , "dE_fiberx_kplus[1000]/D");
  tnew->Branch("dE_fibery_kplus",  &dE_fibery_kplus  , "dE_fibery_kplus[1000]/D");
  tnew->Branch("dE_fiberx_xi",     &dE_fiberx_xi     , "dE_fiberx_xi[1000]/D");
  tnew->Branch("dE_fibery_xi",     &dE_fibery_xi     , "dE_fibery_xi[1000]/D");
  tnew->Branch("dE_fiber_kkpath",  &dE_fiber_kkpath  , "dE_fiber_kkpath/D");
  tnew->Branch("dE_fiber_kkpath_pure",  &dE_fiber_kkpath_pure  , "dE_fiber_kkpath_pure/D");
  
  //double ent;
  //ent = 500;
  // =============== Create histograms =======================
  TH1F* hist1 = new TH1F("hist1","-B_{#Xi}",200,-20,20);
  hist1->GetXaxis()->SetTitle("-B_{#Xi^{-}}[MeV]");
  hist1->GetYaxis()->SetTitle("[Counts / 0.2 MeV]");
  TH1F* hist2 = new TH1F("hist2","dE_{K^{-}}",100,0.0,50.0);
  hist2->GetXaxis()->SetTitle("[MeV]");
  hist2->GetYaxis()->SetTitle("[Counts / 0.5 MeV]");
  TH1F* hist3 = (TH1F*)hist2->Clone("hist3");
  hist3->SetTitle("dE_{K^{+}}");
  
  bool hit_kkpath = false;
  bool hit_kmxpath = false;
  bool hit_kmypath = false;
  bool hit_kpxpath = false;
  bool hit_kpypath = false;
  double dE_th = 0.001; // [MeV]

  // ------------------------------------   //
  //  The energy resolution data            //
  //  obtained from a test at RCNP.         //
  //  A result was taken from               //
  //  Koshikawa-san's presentation (2017).  //
  // -------------------------------------  //
  TF1* func1 = new TF1("func1","[0]+[1]/sqrt(x)");
  func1->SetParameters(-0.0334112, 8.07186);
  bool fresoFlag = false;
  //bool fresoFlag = true;
  TRandom3* dragon = new TRandom3();
  dragon->SetSeed();
  
  // ============== Main Loop starts =========================
  for(int i=0 ; i<ent ;i++){
    t1->GetEntry(i);
    //if(xi_edep > -1.0){
    
    dE_fiber_kkpath = 0.0;
    dE_fiber_kkpath_pure = 0.0;
    dpk1_fall = 0.0;
    dpk2_fall = 0.0;
    dpk1_purekk = 0.0;
    dpk2_purekk = 0.0;
    hit_kkpath = false;
    hit_kmxpath = false;
    hit_kmypath = false;
    hit_kpxpath = false;
    hit_kpypath = false;
    //if(kp_dE > -1.0){
    if(p > -1.0){
      // ~~~~~~ Fiber hit information ~~~~~~~~~~~
      for(int jj=0 ; jj<1000 ; jj++){
	if(dE_fiberx_kminus[jj]>dE_th){
	  //dE_fiber_kkpath = dE_fiber_kkpath + dE_fiberx[jj];
	  hit_kmxpath = true;
	}
	else hit_kmxpath = false;
	
	if(dE_fibery_kminus[jj]>dE_th){
	  hit_kmypath = true;
	}
	else hit_kmypath = false;
	
	if(dE_fiberx_kplus[jj]>dE_th){
	  hit_kpxpath = true;
	}
	else hit_kpxpath = false;
	
	if(dE_fibery_kplus[jj]>dE_th){
	  hit_kpypath = true;
	}
	else hit_kpypath = false;
	
	if(hit_kmxpath==true || hit_kmypath==true
	   || hit_kpxpath==true || hit_kpypath==true ){
	  hit_kkpath = true;
	}
	else hit_kkpath = false;
	
	if(hit_kkpath==true) {
	  //dE_fiber_kkpath = dE_fiber_kkpath + dE_fiberx[jj] + dE_fibery[jj]; // Original
	  //double freso = 10;
	  dE_fiber_kkpath_pure = dE_fiber_kkpath_pure
	    + dE_fiberx_kminus[jj] + dE_fibery_kminus[jj]
	    + dE_fiberx_kplus[jj]  + dE_fibery_kplus[jj];
	  //	  dE_fiber_kkpath = dE_fiber_kkpath + dE_fiberx[jj] + dE_fibery[jj]; // Original
	  dE_fiber_kkpath = dE_fiber_kkpath 
	    + dE_fiberx_kminus[jj] + dE_fibery_kminus[jj]
	    + dE_fiberx_kplus[jj] + dE_fibery_kplus[jj]
	    + dE_fiberx_xi[jj] + dE_fibery_xi[jj];
//	  cout << i << "-" <<  jj << ": "
//	       << dE_fiberx_kminus[jj]  << " + " << dE_fibery_kminus[jj] << " + "
//	       << dE_fiberx_kplus[jj] << " + " << dE_fibery_kplus[jj] << " + "
//	       << dE_fiberx_xi[jj] << " + " <<  dE_fibery_xi[jj] << endl;
	  
	  //dpk1_fall = dpk1_fall 
	  //  + dE_fiberx_kminus[jj] + dE_fibery_kminus[jj]
	  //  + dE_fiberx_xi[jj] + dE_fibery_xi[jj];
	  //dpk2_fall = dpk2_fall 
	  //  + dE_fiberx_kplus[jj] + dE_fibery_kplus[jj]
	  //  + dE_fiberx_xi[jj] + dE_fibery_xi[jj];
	  //dpk1_purekk = dpk1_purekk
	  //  + dE_fiberx_kminus[jj] + dE_fibery_kminus[jj];
	  //dpk2_purekk = dpk2_purekk
	  //  + dE_fiberx_kplus[jj] + dE_fibery_kplus[jj];
	  
	  double temp, tempd;
	  temp = dE_fiberx_kminus[jj] + dE_fibery_kminus[jj]
	    + dE_fiberx_xi[jj] + dE_fibery_xi[jj];
	  if(temp!=0 && fresoFlag==true){
	    tempd = func1->Eval(temp) * temp / 100.0;
	    //cout << temp << " " << tempd << endl;
	    temp = dragon->Gaus(temp,tempd);
	  }
	  dpk1_fall = dpk1_fall +temp;
	  
	  temp = 0;
	  temp = dE_fiberx_kplus[jj] + dE_fibery_kplus[jj]
	    + dE_fiberx_xi[jj] + dE_fibery_xi[jj];
	  if(temp!=0 && fresoFlag==true){
	    tempd = func1->Eval(temp) * temp / 100.0;
	    temp = dragon->Gaus(temp,tempd);
	  }
	  dpk2_fall = dpk2_fall + temp;
	  
	  temp = 0;
	  temp = dE_fiberx_kminus[jj] + dE_fibery_kminus[jj];
	  if(temp!=0 && fresoFlag==true){
	    tempd = func1->Eval(temp) * temp / 100.0;
	    temp = dragon->Gaus(temp,tempd);
	  }
	  dpk1_purekk = dpk1_purekk + temp;
	  
	  temp = 0;
	  temp = dE_fiberx_kplus[jj] + dE_fibery_kplus[jj];
	  if(temp!=0 && fresoFlag==true){
	    tempd = func1->Eval(temp) * temp / 100.0;
	    temp = dragon->Gaus(temp,tempd);
	  }
	  dpk2_purekk = dpk2_purekk + temp;
	}
      } // loop in jj
      
      
      a[0] = km_px0*1.0e-3;
      a[1] = km_py0*1.0e-3;
      a[2] = km_pz0*1.0e-3;
      b[0] = px*1.0e-3;
      b[1] = py*1.0e-3;
      b[2] = pz*1.0e-3;
      c[0] = km_px*1.0e-3;
      c[1] = km_py*1.0e-3;
      c[2] = km_pz*1.0e-3;
      km_p0 = sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
      km_p0 = km_p0 * 1.0e+3;
      km_p  = sqrt(c[0]*c[0]+c[1]*c[1]+c[2]*c[2]);
      km_p = km_p * 1.0e+3;
      
      TVector3 kp1(km_px,km_py,km_pz);
      TVector3 kp2(px,py,pz);
      E1 = sqrt(kp1*kp1+mk*mk*1.0e+6);
      E2 = sqrt(kp2*kp2+mk*mk*1.0e+6);
      momtrans = sqrt((kp1-kp2)*(kp1-kp2));
      enetrans = E1-E2;
      //cout << E1 << " " << E2 << " " << E1-E2 << endl;
      
      //dpk1 = dpk1*1.0e-3;
      //dpk2 = dpk2*1.0e-3;
      dpk1_fall = dpk1_fall*1.0e-3;
      dpk2_fall = dpk2_fall*1.0e-3;
      dpk1_purekk = dpk1_purekk*1.0e-3;
      dpk2_purekk = dpk2_purekk*1.0e-3;
      
      // ################################ //
      // ### Missing mass calculation ### //
      //mm = CalcMM(a,b,mtar); // ######### //
      mm     = CalcMM2(a,b,mtar,dpk1,dpk2); 
      mm_cor = CalcMM2(a,b,mtar,dpk1_fall,dpk2_fall); 
      mm_cor_purekk = CalcMM2(a,b,mtar,dpk1_purekk,dpk2_purekk); 
      // ################################ //
      
      mm = mm - m11B - mxi; // 12C(K-,K+)12XiBe
      mm_cor = mm_cor - m11B - mxi; // 12C(K-,K+)12XiBe
      mm_cor_purekk = mm_cor_purekk - m11B - mxi; // 12C(K-,K+)12XiBe
      //mm = mm - m11B - mxi; // 12C(K-,K+)12XiBe
      //mm = mm - mxi; // p(K-,K+)Xi
      //mm = mm - mHe6 - mxi; // 7Li(K-,K+)7XiH
      //mm = mm - mBe9 - mxi; // 10Li(K-,K+)10XiLi
      mm = mm * 1.0e+3;
      mm_cor = mm_cor * 1.0e+3;
      mm_cor_purekk = mm_cor_purekk * 1.0e+3;
      
      //cout << mm << endl;
      //cout << sqrt(pow(km_p0-km_p,2.0)+pow(mk,2.0)) << endl; 
      //cout << beamp-p << endl;
      //cout << sqrt(pow(kp_p0-kp_p,2.0)+pow(mk,2.0)) << endl;
      
      // ############# Energy Loss calculations ########## //
      km_dE = sqrt(pow(km_p0-km_p,2.0)+pow(mk,2.0)); //### //
      kp_dE = sqrt(pow(beamp-p,2.0)+pow(mk,2.0));// ###### //
      // ################################################# //
      //cout << km_dE << endl;
      //cout << kp_dE << endl;
      hist1->Fill(mm);
      hist2->Fill(km_dE);
      hist3->Fill(kp_dE);
      
      // ======= Fill to new ROOT file ===== //
      tnew->Fill();// ====================== //
      // =================================== //
      //cout << mm*1.0e+3 << endl;
    }
  }
  tnew->Write();
  TObjArray hlist(0);
  hlist.Add(hist1);
  hlist.Add(hist2);
  hlist.Add(hist3);
  hlist.Write();
  fnew->Close();

  // =========== Check the missing mass histogram =======
  TFile* fnew2 = new TFile(newfilename);
  TTree* tnew2 = (TTree*)fnew2->Get("tree");
  TH1F* hhh1 = (TH1F*)fnew2->Get("hist1");
  hhh1->SetFillColor(38);
  hhh1->SetFillStyle(1001);
  TCanvas* c1 = new TCanvas("c1","c1");
  gPad->SetGridx();gPad->SetGridy();
  hhh1->Draw();
  
  // ========== Comments ======================
  cout << endl;
  cout << " Input ROOT file: " << inputfile << endl;
  cout << "  --> Output ROOT file: " << newfilename << endl;
  cout << endl;
  
}

//Double_t CalcMM(double *a, double* b, ParamMan* param){
Double_t CalcMM(double *a, double* b, double mtar){
  double delphi, deltheta;
  double theta, phi;
  
  deltheta = dragonrand->Gaus( 0.0,dtheta/(2.0*sqrt(2.0*log(2.0))) );
  delphi   = dragonrand->Gaus( 0.0,dphi/(2.0*sqrt(2.0*log(2.0))) );
  //cout << deltheta << " "  << delphi << endl;
  //Double_t mtar;
  //mcore = param->GetMcore();
  //mtar  = param->GetMtar();
  //mhyp  = param->GetMhyp();
  //Double_t dpk1 = 4.4e-3;//MPV
  //Double_t dpk2 = 4.4e-3;//MPV
  //Double_t dpk1 = 0.0e-3;
  //Double_t dpk2 = 0.0e-3;
  //Double_t dpk1 = 9.08459e-3;//mean value (10cm CH2)
  //Double_t dpk2 = 9.08459e-3;//mean value (10cm CH2)
  //Double_t mom1   = a[0];
  //Double_t theta1 = a[1];
  //Double_t phi1   = a[2];
  //Double_t mom2   = b[0];
  //Double_t theta2 = b[1];
  //Double_t phi2   = b[2];
  //Double_t xp1,yp1;
  //Double_t xp2,yp2;
  Double_t kpx1, kpy1, kpz1;
  Double_t kpx2, kpy2, kpz2;
  kpx1 = a[0];
  kpy1 = a[1];
  kpz1 = a[2];
  kpx2 = b[0];
  kpy2 = b[1];
  kpz2 = b[2];
  //cout << kpx1 << " "  << kpy1 << " " << kpz1 << endl;
  //cout << kpx2 << " "  << kpy2 << " " << kpz2 << endl;
  Double_t xp1, yp1;
  Double_t xp2, yp2;
  xp1 = kpx1/kpz1;
  yp1 = kpy1/kpz1;
  xp2 = kpx2/kpz2;
  yp2 = kpy2/kpz2;
  Double_t mom1, mom2;
  mom1 = sqrt(kpx1*kpx1+kpy1*kpy1+kpz1*kpz1);
  mom2 = sqrt(kpx2*kpx2+kpy2*kpy2+kpz2*kpz2);
  theta = acos(kpz2/mom2);
  //phi   = atan(kpy2/kpx2);
  phi   = atan2(kpy2,kpx2);
  
  // ~~~~ Momentum resolutions of the spectrometers ~~~~
  if(momreso==true){
    dp1 = mom1 * dp_o_p1 / (2.0*sqrt(2.0*log(2.0)));
    dp2 = mom2 * dp_o_p2 / (2.0*sqrt(2.0*log(2.0)));
    mom1 = dragonrand->Gaus(mom1,dp1);
    mom2 = dragonrand->Gaus(mom2,dp2);
  }
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
  // ~~~~ Angular resolutions of the spectrometers ~~~~
  if(angreso==true){
    // ~~~ theta ~~~
    xp2 = xp2 + 1./pow(cos(theta),2.0)*cos(phi)*deltheta;
    yp2 = yp2 + 1./pow(cos(theta),2.0)*sin(phi)*deltheta;
    // ~~~ phi ~~~~
    xp2 = xp2 - tan(theta)*sin(phi)*delphi;
    yp2 = yp2 + tan(theta)*cos(phi)*delphi;
  }
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
  //cout << mom2 << endl;
  mom1 = mom1 - dpk1;
  mom2 = mom2 + dpk2;
  kpz1 = mom1/( sqrt(1+xp1*xp1+yp1*yp1) );
  kpx1 = kpz1 * xp1;
  kpy1 = kpz1 * yp1;
  kpz2 = mom2/( sqrt(1+xp2*xp2+yp2*yp2) );
  kpx2 = kpz2 * xp2;
  kpy2 = kpz2 * yp2;
  
  TVector3 kp1(kpx1, kpy1, kpz1);
  TVector3 kp2(kpx2, kpy2, kpz2);
  //cout << sqrt(kp1*kp1)-mom1 << endl;
  //cout << sqrt(kp2*kp2)-mom2 << endl;
  Double_t E1, E2, Et;
  E1 = sqrt(mom1*mom1+mk*mk);
  E2 = sqrt(mom2*mom2+mk*mk);
  Et = mtar;
  //cout << Et << endl;
  //cout << E1 << endl;
  //cout << E2 << endl;
  
  Double_t dE = (E1+Et-E2)*(E1+Et-E2);
  Double_t dp = (kp1-kp2)*(kp1-kp2);
  Double_t mm = 0.0;
  mm = sqrt(dE - dp);
  //cout << dp << endl;
  //cout << mm << endl;
  return mm;
}

Double_t CalcMM2(double *a, double* b, double mtar,
		 double momcor1, double momcor2){
  //double delphi, deltheta;
  double theta, phi;
  //dpk1 = momcor1;
  //dpk2 = momcor2;
  // deltheta = dragonrand->Gaus( 0.0,dtheta/(2.0*sqrt(2.0*log(2.0))) );
  //delphi   = dragonrand->Gaus( 0.0,dphi/(2.0*sqrt(2.0*log(2.0))) );
  //cout << deltheta << " "  << delphi << endl;
  //Double_t mtar;
  //mcore = param->GetMcore();
  //mtar  = param->GetMtar();
  //mhyp  = param->GetMhyp();
  //Double_t dpk1 = 4.4e-3;//MPV
  //Double_t dpk2 = 4.4e-3;//MPV
  //Double_t dpk1 = 0.0e-3;
  //Double_t dpk2 = 0.0e-3;
  //Double_t dpk1 = 9.08459e-3;//mean value (10cm CH2)
  //Double_t dpk2 = 9.08459e-3;//mean value (10cm CH2)
  //Double_t mom1   = a[0];
  //Double_t theta1 = a[1];
  //Double_t phi1   = a[2];
  //Double_t mom2   = b[0];
  //Double_t theta2 = b[1];
  //Double_t phi2   = b[2];
  //Double_t xp1,yp1;
  //Double_t xp2,yp2;
  Double_t kpx1, kpy1, kpz1;
  Double_t kpx2, kpy2, kpz2;
  kpx1 = a[0];
  kpy1 = a[1];
  kpz1 = a[2];
  kpx2 = b[0];
  kpy2 = b[1];
  kpz2 = b[2];
  //cout << kpx1 << " "  << kpy1 << " " << kpz1 << endl;
  //cout << kpx2 << " "  << kpy2 << " " << kpz2 << endl;
  Double_t xp1, yp1;
  Double_t xp2, yp2;
  xp1 = kpx1/kpz1;
  yp1 = kpy1/kpz1;
  xp2 = kpx2/kpz2;
  yp2 = kpy2/kpz2;
  Double_t mom1, mom2;
  mom1 = sqrt(kpx1*kpx1+kpy1*kpy1+kpz1*kpz1);
  mom2 = sqrt(kpx2*kpx2+kpy2*kpy2+kpz2*kpz2);
  theta = acos(kpz2/mom2);
  //phi   = atan(kpy2/kpx2);
  phi   = atan2(kpy2,kpx2);
  
  // ~~~~ Momentum resolutions of the spectrometers ~~~~
  if(momreso==true){
    dp1 = mom1 * dp_o_p1 / (2.0*sqrt(2.0*log(2.0)));
    dp2 = mom2 * dp_o_p2 / (2.0*sqrt(2.0*log(2.0)));
    mom1 = dragonrand->Gaus(mom1,dp1);
    mom2 = dragonrand->Gaus(mom2,dp2);
  }
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
  // ~~~~ Angular resolutions of the spectrometers ~~~~
  if(angreso==true){
    // ~~~ theta ~~~
    //xp2 = xp2 + 1./pow(cos(theta),2.0)*cos(phi)*deltheta;
    //yp2 = yp2 + 1./pow(cos(theta),2.0)*sin(phi)*deltheta;
    // ~~~ phi ~~~~
    //xp2 = xp2 - tan(theta)*sin(phi)*delphi;
    //yp2 = yp2 + tan(theta)*cos(phi)*delphi;
    xp2 = dragonrand->Gaus(xp2,
			   dtheta/(2.0*sqrt(2.0*log(2.0))) );
    yp2 = dragonrand->Gaus(yp2,
			   dtheta/(2.0*sqrt(2.0*log(2.0))) );
  }
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
  //cout << mom2 << endl;
  mom1 = mom1 - momcor1;
  mom2 = mom2 + momcor2;
  kpz1 = mom1/( sqrt(1+xp1*xp1+yp1*yp1) );
  kpx1 = kpz1 * xp1;
  kpy1 = kpz1 * yp1;
  kpz2 = mom2/( sqrt(1+xp2*xp2+yp2*yp2) );
  kpx2 = kpz2 * xp2;
  kpy2 = kpz2 * yp2;
  
  TVector3 kp1(kpx1, kpy1, kpz1);
  TVector3 kp2(kpx2, kpy2, kpz2);
  //cout << sqrt(kp1*kp1)-mom1 << endl;
  //cout << sqrt(kp2*kp2)-mom2 << endl;
  Double_t E1, E2, Et;
  E1 = sqrt(mom1*mom1+mk*mk);
  E2 = sqrt(mom2*mom2+mk*mk);
  Et = mtar;
  //cout << Et << endl;
  //cout << E1 << endl;
  //cout << E2 << endl;
  
  Double_t dE = (E1+Et-E2)*(E1+Et-E2);
  Double_t dp = (kp1-kp2)*(kp1-kp2);
  Double_t mm = 0.0;
  mm = sqrt(dE - dp);
  //cout << dp << endl;
  //cout << mm << endl;
  return mm;
}
