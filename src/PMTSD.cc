#include "PMTSD.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"

G4Allocator<OpPhotHit> OpPhotHitAllocator;

PMTSD::PMTSD(G4String SDname):
G4VSensitiveDetector(SDname),PhotonsCollectionID(-1)
{
  collectionName.insert("PhotonsCollection");
}

PMTSD::~PMTSD(){}

void PMTSD::Initialize(G4HCofThisEvent* HCE)
{
  PhotonsCollection = new OpPhotHitsCollection("PMT","PhotonsCollection");
}


G4bool PMTSD::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  G4String VolName       = aStep->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetName();
  G4String particleName  = aStep->GetTrack()->GetParticleDefinition()->GetParticleName();

  if(particleName=="opticalphoton" and VolName=="PMT") {
    OpPhotHit* photon = new OpPhotHit();
    photon->SetParentID(aStep->GetTrack()->GetParentID());
    photon->SetTrackID(aStep->GetTrack()->GetTrackID());
    photon->SetChamberNb(aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber());
    photon->SetEdep(aStep->GetTrack()->GetKineticEnergy());
    photon->SetMaterial(aStep->GetPreStepPoint()->GetMaterial()->GetName());
    PhotonsCollection->insert(photon);
  }

  return true;
}

#include "G4RunManager.hh"

void PMTSD::EndOfEvent(G4HCofThisEvent* HCE)
{
  if(PhotonsCollectionID < 0) PhotonsCollectionID = G4SDManager::GetSDMpointer() -> GetCollectionID("PhotonsCollection");
  HCE -> AddHitsCollection(PhotonsCollectionID,PhotonsCollection);
}

