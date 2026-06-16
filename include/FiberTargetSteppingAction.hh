/*
  FiberTargetSteppingAction.hh
  Oct 2015,  T.Gogami
*/

#ifndef FiberTargetSteppingAction_h
#define FiberTargetSteppingAction_h 1

#include "G4UserSteppingAction.hh"


class FiberTargetSteppingAction : public G4UserSteppingAction
{
  public:
    FiberTargetSteppingAction();
   ~FiberTargetSteppingAction(){};

    void UserSteppingAction(const G4Step*);
};


#endif
