/*
  FiberTargetSteppingAction.cc
  Oct 2015,  T.Gogami
*/

#include "FiberTargetSteppingAction.hh"
#include "G4SteppingManager.hh"

#include "FiberTargetEventAction.hh"
#include "FiberTargetRunAction.hh"


FiberTargetSteppingAction::FiberTargetSteppingAction()
{ }

void FiberTargetSteppingAction::UserSteppingAction(const G4Step* aStep)
{ 
  G4StepPoint* endPoint = aStep->GetPostStepPoint();
  G4String procName = endPoint->GetProcessDefinedStep()->GetProcessName();
  //G4cout << procName <<G4endl;

}
