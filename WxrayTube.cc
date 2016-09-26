#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "SteppingAction.hh"
#include "EventAction.hh"
#include "RunAction.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

int main(int argc, char* argv[])
{
  // setting random generator
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine()); // the seed is set at the beginning of each run

  // construct the run manager
  G4RunManager* runManager = new G4RunManager();

  // instruction for the detector construction
  runManager->SetUserInitialization(new DetectorConstruction());

  // physics list
  G4VModularPhysicsList* physicsList = new PhysicsList();
  //G4VUserPhysicsList* physicsList = new PhysicsList();
  runManager->SetUserInitialization(physicsList);

  // particle generation
  runManager->SetUserAction(new PrimaryGeneratorAction());

  // stepping action, here the particle passing the detector volume are measured
  runManager->SetUserAction(new SteppingAction());

  // event action, eventually cout the event number
  runManager->SetUserAction(new EventAction());

  // run action the instance of the class to store the data is initialized and the data saved at the end of the run
  runManager->SetUserAction(new RunAction());

  // start the g4 kernel
  runManager->Initialize();

#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive();
  visManager->Initialize();
#endif

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (argc!=1)
    {
      // batch mode
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);
    }
  else
    {
      // interactive mode : define UI session
#ifdef G4UI_USE
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
      UImanager->ApplyCommand("/control/execute init_vis.mac"); 
#else
      UImanager->ApplyCommand("/control/execute init.mac"); 
#endif
      ui->SessionStart();
      delete ui;
#endif
    }

#ifdef G4VIS_USE
  // to avoid the picture being killed immediately
  // char a;
  // std::cout << "\n\n\ninput any character to exit\n";
  // std::cin >> a;
  delete visManager;
#endif

  delete runManager;

  return 0;
}
