/**
 *The methods implemented in this class are called at the begin and end of each event.
 *They are used to print out the event number.
 */

#ifndef EVENTACTION_H
#define EVENTACTION_H

#include "G4UserEventAction.hh"
#include "globals.hh"

class EventAction : public G4UserEventAction
{
public:
  EventAction();
  ~EventAction();

  void BeginOfEventAction(const G4Event* event);
  void EndOfEventAction(const G4Event* event);
};

#endif // #ifndef EVENTACTION_H
