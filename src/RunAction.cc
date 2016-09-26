#include "RunAction.hh"

#include "G4Run.hh"
#include "G4UserRunAction.hh"
#include "G4RunManager.hh"

#include "TRandom3.h"

#include "stdlib.h"

#include "StoreData.hh"

RunAction::RunAction()
{
}

RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run* theRun)
{
  G4cout << "################## Run " << theRun->GetRunID() << " start" << G4endl;

  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  CLHEP::HepRandom::setTheSeed(CreateSeed());

  new StoreData(theRun->GetRunID(), getpid()); // both run and pid so multiple processes can write the output files at the same time
}

void RunAction::EndOfRunAction(const G4Run* theRun)
{
  StoreData::GetInstance()->WriteData();
  delete StoreData::GetInstance();

  G4cout << "################## Run " << theRun->GetRunID() << " end" << G4endl;
}

G4long RunAction::CreateSeed()
{
  G4long s = time(0); // time, "good" for same process and consecutive (long) runs
  s += getpid(); // pid, "good" for more processes starting at the same time

  TRandom3* rand = new TRandom3();
  rand->SetSeed(s);

  G4long seed = round(1000000 * rand->Uniform());
  return seed;
}
