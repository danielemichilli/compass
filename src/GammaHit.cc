#include "GammaHit.hh"


GammaHit::GammaHit(){}

GammaHit::~GammaHit() {}

GammaHit::GammaHit(const GammaHit& right)
  : G4VHit()
{
  chamberNb = right.chamberNb;
  edep      = right.edep;
  pos       = right.pos;
  proc      = right.proc;
  TID       = right.TID;
  material  = right.material;
}

const GammaHit& GammaHit::operator=(const GammaHit& right)
{
  chamberNb = right.chamberNb;
  edep      = right.edep;
  pos       = right.pos;
  proc      = right.proc;
  TID       = right.TID;
  material  = right.material;
  return *this;
}

G4int GammaHit::operator==(const GammaHit& right) const
{
  return (this==&right) ? 1 : 0;
}
