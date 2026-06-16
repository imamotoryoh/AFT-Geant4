/*
  FiberTargetPrimaryGeneratorAction.cc
  Oct 2015, T.Gogami
*/

#include "FiberTargetPrimaryGeneratorAction.hh"
#include "FiberTargetParamManager.hh"
#include "G4ParticleTable.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ThreeVector.hh"
#include "G4Geantino.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "G4String.hh"
#include <TMath.h>
#include "G4SystemOfUnits.hh"
//#include <TMathCore.hh>
#include <TRandom3.h>
#include <fstream>
#include <string>
//ifstream* dragon_seed;// = new ifstream("./seed/fiber_10.08cm_elementary.seed");

class G4ParticleGun;
class G4Event;


FiberTargetPrimaryGeneratorAction::FiberTargetPrimaryGeneratorAction()
{
  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);
  particleGun -> SetParticleDefinition( G4Geantino::GeantinoDefinition() );
  particleGun -> SetParticleEnergy( 2.344*GeV );
  particleGun -> SetParticlePosition( G4ThreeVector(0*m , 0*m , -2.0*cm) );
}
FiberTargetPrimaryGeneratorAction::FiberTargetPrimaryGeneratorAction(FiberTargetParamManager* PManDragon)
  :paramMan(PManDragon),
   Beamx(0.0),Beamy(0.0),Beamz(0.0),
   BeamOffset( G4ThreeVector( 0.0 , 0.0 , 0.0 ) ),
   GenFlag(0),rasterX(0.0),rasterY(0.0),rasterZ(0.0),
   ParticleFlag(0),
   bpc(0.0),bpw(0.0),
   bthc(0.0),bthw(0.0),
   bphc(0.0),bphw(0.0),
   SeedFlag(0)
   //evID(0)
{
  //G4double beamenergy = paramMan->GetBeamEnergy();//Momentum
  BeamMom = paramMan->GetBeamEnergy();//Momentum
  GenFlag = paramMan->GetPGenFlag();
  rasterX = paramMan->GetRasterX();
  rasterY = paramMan->GetRasterY();
  rasterZ = paramMan->GetRasterZ();
  BeamOffset = paramMan->GetBeamOffset();
  ParticleFlag = paramMan->GetParticleFlag();
  bpc  = BeamMom;
  bpw  = paramMan->GetBeamP_w();
  bthc = paramMan->GetBeamTheta();
  bthw = paramMan->GetBeamTheta_w();
  bphc = paramMan->GetBeamPhi();
  bphw = paramMan->GetBeamPhi_w();
  SeedFlag = paramMan->GetSeedFlag();
  SeedFile = paramMan->GetSeedFile();
  sfile=fopen( paramMan->GetSeedFile(), "r");
  //dragon_seed = new ifstream(paramMan->GetSeedFile().c_str());
  //dragon_seed = new ifstream(paramMan->GetSeedFile());
  dragon_seed = new ifstream(SeedFile.c_str());
  //dragon_seed = new ifstream("./seed/fiber_10.08cm_elementary.seed");
  //dragon_seed = new ifstream(SeedFile);
  //ifstream* dragon_seed = new ifstream(SeedFile.c_str());
  //ifstream* seed = new ifstream(SeedFile.c_str());
  //dragon_seed.open(SeedFile);
  //SetSeed(seed);
  
  //TRandom3*aaa = new TRandom3();
  trand = new TRandom3();
  
  //for(int i=0 ; i<10000 ; i++){
  //  cout << GenFlag << endl;
  //}
  //particleGun = new G4ParticleGun(n_particle);
  //particleGun = new G4ParticleGun();
  //particleGun -> SetParticleDefinition( G4Geantino::GeantinoDefinition() );
  G4ParticleTable* particleTable  = G4ParticleTable::GetParticleTable();
  //particleGun -> SetParticleEnergy( beamenergy * GeV );
  //particleGun -> SetParticleMomentum( BeamMom * GeV );//Momentum
  //particleGun -> SetParticlePosition( G4ThreeVector(0*m , 0*m , -2.0*cm) );
  //for(int i=0 ; i<900000 ; i++){
  //    G4cout << ParticleFlag  << G4endl;
  //}
  if(ParticleFlag==0){
    particle = particleTable->FindParticle("e-");
  }
  else if(ParticleFlag==1){
    particle = particleTable->FindParticle("e+");
  }
  // ----- pi-, pi+, mu- were added for energy loss simulation------
  // ----------------------- Toshi , 18Oct2013 --------------------
  else if(ParticleFlag==2){ 
    particle = particleTable->FindParticle("pi-");
  }
  else if(ParticleFlag==3){
    particle = particleTable->FindParticle("pi+");
  }
  else if(ParticleFlag==4){
    particle = particleTable->FindParticle("mu-");
  }
  else if(ParticleFlag==5){
    particle = particleTable->FindParticle("proton");
  }
  else if(ParticleFlag==6){
    particle = particleTable->FindParticle("kaon-");
  }
  else if(ParticleFlag==7){
    particle = particleTable->FindParticle("kaon+");
  }
  else if(ParticleFlag==8){
    particle = particleTable->FindParticle("xi-");
  }
  else if(ParticleFlag==111){ // Quasi-free Xi
    particle = particleTable->FindParticle("pesudoXi");
  }
  else if(ParticleFlag==222){ // (11B + Xi) nucleus 
    particle = particleTable->FindParticle("12XiBe");
  }
  // ----------------------------------------------------
  else{
    particle = particleTable->FindParticle("e-"); // comment out 17Oct2013
    //particle = particleTable->FindParticle("pi+");  // for test
  }
  
}

