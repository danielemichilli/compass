#include "G4VSensitiveDetector.hh"
#include "OpPhotHit.hh"
#include "G4SystemOfUnits.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class PMTSD : public G4VSensitiveDetector
{
  public:
    PMTSD(G4String);
    virtual ~PMTSD();

  public:
    void Initialize(G4HCofThisEvent*);
    G4bool ProcessHits(G4Step*,G4TouchableHistory*);
    void EndOfEvent(G4HCofThisEvent*);

  private:
    G4int PhotonsCollectionID;
    OpPhotHitsCollection* PhotonsCollection;
};
