/*
  FiberTargetEventAction.hh
  Oct 2015, T.Gogami
*/

#ifndef FiberTargetEventAction_h
#define FiberTargetEventAction_h 1

#include "G4UserEventAction.hh"
#include <TTree.h>
#include <TFile.h>
#include <TObjArray.h>
#include "FiberTargetParamManager.hh"
#include <time.h>
class G4Event;

class FiberTargetEventAction : public G4UserEventAction
{
public:
  FiberTargetEventAction();
  ~FiberTargetEventAction();
  FiberTargetEventAction(FiberTargetParamManager*);
  
public:
  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);
private:
  G4int primaryEventID;
  FiberTargetParamManager * paramMan;
  time_t start,end;
  
  //ROOT FILE
public:
  void definetree(const G4Event*);
  void filldata(const G4Event*);
private:
  TTree* tree;      //Tree
  TFile* file;      //ROOT file
  G4int evID;       //event ID
  G4int evID2;      //event ID
  G4int trackID;    //track ID
  G4int SeedFlag;
  G4double beamx,beamy,beamz; // Beam x,y,z position 
  G4double beamz_recon;       // Beam z position (reconstructed)
  G4double beamp;             // Beam Momentum
  G4double beampx;            // Beam Momentum
  G4double beampy;            // Beam Momentum
  G4double beampz;            // Beam Momentum
  G4double beam_theta;        // Beam Angle
  G4double beam_phi;          // Beam Angle
  G4double x,y,z;   //position on virtual detector
  G4double px,py,pz;//momentum on virtual detector
  G4double p;       //momentum on virtual detector
  G4double theta;   //theta on virtual detector
  G4double phi;     //phi on virtual detector
  G4double km_px,km_py,km_pz;//momentum 
  G4double km_p;       //momentum 
  G4double kp_px,kp_py,kp_pz;//momentum 
  G4double kp_p;       //momentum 
  G4double km_theta;   //theta
  G4double km_phi;     //phi
  G4double km_px0,km_py0,km_pz0;//momentum 
  G4double km_p0;       //momentum 
  G4double kp_px0,kp_py0,kp_pz0;//momentum 
  G4double kp_p0;       //momentum 
  G4double km_theta0;   //theta
  G4double km_phi0;     //phi
  G4double charge;  // charge of a detected particle
  G4String pname;   // particle name
  G4int pioflag;    // pion flag
  G4int kaoflag;    // kaon flag
  G4int proflag;    // proton flag
  G4int eflag;      // ?
  G4int phoflag;    // photon flag
  G4int posflag;    // positron flag
  G4int eleflag;    // electron flag
  G4int muoflag;    // muon flag
  G4int neuflag;    // neutron flag
  G4int PosiAnalFlag;
  G4int ProcID;     //Process ID
  G4int passth;     //beam passed through the target
  G4int eIoni;      //e Ionization
  G4int eBrem;      //e Brems
  G4int compt;      //compton scatterting
  G4int conv;       //photon conversion
  G4int phoele;     //photoelectric effect
  G4double minMom;  //min Momentum
  G4double maxMom;  //max Momentum
  G4double minTheta;//min Theta
  G4double maxTheta;//max Theta
  G4int btrig;      // trigger flag (test , 12Dec2013)
  G4double dE_fiberx[1000];
  G4double dE_fibery[1000];
  G4double dE_fiberx_kminus[1000];
  G4double dE_fibery_kminus[1000];
  G4double dE_fiberx_kplus[1000];
  G4double dE_fibery_kplus[1000];
  G4double dE_fiberx_xi[1000];
  G4double dE_fibery_xi[1000];
  //G4double time_fiber[2000];
  //G4double mom_fiber[2000];
  //G4double x_fiber[2000];
  //G4double y_fiber[2000];
  //G4double z_fiber[2000];

};
#endif
