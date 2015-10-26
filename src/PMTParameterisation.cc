#include "PMTParameterisation.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4Box.hh"
#include "G4Material.hh"

PMTParameterisation::PMTParameterisation(  
 G4int      baseNb, 
 G4double   baseDim)
{
  fbaseNb  = baseNb; 
  fbaseDim = baseDim;
}

PMTParameterisation::~PMTParameterisation(){}

void PMTParameterisation::ComputeTransformation
 (const G4int copyNo, G4VPhysicalVolume* physVol) const
{
  G4int row = copyNo / fbaseNb;
  G4int col = copyNo % fbaseNb;
  G4double Xpos = fbaseDim*(row-fbaseNb/2.+0.5);
  G4double Ypos = fbaseDim*(col-fbaseNb/2.+0.5);
  G4ThreeVector origin(Xpos,Ypos,1.*cm-1.5/2*mm);
  physVol->SetTranslation(origin);
}

