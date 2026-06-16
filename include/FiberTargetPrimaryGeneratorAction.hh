/*
  FiberTargetPrimaryGeneratorAction.cc
  Oct 2015, T.Gogami
*/

#ifndef FiberTargetPrimaryGeneratorAction_h
#define FiberTargetPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "FiberTargetParamManager.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "G4String.hh"
#include <TRandom3.h>
#include <fstream>
using namespace std;

class G4ParticleGun;
class G4Event;

class FiberTargetPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  FiberTargetPrimaryGeneratorAction();
    ~FiberTargetPrimaryGeneratorAction();
  FiberTargetPrimaryGeneratorAction(FiberTargetParamManager*);
  
  void GeneratePrimaries( G4Event* anEvent );
  void GenPosRandinTarget( G4Event* anEvent );
  
private:
  G4ParticleGun* particleGun;
  FiberTargetParamManager *paramMan;
  G4int GenFlag;
  G4int SeedFlag;
  G4String SeedFile;
  FILE* sfile;
  //ifstream* dragon_seed;
  ifstream* dragon_seed;
  //char sfile[500];
  G4double rasterX,rasterY,rasterZ;
  G4double bpc,bpw;  // Beam momentum (cetner,width)
  G4double bthc,bthw;// Beam theta    (cetner,width)
  G4double bphc,bphw;// Beam phi      (cetner,width)
  //G4double BeamOffx,BeamOffy,BeamOffz;
  G4ThreeVector BeamOffset;
  G4double Beamx,Beamy,Beamz;
  G4double BeamMom;
  G4int ParticleFlag;
  G4ParticleDefinition* particle;
  void GenParPoint(G4Event* );
  void GenParUni(G4Event* );
  void SetMom(G4ThreeVector , G4ThreeVector , G4Event* , G4int );
  //void SetSeed(ifstream* ifs){
  //  dragon_seed = ifs;
  //};
  //ifstream* GetSeed(){return dragon_seed; };

  
  //void GenParSeed(G4Event* );
  TRandom3* trand;

};
#endif
