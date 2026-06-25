/*
  FiberTargetDetectorConstruction.cc
  Oct 2015, T.Gogami
*/

#include "FiberTargetDetectorConstruction.hh"
#include "FiberTargetvdHit.hh"
#include "FiberTargetvdSD.hh"
#include "TargetHit.hh"
#include "TargetSD.hh"
#include "FiberTargetParamManager.hh"

//=== Material Define ===//
#include "MaterialList.hh"

//=== Geometry Define ===//
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4DisplacedSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4UserLimits.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4AffineTransform.hh"
#include "G4SDManager.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4EllipticalTube.hh"

//=== ElectoMagnetic Field ===//
//#include "FiberTargetField.hh"

#include <sstream>
#include <iomanip>
#include <string>

//#include "FiberTargetParamMan.hh"
//#include "FiberTargetPrimaryGeneratorAction.hh"

//=== Program Start ===//
///////////////////////////////////////////////////
FiberTargetDetectorConstruction::FiberTargetDetectorConstruction()
  : mList_(0)
///////////////////////////////////////////////////
{
}
FiberTargetDetectorConstruction::FiberTargetDetectorConstruction(FiberTargetParamManager*PManDragon)
  : mList_(0),paramMan(PManDragon)
///////////////////////////////////////////////////
{
  TargetMaterial = paramMan->GetTarget();     // Target material
  TThickness     = paramMan->GetTThickness(); // Target thickness [cm]
  //G4cout << paramMan->GetBeamEnergy() << G4endl;
  
  // ---- Check the target information -------------------------------
  /* 
  for(int i=0 ; i<10000 ; i++){
    G4cout << TargetMaterial << " " << TThickness << " cm "  << G4endl;
  }
  */
  // ------------------------------------------------------------------

}

////////////////////////////////////////////////////
FiberTargetDetectorConstruction::~FiberTargetDetectorConstruction()
///////////////////////////////////////////////////
{
  delete mList_;
}

/////////////////////////////////////////////////////////
MaterialList *FiberTargetDetectorConstruction::DefineMaterials()
/////////////////////////////////////////////////////////
{
  MaterialList *ml = new MaterialList;

  return ml;
}

////////////////////////////////////////////////////////
G4VPhysicalVolume *FiberTargetDetectorConstruction::Construct()
///////////////////////////////////////////////////////
{
  mList_ = DefineMaterials();

  G4VPhysicalVolume *world = ConstructPayload();

  return world;
}

/////////////////////////////////////////////////////////
//FiberTargetField *FiberTargetDetectorConstruction::MakeDetectorField()
/////////////////////////////////////////////////////////
//{
//  return new FiberTargetField();
//}

