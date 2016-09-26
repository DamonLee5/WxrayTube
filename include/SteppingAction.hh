/**
 *The UserSteppingAction method is called in each step and is used to select the 
 *x-rays that travel into the detector volume and store their energy into the histogram of StoreData
 */

#ifndef STEPPINGACTION_H
#define STEPPINGACTION_H

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class G4Step;

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction();
  ~SteppingAction();

  void UserSteppingAction(const G4Step* step);
};

#endif // #ifndef STEPPINGACTION_H
