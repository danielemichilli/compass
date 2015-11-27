#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "G4UnitsTable.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction(G4double hSource)
{
  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);
  
  heigth_source = hSource;

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("gamma");

  particleGun->SetParticleDefinition(particle);
  G4double energy=20*keV;
  particleGun->SetParticleEnergy(energy);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4RunManager* runManager = G4RunManager::GetRunManager();
  DetectorConstruction* detector_construction = (DetectorConstruction*) runManager->GetUserDetectorConstruction();
  G4int    n   = detector_construction->GetSideNb();
  G4int    h   = detector_construction->GetHeightOfBacc()/2;
  G4double len = detector_construction->GetPixelSize();
  G4double lab = detector_construction->GetLabSize();
  G4int evtNb = anEvent->GetEventID();
  
  if (n == 1) {
    particleGun->SetParticlePosition(G4ThreeVector(2.*cm,0.*mm,heigth_source-h));  //sorgente puntiforme fissa
    particleGun->SetParticleMomentumDirection(G4ThreeVector(-1.,0.,0.));
  }
  else {
    n = n  / 2 - 1;
    particleGun->SetParticlePosition(G4ThreeVector((2.*G4UniformRand()-1.)*n*len,(2.*G4UniformRand()-1.)*n*len,h));  //illuminazione uniforme
    particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,-1.));
  }
  
  particleGun->GeneratePrimaryVertex(anEvent);
}
