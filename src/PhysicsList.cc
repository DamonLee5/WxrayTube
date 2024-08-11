#include "PhysicsList.hh"
#include "G4ParticleTypes.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4ProcessManager.hh"
#include "G4PhysicsListHelper.hh"
#include "G4Decay.hh"

// Additional processes
#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4eplusAnnihilation.hh"

using namespace CLHEP; // for units

PhysicsList::PhysicsList()
  : builtInPhysicsList(new G4EmPenelopePhysics())
{
}

PhysicsList::~PhysicsList()
{
  delete builtInPhysicsList;
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

  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
  G4Decay* decayProcess = new G4Decay();

  // Add decay process for all particles
  auto particleIterator = GetParticleIterator();
  particleIterator->reset();
  while ((*particleIterator)())
  {
    G4ParticleDefinition* particle = particleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();

    if (!pmanager)
    {
      pmanager = new G4ProcessManager(particle);
      particle->SetProcessManager(pmanager);
    }

    // Add decay process for all particles
    if (decayProcess->IsApplicable(*particle))
    {
      ph->RegisterProcess(decayProcess, particle);
    }
  }
}

void PhysicsList::SetCuts()
{
  SetDefaultCutValue(0.0001 * mm);
  G4VUserPhysicsList::SetCuts();
}

void PhysicsList::ConstructBosons()
{
  // Pseudo-particles
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();

  // Gamma
  G4Gamma::GammaDefinition();

  // Optical photon
  G4OpticalPhoton::OpticalPhotonDefinition();
}

void PhysicsList::ConstructLeptons()
{
  // Leptons
  // e+/-
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
  // Mesons
  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();
  G4PionZero::PionZeroDefinition();
  G4KaonPlus::KaonPlusDefinition();
  G4KaonMinus::KaonMinusDefinition();
}

void PhysicsList::ConstructBaryons()
{
  // Baryons
  G4Proton::ProtonDefinition();
  G4AntiProton::AntiProtonDefinition();
  
  G4Neutron::NeutronDefinition();
  G4AntiNeutron::AntiNeutronDefinition();
  
  G4GenericIon::GenericIonDefinition();
}