FiberTargetPrimaryGeneratorAction::~FiberTargetPrimaryGeneratorAction()
{
  delete particleGun;
}

void FiberTargetPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // ~~~~~~~~ Original ~~~~~~~~~~~
  //GenParPoint(anEvent);
  
  // ~~~~~~~~ Uniform generation ~~~~~
  //GenParUni(anEvent);
  if( SeedFlag==0 || SeedFlag==1 
      || SeedFlag==2 || SeedFlag==3){
    GenParUni(anEvent);
  }
  //else{
  //  GenParSeed();
  //}
}


//############################################ 
//   GenParPoint                        ######
//############################################ 
void FiberTargetPrimaryGeneratorAction::GenParPoint(G4Event* anEvent){
  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);
  particleGun -> SetParticleEnergy(0.0*GeV);// reset
  paramMan->SetBeamPos( G4ThreeVector(0.0*cm,0.0*cm,0.0*cm) );// reset
  //particleGun -> SetParticleDefinition( G4Geantino::GeantinoDefinition() );
  //BeamMom = BeamMom + ( 50.0e-3 * ( 2.0*G4UniformRand()-1.0 ) ) ;
  particleGun -> SetParticleMomentum( BeamMom * GeV );// Momentum in GeV
  particleGun -> SetParticleDefinition( particle );   // Particle type
  particleGun -> SetParticleMomentumDirection( G4ThreeVector(0.0,0.0,1.0) );
  G4double boffx = BeamOffset.x();
  G4double boffy = BeamOffset.y();
  G4double boffz = BeamOffset.z();
  if(GenFlag==2){ // Rastering
    //Beamx = rasterX*(G4UniformRand()-0.5)*2./2.0*cm;//toshi
    //Beamy = rasterY*(G4UniformRand()-0.5)*2./2.0*cm;//toshi
    //Beamz = rasterZ*(G4UniformRand()-0.5)*2./2.0*cm;//toshi
    Beamx = ( rasterX*(G4UniformRand()-0.5)*2./2.0 + boffx )*cm;//toshi
    Beamy = ( rasterY*(G4UniformRand()-0.5)*2./2.0 + boffy )*cm;//toshi
    Beamz = ( rasterZ*(G4UniformRand()-0.5)*2./2.0 + boffz )*cm;//toshi
    particleGun -> SetParticlePosition( G4ThreeVector( Beamx , Beamy , Beamz ) );
    //G4cout << G4UniformRand() << G4endl;
    //G4cout << Beamx/cm << " , " << Beamy/cm << " , " << Beamz/cm << G4endl;
  }
  else{ // No Rastering
    Beamx  =  (0.0 + boffx )*cm;
    Beamy  =  (0.0 + boffy )*cm;
    //Beamz  =  (-2.0 + boffz)*cm;
    Beamz  =  (-8.0 + boffz)*cm;
    particleGun -> SetParticlePosition( G4ThreeVector( Beamx , Beamy , Beamz ) );
  }
  paramMan->SetBeamPos( G4ThreeVector(Beamx/cm,Beamy/cm,Beamz/cm) );
  //G4int i = anEvent -> GetEventID() % 3;
  //switch(i)
  // {
  //  case 0:
  //    particleGun -> SetParticleMomentumDirection(G4ThreeVector(1.0,0.0,0.0));
  //  case 1:
  //    particleGun -> SetParticleMomentumDirection(G4ThreeVector(0.0,1.0,0.0));
  //  case 2:
  //    particleGun -> SetParticleMomentumDirection(G4ThreeVector(0.0,0.0,1.0));
  //  }
  particleGun -> GeneratePrimaryVertex(anEvent);
}

