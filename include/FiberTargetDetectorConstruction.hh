/*
  FiberTargetDetectorConstruction.hh
  Oct 2015,  T.Gogami
*/

#ifndef FiberTargetDetectorConstruction_h

#define FiberTargetDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"
#include "FiberTargetParamManager.hh"
#include "G4LogicalVolume.hh"

class MaterialList;
//class FiberTargetField;

class FiberTargetDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  FiberTargetDetectorConstruction();
  ~FiberTargetDetectorConstruction();
  FiberTargetDetectorConstruction(FiberTargetParamManager*);

  G4VPhysicalVolume* Construct();

private:
  MaterialList *mList_;
  MaterialList *DefineMaterials( void );
  G4String TargetMaterial;
  G4double TThickness;    // Added by Toshi , 18Oct2013
  G4LogicalVolume *targetLV; 

  //FiberTargetField *MakeDetectorField( void );
  //FiberTargetField *EMField_;

  G4VPhysicalVolume* ConstructPayload();
  FiberTargetParamManager* paramMan;
};

#endif

