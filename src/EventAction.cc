#include "EventAction.hh"
#include "RunAction.hh"
#include "GammaHit.hh"
#include "OpPhotHit.hh"
#include "ChargeCalculation.hh"
#include "DetectorConstruction.hh"

#include "G4Event.hh"
#include "G4Run.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4ios.hh"
#include "G4SDManager.hh"
#include "G4VProcess.hh"

#include "G4UnitsTable.hh"
#include <iostream>
#include <fstream>
#include <vector>

EventAction::EventAction():
  GammasCollectionIndex(-1),PhotonsCollectionIndex(-1),num(0),num2(0)
{}
EventAction::~EventAction(){}
 
void EventAction::BeginOfEventAction(const G4Event* event)
{
  if(PhotonsCollectionIndex < 0) PhotonsCollectionIndex = G4SDManager::GetSDMpointer() -> GetCollectionID("PhotonsCollection");
  if(GammasCollectionIndex < 0) GammasCollectionIndex = G4SDManager::GetSDMpointer() -> GetCollectionID("GammasCollection");
//CLHEP::HepRandom::showEngineStatus();
}


void EventAction::EndOfEventAction(const G4Event* event)
{
  static const G4double pC = 1.e-12*coulomb;
  if(PhotonsCollectionIndex<0) return;
//  if(GammasCollectionIndex<0) return;

  G4RunManager* runManager = G4RunManager::GetRunManager();
  DetectorConstruction* detector_construction = (DetectorConstruction*) runManager->GetUserDetectorConstruction();
  G4int n = detector_construction->GetSideNb();

  G4HCofThisEvent* HCE = event -> GetHCofThisEvent();
//  std::ofstream scorefile_phot;
//  std::ofstream scorefile_gamma;

  G4int evtNb = event -> GetEventID();
  G4int chamb_arr[n*n];
  for(int i=0;i<n*n;i++) chamb_arr[i]=0;
  G4int flag = 0;
  G4int flagEavy = 0;

//  scorefile_gamma.open("scorefile_gamma.dat",std::fstream::app);
  GammaHitsCollection* hitsCollectionGamma = 0;
  if(HCE) hitsCollectionGamma = (GammaHitsCollection*)(HCE -> GetHC(GammasCollectionIndex));
  if(hitsCollectionGamma) {
    int numberHitsGamma = hitsCollectionGamma -> entries();
    if (numberHitsGamma != 0) {
      for(int i = 0; i < numberHitsGamma ; i++) {
        GammaHit* hitGamma     = (*hitsCollectionGamma)[i];
        if(hitGamma->GetMaterial()==detector_construction->GetEavyMaterial()) {
          chamb_arr[hitGamma->GetChamberNb()]=-1;
          flagEavy=1;
        }

//        G4int chamberGamma     = hitGamma -> GetChamberNb();
//        G4double edepGamma     = hitGamma -> GetEdep();
//        G4ThreeVector posGamma = hitGamma -> GetPos();
//        G4String procGamma     = hitGamma -> GetProcess();
//        G4int trackGamma       = hitGamma -> GetTrackID();
//
//        scorefile_gamma << "Evento n. "                  << std::setw(2)  << evtNb 
//                        << ","                           << std::setw(2)  << trackGamma
//                        << " - Base n. "                 << std::setw(2)  << chamberGamma
//                        << " - Energia depositata(keV):" << std::setw(10) << edepGamma/keV 
//                        << " - Posizione interazione:"   << std::setw(10) << posGamma
//                        << " - Processo: "               << std::setw(10) << procGamma
//                        << G4endl;

      }
    }
  } 
//  scorefile_gamma.close();

  std::ofstream scorefile_theta;
  scorefile_theta.open("scorefile_theta.dat",std::fstream::app);
  std::ofstream scorefile_charge;
  scorefile_charge.open("scorefile_charge.dat",std::fstream::app);
        
  G4double op_charge[n*n];
  for(int i=0;i<n*n;i++) op_charge[i]=0;
//  scorefile_phot.open("scorefile_photon.dat",std::fstream::app);
  OpPhotHitsCollection* hitsCollectionOpPhot = 0;
  if(HCE) hitsCollectionOpPhot = (OpPhotHitsCollection*)(HCE -> GetHC(PhotonsCollectionIndex));
  if(hitsCollectionOpPhot) {
    int numberHitsOpPhot = hitsCollectionOpPhot -> entries();
    if(numberHitsOpPhot != 0) {
      for(int j=0;j<numberHitsOpPhot;j++) {
        OpPhotHit* hitOpPhot = (*hitsCollectionOpPhot)[j];
        G4int chamberOpPhot  = hitOpPhot->GetChamberNb();

//        G4int trackOpPhot       = hitOpPhot -> GetTrackID();
//        G4int parentOpPhot      = hitOpPhot -> GetParentID();

        chamb_arr[chamberOpPhot]+=1;


//        scorefile_phot << "Evento n. "                  << std::setw(2)  << evtNb 
//                       << ","                           << std::setw(2)  << parentOpPhot
//                       << ","                           << std::setw(2)  << trackOpPhot
//                       << " - Base n. "                 << std::setw(2)  << chamberOpPhot
//                       << " - Energia depositata(nm): " << std::setw(10) << edepOpPhot/nm
//                       << G4endl;

      }
      ChargeCalculation* charge = new ChargeCalculation();


      for(int i=0;i<n*n;i++) {
        if(chamb_arr[i]>0) {
          G4double op_edep[chamb_arr[i]];
          G4int k=0;
          for(int j=0;j<numberHitsOpPhot;j++) {
            OpPhotHit* hitOpPhot = (*hitsCollectionOpPhot)[j];
            G4double ene = hitOpPhot->GetEdep();
            if(hitOpPhot->GetChamberNb()==i) {
              op_edep[k] = ene;
              k++;
            }
          }
          op_charge[i] = charge -> Calculate(op_edep,chamb_arr[i]);
        }
      }


      for(int i=0;i<n*n;i++) if(op_charge[i]>0.) flag+=1;

//  if(not(abs(chambs[0]%n-chambs[1]%n)>2 and abs(chambs[0]/n-chambs[1]/n)>2)) flag=0;      //se le bacchette sono troppo vicine non calcola

      if(flag==1 and flagEavy==1) {
        G4int scat=-1,abs=-1;
        for(int i=0;i<n*n;i++) {
          if(op_charge[i]>0.) scat = i;
          else if(chamb_arr[i]<0.) abs = i;
        }
        if (n > 1) {
          if(scat==-1 or abs==-1) {
            G4cout<<"Errore in Event Action 1"<<G4endl;
            return;
          }
          G4double theta = charge->Theta(scat,abs);
          scorefile_theta << theta << G4endl;
        }
        scorefile_charge << op_charge[scat] << G4endl;
        num += 1;
      }
/*
      else if(flag==2) {
        std::ofstream scorefile_theta2;
        scorefile_theta2.open("scorefile_theta2.dat",std::fstream::app);
        std::ofstream scorefile_charge2;
        scorefile_charge2.open("scorefile_charge2.dat",std::fstream::app);
        G4int scat[2];
        G4int j=0;
        for(int i=0;i<n*n;i++) {
          if(op_charge[i]>0.) {
            scat[j]=i;
            if(j>1) {
              G4cout<<"Errore in Event Action 2"<<G4endl;
              return;
            }
            j++;
          }
        }
        G4double theta = charge->Theta2(scat[0],scat[1]);
        scorefile_theta2 << theta << G4endl;
        scorefile_charge2 << op_charge[scat[0]] << G4endl
                                                << op_charge[scat[1]] << G4endl;
        num2+=1;
        scorefile_theta2.close();
        scorefile_charge2.close();
      }
*/      

      scorefile_theta.close();
      scorefile_charge.close();
      delete charge;
    }
  }
  

//  scorefile_phot.close();


  evtNb+=1;
  if(evtNb%100==0) {
    G4int eventTOT = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();
    G4cout << "  [" << G4int(G4double(evtNb)/eventTOT*100) << "%]" << G4endl;
  }


}
