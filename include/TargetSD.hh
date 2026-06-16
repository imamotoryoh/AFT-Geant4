/*
  TargetSD.hh
  Oct 2015,  T.Gogami
*/

#ifndef TargetSD_h
#define TargetSD_h 1

#include "TargetHit.hh"

#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"

class TargetSD : public G4VSensitiveDetector
{
  
public:
  TargetSD(G4String name, G4int nCells, G4String colName);
  ~TargetSD();
  
  void Initialize(G4HCofThisEvent*HCE);
  G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
  void EndOfEvent(G4HCofThisEvent*HCE);
  void clear();
  void DrawAll();
  void PrintAll();
  
private:
  TargetHitsCollection *TargetCollection;
  
  int* CellID;
  int numberOfCells;
  int HCID;
  int procID;
};




#endif

