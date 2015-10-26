#include "G4VSensitiveDetector.hh"
#include "GammaHit.hh"
#include "G4SystemOfUnits.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class ScintillatorSD : public G4VSensitiveDetector
{
  public:
    ScintillatorSD(G4String);
    virtual ~ScintillatorSD();

  public:
    void Initialize(G4HCofThisEvent*);
    G4bool ProcessHits(G4Step*,G4TouchableHistory*);
    void EndOfEvent(G4HCofThisEvent*);

  private:
    G4int GammasCollectionID;
    GammaHitsCollection* GammasCollection;
};
