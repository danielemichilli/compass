#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

#include "G4ThreeVector.hh"
#include <iostream>
#include <fstream>

class RunAction;

class EventAction : public G4UserEventAction 
{
  public:
    EventAction();
    virtual ~EventAction();

    void BeginOfEventAction(const G4Event*);
    void EndOfEventAction(const G4Event*);

    inline G4int GetNum() {return num;}
    inline G4int GetNum2() {return num2;}

  private:
    G4int GammasCollectionIndex;
    G4int PhotonsCollectionIndex;
    G4int num,num2;
};