///////////////////////////////////////////////////////////////
G4VPhysicalVolume *FiberTargetDetectorConstruction::ConstructPayload()
//////////////////////////////////////////////////////////////
{ 
  //=== World ===//
  G4double WorldSizeX = 100.0*cm;
  G4double WorldSizeY = 100.0*cm;
  G4double WorldSizeZ = 100.0*cm;
  G4Box *worldSolid = new G4Box( "World",WorldSizeX/2.0,
				 WorldSizeY/2.0,WorldSizeZ/2.0 );
  
  G4LogicalVolume *worldLV =
    new G4LogicalVolume( worldSolid, mList_->Vacuum, "World LV");
    //new G4LogicalVolume( worldSolid, mList_->Air, "World LV");
  
  G4VPhysicalVolume *world = 
    new G4PVPlacement( 0, 
		       G4ThreeVector( 0.*m, 0.*m, 0.*m ),
		       worldLV,
		       "World",
		       0,
		       false, 
		       0);
  //for(int i=0 ; i<100000 ; i++){
  //  G4cout << "Target =====" << TargetMaterial <<G4endl;
  //}
  //=== Target ===//
  //G4double TargetThickness = 500.0;
  //G4double TargetThickness = 100.0;
  //std::string TargetMaterial = "Carbon";
  //std::string TargetMaterial = "Chromium";
  //std::string TargetMaterial = "Lead";
  G4Material *Target =mList_->Vacuum;
  //G4double TargetSizeX = 1.0 * cm ;
  //G4double TargetSizeY = 1.0 * cm ;
  G4double TargetSizeX = 8.0 * cm ;
  G4double TargetSizeY = 8.0 * cm ;
  G4double TargetSizeZ = 0.1 * 0.1 * cm ;
  TargetSizeZ = TThickness * cm ; // Added by Toshi , 18Oct2013
  //for(int i=0 ; i<100000 ; i++){
  //  G4cout << TThickness << G4endl;
  //}
  
  G4RotationMatrix* rotTarget = new G4RotationMatrix();//Rotaion of target
  //rotTarget->rotateY(-17.0 * deg);    //Rotation of the target for HKS-HES
  //rotTarget->rotateY(-54.0 * deg);    //Rotation of the target for KaoS
  rotTarget->rotateY( 0.0 * deg);      // For energy loss study, 18Oct2013
  
  //G4double TargetSizeZ = (TargetThickness)/(TargetDensity/(g/cm3)*1000)*cm;
  //if( TargetMaterial == "Vanadium"){
  //  Target = mList_->V51;
  //}
  //else if( TargetMaterial == "Yttrium"){
  //  Target = mList_->Y89;
  //}
  
  if( TargetMaterial == "Carbon"){
    Target = mList_->C12;
    //TargetSizeZ = TThickness * cm ;
    //TargetSizeZ = 0.50000 * 0.1 * cm ; // 500 [um]
    //TargetSizeZ = 0.522846 * 0.1 * cm ; // 500 [um]
    //TargetSizeZ = 0.517102997 * 0.1 * cm ; // 500 [um]
    //TargetSizeZ = 0.5000001547 * 0.1 * cm ; // 500 [um]
    //TargetSizeZ = 0.44444 * 0.1 * cm ; // 500 [um]// for test 15/Apr/2013
    //TargetSizeZ = 0.50000 * 0.1 * cm ; // 500 [um]
    //TargetSizeZ = 0.3 * 0.1 * cm ; //0.3 [mm]
    //TargetSizeZ = 0.4 * 0.1 * cm ; //0.4 [mm]
    //TargetSizeZ = 0.5 * 0.1 * cm ; //0.5 [mm]
    //TargetSizeZ = 1.0 * 0.1 * cm ; //4.0 [mm]
    //TargetSizeZ = 2.0 * 0.1 * cm ; //2.0 [mm]
    //TargetSizeZ = 3.0 * 0.1 * cm ; //3.0 [mm]
    //TargetSizeZ = 4.0 * 0.1 * cm ; //4.0 [mm]
    //TargetSizeZ = 5.0 * 0.1 * cm ; //5.0 [mm]
    //TargetSizeZ = 8.0 * 0.1 * cm ; //8.0 [mm]
  }
  //else if( TargetMaterial == "Silicon"){
  //  Target = mList_->Si28;
  //}
  //else if( TargetMaterial == "Polyethylene"){
  else if( TargetMaterial == "ch2"){
    Target = mList_->CH2;
    //TargetSizeZ = 5.06516 * 0.1 * cm ; // 5.1 [mm]
    //TargetSizeZ = 5.013113811 * 0.1 * cm ; // 5.1 [mm]
    //TargetSizeZ = 4.8473083553845 * 0.1 * cm ; // 5.1 [mm]
    //TargetSizeZ = 0.3 * 0.1 * cm ; //0.3 [mm]
    //TargetSizeZ = 0.4 * 0.1 * cm ; //0.4 [mm]
    //TargetSizeZ = 0.50000 * 0.1 * cm ; // 500 [um]
    //TargetSizeZ = 1.0 * 0.1 * cm ; //1.0 [mm]
    //TargetSizeZ = 2.0 * 0.1 * cm ; //2.0 [mm]
    //TargetSizeZ = 3.0 * 0.1 * cm ; //3.0 [mm]
    //TargetSizeZ = 4.0 * 0.1 * cm ; //4.0 [mm]
    //TargetSizeZ = 5.0 * 0.1 * cm ; //5.0 [mm]
    //TargetSizeZ = 8.0 * 0.1 * cm ; //8.0 [mm]
  }
  else if( TargetMaterial == "Lithium"){
    Target = mList_->Li7;
    //TargetSizeZ = 0.1370 * 0.1 * cm ; // 137 [um]
    //TargetSizeZ = 0.360313 * 0.1 * cm ; //
    //TargetSizeZ =  3.563552794 * 0.1 * cm ; //
    //TargetSizeZ =  3.44569061 * 0.1 * cm ; //
    //TargetSizeZ = 0.3 * 0.1 * cm ; //0.3 [mm]
    //TargetSizeZ = 0.4 * 0.1 * cm ; //0.4 [mm]
    //TargetSizeZ = 0.5 * 0.1 * cm ; //0.5 [mm]
    //TargetSizeZ = 1.0 * 0.1 * cm ; //1.0 [mm]
    //TargetSizeZ = 2.0 * 0.1 * cm ; //2.0 [mm]
    //TargetSizeZ = 3.0 * 0.1 * cm ; //3.0 [mm]
    //TargetSizeZ = 4.0 * 0.1 * cm ; //4.0 [mm]
    //TargetSizeZ = 5.0 * 0.1 * cm ; //5.0 [mm]
    //TargetSizeZ = 8.0 * 0.1 * cm ; //8.0 [mm]
  }
  else if( TargetMaterial == "Berylium"){
    Target = mList_->Be9;
    //TargetSizeZ = 0.1189 * 0.1 * cm ; // 118.9 [um]
    //TargetSizeZ = 1.06436 * 0.1 * cm ; // 1 [mm]
    //TargetSizeZ = 1.0178526080 * 0.1 * cm ; // 1 [mm]
    //TargetSizeZ = 1.018 * 0.1 * cm ; // 1 [mm]
    //TargetSizeZ = 0.3 * 0.1 * cm ; //0.3 [mm]
    //TargetSizeZ = 0.4 * 0.1 * cm ; //0.4 [mm]
    //TargetSizeZ = 0.5 * 0.1 * cm ; //0.5 [mm]
    //TargetSizeZ = 1.0 * 0.1 * cm ; //1.0 [mm]
    //TargetSizeZ = 2.0 * 0.1 * cm ; //2.0 [mm]
    //TargetSizeZ = 3.0 * 0.1 * cm ; //3.0 [mm]
    //TargetSizeZ = 4.0 * 0.1 * cm ; //4.0 [mm]
    //TargetSizeZ = 5.0 * 0.1 * cm ; //5.0 [mm]
    //TargetSizeZ = 8.0 * 0.1 * cm ; //8.0 [mm]
  }
  else if( TargetMaterial == "Boron"){
    Target = mList_->B10;
    //TargetSizeZ = 0.1875 * 0.1 * cm ; // 187.5 [um]
    //TargetSizeZ = 0.24794430 * 0.1 * cm ; // 187.5 [um]
    //TargetSizeZ = 0.23974370807 * 0.1 * cm ; // 187.5 [um]
    //TargetSizeZ = 0.3 * 0.1 * cm ; //0.3 [mm]
    //TargetSizeZ = 0.4 * 0.1 * cm ; //0.4 [mm]
    //TargetSizeZ = 0.5 * 0.1 * cm ; //0.5 [mm]
    //TargetSizeZ = 1.0 * 0.1 * cm ; //1.0 [mm]
    //TargetSizeZ = 2.0 * 0.1 * cm ; //2.0 [mm]
    //TargetSizeZ = 3.0 * 0.1 * cm ; //3.0 [mm]
    //TargetSizeZ = 4.0 * 0.1 * cm ; //4.0 [mm]
    //TargetSizeZ = 5.0 * 0.1 * cm ; //5.0 [mm]
    //TargetSizeZ = 8.0 * 0.1 * cm ; //8.0 [mm]
  }
  else if( TargetMaterial == "Li3N"){
    Target = mList_->Li3N;
  }
  //else if( TargetMaterial == "Calcium"){
  //  Target = mList_->Ca40;
  //}
  else if( TargetMaterial == "Water"){
    Target = mList_->Water;
    //TargetSizeZ = 5.0 * 0.1 * cm ; // 5 [mm]
    //TargetSizeZ = 0.3 * 0.1 * cm ; //0.3 [mm]
    //TargetSizeZ = 0.4 * 0.1 * cm ; //0.4 [mm]
    //TargetSizeZ = 0.5 * 0.1 * cm ; //0.5 [mm]
    //TargetSizeZ = 1.0 * 0.1 * cm ; //1.0 [mm]
    //TargetSizeZ = 2.0 * 0.1 * cm ; //2.0 [mm]
    //TargetSizeZ = 3.0 * 0.1 * cm ; //3.0 [mm]
    //TargetSizeZ = 4.0 * 0.1 * cm ; //4.0 [mm]
    //TargetSizeZ = 5.0 * 0.1 * cm ; //5.0 [mm]
    //TargetSizeZ = 8.0 * 0.1 * cm ; //8.0 [mm]
    
    //Foil for water target
    //G4Material *Foil =mList_->Vacuum;
    G4Material *Foil =mList_->Harver;
    G4double foilz = 0.025 * 0.1 * cm ; // 25 [um]
    //G4double foilz = 0.25 * 0.1 * cm ; // 25 [um]
    G4Box *foilSolid = new G4Box( "foilSolid_box",
				  TargetSizeX/2+2.0/2.0*mm,TargetSizeY/2,foilz/2 );
    G4RotationMatrix* noRot = new G4RotationMatrix();
    G4DisplacedSolid* targetf1 = new G4DisplacedSolid("targetf1",
						      foilSolid,
						      noRot ,
						      G4ThreeVector( 0.0 , 0.0 , -TargetSizeZ/2.0 - foilz/2.0 )
						      );
    G4UnionSolid* targetf2 = new G4UnionSolid("targetf2",
					      targetf1,
					      foilSolid,
					      noRot,
					      G4ThreeVector( 0.0 , 0.0 , TargetSizeZ/2.0 + foilz/2.0 )
					      );
    G4LogicalVolume *foilLV = 
      //new G4LogicalVolume( foilSolid, Foil , "Foil LV");
      new G4LogicalVolume( targetf2, Foil , "Foil LV");
    //    G4double poszFoil_1  = -TargetSizeZ/2.0 - foilz/2.0 ;
//    G4double poszFoil_2  = TargetSizeZ/2.0 + foilz/2.0 ;
//    new G4PVPlacement( rotTarget
//		       , G4ThreeVector(0*cm, 0*cm, poszFoil_1)
//		       , foilLV
//		       , "Foil" 
//		       , worldLV 
//		       , false
//		       , 0 );
    //    new G4PVPlacement( rotTarget
//		       , G4ThreeVector(0*cm, 0*cm, poszFoil_2)
//		       , foilLV
//		       , "Foil" 
//		       , worldLV 
//		       , false
//		       , 1 );
    new G4PVPlacement( rotTarget
		       , G4ThreeVector(0.0*cm, 0.0*cm, 0.0*cm)
		       , foilLV
		       , "Foil" 
		       , worldLV 
		       , false
		       , 0 );
    //=== Visualization for foil of water target ===//
    //G4Colour colourFoil(1.0, 1.0, 1.0); // White
    G4Colour colourFoil(1.0, 0.8, 1.0); //
    G4VisAttributes *foilVisAtt = new G4VisAttributes(true, colourFoil);
    foilLV->SetVisAttributes(foilVisAtt);
    
  }
  else if( TargetMaterial == "Chromium"){
    Target = mList_->Cr52;
    //TargetSizeZ = 0.14734 * 0.1 * cm ; // 147 [um]
    //TargetSizeZ = 0.16972 * 0.1 * cm ; //170 [um]
    //TargetSizeZ = 0.2227263766 * 0.1 * cm ; //170 [um]
    //TargetSizeZ = 0.21538471146956 * 0.1 * cm ; //170 [um]
    //TargetSizeZ = 0.3 * 0.1 * cm ; //0.3 [mm]
    //TargetSizeZ = 0.4 * 0.1 * cm ; //0.4 [mm]
    //TargetSizeZ = 0.5 * 0.1 * cm ; //0.5 [mm]
    //TargetSizeZ = 1.0 * 0.1 * cm ; //1.0 [mm]
    //TargetSizeZ = 2.0 * 0.1 * cm ; //2.0 [mm]
    //TargetSizeZ = 3.0 * 0.1 * cm ; //3.0 [mm]
    //TargetSizeZ = 4.0 * 0.1 * cm ; //4.0 [mm]
    //TargetSizeZ = 5.0 * 0.1 * cm ; //5.0 [mm]
    //TargetSizeZ = 8.0 * 0.1 * cm ; //8.0 [mm]
 
  }
  else if( TargetMaterial == "Lead"){
    Target = mList_->Pb208;
    //TargetSizeZ = 0.0440839358 * 0.1 * cm ; //  50 [mg/cm2]// for test 16/Apr/2013
    //TargetSizeZ = 0.08816787 * 0.1 * cm ; // 100 [mg/cm2]// for test 15/Apr/2013
    //TargetSizeZ = 0.17633574 * 0.1 * cm ; // 200 [mg/cm2]// for test 15/Apr/2013
  }
  else if( TargetMaterial == "Tungsten"){
    Target = mList_->W;
    //TargetSizeZ = 0.17633574 * 0.1 * cm ; // 200 [mg/cm2]// for test 15/Apr/2013
  }
  else {
    G4cout << "You choose target '" << TargetMaterial 
	   << "...  I don't know such  target...sorry. " 
	   << G4endl;
    G4cout << "I am gonna use no target in this Run." 
	   <<G4endl;
    Target = mList_->Vacuum;
  }
  //G4double TargetDensity = Target->GetDensity();
  //G4double TargetSizeX = 1.*cm;
  //G4double TargetSizeY = 1.*cm;
  //G4double TargetSizeZ = (TargetThickness)/(TargetDensity/(g/cm3)*1000)*cm;
  //G4RotationMatrix* rotTarget = new G4RotationMatrix();//Rotaion of target
  //rotTarget->rotateY(-17.0 * deg);//Rotation of target

  
  //G4LogicalVolume *targetLV;
  int fiberflag = 1;
  
  G4Box *targetSolid = new G4Box( "Target",
				  TargetSizeX/2,TargetSizeY/2,TargetSizeZ/2 );
  targetLV =  new G4LogicalVolume( targetSolid, Target, "Target LV");

//  G4Box *targetSolid2 = new G4Box( "Target2",
//				   3.0/2.0*mm , 50.0/2.0*mm, 3.0/2.0*mm);
  
  // ===== Define fiber targets ================================
  G4Material *Fiber_Core_Material =mList_->PS;   // Polystylene
  G4Material *Fiber_Clad_Material =mList_->PMMA; // PMMA
  // ~~~~~~~ Create logical volumes of x layers ~~~~~~~~~~
  double fiber_phi_core = 2.94*mm;
  double fiber_phi_clad = 0.06*mm;
  double fiber_phi = fiber_phi_core + fiber_phi_clad;
  double fiber_distance_xxp = 2.7*mm;
  double fiber_distance_xpy = 3.1*mm;
  double fiber_distance_seg = 3.1*mm;
  
  G4EllipticalTube* fiber_core_solid_x 
    = new G4EllipticalTube("fiber_core_solid_x",
			   fiber_phi_core/2.0, fiber_phi_core/2.0, 50.0/2.0*mm );
  G4Tubs* fiber_clad_solid_x
    = new G4Tubs("fiber_clad_solid_x",
		 fiber_phi_core/2.0, fiber_phi/2.0, //Min and Max radii
		 5.0*cm/2.0,//height
		 0.0,//[rad]
		 2.0*3.14159);//[rad]
  
  //  G4RotationMatrix* rot_core_clad = new G4RotationMatrix();
  //  G4UnionSolid* fiber_solid = 
  //    new G4UnionSolid("fiber_solid",
  //		     fiber_core_solid,
  //		     fiber_clad_solid,
  //		     rot_core_clad,
  //		     G4ThreeVector( 0.0 , 0.0 , 0.0)
  //		     );
  G4LogicalVolume* FiberXLV 
    =  new G4LogicalVolume( fiber_core_solid_x, Fiber_Core_Material, "Fiber x LV");
  G4LogicalVolume* CladXLV 
    =  new G4LogicalVolume( fiber_clad_solid_x, Fiber_Clad_Material, "Clad x LV");
  G4RotationMatrix* rotFiberx = new G4RotationMatrix();//Rotaion of fiber target
  rotFiberx->rotateX(90.0 * deg);//Rotation of fiber target
  
  // ~~~~~~~ Create logical volumes of y layers ~~~~~~~~~~
  G4EllipticalTube* fiber_core_solid_y 
    = new G4EllipticalTube("fiber_core_solid_y",
			   fiber_phi_core/2.0, fiber_phi_core/2.0, 10.0/2.0*cm );
  G4Tubs* fiber_clad_solid_y
    = new G4Tubs("fiber_clad_solid_y",
		 fiber_phi_core/2.0, fiber_phi/2.0, //Min and Max radii
		 10.0*cm/2.0,//height
		 0.0,//[rad]
		 2.0*3.14159);//[rad]
  
  G4LogicalVolume* FiberYLV 
    =  new G4LogicalVolume( fiber_core_solid_y, Fiber_Core_Material, "Fiber y LV");
  G4LogicalVolume* CladYLV 
    =  new G4LogicalVolume( fiber_clad_solid_y, Fiber_Clad_Material, "Clad y LV");
  G4RotationMatrix* rotFibery = new G4RotationMatrix();//Rotaion of fiber target
  rotFibery->rotateY(90.0 * deg);//Rotation of fiber target
  
  //double fiber_x = 150.0*mm;
  //double fiber_y =  50.0*mm;
  //double fiber_z = 100.0*mm;

  int fiber_tot_layers = 9; // 1-layer = xx'yy' // default

  double init_poszx0 = 349.0; //AFT X0(the most front X plane) from FF (mm)
  double init_poszy0 = 354.8; //AFT Y0(the most front Y plane) from FF (mm)
  double z_offaft = -2.86*mm;
  double fiber_poszx = init_poszx0 - z_offaft;
  double fiber_poszy = init_poszy0 - z_offaft;
  
  int fiber_nx = 32;
  //int count_x_fiber=0;
  double xkegaki_off = -0.2*mm;
  double x_offaft = 0.018729*mm; // X geo off (which was made by BT analysis)
  double fiber_startx = xkegaki_off - fiber_distance_seg*15.5 - x_offaft;
  double fiber_startxp = xkegaki_off - fiber_distance_seg*16.0 - x_offaft;
  
  int fiber_ny = 16; // default
  //int count_y_fiber = 0;
  double ykegaki_off = 0.25*mm;
  double y_offaft = 0.582233*mm; // Y geo off (which was made by BT analysis)
  double fiber_starty = ykegaki_off - fiber_distance_seg*7.5 - y_offaft;
  double fiber_startyp = ykegaki_off - fiber_distance_seg*8.0 - y_offaft;

  int count_fiber = 0;
  int countx=0;//, countxp=0;
  int county=0;//, countyp=0;
  double target_shift = 2.0 * (fiber_phi+fiber_phi/2.0*sqrt(3.0)) * fiber_tot_layers;
  target_shift = target_shift / 2.0 ;

  
  //if (TargetMaterial != "Vacuum"){
  if (TargetMaterial != "Vacuum" && fiberflag!=1){
    new G4PVPlacement( rotTarget
		       , G4ThreeVector(0*cm, 0*cm, 0*cm)
		       ,targetLV
		       , "Target" 
		       , worldLV 
		       , false
		       , 0 );
  }
  else if ( TargetMaterial!="Vacuum" && fiberflag==1 ){
    for(int layer=0 ; layer<fiber_tot_layers ; layer++){
      for(int i=0 ; i<fiber_nx ; i++){
	countx++;
	// -- x --
	new G4PVPlacement( rotFiberx
			   , G4ThreeVector(fiber_startx+fiber_distance_seg*i, 0*cm
					   , fiber_poszx + (2*fiber_distance_xxp + 2*fiber_distance_xpy)*layer )
			   ,FiberXLV
			   ,"FiberX" 
			   , worldLV 
			   , false
			   , countx-1 );
			   //, 1000 );
	new G4PVPlacement( rotFiberx
			   , G4ThreeVector(fiber_startx+fiber_distance_seg*i, 0*cm
					   , fiber_poszx + (2*fiber_distance_xxp + 2*fiber_distance_xpy)*layer)
			   , CladXLV 
			   ,"CladX" 
			   , worldLV 
			   , false
			   , countx-1 );
	
      }
      for(int i=0 ; i<fiber_nx ; i++){
	//if(countx!=0) countx++;
	//countx++;
	// -- x' --
	countx++;
	new G4PVPlacement( rotFiberx
			   , G4ThreeVector(fiber_startxp+fiber_distance_seg*i, 0*cm
					   , fiber_poszx + fiber_distance_xxp  + (2*fiber_distance_xxp + 2*fiber_distance_xpy)*layer)
			   ,FiberXLV
			   ,"FiberX" 
			   , worldLV 
			   , false
			   , countx-1 );
			   //, 1001 );
	new G4PVPlacement( rotFiberx
			   , G4ThreeVector(fiber_startxp+fiber_distance_seg*i, 0*cm
					   , fiber_poszx + fiber_distance_xxp  + (2*fiber_distance_xxp + 2*fiber_distance_xpy)*layer)
			   ,CladXLV
			   ,"CladX" 
			   , worldLV 
			   , false
			   , countx-1 );
	
      }
      
      // -- y' ------------------------------------
      for(int i=0 ; i<fiber_ny ; i++){
	county++;
	// -- y --
	new G4PVPlacement( rotFibery
			   , G4ThreeVector( 0*cm,fiber_starty+fiber_distance_seg*i
					    ,fiber_poszy + (2*fiber_distance_xxp + 2*fiber_distance_xpy)*layer)
			   ,FiberYLV
			   ,"FiberY" 
			   , worldLV 
			   , false
			   , county-1 );
	new G4PVPlacement( rotFibery
			   , G4ThreeVector( 0*cm,fiber_starty+fiber_distance_seg*i
					    , fiber_poszy + (2*fiber_distance_xxp + 2*fiber_distance_xpy)*layer)
			   ,CladYLV
			   ,"CladY" 
			   , worldLV 
			   , false
			   , county-1 );
      }
      // -- y' ------------------------------------
      for(int i=0 ; i<fiber_ny ; i++){
	//if(county!=0) county++;
	county++;
	new G4PVPlacement( rotFibery
			   , G4ThreeVector( 0*cm, fiber_startyp+fiber_distance_seg*i
					    , fiber_poszy + fiber_distance_xxp + (2*fiber_distance_xxp + 2*fiber_distance_xpy)*layer)
			   ,FiberYLV
			   ,"FiberY" 
			   , worldLV 
			   , false
			   , county-1 );
	new G4PVPlacement( rotFibery
			   , G4ThreeVector( 0*cm, fiber_startyp+fiber_distance_seg*i
					    , fiber_poszy + fiber_distance_xxp + (2*fiber_distance_xxp + 2*fiber_distance_xpy)*layer)
			   ,CladYLV
			   ,"CladY" 
			   , worldLV 
			   , false
			   , county-1 );
      }
    }
    
  }
  
  G4SDManager *SDman = G4SDManager::GetSDMpointer();
  G4String TargetSDname_X = "/FiberTarget/X";
  G4String TargetSDname_Y = "/FiberTarget/Y";
  TargetSD* targetSD_X = new TargetSD( TargetSDname_X , 1 , "TargetCollection_X" );
  TargetSD* targetSD_Y = new TargetSD( TargetSDname_Y , 1 , "TargetCollection_Y" );
  SDman -> AddNewDetector( targetSD_X );
  SDman -> AddNewDetector( targetSD_Y );
  FiberXLV -> SetSensitiveDetector( targetSD_X );
  FiberYLV -> SetSensitiveDetector( targetSD_Y );
  
  
  //Virtual Detector
  G4ThreeVector vd1pos(0.0*cm , 0.0*cm , 8.5*cm);// for 15  cm target
  G4double vd1_x = 50.0*cm;   // [cm]
  G4double vd1_y = 50.0*cm;   // [cm]
  G4double vd1_z = 0.001*mm; // [mm] // Default
  G4Box *vd1_box = new G4Box( "VD1_BOX",vd1_x/2.0 , vd1_y/2.0 , vd1_z/2.0 );
  G4LogicalVolume *vd1LV
    = new G4LogicalVolume( vd1_box , mList_->Vacuum , "VD1_LV");
  
  G4PVPlacement *vd1 
    = new G4PVPlacement( 0 ,      //rotaion
			 vd1pos , //position
			 vd1LV ,  //its logical volume
			 "VD1" ,  //its name
			 worldLV ,//its mother
			 false ,  //no boolean operation
			 0 );     //copy number
  
  G4String vdSDname1 = "/FiberTarget/VD1";
  FiberTargetvdSD* vdSD1 = new FiberTargetvdSD( vdSDname1 , 1 , "VDCollection1" );
  SDman -> AddNewDetector( vdSD1 );
  vd1LV -> SetSensitiveDetector( vdSD1 );
  
  //VD2
  //G4ThreeVector vd2pos(0.0*cm , 0.0*cm , 2.5*cm);
  //G4LogicalVolume *vd2LV
  //  = new G4LogicalVolume( vd1_box , mList_->Vacuum , "VD1_LV");
  //G4PVPlacement *vd2 
  //  = new G4PVPlacement( 0 ,      //rotaion
  //			 vd2pos , //position
  //			 vd2LV ,  //its logical volume
  //			 "VD2" ,  //its name
  //			 worldLV ,//its mother
  //			 false ,  //no boolean operation
  //			 0 );     //copy number
  ////G4SDManager *SDman = G4SDManager::GetSDMpointer();
  //G4String vdSDname2 = "/FiberTarget/VD2";
  //FiberTargetvdSD* vdSD2 = new FiberTargetvdSD( vdSDname2 , 1 , "VDCollection2" );
  //SDman -> AddNewDetector( vdSD2 );
  //vd2LV -> SetSensitiveDetector( vdSD2 );
  
  ///////////////////
  // Visualisation //
  ///////////////////
  
  //=== World ===//
  worldLV->SetVisAttributes(G4VisAttributes::GetInvisible());
  
  //=== Target ===//
  G4Colour colourTarget(0., 1., 1.); // Cyan
  G4VisAttributes *targetVisAtt = new G4VisAttributes(true, colourTarget);
  targetLV->SetVisAttributes(targetVisAtt);
  
  G4Colour colourFiber(0., 1., 1.); // Cyan
  G4VisAttributes *FiberVisAtt = new G4VisAttributes(true, colourFiber);
  FiberXLV->SetVisAttributes(FiberVisAtt);
  FiberYLV->SetVisAttributes(FiberVisAtt);
  
  G4Colour colourClad(1., 1., 1.); 
  G4VisAttributes *CladVisAtt = new G4VisAttributes(true, colourClad);
  CladXLV->SetVisAttributes(CladVisAtt);
  
  //=== Virtual Detector ===//
  //vd1LV->SetVisAttributes(G4VisAttributes::Invisible);
  G4Colour colourvd1(0.6, 0.5, 0.6); // 
  G4VisAttributes *vd1VisAtt = new G4VisAttributes(true, colourvd1);
  vd1LV->SetVisAttributes(vd1VisAtt);
  

  return world;
}

