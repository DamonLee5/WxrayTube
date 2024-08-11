#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "SteppingAction.hh"
#include "EventAction.hh"
#include "RunAction.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

void ExecuteBeamOnChunks(G4UImanager* UImanager, G4int numberOfChunks, G4int eventsPerChunk) {

  for (G4int i = 0; i < numberOfChunks; ++i) {
    std::stringstream command;
    command << "/run/beamOn " << eventsPerChunk;
    UImanager->ApplyCommand(command.str());
  }

}

int main(int argc, char* argv[])
{
  // Setting random generator
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine()); // The seed is set at the beginning of each run

  // Construct the run manager
  G4RunManager* runManager = new G4RunManager();

  // Instruction for the detector construction
  runManager->SetUserInitialization(new DetectorConstruction());

  // Physics list
  G4VModularPhysicsList* physicsList = new PhysicsList();
  runManager->SetUserInitialization(physicsList);

  // Particle generation
  runManager->SetUserAction(new PrimaryGeneratorAction());

  // Stepping action, here the particle passing the detector volume are measured
  runManager->SetUserAction(new SteppingAction());

  // Event action, eventually cout the event number
  runManager->SetUserAction(new EventAction());

  // Run action the instance of the class to store the data is initialized and the data saved at the end of the run
  runManager->SetUserAction(new RunAction());

  // Start the G4 kernel
  runManager->Initialize();

  // Visualization manager
  G4VisManager* visManager = new G4VisExecutive();
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (argc != 1)
  {
    // Batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command + fileName);

    // Execute beamOn in chunks
    G4int numberOfChunks = 100; // Total number of electrons
    G4int eventsPerChunk = 1000; // Adjust the chunk size as needed

    ExecuteBeamOnChunks(UImanager, numberOfChunks, eventsPerChunk);
  
  }
  else
  {
    // Interactive mode: define UI session
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    UImanager->ApplyCommand("/control/execute init_vis.mac"); 
    ui->SessionStart();
    delete ui;
  }

  // Delete visualization manager
  delete visManager;

  // Delete run manager
  delete runManager;

  return 0;
}
