#include "PrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4RunManager.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

using namespace CLHEP; // for units

PrimaryGeneratorAction::PrimaryGeneratorAction():
  G4VUserPrimaryGeneratorAction(),
  particleGun(0)
{
  G4int numOfParticles = 10000;
  particleGun = new G4ParticleGun(numOfParticles);

  G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("e-");

  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, 1)); // along x
  particleGun->SetParticleEnergy(40 * keV);
  
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  particleGun->SetParticlePosition(G4ThreeVector(0., 0., -0.01*mm));
  particleGun->GeneratePrimaryVertex(event);
  return;
}
