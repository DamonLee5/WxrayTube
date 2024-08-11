#include "EventAction.hh"

#include "G4Event.hh"

EventAction::EventAction()
{
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event* event)
{
  G4int evtNum = event->GetEventID();
  if(!(evtNum % 100)) G4cout << "Event " << evtNum << G4endl;

  return;
}

void EventAction::EndOfEventAction(const G4Event* /*event*/)
{
  // G4int evtNum = event->GetEventID();

  // G4cout << "End of event " << evtNum << G4endl;

  return;
}
