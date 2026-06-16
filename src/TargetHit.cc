/*
  TargetHit.cc
  Oct 2015, T.Gogami
*/

#include "TargetHit.hh"

#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4ios.hh"

G4Allocator<TargetHit> TargetHitAllocator;

TargetHit::TargetHit()
{pLogV=NULL;}

TargetHit::TargetHit(G4LogicalVolume* logVol)
:pLogV(logVol)
{;}

TargetHit::~TargetHit()
{;}

TargetHit::TargetHit(const TargetHit &right)
  : G4VHit()
{
  edep = right.edep;
  pos = right.pos;
  rot = right.rot;
  mom = right.mom;
  pname = right.pname;
  procID= right.procID;
  pLogV = right.pLogV;
}

const TargetHit& TargetHit::operator=(const TargetHit &right)
{
  edep = right.edep;
  fiberID = right.fiberID;
  pos = right.pos;
  rot = right.rot;
  mom = right.mom;
  pname = right.pname;
  procID= right.procID;
  pLogV = right.pLogV;
  
  return *this;
}

G4int TargetHit::operator==(const TargetHit &right) const
{
  return (this==&right) ? 1 : 0;
}

void TargetHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Transform3D trans(rot,pos);
    G4VisAttributes attribs;
    const G4VisAttributes* pVA = pLogV->GetVisAttributes();
    if(pVA) attribs = *pVA;
    G4Colour colour(1.,0.,0.);
    attribs.SetColour(colour);
    attribs.SetForceWireframe(false);
    attribs.SetForceSolid(true);
    pVVisManager->Draw(*pLogV,attribs,trans);
  }
}

void TargetHit::Print()
{
}


