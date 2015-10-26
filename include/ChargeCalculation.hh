#include "globals.hh"
#include "G4SystemOfUnits.hh"

class ChargeCalculation
{ 
  public:
    ChargeCalculation();
    virtual ~ChargeCalculation();

  public:
    G4double Calculate(G4double B[],G4int n);
    G4double Theta(G4int A, G4int B);
    G4double Theta2(G4int A, G4int B);
};
