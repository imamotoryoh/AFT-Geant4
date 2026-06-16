/*
  TargetHit.hh
  Oct 2015, T.Gogami
*/


#ifndef TargetHit_h
#define TargetHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

class TargetHit : public G4VHit
{
public:
  
  TargetHit();
  TargetHit(G4LogicalVolume* logVol);
  ~TargetHit();
  TargetHit(const TargetHit &right);
  const TargetHit& operator=(const TargetHit &right);
  G4int operator==(const TargetHit &right) const;
  
  
  inline void *operator new(size_t);
  inline void operator delete(void *aHit);
  void *operator new(size_t,void*p){return p;}
#ifndef G4NOT_ISO_DELETES
  void operator delete(void *,void*){}
#endif
  
  void Draw();
  void Print();

private:
  G4double edep;
  G4int fiberID;
  G4ThreeVector pos;
  G4ThreeVector mom;
  G4String pname;
  G4double charge;
  G4RotationMatrix rot;
  G4int procID;
  const G4LogicalVolume* pLogV;
  
public:
  inline void SetEdep(G4double de)
  { edep = de; };
  inline void AddEdep(G4double de)
  { edep += de; };
  inline G4double GetEdep()
  { return edep; };
  //Position
  inline void SetPos(G4ThreeVector xyz)
  { pos = xyz; };
  inline G4ThreeVector GetPos()
  { return pos; };
  //Particle Momentum
  inline void SetMom(G4ThreeVector pxpypz)
  { mom = pxpypz; };
  inline G4ThreeVector GetMom()
  { return mom; };
  //Particle name
  inline void SetPname(G4String particlename)
  { pname = particlename; };
  inline G4String GetPname()
  { return pname; };
  //Charge
  inline void SetCharge(G4double particlecharge)
  { charge = particlecharge; };
  inline G4double GetCharge()
  { return charge; };
  //Rotation
  inline void SetRot(G4RotationMatrix rmat)
  { rot = rmat; };
  inline G4RotationMatrix GetRot()
  { return rot; };
  //ProcessID
  inline void SetProcID(G4int processID)
  { procID = processID; };
  inline G4int GetProcID()
  { return procID; };
  inline void SetFiberID(G4int fiber_id)
  { fiberID = fiber_id; };
  inline G4int GetFiberID()
  { return fiberID; };
  //Logical Volume
  inline const G4LogicalVolume * GetLogV()
  { return pLogV; };
  
};

typedef G4THitsCollection<TargetHit> TargetHitsCollection;

extern G4Allocator<TargetHit> TargetHitAllocator;

inline void* TargetHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) TargetHitAllocator.MallocSingle();
  return aHit;
}

inline void TargetHit::operator delete(void *aHit)
{
  TargetHitAllocator.FreeSingle((TargetHit*) aHit);
}

#endif
