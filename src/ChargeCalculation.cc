#include "ChargeCalculation.hh"
#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "Randomize.hh"
#include <math.h>

ChargeCalculation::ChargeCalculation(){}
ChargeCalculation::~ChargeCalculation(){}

G4double ChargeCalculation::Calculate(G4double B[], G4int n)
{
  static const G4double pC = 1.e-12*coulomb;
  G4int gain = 1.5e6;
  G4double single_electron = eplus;
  G4double sigma_val = 0.65;

  const G4int QE_dim = 5;
  G4double QE_ene[QE_dim] = {369.9*nm,400.0*nm,450.0*nm,500.0*nm,510.1*nm};
  G4double QE[QE_dim]     = {0.28    ,0.26    ,0.20    ,0.15    ,0.10    };
  
  G4double mu = 0.;

  for(int j=0;j<n;j++) for(int i=0;i<QE_dim-1;i++) if( B[j]/nm>=QE_ene[i]/nm and B[j]/nm<QE_ene[i+1]/nm ) mu += ( (QE[i]+QE[i+1])/2. );  //calcola il valore aspettato della poissoniana (mu)

  G4double n_electron = CLHEP::RandPoisson::shoot(mu);

  G4double mean  = gain*single_electron/pC;
  G4double sigma = mean*sigma_val;

  G4double charge = fabs( CLHEP::RandGauss::shoot(mean*n_electron,sigma*sqrt(n_electron)) );

  return charge;
}

G4double ChargeCalculation::Theta(G4int A, G4int B)    //A: bacchetta in cui è avvenuto lo scattering - B: bacchetta in cui è avvenuto l'assorbimento
{
  G4RunManager* runManager = G4RunManager::GetRunManager();
  DetectorConstruction* detector_construction = (DetectorConstruction*) runManager->GetUserDetectorConstruction();
  G4int n = detector_construction->GetSideNb();

  G4double theta = atan2(G4double((B%n-A%n)),(B/n-A/n))*180./M_PI;
  return theta;  
}

G4double ChargeCalculation::Theta2(G4int A, G4int B)    //A: bacchetta in cui è avvenuto lo scattering - B: bacchetta in cui è avvenuto l'assorbimento
{
  G4RunManager* runManager = G4RunManager::GetRunManager();
  DetectorConstruction* detector_construction = (DetectorConstruction*) runManager->GetUserDetectorConstruction();
  G4int n = detector_construction->GetSideNb();

  G4double theta = atan(G4double((B%n-A%n))/(B/n-A/n))*180./M_PI;
  return theta;  
}
