#include "globals.hh"
#include "G4VPVParameterisation.hh"
#include "G4SystemOfUnits.hh"

class G4VPhysicalVolume;

class PMTParameterisation : public G4VPVParameterisation
{ 
  public:
    PMTParameterisation(G4int baseNb,G4double baseDim);
    virtual ~PMTParameterisation();

  public:
    void ComputeTransformation (const G4int copyNo,
                                G4VPhysicalVolume* physVol) const;
  
  private:
    G4int       fbaseNb;
    G4double    fbaseDim;
};
