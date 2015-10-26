#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "G4SystemOfUnits.hh"

/// Modular physics list
///
/// It includes the folowing physics builders
/// - G4DecayPhysics
/// - G4RadioactiveDecayPhysics
/// - G4EmStandardPhysics livermore pol

class PhysicsList: public G4VModularPhysicsList
{
public:
  /// constructor
  PhysicsList();
  /// destructor
  virtual ~PhysicsList();

  /// Set user cuts
  virtual void SetCuts();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

