/*
  FiberTargetRunAction.hh
  Oct2015,  T.Gogami
*/

#ifndef FiberTargetRunAction_h
#define FiberTargetRunAction_h 1

#include "FiberTargetParamManager.hh"

#include "G4UserRunAction.hh"
#include "globals.hh"

#include <TFile.h>
#include <TTree.h>
#include <TObjArray.h>
#include <TH1F.h>

class G4Run;

class FiberTargetRunAction : public G4UserRunAction
{
public:
  FiberTargetRunAction();
  virtual ~FiberTargetRunAction();
  FiberTargetRunAction(FiberTargetParamManager*);
  
  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);
  
private:
  FiberTargetParamManager* paramMan;
  
  //File
private:
  G4String RfileName;
  TFile *file;
  TTree *Tree;
  
public:
  TTree* GetTree() { return Tree; };
  TFile* GetFile() { return file; };

};

#endif