//############################################ 
//   GenParUni                          ######
//############################################ 
void FiberTargetPrimaryGeneratorAction::GenParUni( G4Event* anEvent )
{
  //HKSParamMan *paramMan = HKSParamMan::GetParamMan();

  G4double px,py,pz; 
  //G4double CentMom = 0.1;//GeV/c
  G4double CentMom = bpc;
  G4double AcptMom = bpw;
  G4double CentTheta = bthc;
  G4double AcptTheta = bthw;
  G4double CentPhi = bphc;
  G4double AcptPhi = bphw;
  //G4double ThetaLimit = paramMan->GetThetaLimit();
  G4double xR = rasterX;
  G4double yR = rasterY;
  G4double zR = rasterZ;
  
  //---- Beam momentum acceptance (Uniform) ----
  //G4double Mom = (CentMom + (G4UniformRand()-0.5)*2.0*AcptMom);
  
  //---- Beam momentum acceptance (Gaussian) ----
  //G4double Mom = trand -> Gaus( CentMom , AcptMom );
  G4double Mom = G4RandGauss::shoot( CentMom , AcptMom );
  //G4cout << Mom << G4endl;
  
  G4double xp = 0.;
  G4double yp = 0.;
  //G4int RanSign=rand()%2;
  G4double phi=0;
  G4double min = cos(CentTheta+AcptTheta);
  G4double max = cos(CentTheta-AcptTheta);
  G4double da = (max-min)/2. ;
  G4double a = min + da ;

  G4double theta=acos( a + ( 2.*da*(G4UniformRand()-0.5) ) );
  //if(RanSign==1)phi=-1.*(CentPhi+( 2.0*AcptPhi*( G4UniformRand()-0.5 ) ));
  //else if(RanSign==0)phi=1.*(CentPhi+( 2.0*AcptPhi*( G4UniformRand()-0.5 ) ));
  phi=1.*(CentPhi+( 2.0*AcptPhi*( G4UniformRand()-0.5 ) ));
  xp=sin(theta)*cos(phi)/cos(theta);
  yp=sin(theta)*sin(phi)/cos(theta);
  pz = Mom/sqrt(1+xp*xp+yp*yp);
  px = pz*xp;
  py = pz*yp;
  //G4double x = xR*(G4UniformRand()-0.5)*2.*cm;
  //G4double y = yR*(G4UniformRand()-0.5)*2.*cm;
  //G4double z = (zR*(G4UniformRand()-0.5)*2.)*cm;
  //G4double x = xR*(G4UniformRand()-0.5)*2./2.0*cm;//toshi
  //G4double y = yR*(G4UniformRand()-0.5)*2./2.0*cm;//toshi
  //G4double z = (zR*(G4UniformRand()-0.5)*2./2.0)*cm;//toshi
  G4double boffx = BeamOffset.x();
  G4double boffy = BeamOffset.y();
  G4double boffz = BeamOffset.z();
  const int aa=1000;
  char str[aa];
  G4double temp;
  G4int evID;
  G4double pkmx, pkmy, pkmz;
  G4double pkmx0,pkmy0,pkmz0;
  G4double pkpx, pkpy, pkpz;
  G4double pkpx0, pkpy0, pkpz0;
  
  if(SeedFlag==0){
    // ~~~~~~~~~~ Rastering ~~~~~~~~~~~~~~~
    if(GenFlag==2){ 
      Beamx = ( xR*(G4UniformRand()-0.5)*2./2.0 + boffx )*cm;//toshi
      Beamy = ( yR*(G4UniformRand()-0.5)*2./2.0 + boffy )*cm;//toshi
      Beamz = ( zR*(G4UniformRand()-0.5)*2./2.0 + boffz )*cm;//toshi
      //particleGun -> SetParticlePosition( G4ThreeVector( Beamx , Beamy , Beamz ) );
      //G4cout << G4UniformRand() << G4endl;
      //G4cout << Beamx/cm << " , " << Beamy/cm << " , " << Beamz/cm << G4endl;
    }
    // ~~~~~~~~~~ No rastering ~~~~~~~~~~~~~~~
    else{ 
      Beamx  =  (0.0 + boffx )*cm;
      Beamy  =  (0.0 + boffy )*cm;
      //Beamz  =  (-2.0 + boffz)*cm;
      Beamz  =  (-8.0 + boffz)*cm;
      //particleGun -> SetParticlePosition( G4ThreeVector( Beamx , Beamy , Beamz ) );
    }
    //paramMan->SetBeamPos( G4ThreeVector(Beamx/cm,Beamy/cm,Beamz/cm) );
    paramMan->SetBeamPos( G4ThreeVector(Beamx/cm,Beamy/cm,Beamz/cm) );
    //Beamx = Beamx;
    //Beamy = Beamy;
    //Beamz = Beamz;
//    G4cout << Beamx/cm << " " << Beamy/cm << " " << Beamz/cm 
//	   << G4endl;
  }
  else if (SeedFlag==99){ //~~~~~~~~~~~~ From Seed file (for HES/HKS) ~~~~~~~~~~~~~~~~
    //fgets( str, aa, SeedFile );
    fgets( str, aa, sfile );
    sscanf( str, "%d %lf %lf %lf %lf %lf %lf %lf %lf %lf",
	    &evID,  &Beamx, &temp, &Beamy, &temp, &temp, &Beamz, &temp, &temp, &temp);
    paramMan->SetEVID(evID);
    paramMan->SetBeamPos( G4ThreeVector(Beamx/cm,Beamy/cm,Beamz/cm) );
    //oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO//
    // Rastering in z = 0.5069 cm for 450.8[mg/cm2]  CH2 target   //
    //oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO//
    //Beamz = -1.0 * Beamz; // Reverse the sign
    Double_t tang = 17.0;   // Target angle
    //Double_t tang = 0.0;   // Target angle
    tang = tang * 3.14159 / 180.0; // degree --> rad
//    G4cout << "     Gen:" 
//	   << evID << " " << Beamx << " " << Beamy << " " << Beamz <<G4endl;
    /*
    // ~~~~~~~~~ Pattern 1 ~~~~~~~~~~
    //Beamz = -1.0 * (Beamz - Beamx*tan(tang)) + x*tan(tang); // Reverse the sign
    Beamz = -1.0 * Beamz +2.0*Beamx*tan(tang);
    */
    // ~~~~~~~~~ Pattern 2 ~~~~~~~~~~
    //Beamz = -1.0 * (Beamz - Beamx*tan(tang)) + x*tan(tang); // Reverse the sign
    //Beamz = -1.0 * (Beamz - Beamx*tan(tang) );
    //Beamz = -1.0 * (Beamz + Beamx*tan(tang) ); // 11Dec2013 , Toshi
    //G4cout << Beamz << G4endl;
    Beamz = -1.0 * (Beamz + 2.0 * Beamx*tan(tang) ); // 11Dec2013 , Toshi
    //Beamz = -1.0 * (Beamz - 2.0 * Beamx*tan(tang) ); // 11Dec2013 , Toshi (opposite sign for test)
    //G4cout << Beamz << G4endl;
    //G4cout << G4endl;
    //Beamx = 0.0;
    //Beamy = 0.0;
    // ~~~~~~~

    Beamx = Beamx * cm;
    Beamy = Beamy * cm;
    Beamz = Beamz * cm;
  }
  else if (SeedFlag==1){ //~~~~~~~~~~~~ From Seed file (for S-2S) ~~~~~~~~~~~~~~~~
    //fgets( str, aa, SeedFile );
    fgets( str, aa, sfile );
    sscanf( str, "%d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
	    &evID,
	    &Beamx, &Beamy, &Beamz,
	    &pkmx0,&pkmy0,&pkmz0,
	    &pkmx, &pkmy, &pkmz,
	    &pkpx, &pkpy, &pkpz);
    paramMan->SetEVID(evID);
    //G4cout << evID << " " << Beamx << " " << Beamy << " " << Beamz <<G4endl;
    Beamx = Beamx * cm;
    Beamy = Beamy * cm;
    Beamz = Beamz * cm;
    paramMan->SetBeamPos( G4ThreeVector(Beamx/cm,Beamy/cm,Beamz/cm) );
    px = pkpx;
    py = pkpy;
    pz = pkpz;
    Mom = sqrt(px*px + py*py + pz*pz);
    theta = acos(pz/Mom);
    phi   = atan2(py,px);
    paramMan->SetPKm0(pkmx0, pkmy0, pkmz0);
    paramMan->SetPKm(pkmx, pkmy, pkmz);
    //G4cout << pkmx0 << " "  << pkmy0 << " " << pkmz0 << G4endl;
  }
  else if (SeedFlag==2){ //~~~~~~~~~~~~ From Seed file (for Fiber target) ~~~~~~~~~~~~~~~~//    
    paramMan->ResetFiberInfo();
    *dragon_seed >> evID >> Beamx >> Beamy >> Beamz
		 >> pkmx0 >> pkmy0 >> pkmz0 
		 >> pkmx >> pkmy >> pkmz 
		 >> pkpx >> pkpy >> pkpz;
//    G4cout << " (Primary Generator Action, SeedFlag==2): " 
//	   << evID << " "
//	   << Beamx << " " << Beamy << " " << Beamz
//	   << G4endl;
    Int_t nhit = 0;
    Int_t tempnumx[300], tempnumy[300];
    Double_t tempdEx[300] , tempdEy[300];
    for(int kk=0 ; kk<300 ; kk++){
      tempnumx[kk] = 999;
      tempnumy[kk] = 999;
      tempdEx[kk]  = 0.0;
      tempdEy[kk]  = 0.0;
    }
    *dragon_seed >> nhit;
    //G4cout << nhit << ": x : ";
    //G4cout << " THIS THIS THIS: " << nhit << G4endl;
    for(int i=0 ; i<nhit ; i++){
      *dragon_seed >> tempnumx[i] >> tempdEx[i];
      //G4cout << tempnum << " " << tempdE << G4endl;
    }
    paramMan->SetAlreadyFiberHitx(nhit,tempnumx,tempdEx);
    nhit = 0;
    *dragon_seed >> nhit;
    for(int i=0 ; i<nhit ; i++){
      *dragon_seed >> tempnumy[i] >> tempdEy[i];
    }
    paramMan->SetAlreadyFiberHity(nhit,tempnumy,tempdEy);
    
    paramMan->SetEVID(evID);
    //G4cout << evID << " " << Beamx << " " << Beamy << " " << Beamz <<G4endl;
    Beamx = Beamx * mm;
    Beamy = Beamy * mm;
    Beamz = Beamz * mm;
    paramMan->SetBeamPos( G4ThreeVector(Beamx/cm,Beamy/cm,Beamz/cm) );
    px = pkpx;
    py = pkpy;
    pz = pkpz;
    Mom = sqrt(px*px + py*py + pz*pz);
    theta = acos(pz/Mom);
    phi   = atan2(py,px);
    paramMan->SetPKm0(pkmx0, pkmy0, pkmz0);
    paramMan->SetPKm(pkmx, pkmy, pkmz);
    //G4cout << pkmx0 << " "  << pkmy0 << " " << pkmz0 << G4endl;
  }
  else if (SeedFlag==3){ //~~~~~~~~~~~~ From Seed file (for Fiber target) ~~~~~~~~~~~~~~~~//
    paramMan->ResetFiberInfo();
    *dragon_seed >> evID >> Beamx >> Beamy >> Beamz
		 >> pkmx0 >> pkmy0 >> pkmz0 
		 >> pkmx >> pkmy >> pkmz 
		 >> pkpx0 >> pkpy0 >> pkpz0
		 >> pkpx >> pkpy >> pkpz;
//    G4cout << " (Primary Generator Action, SeedFlag==3): " 
//	   << evID << " "
//           << Beamx << " " << Beamy << " " << Beamz << " " 
//	   << pkmx0 << " " << pkmy0 << " " << pkmz0 << " " 
//	   << pkmx << " " << pkmy << " " << pkmz << " " 
//	   << pkpx << " " << pkpy << " " << pkpz << " " 
//	   << pkpx0 << " "  << pkpy0 << " "  << pkpz0 << " " 
//	   << pkpx << " " << pkpy << " " << pkpz << G4endl;
    Int_t nhit = 0;
    Int_t tempnumx[1000], tempnumy[1000];
    Double_t tempdEx[1000] , tempdEy[1000];
    //Int_t tempnumx2[300], tempnumy2[300];
    //Double_t tempdEx2[300] , tempdEy2[300];
    for(int kk=0 ; kk<1000 ; kk++){
      tempnumx[kk]  = 999;
      tempnumy[kk]  = 999;
      tempdEx[kk]   = 0.0;
      tempdEy[kk]   = 0.0;
    }
    // ~~~ dE info for K- ~~~
    *dragon_seed >> nhit;
    //G4cout << nhit << G4endl;
    for(int i=0 ; i<nhit ; i++){
      *dragon_seed >> tempnumx[i] >> tempdEx[i];
      //G4cout << tempnum << " " << tempdE << G4endl;
    }
    paramMan->SetAlreadyFiberHitx(nhit,tempnumx,tempdEx);
    nhit = 0;
    *dragon_seed >> nhit;
    for(int i=0 ; i<nhit ; i++){
      *dragon_seed >> tempnumy[i] >> tempdEy[i];
    }
    paramMan->SetAlreadyFiberHity(nhit,tempnumy,tempdEy);
    // ~~~ dE info K+ ~~~
    for(int kk=0 ; kk<1000 ; kk++){
      tempnumx[kk]  = 999;
      tempnumy[kk]  = 999;
      tempdEx[kk]   = 0.0;
      tempdEy[kk]   = 0.0;
    }
    nhit = 0;
    *dragon_seed >> nhit;
    for(int i=0 ; i<nhit ; i++){
      *dragon_seed >> tempnumx[i] >> tempdEx[i];
      //G4cout << tempnum << " " << tempdE << G4endl;
    }
    paramMan->SetAlreadyFiberHitx2(nhit,tempnumx,tempdEx);
    nhit = 0;
    *dragon_seed >> nhit;
    for(int i=0 ; i<nhit ; i++){
      *dragon_seed >> tempnumy[i] >> tempdEy[i];
    }
    paramMan->SetAlreadyFiberHity2(nhit,tempnumy,tempdEy);
    
    paramMan->SetEVID(evID);
    //G4cout << evID << " " << Beamx << " " << Beamy << " " << Beamz <<G4endl;
    Beamx = Beamx * cm;
    Beamy = Beamy * cm;
    Beamz = Beamz * cm;
    paramMan->SetBeamPos( G4ThreeVector(Beamx/cm,Beamy/cm,Beamz/cm) );
    G4ThreeVector temp1 (pkmx, pkmy, pkmz);
    G4ThreeVector temp2 (pkpx0, pkpy0, pkpz0);
    G4ThreeVector temp3 = temp1-temp2;
    //px = pkpx;
    //py = pkpy;
    //pz = pkpz;
    px = temp3.x();
    py = temp3.y();
    pz = temp3.z();
    Mom = sqrt(px*px + py*py + pz*pz);
    theta = acos(pz/Mom);
    phi   = atan2(py,px);
    paramMan->SetPKm0(pkmx0, pkmy0, pkmz0);
    paramMan->SetPKp0(pkpx0, pkpy0, pkpz0);
    paramMan->SetPKm(pkmx, pkmy, pkmz);
    paramMan->SetPKp(pkpx, pkpy, pkpz);
    //G4cout << pkmx0 << " "  << pkmy0 << " " << pkmz0 << G4endl;
  }

  //paramMan->SetBeamPos( G4ThreeVector(Beamx/cm,Beamy/cm,Beamz/cm) );// moved , Toshi , 22Dec2013
  paramMan->SetGenP(Mom); // [GeV]
  paramMan->SetGenPvec(px,py,pz);// [GeV]
  paramMan->SetGenTheta(theta);
  paramMan->SetGenPhi(phi);
  
  //G4ThreeVector gPos(x, y, z);
  G4ThreeVector gPos(Beamx, Beamy, Beamz);
  G4ThreeVector gMom(px*GeV, py*GeV, pz*GeV);
    
  SetMom(gMom,gPos,anEvent,0);
}


