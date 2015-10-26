#include "ScintillatorSD.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"

G4Allocator<GammaHit> GammaHitAllocator;

ScintillatorSD::ScintillatorSD(G4String SDname):
G4VSensitiveDetector(SDname),GammasCollectionID(-1)
{
  collectionName.insert("GammasCollection");
}

ScintillatorSD::~ScintillatorSD(){}

void ScintillatorSD::Initialize(G4HCofThisEvent* HCE)
{
  GammasCollection = new GammaHitsCollection("Scintillator","GammasCollection");
}


G4bool ScintillatorSD::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  G4String particleName  = aStep->GetTrack()->GetParticleDefinition()->GetParticleName();
  G4int parentID         = aStep->GetTrack()->GetParentID();
  G4String processName   = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

  if(parentID==0 and particleName=="gamma" and processName!="Transportation") {
    GammaHit* gamma = new GammaHit();
    gamma->SetProcess(aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName());
    gamma->SetTrackID(aStep->GetTrack()->GetTrackID());
    gamma->SetChamberNb(aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber());
    gamma->SetEdep(aStep->GetTotalEnergyDeposit());
    gamma->SetPos(aStep->GetPostStepPoint()->GetPosition());
    gamma->SetMaterial(aStep->GetPreStepPoint()->GetMaterial()->GetName());
    GammasCollection->insert(gamma);
  }

  return true;
}

#include "G4RunManager.hh"

void ScintillatorSD::EndOfEvent(G4HCofThisEvent* HCE)
{
  if(GammasCollectionID < 0)  GammasCollectionID  = G4SDManager::GetSDMpointer() -> GetCollectionID("GammasCollection");
  HCE -> AddHitsCollection(GammasCollectionID,GammasCollection);
}

