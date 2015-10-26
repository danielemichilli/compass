#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

class G4Run;

class RunAction : public G4UserRunAction
{
  public:
    RunAction();
   ~RunAction();

  public:
    void BeginOfRunAction(const G4Run*);
    void EndOfRunAction(const G4Run*);
};

