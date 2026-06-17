/*
  FiberTargetTransportation.hh

  T. Gogami
*/

#ifndef FiberTargetTransportation_h

#define FiberTargetTransportation_h 1

#include "G4Transportation.hh"

class FiberTargetTransportation : public G4Transportation
{
public:
  FiberTargetTransportation()
    : G4Transportation() {}
  ~FiberTargetTransportation() {}
  
  G4double AlongStepGetPhysicalInteractionLength( const G4Track & track, 
						  G4double previousStepSize,
						  G4double currentMinimumStep, 
						  G4double & currentSafety,
						  G4GPILSelection* selection );
  
};

#endif
