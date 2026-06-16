/*
  create_seed.cc
  
  Toshi Gogami , 9July2014
*/

#include "constant.hh"
#include "create_seed.hh"
#include "ParamMan.hh"

#include <TMath.h>
#include <TFile.h>
#include <TTree.h>
#include <TRandom3.h>
#include <fstream>
#include <iostream>
#include <TVector3.h>
using namespace std;

//void create_seed(char* inputfile){
int main(int argc, char** argv){
  // ======= Open input ROOT file ===============
  char inputfile[300];
  if(argc==2) sprintf(inputfile,"%s",argv[1]);
  else sprintf(inputfile,"ch2_5cm.root");
  cout << " Opened an input ROOT file: " 
       << inputfile << endl;
  cout << endl;
  TFile* f1 = new TFile(inputfile);
  TTree* t1 = (TTree*)f1->Get("tree");
  
  // ======= 
  //Double_t evID, trackID;
  Int_t evID, trackID;
  Double_t beamx, beamy, beamz;
  Double_t beamz_recon;
  Double_t beampx, beampy, beampz;;
  Double_t beamp;
  Double_t beam_theta;
  Double_t beam_phi;
  Double_t x, y, z, p;
  Double_t px, py, pz;
  Double_t theta, phi;
  Double_t charge;
  Double_t dE_fiberx_kminus[1000];
  Double_t dE_fibery_kminus[1000];
  Int_t posflag, eleflag, phoflag, proflag;
  Int_t kaoflag, neuflag, pioflag, muoflag;
  Double_t ent;
  Int_t count = 0;
  ent = t1->GetEntries();
  t1->SetBranchAddress("eventid" ,  &evID);
  t1->SetBranchAddress("trackID",&trackID);
  t1->SetBranchAddress("xBeam",  &beamx  );
  t1->SetBranchAddress("yBeam",  &beamy  );
  t1->SetBranchAddress("zBeam",  &beamz  );
  t1->SetBranchAddress("zBeamR", &beamz_recon);
  t1->SetBranchAddress("pxBeam", &beampx);
  t1->SetBranchAddress("pyBeam", &beampy);
  t1->SetBranchAddress("pzBeam", &beampz);
  t1->SetBranchAddress("pBeam",  &beamp );
  t1->SetBranchAddress("thetaBeam", &beam_theta);
  t1->SetBranchAddress("phiBeam", &beam_phi);
  t1->SetBranchAddress("xVD"  ,  &x);
  t1->SetBranchAddress("yVD"  ,  &y);
  t1->SetBranchAddress("zVD"  ,  &z);
  t1->SetBranchAddress("pxVD" ,  &px);
  t1->SetBranchAddress("pyVD" ,  &py);
  t1->SetBranchAddress("pzVD" ,  &pz);
  t1->SetBranchAddress("pVD"  ,  &p);
  t1->SetBranchAddress("thetaVD",  &theta );
  t1->SetBranchAddress("phiVD"  ,  &phi   );
  t1->SetBranchAddress("charge" , &charge );
  t1->SetBranchAddress("posflag", &posflag);
  t1->SetBranchAddress("eleflag", &eleflag);
  t1->SetBranchAddress("phoflag", &phoflag);
  t1->SetBranchAddress("proflag", &proflag);
  t1->SetBranchAddress("kaoflag", &kaoflag);
  t1->SetBranchAddress("neuflag", &neuflag);
  t1->SetBranchAddress("pioflag", &pioflag);
  t1->SetBranchAddress("muoflag", &muoflag);
  t1->SetBranchAddress("dE_fiberx_kminus", &dE_fiberx_kminus);
  t1->SetBranchAddress("dE_fibery_kminus", &dE_fibery_kminus);
  double dE_th = 0.1; // [MeV]
  
  Double_t kpmomc,kpmomb, kpmom;
  Double_t kptheta, kpphi;
  Double_t kmtheta, kmphi;
  Double_t theta_min, theta_max;
  Double_t mm;
  TFile* f2 = new TFile("check.root","recreate");
  TTree* t2 = new TTree("tree","check");
  t2->Branch("eventid" ,  &evID );
  t2->Branch("trackID",&trackID);
  t2->Branch("xBeam",  &beamx);
  t2->Branch("yBeam",  &beamy);
  t2->Branch("zBeam",  &beamz);
  t2->Branch("zBeamR", &beamz_recon);
  t2->Branch("pxBeam", &beampx);
  t2->Branch("pyBeam", &beampy);
  t2->Branch("pzBeam", &beampz);
  t2->Branch("pkm_incident",  &beamp );
  //t2->Branch("thetaBeam", &beam_theta);
  //t2->Branch("phiBeam", &beam_phi);
  //t2->Branch("xVD"  ,  &x );
  //t2->Branch("yVD"  ,  &y );
  //t2->Branch("zVD"  ,  &z );
  //t2->Branch("pkm_x",  &px);
  //t2->Branch("pkm_y",  &py);
  //t2->Branch("pkm_z",  &pz);
  t2->Branch("p_km"  ,  &p );
  t2->Branch("theta_km",&kmtheta );
  t2->Branch("phi_km",  &kmphi );
  t2->Branch("p_kp"  ,  &kpmom );
  t2->Branch("theta_kp",&kptheta );
  t2->Branch("phi_kp",  &kpphi );
  t2->Branch("dE_fiberx_kminus",  &dE_fiberx_kminus, "dE_fiberx_kminus[1000]/D" );
  t2->Branch("dE_fibery_kminus",  &dE_fibery_kminus, "dE_fibery_kminus[1000]/D" );
  //t2->Branch("thetaVD", &theta );
  //t2->Branch("phiVD"  , &phi   );
  t2->Branch("mm",      &mm ); 
  //t2->Branch("charge" , &charge );
  //t2->Branch("posflag", &posflag);
  //t2->Branch("eleflag", &eleflag);
  //t2->Branch("phoflag", &phoflag);
  //t2->Branch("proflag", &proflag);
  //t2->Branch("kaoflag", &kaoflag);
  //t2->Branch("neuflag", &neuflag);
  //t2->Branch("pioflag", &pioflag);
  //t2->Branch("muoflag", &muoflag);
  //ent = 3;
  //ent = 30000;
  ent = 100000;
  
  // ============ Read Input file =================

  ParamMan* param = new ParamMan();
  param->ReadInputFile("input.dat");
  //cout << "test" << endl;  
  
  // ============ Read Input file =================
  TRandom3* rand = new TRandom3();
  bool ok, bad;
  Double_t a[3];
  Double_t b[3];
  //Double_t mcore, mtar, mhyp;
  //Double_t mcore, mtar;
  Double_t mcore;
  //TVector3 pkm(0.0 , 0.0 , 0.0);
  //TVector3 pkp(0.0 , 0.0 , 0.0);
  //Double_t pkm[3];
  //Double_t pkp[3];
  //double* pkm;
  //double* pkp;
  
  Int_t nhitfibers_x=0;
  Int_t nhitfibers_y=0;
  Int_t hitx[1000];
  Int_t hity[1000];
  Double_t dex[1000];
  Double_t dey[1000];
  
  
  
  kpmomc = param->GetKpMomCenter();
  kpmomb = param->GetKpMomBite();
  theta_min = param->GetThetaMin();
  theta_max = param->GetThetaMax();
  mcore = param->GetMcore();
  //mtar  = param->GetMtar();
  //mhyp  = param->GetMhyp();
  Int_t maxtrial = 500;
  Int_t trial;
  //int percent;
  for(Int_t i=0 ; i<ent ; i++){
    
    t1->GetEntry(i);
    
    // ~~~~~ Production points ~~~~~~~~~~~~~~~~~~~~~~
    // ~~~~ sign of z-position should be reversed ~~~
    param->SetProductionPoint(beamx, beamy, -beamz);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //cout << beamx << " " << beamy << " " << -beamz << endl;
    
    ok=false;
    bad=false;
    trial = 0;
    if( kaoflag==1 && theta*180./3.14159<1.0 ){
      do{
	//cout << " test " << endl;
	trial++;
	kpmom = rand->Uniform(kpmomc-kpmomb,kpmomc+kpmomb);
	//kptheta = theta_calc12xiBe(beamp*1.0e-3,kpmom);// ([GeV],[GeV])
	//kptheta = theta_calc12xiBe(p*1.0e-3,kpmom);// ([GeV],[GeV])
	kptheta = CalcTheta(p*1.0e-3, kpmom, param);
	//kptheta = rand->Uniform(0.0 , 0.174);// 0-10 deg
	//kpmom = clacmom(kptheta,beamp*1.0e-3);
	//kptheta = calctheta(beamp*1.0e-3, kpmom);
	if(theta_min<kptheta && kptheta<theta_max) ok =true;
	if(trial==maxtrial)bad=true; 
	//cout << kptheta << endl;
	//cout << trial << endl;
      }while (ok==false && bad==false);
	//}while (ok==false);
      
      //cout << kptheta*180./3.14159 << endl;
      //cout << kptheta << endl;
      a[0] = p*1.0e-3;  //mom [GeV] K-
      a[1] = 0.0;       //theta [rad] K-
      a[2] = 0.0;       //phi [rad] K-
      b[0] = kpmom;     //mom [GeV] K+
      b[1] = kptheta;   //theta [rad] K+
      b[2] = rand->Uniform(0.0,2.0*3.14159);  //phi [rad]

      kmtheta = a[1];
      kmphi = a[2];
      kpphi = b[2];
      p = p*1.0e-3; // [MeV] --> [GeV]
      beamp  = beamp*1.0e-3; // [MeV] --> [GeV]
      beampx = beampx*1.0e-3;// [MeV] --> [GeV]
      beampy = beampy*1.0e-3;// [MeV] --> [GeV]
      beampz = beampz*1.0e-3;// [MeV] --> [GeV]
      
      mm = CalcMM(a,b,param);
      param->SetMomKm0(0.0, 0.0, beamp);
      mm = mm-mcore-mxi;
      //cout << mm << endl;

      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // ~~~~~~~ dE information in x fibers ~~~~~~~~~~~~~~~~
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      nhitfibers_x = 0;
      for(int jj=0 ; jj<1000 ; jj++){
	if(dE_fiberx_kminus[jj] > dE_th){
	  hitx[nhitfibers_x] = jj;
	  dex[nhitfibers_x]  = dE_fiberx_kminus[jj];
	  nhitfibers_x++;
	  //cout << hitx[nhitfibers_x] << " " 
	    //     << dex[nhitfibers_x] << endl;
	}
	else{
	  hitx[nhitfibers_x] = 0;
	  dex[nhitfibers_x]  = -2222.0;
	}
      }
      param->SetHitFiberx(nhitfibers_x,hitx,dex);
      
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // ~~~~~~~ dE information in y fibers ~~~~~~~~~~~~~~~~
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      nhitfibers_y = 0;
      for(int jj=0 ; jj<1000 ; jj++){
	if(dE_fibery_kminus[jj] > dE_th){
	  hity[nhitfibers_y] = jj;
	  dey[nhitfibers_y]  = dE_fibery_kminus[jj];
	  nhitfibers_y++;
	  //cout << hitx[nhitfibers_x] << " " 
	    //     << dex[nhitfibers_x] << endl;
	}
	else{
	  hity[nhitfibers_y] = 0;
	  dey[nhitfibers_y]  = -2222.0;
	}
      }
      param->SetHitFibery(nhitfibers_y,hity,dey);
      
      if(ok==true && bad==false){
	t2->Fill();
	param->SetEVID(count);
	// ~~~~~~~~~~ Seed file ~~~~~~~~~~~~~~~~~
	//if(count==0) param->SeedOutStart();
	//else param->SeedOut();
	if(count==0) param->SeedOutStart_Fiber();
	else param->SeedOut_Fiber();
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	count++;
      }
      
      //cout << m12xiBe << endl;
      //cout << kpmom << endl;
      //cout << p << endl;
      
      //percent = (double)i/(double)ent * 100.0;
      //cout << percent <<"%"<< endl;
      //if(percent % 5) cout << "~~~~~~~~~~" << i/ent*100. <<"%"<< endl;
      
      //cout << i << " " << trial<<endl;
      
    }
    
  }
  param->SeedOutEnd();
  t2->Write();
  f2->Close();
  cout << endl;
  cout << " test.root for check --> " << endl;
  cout << "  The number of filled event: " << count << endl;
  //cout << " OK"  << endl;
  return 0;
}


