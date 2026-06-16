/*
  FiberTarget ParamManager.hh
  Oct 2015, T.Gogami
*/

#ifndef FiberTargetParamManager_h
#define FiberTargetParamManager_h 1

//#include<TFile.h>
#include "G4String.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include <fstream>
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "G4Geantino.hh"
#include <TMath.h>
using namespace std;

class FiberTargetParamManager
{
public:
  FiberTargetParamManager(G4String fname);
  ~FiberTargetParamManager();
  
public:  
  G4bool readparam();
private:
  G4String Filename;    //Input file name
  G4String ROOTFilename;//Output ROOT file name
  G4int RFnum;          //Number of Output ROOT file name
  G4double BeamEnergy;  //Incident beam momentum (input)
  G4double BeamP_w;     //Incident beam momentum width (input)
  G4double BeamTheta;   //Incident beam theta (input)
  G4double BeamTheta_w; //Incident beam theta width (input)
  G4double BeamPhi;     //Incident beam phi (input)
  G4double BeamPhi_w;   //Incident beam phi width (input)
  G4double GenP;        // Generated particle momentum
  G4double GenPx;// Generated particle momentum
  G4double GenPy;// Generated particle momentum
  G4double GenPz;// Generated particle momentum
  G4double GenTheta;    // Generated particle theta
  G4double GenPhi;      // Generated particle phi
  G4int PosiAnalFlag;   //POSITRON analysis flag
  G4String Target;      //Target
  G4String SeedFile;    //Seed file
  G4double TThickness;  //Target thickness [cm]
  G4double minMom;      //Mimimum momentum of analyzed particle
  G4double maxMom;      //Maximum momentum of analyzed particle
  G4double mintheta;    //Mimimum theta of analyzed particle
  G4double maxtheta;    //Maximum theta of analyzed particle
  G4int EMFlag;         //ElectroMagnetic Flag
  G4int DecayFlag;      //Decay Flag
  G4int HadronFlag;     //Hadron Flag
  G4int pGenerationFlag;//Particle Generation Flag
  G4double rasterx,rastery,rasterz;   //raster[cm]
  G4double beamoffx,beamoffy,beamoffz;//Beam Position Offset [cm]
  G4double beamx , beamy , beamz; 
  G4int ParticleFlag;   // Particle Flag
  G4int SeedFlag;       // Seed Flag
  G4int evid;        // Event ID 
  G4int evnum;       // Number of event
  G4double pkm[3];
  G4double pkm0[3];
  G4double pkp[3];
  G4double pkp0[3];
  //~~~~ Fiber hit for K- ~~~~~~~
  Int_t nhitx;
  Int_t hitnumx[1000];
  Double_t dE_xf[1000];
  Int_t nhity;
  Int_t hitnumy[1000];
  Double_t dE_yf[1000];
  //~~~~ Fiber hit for K+ ~~~~~~~
  Int_t nhitx2;
  Int_t hitnumx2[1000];
  Double_t dE_xf2[1000];
  Int_t nhity2;
  Int_t hitnumy2[1000];
  Double_t dE_yf2[1000];
  
  

public:
  G4String GetFileName()     { return Filename;       };
  G4String GetROOTFileName() { return ROOTFilename;   };
  G4int    GetRFnum()        { return RFnum;     };
  //G4int    SetRFnum(G4int rfn){ RFnum=rfn;       };
  void     SetRFnum(G4int rfn){ RFnum=rfn;       };
  void     SetBeamPos(G4ThreeVector bpos){
    beamx = bpos.x();
    beamy = bpos.y();
    beamz = bpos.z();
  }
  void SetGenP(G4double bp)        { GenP = bp; }
  void SetPKm(G4double a, G4double b, G4double c){
    pkm[0] = a;
    pkm[1] = b;
    pkm[2] = c;
  }
  void SetPKp(G4double a, G4double b, G4double c){
    pkp[0] = a;
    pkp[1] = b;
    pkp[2] = c;
  }
  void SetPKm0(G4double a, G4double b, G4double c){
    pkm0[0] = a;
    pkm0[1] = b;
    pkm0[2] = c;
  }
  void SetPKp0(G4double a, G4double b, G4double c){
    pkp0[0] = a;
    pkp0[1] = b;
    pkp0[2] = c;
  }
  void SetGenPvec(G4double a , G4double b, G4double c){ 
    GenPx = a;
    GenPy = b;
    GenPz = c;
  }
  void SetGenTheta(G4double btheta){ GenTheta = btheta; }
  void SetGenPhi(G4double bphi)    { GenPhi = bphi; }
  G4double GetGenP()         { return GenP; }
  G4ThreeVector GetGenPvec() { return G4ThreeVector(GenPx,GenPy,GenPz); }
  G4ThreeVector GetPKm(){ return G4ThreeVector(pkm[0],pkm[1],pkm[2]); }
  G4ThreeVector GetPKm0(){ return G4ThreeVector(pkm0[0],pkm0[1],pkm0[2]); }
  G4ThreeVector GetPKp(){ return G4ThreeVector(pkp[0],pkp[1],pkp[2]); }
  G4ThreeVector GetPKp0(){ return G4ThreeVector(pkp0[0],pkp0[1],pkp0[2]); }
  G4double GetGenTheta()     { return GenTheta; }
  G4double GetGenPhi()       { return GenPhi; }
  G4int GetEVID()            { return evid; }
  //G4int SetEVID(G4int eventid){ evid = eventid; }
  void SetEVID(G4int eventid){ evid = eventid; }
  
