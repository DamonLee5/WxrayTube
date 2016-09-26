/**
 *The methods of this class are called at the start and end of every run.
 *They are used to set the seed of the random generator used by the simulation and to initialize and then delete the instance of the StoreData class.
 */

#ifndef RUNACTION_H
#define RUNACTION_H

#include "globals.hh"
#include "G4UserRunAction.hh"

class G4Run;

class RunAction : public G4UserRunAction
{
  G4long CreateSeed();

public:
  RunAction();
  ~RunAction();
  
  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);
};

#endif // #ifndef EVENTACTION_H
