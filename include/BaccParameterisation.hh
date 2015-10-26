#include "globals.hh"
#include "G4VPVParameterisation.hh"
#include "G4SystemOfUnits.hh"

class G4VPhysicalVolume;

class BaccParameterisation : public G4VPVParameterisation
{ 
  public:
    BaccParameterisation(G4int baseNb,G4double baseDim,
                         G4Material* soft,G4Material* eavy);
    virtual ~BaccParameterisation();

  public:
    void ComputeTransformation (const G4int copyNo,
                                G4VPhysicalVolume* physVol) const;
    virtual G4Material* 
            ComputeMaterial(const G4int copyNo,G4VPhysicalVolume *physVol,
                            const G4VTouchable *Touch=0);
  
  private:
    G4Material *fsoft,*feavy;
    G4int       fbaseNb;
    G4double    fbaseDim;
};
