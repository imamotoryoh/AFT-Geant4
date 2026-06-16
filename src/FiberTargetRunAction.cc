/*
  FiberTargetRunAction.cc
  Oct 2015, T.Gogami
*/

#include "FiberTargetRunAction.hh"
#include "FiberTargetParamManager.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

#include <TFile.h>
#include <TTree.h>
#include <TObjArray.h>
#include <TH1F.h>
#include <stdlib.h>

FiberTargetRunAction::FiberTargetRunAction()
{}

FiberTargetRunAction::~FiberTargetRunAction()
{}

FiberTargetRunAction::FiberTargetRunAction(FiberTargetParamManager* PManDragon)
  :paramMan(PManDragon),RfileName("DRAGON.root")
{
  RfileName = paramMan->GetROOTFileName();
  if( paramMan->GetRFnum()!=0 ){
    char tmp[80];
    sprintf(tmp,"%d",paramMan->GetRFnum());
    RfileName = RfileName+"_"+tmp;

  }
}

void FiberTargetRunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << " " << G4endl;
  G4cout << "// Simulation Start //" << G4endl;
  G4cout << " " << G4endl;
  file = new TFile(RfileName,"recreate"); //Open ROOT file (Recreate)
  //file = new TFile("../analysis/root/test.root","recreate");
  //TTree *tree = new TTree("tree"," FiberTargetTRON Simulation ");//Tree
  TTree *tree = new TTree("tree"," Target Simulation ");//Tree
  Tree = (TTree*)file->Get("tree");
}

void FiberTargetRunAction::EndOfRunAction(const G4Run* aRun)
{
  Tree->Write();  //Write tree into ROOT file
  file -> Close();//Close ROOT file
  G4cout << "  END... bye ! " << G4endl;
  
}
