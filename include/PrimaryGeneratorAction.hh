/**
 *This class contains the implementation of the particle gun used in the simulation
 *The GeneratePrimaries function is called at the beginning of every event
 */

#ifndef PRIMARYGENERATORACTION_H
#define PRIMARYGENERATORACTION_H

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4Event;
class G4ParticleGun;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  G4ParticleGun* particleGun;
  
public:
  PrimaryGeneratorAction();
  ~PrimaryGeneratorAction();

  void GeneratePrimaries(G4Event* event);
};

#endif // #ifndef PRIMARYGENERATORACTION_H
