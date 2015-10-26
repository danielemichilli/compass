#include "OpPhotHit.hh"


OpPhotHit::OpPhotHit(){}

OpPhotHit::~OpPhotHit() {}

OpPhotHit::OpPhotHit(const OpPhotHit& right)
  : G4VHit()
{
  chamberNb = right.chamberNb;
  edep      = right.edep;
  TID       = right.TID;
  PID       = right.PID;
  material  = right.material;
}

const OpPhotHit& OpPhotHit::operator=(const OpPhotHit& right)
{
  chamberNb = right.chamberNb;
  edep      = right.edep;
  TID       = right.TID;
  PID       = right.PID;
  material  = right.material;
  return *this;
}

G4int OpPhotHit::operator==(const OpPhotHit& right) const
{
  return (this==&right) ? 1 : 0;
}
