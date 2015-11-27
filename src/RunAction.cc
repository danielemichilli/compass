#include "RunAction.hh"
#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4SDManager.hh"
#include "G4UnitsTable.hh"
#include <fstream>

RunAction::RunAction(){}
RunAction::~RunAction(){}

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
//  G4RunManager::GetRunManager()->SetRandomNumberStore(true);

  std::ofstream scorefile;
  scorefile.open("scorefile.dat");
  const G4long* table_entry;
  table_entry = CLHEP::HepRandom::getTheSeeds();
  scorefile << "Seed: " << table_entry[0] << "; " << table_entry[1] << G4endl;
  scorefile.close();
  
//  std::ofstream scorefile_gamma;
//  scorefile_gamma.open("scorefile_gamma.dat");
//  scorefile_gamma.close();
//  std::ofstream scorefile_phot;
//  scorefile_phot.open("scorefile_photon.dat");
//  scorefile_phot.close();
  std::ofstream scorefile_theta;
  scorefile_theta.open("scorefile_theta.dat");
  scorefile_theta.close();
  std::ofstream scorefile_charge;
  scorefile_charge.open("scorefile_charge.dat");
  scorefile_charge.close();
  std::ofstream scorefile_theta2;
  scorefile_theta2.open("scorefile_theta2.dat");
  scorefile_theta2.close();
  std::ofstream scorefile_charge2;
  scorefile_charge2.open("scorefile_charge2.dat");
  scorefile_charge2.close();
}

void RunAction::EndOfRunAction(const G4Run*)
{
  G4RunManager* runManager = G4RunManager::GetRunManager();
  EventAction* event_action = (EventAction*) runManager->GetUserEventAction();
  PrimaryGeneratorAction* gen_action = (PrimaryGeneratorAction*) runManager->GetUserPrimaryGeneratorAction();
  DetectorConstruction* detector_construction = (DetectorConstruction*) runManager->GetUserDetectorConstruction();

  //Analisi dei risultati
  const G4int evt = runManager->GetCurrentRun()->GetNumberOfEvent();
  G4int num = event_action->GetNum();
  G4int num2= event_action->GetNum2();
  G4double thetaTEO = atan2(gen_action->GetParticleGun()->GetParticlePolarization().getY(),gen_action->GetParticleGun()->GetParticlePolarization().getX());
  G4double ene = gen_action->GetParticleGun()->GetParticleEnergy();

  G4cout << G4endl
         << "Energia del fotone (keV)..........." << ene/keV  << G4endl
         << "Eventi simulati...................." << evt  << G4endl
         << "Eventi validi nell'assorbitore....." << num  << G4endl
         << "Eventi validi nello scatteratore..." << num2 << G4endl
         << "Angolo di polarizzazione reale....." << thetaTEO/M_PI*180 << "°" << G4endl
         << "Efficienza 1......................." << G4double(num)/evt << G4endl
         << "Efficienza 2......................." << G4double(num2)/evt << G4endl <<
  G4endl << "Numero di bacchette per lato......." << detector_construction->GetSideNb() << G4endl
         << "Altezza delle bacchette............" << detector_construction->GetHeightOfBacc()/cm << " cm" << G4endl
         << "Altezza della sorgente............." << gen_action->GethSource()/cm << " cm" << G4endl;

  std::ofstream scorefile;
  scorefile.open("scorefile.dat",std::fstream::app);
  scorefile << G4endl
            << "Energia del fotone (keV)..........." << ene/keV  << G4endl
            << "Eventi simulati...................." << evt  << G4endl
            << "Eventi validi nell'assorbitore....." << num  << G4endl
            << "Eventi validi nello scatteratore..." << num2 << G4endl
            << "Angolo di polarizzazione reale....." << thetaTEO/M_PI*180 << "°" << G4endl
            << "Tagging efficiency 1..............." << G4double(num)/evt << G4endl
            << "Tagging efficiency 2..............." << G4double(num2)/evt << G4endl <<
     G4endl << "Numero di bacchette per lato......." << detector_construction->GetSideNb() << G4endl
            << "Altezza delle bacchette............" << detector_construction->GetHeightOfBacc()/cm << " cm" << G4endl
            << "Altezza della sorgente............." << gen_action->GethSource()/cm << " cm" << G4endl;
 

  scorefile.close();
}

