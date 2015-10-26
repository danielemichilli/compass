#include "G4VUserPrimaryGeneratorAction.hh"

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"


class G4ParticleGun;
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();
    virtual ~PrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event* anEvent);
    inline G4ParticleGun* GetParticleGun() {return particleGun;}

  private:
    G4ParticleGun* particleGun;
};