////############################################ 
////   GenParSeed                         ######
////############################################
//void FiberTargetPrimaryGeneratorAction::GenParSeed( G4Event *anEvent )
//{
//  //HKSParamMan *paramMan = HKSParamMan::GetParamMan();
//  const int aa=500;
//  char str[aa];
//  fgets( str, aa, SeedFile );
//  
//  G4double px, py, pz, Mom;
//  G4double x0, y0, z0;
//  G4double pe;
//  G4double xp, yp;
//  G4double xp0, yp0;
//  G4double xpk,ypk,pk;
//  G4double xR = paramMan->GetRasterX();
//  G4double yR = paramMan->GetRasterY();
//  G4double zR = paramMan->GetRasterZ();
//  G4int evID;
//  
//  sscanf( str, "%d %lf %lf %lf %lf %lf %lf %lf %lf %lf",
//          &evID,  &x0, &xp0, &y0, &yp0, &pe, &z0, &xpk, &ypk, &pk);
//  Mom = pk;
//  
//  
//  xp = xpk/1000.;
//  yp = ypk/1000.;
//  pz = (Mom/sqrt(1+xp*xp+yp*yp));
//  px = pz*xp;
//  py = pz*yp;
//  G4ThreeVector gMom( px*GeV, py*GeV,pz*GeV);  
//  G4ThreeVector gPos( xR*x0*cm, yR*y0*cm, zR*z0*cm);
//  //G4ThreeVector gPos( 0.*cm, 0.*cm, 0.*cm);
//  
//  SetMom(gMom,gPos,anEvent,evID);
//}

//############################################ 
//   SetMom                             ######
//############################################
void FiberTargetPrimaryGeneratorAction::SetMom
(G4ThreeVector gMom, G4ThreeVector gPos, G4Event* anEvent, G4int evID)
{
  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);
  
  particleGun -> SetParticleEnergy(0.0*GeV); //reset
  particleGun -> SetParticleDefinition(particle); 
  particleGun -> SetParticleMomentum(gMom);
  particleGun -> SetParticlePosition(gPos);
  particleGun -> GeneratePrimaryVertex(anEvent);
  //  G4cout  << gPos.x()/cm << " " << gPos.y()/cm << " "  << gPos.z()/cm << G4endl;

  //if( anaMan ){
  //  anaMan->PrimaryGeneration( gPos, gMom, evID );
  //}
  //delete particleGun;

}
