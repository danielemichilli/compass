#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"

class GammaHit : public G4VHit
{
  public:
    GammaHit();
    virtual ~GammaHit();
  
    GammaHit(const GammaHit &right);
    const GammaHit& operator=(const GammaHit &right);
    int operator==(const GammaHit &right) const;
    inline void * operator new(size_t);
    inline void operator delete(void *aHit);
//    void Draw() const;
//    void Print() const;
  
  private:
    G4int PID;
    G4int TID;
    G4int chamberNb;
    G4double edep;
    G4ThreeVector pos;
    G4String proc;
    G4String material;

  public:
    inline void SetChamberNb(G4int chamb)      {chamberNb = chamb;}
    inline G4int GetChamberNb()  {return chamberNb;}

    inline void SetEdep(G4double de) {edep = de;}
    inline G4double GetEdep() const {return edep;}

    inline void SetPos(G4ThreeVector xyz) {pos = xyz;}
    inline G4ThreeVector GetPos() const {return pos;}

    inline void SetProcess(G4String aProcess) {proc = aProcess;}
    inline G4String GetProcess() const {return proc;}

    inline void SetTrackID(G4int TrackNb) {TID = TrackNb;}
    inline G4int GetTrackID() const {return TID;}

    inline void SetMaterial(G4String mat) {material = mat;}
    inline G4String GetMaterial() const {return material;}
};

typedef G4THitsCollection<GammaHit> GammaHitsCollection;

extern G4Allocator<GammaHit> GammaHitAllocator;

inline void* GammaHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) GammaHitAllocator.MallocSingle();
  return aHit;
}

inline void GammaHit::operator delete(void *aHit)
{
  GammaHitAllocator.FreeSingle((GammaHit*) aHit);
}

