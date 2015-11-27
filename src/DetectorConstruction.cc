#include "DetectorConstruction.hh"
#include "ScintillatorSD.hh"
#include "PMTSD.hh"
#include "BaccParameterisation.hh"
#include "PMTParameterisation.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4PVParameterised.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"

 
DetectorConstruction::DetectorConstruction(G4double h,G4int n)
{
  HeightOfBacc = h;   //scintillators height
  SideNb       = n;   //number of scintillator on a base side
  AnodeSize    = 5.8 *mm;   //single scintillator side lenght         //ATTENZIONE: area anodi esterni più grande
  PixelSize    = 6.08*mm;
  LabSize      = 10.*cm;
}                                                                              
DetectorConstruction::~DetectorConstruction(){}                  

G4VPhysicalVolume* DetectorConstruction::Construct()                            //detector construction 
{
  // ***** materials definition ***** // 

  G4NistManager* man = G4NistManager::Instance();

    //air
  G4Material* Air = man->FindOrBuildMaterial("G4_AIR");
  
    //BC404
  G4Element* H  = man->FindOrBuildElement(1);
  G4Element* C  = man->FindOrBuildElement(6);
  G4Material* BC404 = new G4Material("BC404",1.032*g/cm3,2);
  BC404->AddElement(H,11); 
  BC404->AddElement(C,10);

      //EJ200
  G4Material* EJ200 = new G4Material("EJ200",1.032*g/cm3,2);
  EJ200->AddElement(H,11); 
  EJ200->AddElement(C,10);
  
    //BGO
  G4Material* BGO = man->FindOrBuildMaterial("G4_BGO");

    //lead
  G4Material* Pb = man->FindOrBuildMaterial("G4_Pb");

    //Glass
  G4Material* Glass = man->FindOrBuildMaterial("G4_Pyrex_Glass");


  // ***** volumes definition ***** // 

    //dimensions
  G4double    GlassHeight  = 1.5 *mm;   //PMT Glass height
  softMaterial = BC404  ;   //soft scintillator material                        //***** || Elemet to edit ||
  eavyMaterial = EJ200  ;   //eavy scintillator material                        //***** || Elemet to edit ||  // Also used for single stick

  AnodeSize     /= 2.;
  PixelSize     /= 2.;
  GlassHeight   /= 2.;
  HeightOfBacc  /= 2.;

    //laboratory
  G4Box*                labSol = new G4Box("mylabSolid",LabSize,LabSize,LabSize);
  G4LogicalVolume*      labLog = new G4LogicalVolume(labSol,Air,"mylabLog",0,0,0);
  G4VPhysicalVolume*    labPhy = new G4PVPlacement(0,G4ThreeVector(),labLog,"Laboratorio",0,false,0);

    //detector area
  G4Box*                scatSol = new G4Box("Scatter",PixelSize*SideNb,PixelSize*SideNb,HeightOfBacc+0.1*mm);
  G4LogicalVolume*      scatLog = new G4LogicalVolume(scatSol,Air,"myScatLog",0,0,0);
  G4VPhysicalVolume*    scatPhy = new G4PVPlacement(0,G4ThreeVector(0,0,0.1*mm),scatLog,"Teflon",labLog,false,0);

    //scintillators
  G4Box*                baccSol = new G4Box("aBaccSolid",AnodeSize,AnodeSize,HeightOfBacc);
  G4LogicalVolume*      baccLog = new G4LogicalVolume(baccSol,softMaterial,"aBaccLog",0,0,0);
  
  if (SideNb == 1) {
    G4VPhysicalVolume*    baccPhy = new G4PVPlacement(0,G4ThreeVector(0.,0.,-0.1*mm),baccLog,"Bar",scatLog,false,0);
  } else {
    BaccParameterisation* baccPar = new BaccParameterisation(SideNb,PixelSize*2.,softMaterial,eavyMaterial);
    G4VPhysicalVolume*    baccPhy = new G4PVParameterised("bacchette",baccLog,scatLog,kXAxis,SideNb*SideNb,baccPar);
  }
  
    //PhotoMultiplier Tube
  G4Box*                baseSol = new G4Box("PMT_Sol",PixelSize*SideNb,PixelSize*SideNb,1.0*cm);
  G4LogicalVolume*      baseLog = new G4LogicalVolume(baseSol,Pb,"PMT_Log",0,0,0);
                                  new G4PVPlacement(0,G4ThreeVector(0.,0.,-HeightOfBacc-1.*cm),baseLog,"PMT",labLog,false,0);

    //Glass
  G4Box*                GlassSol = new G4Box("Glass",AnodeSize,AnodeSize,GlassHeight);
  G4LogicalVolume*      GlassLog = new G4LogicalVolume(GlassSol,Glass,"Glass",0,0,0);
  if (SideNb == 1) {
    G4VPhysicalVolume*    GlassPhy = new G4PVPlacement(0,G4ThreeVector(0.,0.,9.25*mm),GlassLog,"Glass",baseLog,false,0);
  } else {
    PMTParameterisation*  GlassPar = new PMTParameterisation(SideNb,PixelSize*2.);
    G4VPhysicalVolume*    GlassPhy = new G4PVParameterised("Glass",GlassLog,baseLog,kXAxis,SideNb*SideNb,GlassPar);
  }
  
  //sensitive detector 
    //bacchette
  G4SDManager*    SDman     = G4SDManager::GetSDMpointer();
  ScintillatorSD* TrackerSD = new ScintillatorSD("Scintillator");
  SDman   -> AddNewDetector(TrackerSD);
  baccLog -> SetSensitiveDetector(TrackerSD);
    //glass
  PMTSD* glassSD = new PMTSD("PMT");
  SDman   -> AddNewDetector(glassSD);
  GlassLog -> SetSensitiveDetector(glassSD);

  
  // ***** optical proprieties ***** // 
    //EJ200 or BC408
  G4MaterialPropertiesTable* EJ200_Scint = new G4MaterialPropertiesTable();
  const G4int EJ200_NUM = 16;
  G4double EJ200_PP[EJ200_NUM]   = {398.0*nm,400.0*nm,405.0*nm,409.0*nm,412.0*nm,420.0*nm,425.0*nm,427.0*nm,437.0*nm,446.0*nm,460.0*nm,462.0*nm,478.0*nm,490.0*nm,500.0*nm  ,600.0*nm};
  G4double EJ200_FAST[EJ200_NUM] = {0.0     ,0.02    ,0.05    ,0.20    ,0.40    ,0.87    ,1.00    ,0.99    ,0.80    ,0.60,    0.42    ,0.40    ,0.20    ,0.10    ,0.06      ,0.0     };
  G4double EJ200_refr[EJ200_NUM] = {1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58      ,1.58    };
  G4double EJ200_abs[EJ200_NUM]  = {380.0*cm,380.0*cm,380.0*cm,380.0*cm,380.0*cm,380.0*cm,380.0*cm,380.0*cm,380.0*cm,380.0*cm,380.0*cm,380.0*cm,380.0*cm,380.0*cm,380.0*cm  ,380*cm  };
  EJ200_Scint->AddProperty("FASTCOMPONENT",EJ200_PP,EJ200_FAST,EJ200_NUM);
  EJ200_Scint->AddConstProperty("SCINTILLATIONYIELD",10000./MeV);
  EJ200_Scint->AddConstProperty("RESOLUTIONSCALE",1.0);
  EJ200_Scint->AddConstProperty("FASTTIMECONSTANT",2.1*ns);
  EJ200_Scint->AddProperty("RINDEX",EJ200_PP,EJ200_refr,EJ200_NUM);
  EJ200_Scint->AddProperty("ABSLENGTH",EJ200_PP,EJ200_abs,EJ200_NUM);
  EJ200->SetMaterialPropertiesTable(EJ200_Scint);  
  
    //BC404 or EJ204
  G4MaterialPropertiesTable* BC404_Scint = new G4MaterialPropertiesTable();
  const G4int BC404_NUM = 15;
  G4double BC404_PP[BC404_NUM]   = {370.0*nm,380.0*nm,387.0*nm,393.0*nm,395.0*nm,398.0*nm,402.0*nm,413.0*nm,418.0*nm,435.0*nm,450.0*nm,460.0*nm,480.0*nm,500.0*nm,510*nm  };
  G4double BC404_FAST[BC404_NUM] = {0.0     ,0.05    ,0.20    ,0.40    ,0.60    ,0.80    ,1.00    ,0.80    ,0.60    ,0.40,    0.20    ,0.12    ,0.06    ,0.01    ,0.0     };
  G4double BC404_refr[BC404_NUM] = {1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    };
  G4double BC404_abs[BC404_NUM]  = {160.0*cm,160.0*cm,160.0*cm,160.0*cm,160.0*cm,160.0*cm,160.0*cm,160.0*cm,160.0*cm,160.0*cm,160.0*cm,160.0*cm,160.0*cm,160.0*cm,160.0*cm};
  BC404_Scint->AddProperty("FASTCOMPONENT",BC404_PP,BC404_FAST,BC404_NUM);
  BC404_Scint->AddConstProperty("SCINTILLATIONYIELD",11832./MeV);
  BC404_Scint->AddConstProperty("RESOLUTIONSCALE",1.0);
  BC404_Scint->AddConstProperty("FASTTIMECONSTANT",1.8*ns);
  BC404_Scint->AddProperty("RINDEX",BC404_PP,BC404_refr,BC404_NUM);
  BC404_Scint->AddProperty("ABSLENGTH",BC404_PP,BC404_abs,BC404_NUM);
  BC404->SetMaterialPropertiesTable(BC404_Scint);

    //VM2000
  G4OpticalSurface* VM2000 = new G4OpticalSurface("VM2000");
  VM2000->SetType(dielectric_LUT);
  VM2000->SetModel(LUT);
  VM2000->SetFinish(polishedvm2000air);
  
    //Teflon
  G4OpticalSurface* Teflon = new G4OpticalSurface("Teflon");
  Teflon->SetType(dielectric_LUT);
  Teflon->SetModel(LUT);
  Teflon->SetFinish(polishedteflonair);

    //Glass
  G4MaterialPropertiesTable* Glass_op = new G4MaterialPropertiesTable();
  const G4int Glass_NUM = 2;
  G4double Glass_PP[Glass_NUM]   = {100.0*nm, 1000.0*nm};
  G4double Glass_refr[Glass_NUM] = {1.52,1.52};
  Glass_op->AddProperty("RINDEX",Glass_PP,Glass_refr,Glass_NUM);
  Glass->SetMaterialPropertiesTable(Glass_op);

  return labPhy;
}

G4String DetectorConstruction::GetEavyMaterial()
{
  G4String eavy = eavyMaterial->GetName();
  return eavy;
}