Double_t CalcTheta(double p1,double p2,ParamMan* param){
  //Double_t mcore, mtar, mhyp;
  Double_t mtar, mhyp;
  //mcore = param->GetMcore();
  mtar  = param->GetMtar();
  mhyp  = param->GetMhyp();

  //Double_t A = pow(m12xiBe,2.0) - (2.0*mk*mk+m12C*m12C);
  Double_t A = pow(mhyp,2.0) - (2.0*mk*mk+mtar*mtar);
  //cout << pow(m12xiBe,2.0) << " " << pow(m12C,2.0) << endl;
  //cout << A << endl;
  Double_t E1, E2;
  E1 = sqrt(p1*p1+mk*mk);
  E2 = sqrt(p2*p2+mk*mk);
  //cout << E1 << " " << E2 << endl;
  Double_t B = 2.0*E1*E2 + 2.0*E2*mtar - 2.0*E1*mtar;
  Double_t thetaKK;
  thetaKK = acos( (A+B)/(2.0*p1*p2));
  //cout << thetaKK <<endl;
  return thetaKK;
}

Double_t CalcMM(double *a, double* b, ParamMan* param){
  //Double_t mcore, mtar, mhyp;
  Double_t mtar;
  //mcore = param->GetMcore();
  mtar  = param->GetMtar();
  //mhyp  = param->GetMhyp();

  Double_t mom1   = a[0];
  Double_t theta1 = a[1];
  Double_t phi1   = a[2];
  Double_t mom2   = b[0];
  Double_t theta2 = b[1];
  Double_t phi2   = b[2];
  //Double_t xp1,yp1;
  //Double_t xp2,yp2;
  Double_t kpx1, kpy1, kpz1;
  Double_t kpx2, kpy2, kpz2;
  kpz1 = mom1 * cos(theta1);
  kpx1 = mom1 * sin(theta1) * cos(phi1);
  kpy1 = mom1 * sin(theta1) * sin(phi1);
  kpz2 = mom2 * cos(theta2);
  kpx2 = mom2 * sin(theta2) * cos(phi2);
  kpy2 = mom2 * sin(theta2) * sin(phi2);
  param->SetMomKm(kpx1, kpy1, kpz1);
  param->SetMomKp(kpx2, kpy2, kpz2);
  
  TVector3 kp1(kpx1, kpy1, kpz1);
  TVector3 kp2(kpx2, kpy2, kpz2);
  //cout << sqrt(kp1*kp1)-mom1 << endl;
  //cout << sqrt(kp2*kp2)-mom2 << endl;
  Double_t E1, E2, Et;
  E1 = sqrt(mom1*mom1+mk*mk);
  E2 = sqrt(mom2*mom2+mk*mk);
  Et = mtar;
  //cout << E1 << endl;
  //cout << E2 << endl;
  
  Double_t dE = (E1+Et-E2)*(E1+Et-E2);
  Double_t dp = (kp1-kp2)*(kp1-kp2);
  Double_t mm = 0.0;
  mm = sqrt(dE - dp);
  //cout << mm << endl;
  return mm;
}

//Double_t theta_calc12xiBe(double p1,double p2){
//  Double_t A = pow(m12xiBe,2.0) - (2.0*mk*mk+m12C*m12C);
//  //cout << pow(m12xiBe,2.0) << " " << pow(m12C,2.0) << endl;
//  //cout << A << endl;
//  Double_t E1, E2;
//  E1 = sqrt(p1*p1+mk*mk);
//  E2 = sqrt(p2*p2+mk*mk);
//  //cout << E1 << " " << E2 << endl;
//  Double_t B = 2.0*E1*E2 + 2.0*E2*m12C - 2.0*E1*m12C;
//  Double_t thetaKK;
//  thetaKK = acos( (A+B)/(2.0*p1*p2));
//  //cout << thetaKK <<endl;
//  return thetaKK;
//}
