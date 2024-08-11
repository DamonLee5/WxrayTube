#include "RunAction.hh"

#include "G4Run.hh"
#include "G4UserRunAction.hh"
#include "G4RunManager.hh"

#include "TRandom3.h"

#include "stdlib.h"
#include <unistd.h>  // Include this header to use getpid()
#include <sys/stat.h> // For mkdir

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

  StoreData::Initialize(theRun->GetRunID(), getpid()); // Initialize the StoreData instance
}

void RunAction::EndOfRunAction(const G4Run* theRun)
{
  // Create output directory if it does not exist
  struct stat info;
  if (stat("output", &info) != 0)
  {
    // Directory does not exist, create it
    if (mkdir("output", 0777) == -1)
    {
      G4cerr << "Error: Could not create directory 'output'" << G4endl;
      return;
    }
  }
  else if (info.st_mode & S_IFDIR)
  {
    // Directory exists, no need to create it
  }
  else
  {
    G4cerr << "Error: 'output' exists but is not a directory" << G4endl;
    return;
  }

  // Write the CSV file
  G4String fileName = "output/outFileRun" + std::to_string(theRun->GetRunID()) + "_pid" + std::to_string(getpid()) + ".csv";
  StoreData::GetInstance()->WriteCSV(fileName);
  StoreData::DeleteInstance(); // Delete the StoreData instance

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
