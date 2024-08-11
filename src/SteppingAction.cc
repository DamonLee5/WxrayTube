#include "SteppingAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"

#include "StoreData.hh"

using namespace CLHEP; // for units

SteppingAction::SteppingAction()
{
}

SteppingAction::~SteppingAction()
{
}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  G4StepPoint* prePoint = step->GetPreStepPoint();

  if(prePoint->GetStepStatus() != fGeomBoundary) return; // extract info only for particles that have just entered the volume

  G4TouchableHandle preTouch = prePoint->GetTouchableHandle();
  G4VPhysicalVolume* volume = preTouch->GetVolume();

  if(volume->GetName() != "detector") return; // extract info only for particles that are in the detector

  G4Track* track = step->GetTrack();
  const G4ParticleDefinition* particle = track->GetParticleDefinition();

  if(particle->GetParticleName() != "gamma"){
    // G4cout << "No gamma " << G4endl;
    return; // select only the gammas
  } 

  G4double kinEnergy = track->GetKineticEnergy();

  StoreData::GetInstance()->FillEnergySpectrum(kinEnergy / keV);

  // G4cout << "Seen " << particle->GetParticleName() << " with kin energy " << kinEnergy / keV << " keV in volume " << volume->GetName() << G4endl;

  return;
}