  G4ThreeVector GetBeamPos() { return G4ThreeVector(beamx,beamy,beamz);}
  G4double GetBeamEnergy()   { return BeamEnergy;     };//momentum
  G4double GetBeamP_w()      { return BeamP_w;        };//momentum
  G4double GetBeamTheta()    { return BeamTheta;      };//Theta 
  G4double GetBeamTheta_w()  { return BeamTheta_w;    };//Theta
  G4double GetBeamPhi()      { return BeamPhi;        };//Phi
  G4double GetBeamPhi_w()    { return BeamPhi_w;      };//Phi
  G4int    GetPosiAnalFlag() { return PosiAnalFlag;   };
  G4String GetTarget()       { return Target;         };
  G4double GetTThickness()   { return TThickness;     };
  G4double GetMinMom()       { return minMom;         };
  G4double GetMaxMom()       { return maxMom;         };
  G4double GetMinTheta()     { return mintheta;       };
  G4double GetMaxTheta()     { return maxtheta;       };
  G4int    GetEMFlag()       { return EMFlag;         };
  G4int    GetDecayFlag()    { return DecayFlag;      };
  G4int    GetHadronFlag()   { return HadronFlag;     };
  G4int    GetPGenFlag()     { return pGenerationFlag;};
  G4double GetRasterX()      { return rasterx;        };
  G4double GetRasterY()      { return rastery;        };
  G4double GetRasterZ()      { return rasterz;        };
  G4String GetSeedFile()     { return SeedFile;       };
  G4int    GetSeedFlag()     { return SeedFlag;       };
  void ResetFiberInfo(){
    for(int i=0 ; i<1000 ; i++){
      hitnumx[i] = 999;
      hitnumy[i] = 999;
      dE_xf[i]   = 0.0;
      dE_yf[i]   = 0.0;
    }
  }
  void SetAlreadyFiberHitx(Int_t nhit, Int_t num[], Double_t de[] ){
    nhitx = nhit;
    for(int i=0 ; i<nhit ; i++){
      hitnumx[i] = num[i];
      dE_xf[i]   = de[i];
      //G4cout << "x: " << hitnumx[i] << " "  << dE_xf[i] << G4endl;
    }
  }
  
  void SetAlreadyFiberHity(Int_t nhit, Int_t num[], Double_t de[] ){
    nhity = nhit;
    for(int i=0 ; i<nhit ; i++){
      hitnumy[i] = num[i];
      dE_yf[i]   = de[i];
      //G4cout << "y: " << hitnumy[i] << " "  << dE_yf[i] << G4endl;
    }
  }
  void SetAlreadyFiberHitx2(Int_t nhit, Int_t num[], Double_t de[] ){
    nhitx2 = nhit;
    for(int i=0 ; i<nhit ; i++){
      hitnumx2[i] = num[i];
      dE_xf2[i]   = de[i];
      //G4cout << "x: " << hitnumx[i] << " "  << dE_xf[i] << G4endl;
    }
  }
  
  void SetAlreadyFiberHity2(Int_t nhit, Int_t num[], Double_t de[] ){
    nhity2 = nhit;
    for(int i=0 ; i<nhit ; i++){
      hitnumy2[i] = num[i];
      dE_yf2[i]   = de[i];
      //G4cout << "y: " << hitnumy[i] << " "  << dE_yf[i] << G4endl;
    }
  }
  
  // ~~~~~ for K- ~~~~~~~~~~~~
  Int_t GetAlreadyFiberHitx_nhit(){  return nhitx;   };
  Double_t GetAlreadyFiberHitx_hitnum(int i){  return hitnumx[i];   };
  Double_t GetAlreadyFiberHitx_dE(int i){  return dE_xf[i];   };
  Int_t GetAlreadyFiberHity_nhit(){  return nhity;   };
  Double_t GetAlreadyFiberHity_hitnum(int i){  return hitnumy[i];   };
  Double_t GetAlreadyFiberHity_dE(int i){  return dE_yf[i];   };
  // ~~~~~ for K+ ~~~~~~~~~~~~
  Int_t GetAlreadyFiberHitx2_nhit(){  return nhitx2;   };
  Double_t GetAlreadyFiberHitx2_hitnum(int i){  return hitnumx2[i];   };
  Double_t GetAlreadyFiberHitx2_dE(int i){  return dE_xf2[i];   };
  Int_t GetAlreadyFiberHity2_nhit(){  return nhity2;   };
  Double_t GetAlreadyFiberHity2_hitnum(int i){  return hitnumy2[i];   };
  Double_t GetAlreadyFiberHity2_dE(int i){  return dE_yf2[i];   };
  
  
  G4ThreeVector GetBeamOffset()
  { return G4ThreeVector(beamoffx,beamoffy,beamoffz);};
  G4int GetParticleFlag()    { return ParticleFlag;   };
  
};

#endif
