/**
 *In this class are specified all the particles used in the simulation and the physics of their interaction.
 *The Penelope library is used for the physics, it implements electromagnetic processes.
 *All the particle available have been initialized in order to avoid errors while running the program.
 */

#ifndef PHYSICSLIST_H
#define PHYSICSLIST_H

#include "globals.hh"
#include "G4VModularPhysicsList.hh"

class PhysicsList : public G4VModularPhysicsList
{
public:
  PhysicsList();
  ~PhysicsList();

  void ConstructParticle();
  void ConstructProcess();
  void SetCuts();

  void ConstructBosons();
  void ConstructLeptons();
  void ConstructMesons();
  void ConstructBaryons();

private:
  G4VPhysicsConstructor* builtInPhysicsList;
};

#endif // #ifndef PHYSICSLIST_H
