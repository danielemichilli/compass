#include "BaccParameterisation.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4Box.hh"
#include "G4Material.hh"

BaccParameterisation::BaccParameterisation(  
 G4int      baseNb, 
 G4double   baseDim,
 G4Material *soft,
 G4Material *eavy)
{
  fbaseNb  = baseNb; 
  fbaseDim = baseDim;
  fsoft    = soft;
  feavy    = eavy; 
}

BaccParameterisation::~BaccParameterisation(){}

void BaccParameterisation::ComputeTransformation
 (const G4int copyNo, G4VPhysicalVolume* physVol) const
{
  G4int row = copyNo / fbaseNb;
  G4int col = copyNo % fbaseNb;
  G4double Xpos = fbaseDim*(row-fbaseNb/2.+0.5);
  G4double Ypos = fbaseDim*(col-fbaseNb/2.+0.5);
  G4ThreeVector origin(Xpos,Ypos,-0.1*mm);
  physVol->SetTranslation(origin);
  physVol->SetRotation(0);
}


G4Material* BaccParameterisation::
 ComputeMaterial(const G4int copyNo,
                 G4VPhysicalVolume* physVol,const G4VTouchable *Touch) 
{
  G4int row = copyNo / fbaseNb;
  G4int col = copyNo % fbaseNb;
  G4Material *material;
  G4LogicalVolume* logVol= physVol->GetLogicalVolume();

  if( row > 0 and row < fbaseNb-1 )
  {
    if( col > 0 and col < fbaseNb-1)
      material = fsoft;
    else
      material = feavy;
  }
  else  material = feavy;

  logVol->SetMaterial(material);

  return material;
}


