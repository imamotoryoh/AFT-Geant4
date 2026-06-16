/*
  genseed.cc
  
  Toshi Gogami , 21Aug2014
*/


#include "constant.hh"
//#include "create_seed.hh"
//#include "ParamMan.hh"

#include <TMath.h>
#include <TFile.h>
#include <TTree.h>
#include <TRandom3.h>
#include <fstream>
#include <iostream>
#include <TVector3.h>
#include <fstream>
using namespace std;

//void create_seed(char* inputfile){
int main(int argc, char** argv){
  // ======= Open input ROOT file ===============
  char inputfile[300];
  if(argc==2) sprintf(inputfile,"%s",argv[1]);
  else sprintf(inputfile,"ch2_5cm.root");
  cout << " Opened an input ROOT file: " 
       << inputfile << endl;
  //cout << endl;
  TFile* f1 = new TFile(inputfile);
  TTree* t1 = (TTree*)f1->Get("tree");
  
  // =========== Read input file ================
  char ofilename[500];
  Double_t maxent=0;
  ifstream* ifs = new ifstream("input.dat");
  *ifs >> ofilename;
  *ifs >> maxent;
  ifs->close();
  
  // =========== Read input ROOT file ============
  Int_t eventid;
  Double_t xBeam, yBeam, zBeam;
  Double_t pxBeam, pyBeam, pzBeam, pBeam;
  Double_t pxVD, pyVD, pzVD, pVD;
  Double_t thetaVD;
  Double_t charge;
  Int_t eleflag, kaoflag;
  Double_t km_px,  km_py,  km_pz,  km_p;
  Double_t km_px0, km_py0, km_pz0, km_p0;
  Double_t dE_fiberx_kminus[1000], dE_fibery_kminus[1000];
  Double_t dE_fiberx_kplus[1000], dE_fibery_kplus[1000];
  //Double_t kp_px,  kp_py,  kp_pz,  kp_p;
  //Double_t kp_px0, kp_py0, kp_pz0, kp_p0;
  Double_t ent = 0.0;
  ent = t1->GetEntries();
  if(ent>maxent) ent = maxent;
  cout << " --> Output seed file: " 
       << ofilename << endl;
  cout << "     (Number of event: "
       << ent << ")" << endl;
  cout << "     (ROOT file for check: check.root)"
       << endl;
  
  t1->SetBranchAddress("eventid"  ,  &eventid);
  t1->SetBranchAddress("xBeam"    ,  &xBeam);
  t1->SetBranchAddress("yBeam"    ,  &yBeam);
  t1->SetBranchAddress("zBeam"    ,  &zBeam); 
  t1->SetBranchAddress("pxBeam"   ,  &pxBeam);
  t1->SetBranchAddress("pyBeam"   ,  &pyBeam);
  t1->SetBranchAddress("pzBeam"   ,  &pzBeam); 
  t1->SetBranchAddress("pBeam"    ,  &pBeam); 
  t1->SetBranchAddress("pxVD"     ,  &pxVD);
  t1->SetBranchAddress("pyVD"     ,  &pyVD);
  t1->SetBranchAddress("pzVD"     ,  &pzVD); 
  t1->SetBranchAddress("pVD"      ,  &pVD); 
  t1->SetBranchAddress("thetaVD"  ,  &thetaVD); 
  t1->SetBranchAddress("charge"   ,  &charge); 
  t1->SetBranchAddress("eleflag"  ,  &eleflag); 
  t1->SetBranchAddress("kaoflag"  ,  &kaoflag); 
  t1->SetBranchAddress("km_px"    ,  &km_px); 
  t1->SetBranchAddress("km_py"    ,  &km_py); 
  t1->SetBranchAddress("km_pz"    ,  &km_pz); 
  t1->SetBranchAddress("km_p"     ,  &km_p); 
  t1->SetBranchAddress("km_px0"   ,  &km_px0); 
  t1->SetBranchAddress("km_py0"   ,  &km_py0); 
  t1->SetBranchAddress("km_pz0"   ,  &km_pz0); 
  t1->SetBranchAddress("km_p0"    ,  &km_p0); 
  t1->SetBranchAddress("dE_fiberx_kminus"    ,  &dE_fiberx_kminus); 
  t1->SetBranchAddress("dE_fibery_kminus"    ,  &dE_fibery_kminus);
  t1->SetBranchAddress("dE_fiberx_kplus"     ,  &dE_fiberx_kplus);
  t1->SetBranchAddress("dE_fibery_kplus"     ,  &dE_fibery_kplus); 
  

  // =============== Create new ROOT file ==================
  Double_t xi_px, xi_py, xi_pz, xi_p;
  TFile* fnew = new TFile("check.root","recreate");
  TTree* tnew = new TTree("tree","ROOT file for check.");
  tnew->Branch("xBeam"    ,  &xBeam,  "xBeam/D");
  tnew->Branch("yBeam"    ,  &yBeam,  "yBeam/D");
  tnew->Branch("zBeam"    ,  &zBeam,  "zBeam/D"); 
  //tnew->Branch("pxBeam"   ,  &pxBeam, "pxBeam/D");
  //tnew->Branch("pyBeam"   ,  &pyBeam, "pyBeam/D");
  //tnew->Branch("pzBeam"   ,  &pzBeam, "pzBeam/D"); 
  //tnew->Branch("pBeam"    ,  &pBeam,  "pBeam/D"); 
  //tnew->Branch("pxVD"     ,  &pxVD,   "pxVD/D");
  //tnew->Branch("pyVD"     ,  &pyVD,   "pyVD/D");
  //tnew->Branch("pzVD"     ,  &pzVD,   "pzVD/D"); 
  //tnew->Branch("pVD"      ,  &pVD,    "pVD/D"); 
  tnew->Branch("charge"   ,  &charge, "charge/D"); 
  tnew->Branch("eleflag"  ,  &eleflag, "eleflag/I"); 
  tnew->Branch("kaoflag"  ,  &kaoflag, "kaoflag/I"); 
  tnew->Branch("km_px"    ,  &km_px,   "km_px/D"); 
  tnew->Branch("km_py"    ,  &km_py,   "km_py/D"); 
  tnew->Branch("km_pz"    ,  &km_pz,   "km_pz/D"); 
  tnew->Branch("km_p"     ,  &km_p,    "km_p/D"); 
  tnew->Branch("km_px0"   ,  &km_px0,  "km_px0/D"); 
  tnew->Branch("km_py0"   ,  &km_py0,  "km_py0/D"); 
  tnew->Branch("km_pz0"   ,  &km_pz0,  "km_px0/D"); 
  tnew->Branch("km_p0"    ,  &km_p0,   "km_p0/D"); 
  tnew->Branch("kp_px"    ,  &pxVD,    "kp_px/D"); 
  tnew->Branch("kp_py"    ,  &pyVD,    "kp_py/D"); 
  tnew->Branch("kp_pz"    ,  &pzVD,    "kp_pz/D"); 
  tnew->Branch("kp_p"     ,  &pVD,     "kp_p/D"); 
  tnew->Branch("kp_px0"   ,  &pxBeam,  "kp_px0/D"); 
  tnew->Branch("kp_py0"   ,  &pyBeam,  "kp_py0/D"); 
  tnew->Branch("kp_pz0"   ,  &pzBeam,  "kp_pz0/D"); 
  tnew->Branch("kp_p0"    ,  &pBeam,   "kp_p0/D"); 
  tnew->Branch("xi_px"    ,  &xi_px,   "xi_px/D"); 
  tnew->Branch("xi_py"    ,  &xi_py,   "xi_py/D"); 
  tnew->Branch("xi_pz"    ,  &xi_pz,   "xi_pz/D"); 
  tnew->Branch("xi_p"     ,  &xi_p,    "xi_p/D"); 
  tnew->Branch("dE_fiberx_kminus",&dE_fiberx_kminus,"dE_fiberx_kminus[1000]/D"); 
  tnew->Branch("dE_fibery_kminus",&dE_fibery_kminus,"dE_fiberx_kminus[1000]/D");
  tnew->Branch("dE_fiberx_kplus",&dE_fiberx_kplus,"dE_fiberx_kplus[1000]/D");
  tnew->Branch("dE_fibery_kplus",&dE_fibery_kplus,"dE_fiberx_kplus[1000]/D"); 
  
  Int_t nhitfibers_x1=0;
  Int_t nhitfibers_y1=0;
  Int_t hitx1[1000];
  Int_t hity1[1000];
  Double_t dex1[1000];
  Double_t dey1[1000];
  Double_t dE_th = 0.001;
  
  Int_t nhitfibers_x2=0;
  Int_t nhitfibers_y2=0;
  Int_t hitx2[1000];
  Int_t hity2[1000];
  Double_t dex2[1000];
  Double_t dey2[1000];

  // ================ Create seed file =====================
  ofstream* ofs = new ofstream(ofilename);
  ofs->precision(8);
  //---- main loop ----
  for(int i=0 ; i<ent ; i++){
    //for(int i=0 ; i<100 ; i++){

    for(int jj=0 ; jj<1000 ; jj++){
      hitx1[jj] = 999; // K-
      hity1[jj] = 999;
      dex1[jj]  = 0.0;
      dey1[jj]  = 0.0;
      
      hitx2[jj] = 999; // K+
      hity2[jj] = 999;
      dex2[jj]  = 0.0;
      dey2[jj]  = 0.0;
    }
    
    t1->GetEntry(i);
    TVector3 p1(km_px, km_py, km_pz);
    TVector3 p2(pxBeam, pyBeam, pzBeam);
    xi_px = p1.X()-p2.X();
    xi_py = p1.Y()-p2.Y();
    xi_pz = p1.Z()-p2.Z();
    TVector3 p3(xi_px, xi_py, xi_pz);
    xi_p = sqrt(p3*p3); // momentum of xi- 
    //cout<< xi_px  << " " << pxBeam << endl;
    //cout << xi_p << endl;
    //    cout << eventid << " " 
    //   << xBeam << " " << yBeam << " " << zBeam << " " 
    //	 << km_px0 << " " << km_py0 << " " << km_pz0 << " " 
    //	 << km_px << " " << km_py << " " << km_pz << " " 
    //	 << pxBeam << " " << pyBeam << " " << pzBeam << " " 
    //	 << pxVD << " " << pyVD << " " << pzVD << " " 
    //	 << endl;
    
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ~~~~~~~ dE information in fibers ~~~~~~~~~~~~~~~~
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    nhitfibers_x1 = 0;
    nhitfibers_y1 = 0;
    nhitfibers_x2 = 0;
    nhitfibers_y2 = 0;
    for(int jj=0 ; jj<1000 ; jj++){
      // oooOOOooo X Fiber for K-  oooOOOooo
      if(dE_fiberx_kminus[jj] > dE_th){
	hitx1[nhitfibers_x1] = jj;
	dex1[nhitfibers_x1]  = dE_fiberx_kminus[jj];
	//cout << hitx1[nhitfibers_x1] << " " 
	//     << dex1[nhitfibers_x1] << endl;
	nhitfibers_x1++;
      }
      else{
	hitx1[nhitfibers_x1] = 0;
	dex1[nhitfibers_x1]  = -2222.0;
      }
      // oooOOOooo Y Fiber for K-  oooOOOooo
      if(dE_fibery_kminus[jj] > dE_th){
	hity1[nhitfibers_y1] = jj;
	dey1[nhitfibers_y1]  = dE_fibery_kminus[jj];
	//cout << hity1[nhitfibers_y1] << " " 
	//     << dey1[nhitfibers_y1] << endl;
	nhitfibers_y1++;
      }
      else{
	hity1[nhitfibers_y1] = 0;
	dey1[nhitfibers_y1]  = -2222.0;
      }
      // oooOOOooo X Fiber for K+  oooOOOooo
      if(dE_fiberx_kplus[jj] > dE_th){
	hitx2[nhitfibers_x2] = jj;
	dex2[nhitfibers_x2]  = dE_fiberx_kplus[jj];
	//cout << hitx2[nhitfibers_x2] << " " 
	//     << dex2[nhitfibers_x2] << endl;
	nhitfibers_x2++;
      }
      else{
	hitx2[nhitfibers_x2] = 0;
	dex2[nhitfibers_x2]  = -2222.0;
      }
      // oooOOOooo Y Fiber for K+  oooOOOooo
      if(dE_fibery_kplus[jj] > dE_th){
	hity2[nhitfibers_y2] = jj;
	dey2[nhitfibers_y2]  = dE_fibery_kplus[jj];
	//cout << hity2[nhitfibers_y2] << " " 
	//     << dey2[nhitfibers_y2] << endl;
	nhitfibers_y2++;
      }
      else{
	hity2[nhitfibers_y2] = 0;
	dey2[nhitfibers_y2]  = -2222.0;
      }
      
    }
    
    *ofs << eventid << " " 
	 << xBeam << " " << yBeam << " " << zBeam << " " 
	 << km_px0*1.0e-3 << " " << km_py0*1.0e-3 << " " << km_pz0*1.0e-3 << " " 
	 << km_px*1.0e-3 << " " << km_py*1.0e-3 << " " << km_pz*1.0e-3 << " " 
	 << pxBeam*1.0e-3 << " " << pyBeam*1.0e-3 << " " << pzBeam*1.0e-3 << " " 
	 << pxVD*1.0e-3 << " " << pyVD*1.0e-3 << " " << pzVD*1.0e-3 << " "; 
    *ofs << nhitfibers_x1 << " ";
    for(int kk=0 ; kk<nhitfibers_x1 ; kk++){
      *ofs << hitx1[kk] << " " << dex1[kk] << " ";
    }
    *ofs << nhitfibers_y1 << " ";
    for(int kk=0 ; kk<nhitfibers_y1 ; kk++){
      *ofs << hity1[kk] << " " << dey1[kk] << " ";
    }
    *ofs << nhitfibers_x2 << " ";
    for(int kk=0 ; kk<nhitfibers_x2 ; kk++){
      *ofs << hitx2[kk] << " " << dex2[kk] << " ";
    }
    *ofs << nhitfibers_y2 << " ";
    for(int kk=0 ; kk<nhitfibers_y2 ; kk++){
      *ofs << hity2[kk] << " " << dey2[kk] << " ";
    }
    *ofs << endl;
    
    if( thetaVD*180.0/3.14159<30.0 ){ // Angle cut 
      //cout << thetaVD*180.0/3.14159 << endl;
      tnew->Fill();
    }
    
  }
  ofs->close(); // Close the seed file.
  tnew->Write();// tnew is written in the ROOT file (fnew).
  fnew->Close();// Close fnew.
  
  
}
