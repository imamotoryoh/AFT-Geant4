/*
  ParamMan.hh
  Parameter manager
  
  Toshi Gogami , 9July2014
*/

#include<TMath.h>
#include<TVector3.h>
#include<fstream>
#include<iostream>
using namespace std;


#ifndef ParamMan_h
#define ParamMan_h 1

class ParamMan{
public:
  ParamMan();
  ~ParamMan();
  
private:
  Double_t kpmom_center;
  Double_t kpmom_bite;
  Double_t theta_min;
  Double_t theta_max;
  Double_t mcore; // Mass of core nucleus
  Double_t mtar;  // Mass of target 
  Double_t mhyp;  // Mass of hypernucleus
  Int_t PID, EID;//1:xi, 12:12xiBe
  //Double_t x, y, z;// Production point (x,y,z)
  double pp[3];// Production point (x,y,z)
  //Double_t kptheta;
  //Double_t kpphi;
  //Double_t kp_x,kp_y,kp_z;
  //TVector3 pkm;
  //TVector3 pkp;
  //double* pkm;
  //double* pkp;
  Double_t pkm[3];
  Double_t pkm0[3];
  Double_t pkp[3];
  Int_t nhitfibers_x;
  Int_t hitx[1000];
  Int_t hity[1000];
  Double_t dex[1000];
  Double_t dey[1000];
  Int_t nhitfibers_y;
  // Int_t evID;
  //Int_t eid;
  //int eventid;
  
  string seedname;
  ofstream* dragon_seed;
  
public:
  //void ReadInputFile(char*);
  void ReadInputFile(string);
  //void ReadInputFile();
  Double_t GetKpMomCenter(){return kpmom_center;};
  Double_t GetKpMomBite()  {return kpmom_bite;}  ;
  Double_t GetThetaMin()   {return theta_min;};
  Double_t GetThetaMax()   {return theta_max;};
  Double_t GetMcore()      {return mcore;};
  Double_t GetMtar()       {return mtar;};
  Double_t GetMhyp()       {return mhyp;};
  Int_t    GetPID()        {return PID;};
  void SetMomKm(double p1, double p2, double p3){
    pkm[0] = p1;
    pkm[1] = p2;
    pkm[2] = p3;
  }
  void SetMomKm0(double p1, double p2, double p3){
    pkm0[0] = p1;
    pkm0[1] = p2;
    pkm0[2] = p3;
  }
  void SetMomKp(double p1, double p2, double p3){
    pkp[0] = p1;
    pkp[1] = p2;
    pkp[2] = p3;
  }
  void SetProductionPoint(double xx, double yy, double zz){
    pp[0] = xx;
    pp[1] = yy;
    pp[2] = zz;
    //cout << "   " << xx << " "<< yy << " " << zz << endl;
  }
  void SetHitFiberx(Int_t nhit, Int_t hitnum[], Double_t de[]){
    nhitfibers_x = nhit;
    for(int i=0 ; i<nhitfibers_x ; i++){
      hitx[i] = hitnum[i];
      dex[i]  = de[i];
      //cout << hitx[i] << " "  << dex[i] << endl;
    }
  }
  void SetHitFibery(Int_t nhit, Int_t hitnum[], Double_t de[]){
    nhitfibers_y = nhit;
    for(int i=0 ; i<nhitfibers_y ; i++){
      hity[i] = hitnum[i];
      dey[i]  = de[i];
      //cout << hitx[i] << " "  << dex[i] << endl;
    }
  }
  
  void SetEVID(Int_t evid){EID = evid;};
  //TVector3 GetMomKm(){ return pkm;};
  //TVector3 GetMomKp(){ return pkp;};
  //double GetMomKm(){ return pkm;};
  //double* GetMomKp(){ return pkp;};
  double* GetProductionPoint(){ return pp;};
  void SeedOutStart();
  void SeedOut();
  void SeedOutStart_Fiber();
  void SeedOut_Fiber();
  void SeedOutEnd();

};

#endif
