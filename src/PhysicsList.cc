#include "PhysicsList.hh"

#include "G4ParticleTypes.hh"
#include "G4EmPenelopePhysics.hh"

using namespace CLHEP; // for units

PhysicsList::PhysicsList()
{
  builtInPhysicsList = new G4EmPenelopePhysics();
}

PhysicsList::~PhysicsList()
{
}

void PhysicsList::ConstructParticle()
{
  ConstructBosons();
  ConstructLeptons();
  ConstructMesons();
  ConstructBaryons();
}

void PhysicsList::ConstructProcess()
{
  AddTransportation();

  builtInPhysicsList->ConstructProcess();
}

void PhysicsList::SetCuts()
{
  SetDefaultCutValue(0.0001 * mm);
}

void PhysicsList::ConstructBosons()
{
  // pseudo-particles
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();

  // gamma
  G4Gamma::GammaDefinition();

  // optical photon
  G4OpticalPhoton::OpticalPhotonDefinition();
}

void PhysicsList::ConstructLeptons()
{
  // leptons
  //  e+/-
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  // mu+/-
  G4MuonPlus::MuonPlusDefinition();
  G4MuonMinus::MuonMinusDefinition();
  // nu_e
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  // nu_mu
  G4NeutrinoMu::NeutrinoMuDefinition();
  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();
}

void PhysicsList::ConstructMesons()
{
  //  mesons
  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();
  G4PionZero::PionZeroDefinition();
}

void PhysicsList::ConstructBaryons()
{
  //  barions
  G4Proton::ProtonDefinition();
  G4AntiProton::AntiProtonDefinition();

  G4Neutron::NeutronDefinition();
  G4AntiNeutron::AntiNeutronDefinition();

  G4GenericIon::GenericIonDefinition();
}
