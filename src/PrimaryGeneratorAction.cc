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
  G4int numOfParticles = 1;
  particleGun = new G4ParticleGun(numOfParticles);

  G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("e-");

  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticleMomentumDirection(G4ThreeVector(1, 0, 0)); // along x
  particleGun->SetParticleEnergy(35 * keV);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  particleGun->SetParticlePosition(G4ThreeVector(-5 * cm, 0 * cm, 0 * cm));
  particleGun->GeneratePrimaryVertex(event);
  return;
}
