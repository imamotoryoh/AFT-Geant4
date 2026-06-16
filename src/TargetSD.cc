/*
  TargetSD.cc
  Oct 2015, T.Gogami
*/

#include "globals.hh"
#include "TargetSD.hh"
#include "TargetHit.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"

TargetSD::TargetSD( G4String name,
		    G4int nCells,
		    G4String colName )
  : G4VSensitiveDetector(name),
    numberOfCells(nCells),//integer
    HCID(-1),//integer
    procID(0)//Process ID
{
  G4String HCname;
  collectionName.insert(HCname=colName);
  CellID = new G4int[numberOfCells];//interger
}

TargetSD::~TargetSD()
{
  delete [] CellID; //interger
}

void TargetSD::Initialize(G4HCofThisEvent*)
{
  TargetCollection = new TargetHitsCollection
    (SensitiveDetectorName,collectionName[0]); 
  for(G4int j=0;j<numberOfCells;j++)
    {
      CellID[j] = -1; //integer
    }
}

G4bool TargetSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  G4double edep = aStep->GetTotalEnergyDeposit();
  //G4cout << " dE=" << edep/MeV << " MeV" << G4endl;
  //========TouchableHistory=======///
  G4TouchableHistory* hist 
    = (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());
  const G4VPhysicalVolume* physVol = hist->GetVolume();
  G4int copyID = hist->GetReplicaNumber();
  //G4cout << " CopyID = " << copyID << G4endl;

  //=========Get Track ===========///
  G4Track *aTrack = aStep->GetTrack();
  
  //==========Get Process=========///
  //const G4VProcess*  vpro = aTrack -> GetCreatorProcess() ;//Get creator process
  //const G4String& proname = vpro->GetProcessName(); //Process name
  //if(vpro!=0){
  //  //G4cout << " " << proname << G4endl;
  //  if (proname=="eIoni")procID = 1 ;   //electron ionization
  //  else if(proname=="eBrem")procID = 2;//electron brems
  //  else if(proname=="compt")procID = 3;//compton scattering
  //  else if(proname=="phot" )procID = 4;//photoelectric effect
  //  else if(proname=="conv" )procID = 5;//photon conversion
  //}
  //else procID = 0;
  //G4cout << procID <<G4endl;
  //if(procID!=0 && procID!=2 && procID!=1){
  //  G4cout << procID << ":::: " <<proname <<G4endl;
  //}
  
  //==========Get Particle info========///
  G4ParticleDefinition *particle =
    aTrack -> GetDynamicParticle() -> GetDefinition();
  
   //==========Virtual Detector hit====///
  TargetHit* tarHit =
    new TargetHit(physVol->GetLogicalVolume());
  tarHit->SetEdep( edep );
  //G4cout << " edep = " << edep << G4endl;
  G4AffineTransform aTrans = hist->GetHistory()->GetTopTransform();
  aTrans.Invert();
  tarHit->SetPos( aStep->GetPreStepPoint()->GetPosition() );
  tarHit->SetMom( aTrack->GetMomentum() );
  tarHit->SetPname( particle->GetParticleName() );
  tarHit->SetCharge( particle->GetPDGCharge() );
  tarHit->SetFiberID( copyID );
  //tarHit->SetProcID( procID );
  G4int icell = TargetCollection->insert( tarHit );
  
  //CellID[copyID] = icell - 1;
  //    if(verboseLevel>0)
//      { G4cout << " New vd Hit on CellID " << copyID << G4endl; }
  
  return true;
}

void TargetSD::EndOfEvent(G4HCofThisEvent*HCE)
{
  if(HCID<0)
  { HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); }
  HCE->AddHitsCollection( HCID, TargetCollection );
}

void TargetSD::clear()
{
} 

void TargetSD::DrawAll()
{
} 

void TargetSD::PrintAll()
{
} 
