#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"

class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4VPVParameterisation;
class G4Material;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction(G4double,G4int);
    virtual ~DetectorConstruction();

  public:
    G4VPhysicalVolume* Construct();
    inline G4int GetSideNb() {return SideNb;}
    inline G4double GetHeightOfBacc() {return HeightOfBacc*2.;}
    G4String GetEavyMaterial();
    inline G4double GetAnodeSize() {return AnodeSize*2.;}
    inline G4double GetPixelSize() {return PixelSize*2.;}
    inline G4double GetLabSize() {return LabSize;}

  private:
    G4int    SideNb;
    G4double HeightOfBacc,AnodeSize,PixelSize,LabSize;
    G4Material *softMaterial,*eavyMaterial;
    
};
