/*
  FiberTargetvdSD.hh
  Oct 2015,  T.Gogami
*/

#ifndef FiberTargetvdSD_h
#define FiberTargetvdSD_h 1

#include "FiberTargetvdHit.hh"

#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"

class FiberTargetvdSD : public G4VSensitiveDetector
{
  
public:
  FiberTargetvdSD(G4String name, G4int nCells, G4String colName);
  ~FiberTargetvdSD();
  
  void Initialize(G4HCofThisEvent*HCE);
  G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
  void EndOfEvent(G4HCofThisEvent*HCE);
  void clear();
  void DrawAll();
  void PrintAll();
  
private:
  FiberTargetvdHitsCollection *VDCollection;
  
  int* CellID;
  int numberOfCells;
  int HCID;
  int procID;
};




#endif

