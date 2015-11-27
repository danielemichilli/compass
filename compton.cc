#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"

#include <time.h>

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"


int main(int argc,char** argv)
{
  time_t start,end;
  time(&start);

  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());                    //set the random engine
  G4long seed = time(NULL);                                                     //method to change the seed
  CLHEP::HepRandom::setTheSeed(seed);

  std::fstream input;
  input.open("input.in");
  G4double h;
  G4int n;
  G4double hSource;
  input>>h;
  h*=cm;
  input>>n;
  input>>hSource;
  hSource*=cm;  
  input.close();
  
  
  G4RunManager* runManager = new G4RunManager();                                //run manager creation

  DetectorConstruction* detector = new DetectorConstruction(h,n);               //detector creation
  runManager->SetUserInitialization(detector);                                  //apply to run manager

  PhysicsList* physics = new PhysicsList();                                     //physics list creation 
  runManager->SetUserInitialization(physics);                                   //apply to run manager

  PrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction(hSource);     //generation action creation
  runManager->SetUserAction(gen_action);                                        //apply to run manager

  EventAction* event_action = new EventAction();
  runManager->SetUserAction(event_action);

  RunAction* run_action = new RunAction();
  runManager->SetUserAction(run_action);
  
  runManager->Initialize();                                                     //inizialization of run manager

//UI->ApplyCommand("/random/resetEngineFrom currentRun.rndm");                  //restore random seed from file

  G4UImanager * UImanager = G4UImanager::GetUIpointer();  
  UImanager->ApplyCommand("/control/execute dati.mac");  

//opzioni per la visualizzazione
//  G4VisManager* visManager = new G4VisExecutive;
//  visManager->Initialize();
//  G4UIExecutive * ui = new G4UIExecutive(argc,argv);
//  ui->SessionStart();
//  delete ui;
//  delete visManager;

  delete runManager;

  time(&end);
  G4cout<<"Tempo impiegato: "<<difftime(end,start)<<" s"<<G4endl;

  return 0;
}
