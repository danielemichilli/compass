#include "PhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
//#include "G4EmStandardPhysics.hh"
#include "G4EmLivermorePolarizedPhysics.hh"
//#include "G4EmLivermorePhysics.hh"
//#include "G4EmPenelopePhysics.hh"
#include "G4OpticalPhysics.hh"

#include "G4Region.hh"
#include "G4RegionStore.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList() 
: G4VModularPhysicsList(){

  // Create a modular physics list and register only a 
  // few modules for it: EM interactions, decay of 
  // particles and radioactive decay. No hadronic physics 
  // is provided in the example.

 // SetVerboseLevel(1);

  // Default Decay Physics
  RegisterPhysics(new G4DecayPhysics());

  // Default Radioactive Decay Physics
  RegisterPhysics(new G4RadioactiveDecayPhysics());

  // Standard EM Physics
  RegisterPhysics(new G4EmLivermorePolarizedPhysics());
  
    // Standard EM Physics
  //RegisterPhysics(new G4EmLivermorePhysics());
 
  // Low emergy Penelope Physics
  //RegisterPhysics(new G4EmPenelopePhysics());
  
  //Optical Photons
  RegisterPhysics(new G4OpticalPhysics);

}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts()
{
  // The method SetCuts() is mandatory in the interface. 
 G4VUserPhysicsList::SetCuts();
 G4double cutForGamma     = 100*um;//10*um;//defaultCutValue;
 G4double cutForElectron  = 100*um;//defaultCutValue;
 G4double cutForPositron  = 100*um;//defaultCutValue;

  SetCutValue(cutForGamma, "gamma");
  SetCutValue(cutForElectron, "e-");
  SetCutValue(cutForPositron, "e+");
 
 //G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(250*eV, 1*GeV);//better do not use energy cuts
 
  
}  


