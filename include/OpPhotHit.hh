#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"

class OpPhotHit : public G4VHit
{
  public:
    OpPhotHit();
    virtual ~OpPhotHit();
  
    OpPhotHit(const OpPhotHit &right);
    const OpPhotHit& operator=(const OpPhotHit &right);
    int operator==(const OpPhotHit &right) const;
    inline void * operator new(size_t);
    inline void operator delete(void *aHit);
//    void Draw() const;
//    void Print() const;
  
  private:
    G4int PID;
    G4int TID;
    G4int chamberNb;
    G4double edep;
    G4String material;

  public:
    inline void SetChamberNb(G4int chamb)      {chamberNb = chamb;}
    inline G4int GetChamberNb()  {return chamberNb;}

    inline void SetEdep(G4double de) {edep = de;}
    inline G4double GetEdep() const {return edep;}

    inline void SetTrackID(G4int TrackNb) {TID = TrackNb;}
    inline G4int GetTrackID() const {return TID;}

    inline void SetParentID(G4int ParentID) {PID = ParentID;}
    inline G4int GetParentID() const {return PID;}

    inline void SetMaterial(G4String mat) {material = mat;}
    inline G4String GetMaterial() const {return material;}
};

typedef G4THitsCollection<OpPhotHit> OpPhotHitsCollection;

extern G4Allocator<OpPhotHit> OpPhotHitAllocator;

inline void* OpPhotHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) OpPhotHitAllocator.MallocSingle();
  return aHit;
}

inline void OpPhotHit::operator delete(void *aHit)
{
  OpPhotHitAllocator.FreeSingle((OpPhotHit*) aHit);
}

