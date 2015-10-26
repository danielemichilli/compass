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
  eavyMaterial = BGO    ;   //eavy scintillator material                        //***** || Elemet to edit ||

  AnodeSize     /= 2.;
  PixelSize     /= 2.;
  GlassHeight   /= 2.;
  HeightOfBacc  /= 2.;

    //laboratory
  G4Box*                labSol = new G4Box("mylabSolid",10.*cm,10.*cm,10.*cm);
  G4LogicalVolume*      labLog = new G4LogicalVolume(labSol,Air,"mylabLog",0,0,0);
  G4VPhysicalVolume*    labPhy = new G4PVPlacement(0,G4ThreeVector(),labLog,"Laboratorio",0,false,0);

    //detector area
  G4Box*                scatSol = new G4Box("Scatter",PixelSize*SideNb,PixelSize*SideNb,HeightOfBacc+0.1*mm);
  G4LogicalVolume*      scatLog = new G4LogicalVolume(scatSol,Air,"myScatLog",0,0,0);
  G4VPhysicalVolume*    scatPhy = new G4PVPlacement(0,G4ThreeVector(0,0,0.1*mm),scatLog,"VM2000",labLog,false,0);

    //scintillators
  G4Box*                baccSol = new G4Box("aBaccSolid",AnodeSize,AnodeSize,HeightOfBacc);
  G4LogicalVolume*      baccLog = new G4LogicalVolume(baccSol,softMaterial,"aBaccLog",0,0,0);
  BaccParameterisation* baccPar = new BaccParameterisation(SideNb,PixelSize*2.,softMaterial,eavyMaterial);
  G4VPhysicalVolume*    baccPhy = new G4PVParameterised("bacchette",baccLog,scatLog,kXAxis,SideNb*SideNb,baccPar);

    //PhotoMultiplier Tube
  G4Box*                baseSol = new G4Box("PMT_Sol",PixelSize*SideNb,PixelSize*SideNb,1.0*cm);
  G4LogicalVolume*      baseLog = new G4LogicalVolume(baseSol,Pb,"PMT_Log",0,0,0);
                                  new G4PVPlacement(0,G4ThreeVector(0.,0.,-HeightOfBacc-1.*cm),baseLog,"PMT",labLog,false,0);

    //Glass
  G4Box*                GlassSol = new G4Box("Glass",AnodeSize,AnodeSize,GlassHeight);
  G4LogicalVolume*      GlassLog = new G4LogicalVolume(GlassSol,Glass,"Glass",0,0,0);
  PMTParameterisation*  GlassPar = new PMTParameterisation(SideNb,PixelSize*2.);
  G4VPhysicalVolume*    GlassPhy = new G4PVParameterised("Glass",GlassLog,baseLog,kXAxis,SideNb*SideNb,GlassPar);

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
    //BC404
  G4MaterialPropertiesTable* BC404_Scint = new G4MaterialPropertiesTable();
  const G4int BC404_NUM = 15;
  G4double BC404_PP[BC404_NUM]   = {370.0*nm,380.0*nm,387.0*nm,393.0*nm,395.0*nm,398.0*nm,402.0*nm,413.0*nm,418.0*nm,435.0*nm,450.0*nm,460.0*nm,480.0*nm,500.0*nm,510*nm  };
  G4double BC404_FAST[BC404_NUM] = {0.0     ,0.05    ,0.20    ,0.40    ,0.60    ,0.80    ,1.00    ,0.80    ,0.60    ,0.40,    0.20    ,0.12    ,0.06    ,0.01    ,0.0     };
  G4double BC404_refr[BC404_NUM] = {1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    ,1.58    };
  G4double BC404_abs[BC404_NUM]  = {140.0*cm,140.0*cm,140.0*cm,140.0*cm,140.0*cm,140.0*cm,140.0*cm,140.0*cm,140.0*cm,140.0*cm,140.0*cm,140.0*cm,140.0*cm,140.0*cm,140.0*cm};
  BC404_Scint->AddProperty("FASTCOMPONENT",BC404_PP,BC404_FAST,BC404_NUM);
  BC404_Scint->AddConstProperty("SCINTILLATIONYIELD",11832./MeV);
  BC404_Scint->AddConstProperty("RESOLUTIONSCALE",1.0);
  BC404_Scint->AddConstProperty("FASTTIMECONSTANT",1.8*ns);
  BC404_Scint->AddProperty("RINDEX",BC404_PP,BC404_refr,BC404_NUM);
  BC404_Scint->AddProperty("ABSLENGTH",BC404_PP,BC404_abs,BC404_NUM);
  BC404->SetMaterialPropertiesTable(BC404_Scint);

    //VM2000
  G4OpticalSurface* VM2000 = new G4OpticalSurface("VM2000");
  new G4LogicalBorderSurface("VM2000",baccPhy,scatPhy,VM2000);
  VM2000->SetType(dielectric_metal);
  VM2000->SetModel(unified);
  VM2000->SetFinish(ground);
  const G4int VM2000_NUM = 2;
  G4double VM2000_PP[VM2000_NUM]      = {100*nm,1000.*nm};
  G4double VM2000_reflect[VM2000_NUM] = {.97,.97};  //coefficiente di riflessione
  G4double specularspike[VM2000_NUM]  = {.98,.98};  //frazione riflessa specularmente
  G4double backscatter[VM2000_NUM]    = {0.,0.};
  G4double specularlobe[VM2000_NUM]   = {0.,0.};
  G4double VM2000_efficie[VM2000_NUM] = {0.,0.};
  G4MaterialPropertiesTable* VM2000_border = new G4MaterialPropertiesTable();
  VM2000_border->AddProperty("REFLECTIVITY",VM2000_PP,VM2000_reflect,VM2000_NUM);
  VM2000_border->AddProperty("SPECULARSPIKECONSTANT",VM2000_PP,specularspike,VM2000_NUM);
  VM2000_border->AddProperty("BACKSCATTERCONSTANT",VM2000_PP,backscatter,VM2000_NUM);
  VM2000_border->AddProperty("SPECULARLOBECONSTANT",VM2000_PP,specularlobe,VM2000_NUM);
  VM2000_border->AddProperty("EFFICIENCY",VM2000_PP,VM2000_efficie,VM2000_NUM);
  VM2000->SetMaterialPropertiesTable(VM2000_border);

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

