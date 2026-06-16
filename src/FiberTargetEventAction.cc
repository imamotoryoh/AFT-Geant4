/*
  FiberTargetEventAction.cc
  Oct 2015, T.Gogami
*/

#include "FiberTargetRunAction.hh"
#include "FiberTargetEventAction.hh"
#include "FiberTargetvdHit.hh"
#include "FiberTargetvdSD.hh"
#include "TargetHit.hh"
#include "TargetSD.hh"
#include "FiberTargetParamManager.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4ios.hh"
#include "G4SDManager.hh"

#include <TTree.h>
#include <TFile.h>
#include <TROOT.h>
#include <math.h>
#include <TObjArray.h>
#include <time.h>
#include "globals.hh"
#include "G4SystemOfUnits.hh"

FiberTargetEventAction::FiberTargetEventAction()//Constructor 
  :primaryEventID(0)
  ,beamx(0.0),beamy(0.0),beamz(0.0),beamp(0.0)
  ,evID(0),trackID(0),x(0),y(0),z(0),px(0),py(0),pz(0),p(0)
  ,theta(0),phi(0),charge(0),pname("DRAGON")
  ,pioflag(0),kaoflag(0),proflag(0),eflag(0),phoflag(0),posflag(0)
  ,paramMan(0),PosiAnalFlag(0)
  ,ProcID(0),passth(0),eIoni(0),eBrem(0),compt(0),phoele(0),conv(0)
  ,minMom(0),maxMom(0),minTheta(0),maxTheta(0)
{}

FiberTargetEventAction::~FiberTargetEventAction()//Destructor
{}

FiberTargetEventAction::FiberTargetEventAction(FiberTargetParamManager* PmanDragon)//Constructor 
  :primaryEventID(0)
  ,beamx(0.0),beamy(0.0),beamz(0.0),beamp(0.0)
  ,evID(0),evID2(0),trackID(0),x(0),y(0),z(0),px(0),py(0),pz(0),p(0)
  ,theta(0),phi(0),charge(0),pname("DRAGON")
  ,pioflag(0),kaoflag(0),proflag(0),eflag(0),phoflag(0),posflag(0)
  ,muoflag(0),neuflag(0),paramMan(PmanDragon),PosiAnalFlag(0)
  ,ProcID(0),passth(0),eIoni(0),eBrem(0),compt(0),phoele(0),conv(0)
  ,minMom(0),maxMom(0),minTheta(0),maxTheta(0)
  ,beampx(0.0),beampy(0.0),beampz(0.0)
  ,beam_theta(0.0),beam_phi(0.0)
  ,SeedFlag(0),btrig(0)
{
  PosiAnalFlag = paramMan->GetPosiAnalFlag();
  minMom   = paramMan->GetMinMom();
  maxMom   = paramMan->GetMaxMom();
  minTheta = paramMan->GetMinTheta();
  maxTheta = paramMan->GetMaxTheta();
  SeedFlag = paramMan->GetSeedFlag();
  start = time(NULL);
  time(&start);
  //for(int i =0 ; i<10000000 ; i++){
  //  G4cout << "PmanDragon ===" << PmanDragon << G4endl;
  //}
}

void FiberTargetEventAction::BeginOfEventAction(const G4Event*evt)
{
  primaryEventID++;
  //if( (evt->GetEventID() ) == 1){
  if( (evt->GetEventID() ) == 0){ // fixed (Toshi , 13Dec2013)
    definetree(evt); //Create Branches for ROOT file
  }
  //G4cout << "Event ID = " << evt->GetEventID() << G4endl;
}

void FiberTargetEventAction::EndOfEventAction(const G4Event* evt)
{
  G4int event_id = evt->GetEventID();
  ///  Int_t event_id = evt->GetEventID();
  char anatime[100];
  
  //Periodic printing
  if((int)event_id % 10000 == 0 ){
    end = time(NULL);
    time(&end);
    sprintf( anatime,"%.0f Sec",difftime(end,start) );
    G4cout << "Event ID : " << evt->GetEventID() 
	   << " ( " << anatime << " ) " << G4endl;
   }
  //G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
  //G4int n_trajectories = 0;
  //if(trajectoryContainer) n_trajectories = trajectoryContainer->entries();
  //G4cout << "trajectory ontainer = " << n_trajectories << G4endl;
  //Analysis and Filling data to ROOT file
  filldata(evt);
}

//=============================================TOP//
//Define Branches for ROOT file
void FiberTargetEventAction::definetree(const G4Event*){
  //G4int SeedFlag = paramMan->GetSeedFlag();
  //TH1F* h;
  tree = dynamic_cast<TTree*>(gFile->Get("tree"));
  //tree->Branch("evID" ,  &evID  , "evID/I"   );
  tree->Branch("eventid" ,  &evID  , "eventid/I"   );
  tree->Branch("evID2",  &evID2 , "evID2/I"  );
  tree->Branch("btrig",  &btrig , "btrig/I"  );
  tree->Branch("trackID",&trackID,"trackID/I");
  tree->Branch("xBeam",  &beamx , "beamx/D");
  tree->Branch("yBeam",  &beamy , "beamy/D");
  tree->Branch("zBeam",  &beamz , "beamz/D");
  tree->Branch("zBeamR", &beamz_recon , "zBeamR/D");
  tree->Branch("pxBeam", &beampx ,"pxBeam/D");
  tree->Branch("pyBeam", &beampy ,"pyBeam/D");
  tree->Branch("pzBeam", &beampz ,"pzBeam/D");
  tree->Branch("pBeam",  &beamp , "pBeam/D");
  tree->Branch("thetaBeam", &beam_theta, "thetaBeam/D");
  tree->Branch("phiBeam", &beam_phi  , "phiBeam/D");
  tree->Branch("xVD"  ,  &x     , "x/D"    );
  tree->Branch("yVD"  ,  &y     , "y/D"    );
  tree->Branch("zVD"  ,  &z     , "z/D"    );
  tree->Branch("pxVD" ,  &px    , "px/D"   );
  tree->Branch("pyVD" ,  &py    , "py/D"   );
  tree->Branch("pzVD" ,  &pz    , "pz/D"   );
  tree->Branch("pVD"  ,  &p     , "p/D"    );
  tree->Branch("thetaVD",  &theta , "theta/D");
  tree->Branch("phiVD"  ,  &phi   , "phi/D"  );
  //tree->Branch("pname",  &pname , "pname/C"  );
  tree->Branch("charge" , &charge , "charge/D"  );
  tree->Branch("posflag", &posflag, "posflag/I" );
  tree->Branch("eleflag", &eleflag, "eleflag/I" );
  tree->Branch("phoflag", &phoflag, "phoflag/I" );
  tree->Branch("proflag", &proflag, "proflag/I" );
  tree->Branch("kaoflag", &kaoflag, "kaoflag/I" );
  tree->Branch("neuflag", &neuflag, "neuflag/I" );
  tree->Branch("pioflag", &pioflag, "pioflag/I" );
  tree->Branch("muoflag", &muoflag, "muoflag/I" );
  tree->Branch("passth" , &passth , "passth/I"  );
  tree->Branch("eIoni"  , &eIoni  , "eIoni/I"   );
  tree->Branch("eBrem"  , &eBrem  , "eBrem/I"   );
  tree->Branch("compt"  , &compt  , "compt/I"   );
  tree->Branch("phoele" , &phoele , "phoele/I"  );
  tree->Branch("conv"   , &conv   , "conv/I"    );
  tree->Branch("dE_fiberx", &dE_fiberx  , "dE_fiberx[1000]/D"    );
  tree->Branch("dE_fibery", &dE_fibery  , "dE_fibery[1000]/D"    );
  if(SeedFlag==1){
    tree->Branch("km_px" ,  &km_px    , "km_px/D"   );
    tree->Branch("km_py" ,  &km_py    , "km_py/D"   );
    tree->Branch("km_pz" ,  &km_pz    , "km_pz/D"   );
    tree->Branch("km_p"  ,  &km_p     , "km_p/D"    );
    //tree->Branch("km_theta",&km_theta , "km_theta/D");
    //tree->Branch("km_phi"  , &km_phi  , "km_phi/D"  );
    tree->Branch("km_px0" ,  &km_px0    , "km_px0/D"   );
    tree->Branch("km_py0" ,  &km_py0    , "km_py0/D"   );
    tree->Branch("km_pz0" ,  &km_pz0    , "km_pz0/D"   );
    tree->Branch("km_p0"  ,  &km_p0     , "km_p0/D"    );
    //tree->Branch("km_theta0",&km_theta0 , "km_theta0/D");
    //tree->Branch("km_phi0"  , &km_phi0  , "km_phi0/D"  );
  }
  if(SeedFlag==2 || SeedFlag==3){
    tree->Branch("km_px" ,  &km_px    , "km_px/D"   );
    tree->Branch("km_py" ,  &km_py    , "km_py/D"   );
    tree->Branch("km_pz" ,  &km_pz    , "km_pz/D"   );
    tree->Branch("km_p"  ,  &km_p     , "km_p/D"    );
    //tree->Branch("km_theta",&km_theta , "km_theta/D");
    //tree->Branch("km_phi"  , &km_phi  , "km_phi/D"  );
    tree->Branch("km_px0" ,  &km_px0    , "km_px0/D"   );
    tree->Branch("km_py0" ,  &km_py0    , "km_py0/D"   );
    tree->Branch("km_pz0" ,  &km_pz0    , "km_pz0/D"   );
    tree->Branch("km_p0"  ,  &km_p0     , "km_p0/D"    );
    //tree->Branch("km_theta0",&km_theta0 , "km_theta0/D");
    //tree->Branch("km_phi0"  , &km_phi0  , "km_phi0/D"  );
    tree->Branch("dE_fiberx_kminus", &dE_fiberx_kminus  , "dE_fiberx_kminus[1000]/D" );
    tree->Branch("dE_fibery_kminus", &dE_fibery_kminus  , "dE_fibery_kminus[1000]/D" );
    tree->Branch("dE_fiberx_kplus", &dE_fiberx_kplus  , "dE_fiberx_kplus[1000]/D" );
    tree->Branch("dE_fibery_kplus", &dE_fibery_kplus  , "dE_fibery_kplus[1000]/D" );   
    if(SeedFlag==3){
      tree->Branch("kp_px" ,  &kp_px    , "kp_px/D"   );
      tree->Branch("kp_py" ,  &kp_py    , "kp_py/D"   );
      tree->Branch("kp_pz" ,  &kp_pz    , "kp_pz/D"   );
      tree->Branch("kp_p"  ,  &kp_p     , "kp_p/D"    );
      tree->Branch("kp_px0",  &kp_px0   , "kp_px0/D"   );
      tree->Branch("kp_py0",  &kp_py0   , "kp_py0/D"   );
      tree->Branch("kp_pz0",  &kp_pz0   , "kp_pz0/D"   );
      tree->Branch("kp_p0" ,  &kp_p0    , "kp_p0/D"    );
      //tree->Branch("xi_edep" ,&xi_edep  , "xi_edep/D"  );
      tree->Branch("dE_fiberx_xi", &dE_fiberx_xi  , "dE_fiberx_xi[1000]/D" );
      tree->Branch("dE_fibery_xi", &dE_fibery_xi  , "dE_fibery_xi[1000]/D" );   
    }
  }

}
//==========================================BOTTOM//


//=============================================TOP//
//Analysis of data and Filling data to ROOT file
void FiberTargetEventAction::filldata(const G4Event*evt){
  //const int nn;
  
  G4int event_id = evt->GetEventID();
  tree = dynamic_cast<TTree*>(gFile->Get("tree"));
  
  G4int colidTarget_X;
  G4int colidTarget_Y;
  G4int colidVD;
  G4HCofThisEvent *HCE = evt -> GetHCofThisEvent();//Hits collection //                                                                     of this event
  G4SDManager *SDMan = G4SDManager::GetSDMpointer();//SD manager
  //
  G4double xBeam = -2222.0 , yBeam= -2222.0 , zBeam= -2222.0;// Beam Position
  G4double pBeam = -2222.0 ; // Beam Momentum
  G4double pxBeam= -2222.0 ; // Beam Momentum
  G4double pyBeam= -2222.0 ; // Beam Momentum
  G4double pzBeam= -2222.0 ; // Beam Momentum
  G4double thBeam= -2222.0 ; // Beam theta
  G4double phBeam= -2222.0 ; // Beam phi
  //G4double xBeam[nn] , yBeam[nn], zBeam[nn];// Beam Position
  //G4double pBeam[nn]; // Beam Momentum
  //G4double pxBeam[nn]; // Beam Momentum
  //G4double pyBeam[nn]; // Beam Momentum
  //G4double pzBeam[nn]; // Beam Momentum
  //G4double thBeam[nn]; // Beam theta
  //G4double phBeam[nn]; // Beam phi
  G4ThreeVector pvecBeam(0.0,0.0,0.0);
  G4double xVD  = -2222.0 , yVD  = -2222.0 , zVD  = -2222.0;//position
  G4double pxVD = -2222.0 , pyVD = -2222.0 , pzVD = -2222.0;//momentum(x,y,z)
  G4double pVD  = -2222.0 ;   // momentum
  G4double thetaVD= -2222.0 ; // theta
  G4double phiVD  = -2222.0 ; // phi
  G4double chargeVD=-2222.0 ; //Charge
  G4int piflagVD  = 0; //pion flag
  G4int posflagVD = 0; //positron flag
  G4int eleflagVD = 0; //electron flag
  G4int phoflagVD = 0; //photon flag
  G4int proflagVD = 0; //proton flag
  G4int kaoflagVD = 0; //proton flag
  G4int pioflagVD = 0; //pion flag
  G4int muoflagVD = 0; //muon flag
  G4int neuflagVD = 0; //neutron flag
  G4int passthVD  = 0; //beam passing through the target
  G4int eIoniVD   = 0; //electron ionization
  G4int eBremVD   = 0; //electron brems
  G4int comptVD   = 0; //compton scattering
  G4int phoeleVD  = 0; //photoelectric effect
  G4int convVD    = 0; //photon conversion

  G4ThreeVector bpos(0.0,0.0,0.0); //Beam position
  G4ThreeVector pos;//Hit position on VD
  G4ThreeVector mom;//Momentum vector of the particle
  bpos   = paramMan -> GetBeamPos();
  xBeam  = bpos.x();
  yBeam  = bpos.y();
  zBeam  = bpos.z();
  beamx = xBeam;
  beamy = yBeam;
  beamz = zBeam;
  G4double zreconreso = 0.1;// [cm]
  beamz_recon = G4RandGauss::shoot( beamz , zreconreso );
  pBeam  = paramMan -> GetGenP();
  beamp= pBeam*1000.0;//GeV--> MeV
  thBeam = paramMan -> GetGenTheta();
  beam_theta = thBeam;
  phBeam = paramMan -> GetGenPhi();
  beam_phi   = phBeam;
  pvecBeam = paramMan ->GetGenPvec();
  beampx = pvecBeam.x() * 1.0e+3; // [GeV/c] --> [MeV/c]
  beampy = pvecBeam.y() * 1.0e+3; // [GeV/c] --> [MeV/c]
  beampz = pvecBeam.z() * 1.0e+3; // [GeV/c] --> [MeV/c]
  evID2 = paramMan->GetEVID();  //Event ID
  G4ThreeVector pkmvec(0.0 , 0.0 , 0.0);
  G4ThreeVector pkmvec0(0.0 , 0.0 , 0.0);
  G4ThreeVector pkpvec(0.0 , 0.0 , 0.0);
  G4ThreeVector pkpvec0(0.0 , 0.0 , 0.0);
  if(SeedFlag==1 || SeedFlag==2 || SeedFlag==3){
    pkmvec  = paramMan->GetPKm();
    pkmvec0 = paramMan->GetPKm0();
    km_px   = pkmvec.x();
    km_py   = pkmvec.y();
    km_pz   = pkmvec.z();
    km_p    = sqrt(km_px*km_px+km_py*km_py+km_pz*km_pz);
    km_px0  = pkmvec0.x();
    km_py0  = pkmvec0.y();
    km_pz0  = pkmvec0.z();
    km_p0   = sqrt(km_px0*km_px0 + km_py0*km_py0 + km_pz0*km_pz0);
    km_px   = km_px * 1.0e+3;
    km_py   = km_py * 1.0e+3;
    km_pz   = km_pz * 1.0e+3;
    km_p    = km_p * 1.0e+3;
    km_px0  = km_px0 * 1.0e+3;
    km_py0  = km_py0 * 1.0e+3;
    km_pz0  = km_pz0 * 1.0e+3;
    km_p0   = km_p0 * 1.0e+3;
    if(SeedFlag==3){
      pkpvec  = paramMan->GetPKp();
      pkpvec0 = paramMan->GetPKp0();
      kp_px   = pkpvec.x();
      kp_py   = pkpvec.y();
      kp_pz   = pkpvec.z();
      kp_p    = sqrt(kp_px*kp_px+kp_py*kp_py+kp_pz*kp_pz);
      kp_px0  = pkpvec0.x();
      kp_py0  = pkpvec0.y();
      kp_pz0  = pkpvec0.z();
      kp_p0   = sqrt(kp_px0*kp_px0 + kp_py0*kp_py0 + kp_pz0*kp_pz0);
      kp_px   = kp_px * 1.0e+3;
      kp_py   = kp_py * 1.0e+3;
      kp_pz   = kp_pz * 1.0e+3;
      kp_p    = kp_p * 1.0e+3;
      kp_px0  = kp_px0 * 1.0e+3;
      kp_py0  = kp_py0 * 1.0e+3;
      kp_pz0  = kp_pz0 * 1.0e+3;
      kp_p0   = kp_p0 * 1.0e+3;
    }
  }
  //G4cout << km_p << endl;
  //G4cout << SeedFlag << " " << km_pz <<G4endl;
  //G4cout << SeedFlag << " " << km_pz0 <<G4endl;

  //mom = 0.0;
  pxVD = -2222.0;     //Momentum x [MeV]
  pyVD = -2222.0;     //Momentum y [MeV]
  pzVD = -2222.0;     //Momentum z [MeV]
  //G4cout << evID2 << " " << xBeam << " " << yBeam << " " << zBeam << G4endl;
  btrig = 1;

  colidTarget_X = SDMan -> GetCollectionID("TargetCollection_X");
  colidTarget_Y = SDMan -> GetCollectionID("TargetCollection_Y");
  colidVD     = SDMan -> GetCollectionID("VDCollection1");
  FiberTargetvdHitsCollection *vdHC;
  vdHC  = HCE ? (FiberTargetvdHitsCollection*)( HCE->GetHC(colidVD) ) : 0 ;
  
  TargetHitsCollection *tgtHC_X;
  tgtHC_X  = HCE ? (TargetHitsCollection*)( HCE->GetHC(colidTarget_X) ) : 0 ;
  
  TargetHitsCollection *tgtHC_Y;
  tgtHC_Y  = HCE ? (TargetHitsCollection*)( HCE->GetHC(colidTarget_Y) ) : 0 ;
  
 // 
 // for(int i=0 ; i<nn ; i++){
 //   xBeam[i] = -2222.0;
 //   yBeam[i] = -2222.0;
 //   zBeam[i] = -2222.0; // Beam Position
 //   pBeam[i] = -2222.0; // Beam Momentum
 //   pxBeam[i]= -2222.0; // Beam Momentum
 //   pyBeam[i]= -2222.0; // Beam Momentum
 //   pzBeam[i]= -2222.0; // Beam Momentum
 //   thBeam[i]= -2222.0; // Beam theta
 //   phBeam[i]= -2222.0; // Beam phi
 // }
 // 
  
  bool TargetHitFlag = false;
  for(int i=0 ; i<1000 ; i++){
    dE_fiberx[i]          = 0.0;
    dE_fibery[i]          =  0.0;
    dE_fiberx_kminus[i]   = 0.0;
    dE_fibery_kminus[i]   = 0.0;
    dE_fiberx_kplus[i]    = 0.0;
    dE_fibery_kplus[i]    = 0.0;
    dE_fiberx_xi[i]       = 0.0;
    dE_fibery_xi[i]       = 0.0;
    //time_fiber[i] = -2222.0;
    ///mom_fiber[i]  = 0.0;
  }
  
  if(tgtHC_X){
    G4int entTarget = tgtHC_X->entries();
    //G4cout << " entTarget(x)=" << entTarget << G4endl;
    //G4cout << " ----- "  << entTarget << " ------" << G4endl;
    for(int i=0 ; i<entTarget ; i++){
      TargetHit *tgtHit = (*tgtHC_X)[i];
      int fid = tgtHit->GetFiberID();
      //G4cout << " ----- "  << fid << " ------" << G4endl;
      double dE_this_fiber = tgtHit->GetEdep();
      dE_fiberx[fid] = dE_fiberx[fid] + dE_this_fiber;
      //G4cout << fid << ": " << dE_fiberx/MeV << " MeV" <<G4endl;
    }
  }
  
  if(tgtHC_Y){
    G4int entTarget = tgtHC_Y->entries();
    //G4cout << " entTarget(y)=" << entTarget << G4endl;
    //G4cout << " ----- "  << entTarget << " ------" << G4endl;
    for(int i=0 ; i<entTarget ; i++){
      TargetHit *tgtHit = (*tgtHC_Y)[i];
      int fid = tgtHit->GetFiberID();
      double dE_this_fiber = tgtHit->GetEdep();
      dE_fibery[fid] = dE_fibery[fid] + dE_this_fiber;
      //G4cout << fid << ": " << dE_fibery[fid]/MeV << " MeV" <<G4endl;
    }
  }
  
  // ------- K- hit fibers in the previous Monte Calro Sim. --------
  Int_t f_nhit=0;
  Int_t f_num;
  Double_t f_de;
  if(SeedFlag==2 || SeedFlag==3){
    // ~~~~  x fibers ~~~~~~ 
    f_nhit = paramMan->GetAlreadyFiberHitx_nhit();
    for(int j=0 ; j<f_nhit ; j++){
      f_num = paramMan->GetAlreadyFiberHitx_hitnum(j);
      f_de  = paramMan->GetAlreadyFiberHitx_dE(j);
      dE_fiberx_kminus[f_num] = f_de;
      //G4cout << " ------- " << f_num << " " << f_de << endl;
    }
    // ~~~~  y fibers ~~~~~~ 
    f_nhit = paramMan->GetAlreadyFiberHity_nhit();
    for(int j=0 ; j<f_nhit ; j++){
      f_num = paramMan->GetAlreadyFiberHity_hitnum(j);
      f_de  = paramMan->GetAlreadyFiberHity_dE(j);
      dE_fibery_kminus[f_num] = f_de;
    }
  }
  
  // ------- K+ hit fibers in the previous Monte Calro Sim. --------
  if(SeedFlag==3){
    f_nhit=0;
    f_num=0;
    f_de=0;
    // ~~~~  x fibers ~~~~~~ 
    f_nhit = paramMan->GetAlreadyFiberHitx2_nhit();
    for(int j=0 ; j<f_nhit ; j++){
      f_num = paramMan->GetAlreadyFiberHitx2_hitnum(j);
      f_de  = paramMan->GetAlreadyFiberHitx2_dE(j);
      dE_fiberx_kplus[f_num] = f_de;
      //G4cout << " ------- " << f_num << " " << f_de << endl;
    }
    // ~~~~  y fibers ~~~~~~ 
    f_nhit = paramMan->GetAlreadyFiberHity2_nhit();
    for(int j=0 ; j<f_nhit ; j++){
      f_num = paramMan->GetAlreadyFiberHity2_hitnum(j);
      f_de  = paramMan->GetAlreadyFiberHity2_dE(j);
      dE_fibery_kplus[f_num] = f_de;
    }
  }
  
  for(int i=0 ; i<1000 ; i++){
    // ~~~~ Fiber target flag ~~~~~~~~~
    if(dE_fiberx[i]>0.0 || dE_fibery[i]>0.0) TargetHitFlag = true;
    
    // ~~~~ 
    if(SeedFlag==2 ){
      dE_fiberx_kplus[i] = dE_fiberx[i];
      dE_fibery_kplus[i] = dE_fibery[i];
//      if(dE_fiberx[i]>0){
//	G4cout << "!!!!!!! " << dE_fiberx[i]// << " " << dE_fibery[i]
//	       << G4endl;
//      }
      dE_fiberx[i] = dE_fiberx_kplus[i] + dE_fiberx_kminus[i];
      dE_fibery[i] = dE_fibery_kplus[i] + dE_fibery_kminus[i];
    }
    if(SeedFlag==3 ){
      dE_fiberx_xi[i] = dE_fiberx[i];
      dE_fibery_xi[i] = dE_fibery[i];
//      if(dE_fiberx[i]>0){
//	G4cout << "!!!!!!! " << dE_fiberx[i]// << " " << dE_fibery[i]
//	       << G4endl;
//      }
      dE_fiberx[i] = dE_fiberx_kplus[i] 
	+ dE_fiberx_kminus[i]
	+ dE_fiberx_xi[i];
      dE_fibery[i] = dE_fibery_kplus[i] 
	+ dE_fibery_kminus[i]
	+ dE_fibery_xi[i];
    }
  }
  
  if(vdHC){
    //G4ThreeVector bpos(0.0,0.0,0.0); //Beam position
    //G4ThreeVector pos;//Hit position on VD
    //G4ThreeVector mom;//Momentum vector of the particle
    G4int entVD = vdHC->entries();
    //G4cout << "entries() " <<entVD <<G4endl;
    //G4cout << " entVD=" << entVD << G4endl;
    //xBeam  =  -2222.0 ; yBeam= -2222.0 ; zBeam= -2222.0 ;
    //pBeam  =  -2222.0 ;
    //bpos   = paramMan -> GetBeamPos();
    //xBeam  = bpos.x();
    //yBeam  = bpos.y();
    //zBeam  = bpos.z();
    //pBeam  = paramMan -> GetBeamEnergy();
    //pBeam  = paramMan -> GetGenP();
    //pBeam  = pBeam*1000.0; // GeV --> MeV
    //cout << entVD << endl;
 
    // oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
    // Only one evnent will be stored when SeedFlag==1
    if(SeedFlag==1 || SeedFlag==2 || SeedFlag==3){ 
      if(entVD!=0) entVD=1;
    }
    // oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO
 
    for(G4int j=0 ; j<entVD ; j++){
      //Initializing
      beamx = -2222.0;
      beamy = -2222.0 ;
      beamz = -2222.0 ;
      beamz_recon = -2222.0 ;
      beamp =  -2222.0 ;
      //beampx=  -2222.0 ;
      //beampy=  -2222.0 ;
      //beampz=  -2222.0 ;
      xVD   =  -2222.0 ; yVD  = -2222.0 ; zVD  = -2222.0 ;
      pxVD  =  -2222.0 ; pyVD = -2222.0 ; pzVD = -2222.0 ;
      pVD   =  -2222.0 ;
      thetaVD=  -2222.0 ;
      phiVD  =  -2222.0 ;
      chargeVD= -2222.0 ;
      piflagVD  = 0; 
      posflagVD = 0;
      eleflagVD = 0;
      phoflagVD = 0;
      proflagVD = 0;
      kaoflagVD = 0;
      pioflagVD = 0;
      muoflagVD = 0;
      neuflagVD = 0;
      passthVD  = 0;
      eIoniVD   = 0;
      eBremVD   = 0;
      comptVD   = 0;
      phoeleVD  = 0;
      convVD    = 0;
   
      //bpos   = paramMan -> GetBeamPos();
      //xBeam  = bpos.x();
      //yBeam  = bpos.y();
      //zBeam  = bpos.z();
      //pBeam  = paramMan -> GetGenP();
      //thBeam = paramMan -> GetGenTheta();
      //phBeam = paramMan -> GetGenPhi();
      //pvecBeam = paramMan ->GetGenPvec();
      //beampx = pvecBeam.x() * 1.0e+3; // [GeV/c] --> [MeV/c]
      //beampy = pvecBeam.y() * 1.0e+3; // [GeV/c] --> [MeV/c]
      //beampz = pvecBeam.z() * 1.0e+3; // [GeV/c] --> [MeV/c]
   
      FiberTargetvdHit *vdHit = (*vdHC)[j];
      beamx= xBeam;
      beamy= yBeam;
      beamz= zBeam;
      beamz_recon = G4RandGauss::shoot( beamz , zreconreso );
      beamp= pBeam*1000.0;//GeV--> MeV
      beam_theta = thBeam;
      beam_phi   = phBeam;
      pos  = vdHit->GetPos(); //Get hit position on Virtual detector
      xVD  = pos.x();      //x on VD [cm]
      yVD  = pos.y();      //y on VD [cm]
      zVD  = pos.z();      //z on VD [cm]
      mom  = vdHit->GetMom(); //Get momentum on Virtual detector
      pxVD = mom.x();     //Momentum x [MeV]
      pyVD = mom.y();     //Momentum y [MeV]
      pzVD = mom.z();     //Momentum z [MeV]
      pVD  = sqrt( pxVD*pxVD + pyVD*pyVD + pzVD*pzVD );//Momentum [MeV]
      thetaVD = acos( pzVD/pVD  ); // Theta
      //phiVD   = atan( pyVD/pxVD ); // Phi
      phiVD   = atan2( pyVD,pxVD ); // Phi
      chargeVD= vdHit->GetCharge();// Charge
      pname = vdHit->GetPname();       //Get particle name
      if(pname=="e+")    posflagVD = 1;//positron  flag
      else posflagVD = 0;
      if(pname=="e-")    eleflagVD = 1;//electron flag
      else eleflagVD = 0;
      if(pname=="gamma") phoflagVD = 1;//photon flag
      else phoflagVD = 0;
      if(pname=="proton")proflagVD = 1;//proton flag
      else proflagVD = 0;
      if(pname=="kaon+" || pname=="kaon-" || pname =="kaon0") kaoflagVD = 1;//kaon flag
      else kaoflagVD = 0;
      if(pname=="pi+" || pname=="pi-" || pname =="pi0") pioflagVD = 1;//pion flag
      else pioflagVD = 0;
      if(pname=="mu-" || pname=="mu+") muoflagVD = 1;//muon flag
      else muoflagVD = 0;
      if(pname=="neutron" || pname=="anti_neutron") neuflagVD = 1;//neutron flag
      else neuflagVD = 0;
      ProcID = vdHit -> GetProcID();   //Get Process ID
      if(ProcID == 0)passthVD= 1;      //Beam passing through the target
      else passthVD= 0;
      if(ProcID == 1)eIoniVD = 1;      //electron ionization
      else eIoniVD = 0;
      if(ProcID == 2)eBremVD = 1;      //electron brems
      else eBremVD = 0;
      if(ProcID == 3)comptVD = 1;      //compton scattering
      else comptVD = 0;
      if(ProcID == 4)phoeleVD= 1;      //photoelectric effect
      else phoeleVD= 0;
      if(ProcID == 5)convVD  = 1;      //photon conversion
      else convVD  = 0;

//      cout << "pname(ProcID): " << pname 
//	   <<"(" << ProcID << "): " << j << endl;

      // Fill data to tree
      //evID = primaryEventID;  //Event ID
      //trackID = event_id;     //Track ID
      evID  = event_id;  //Event ID
      //evID2 = paramMan->GetEVID();  //Event ID      
      
      x   =   xVD ;     //x on VD [cm]
      y   =   yVD ;     //y on VD [cm]
      z   =   zVD ;     //z on VD [cm]
      px  =   pxVD;     //Momentum x [GeV/c]
      py  =   pyVD;     //Momentum y [GeV/c]
      pz  =   pzVD;     //Momentum z [GeV/c]
      p   =   pVD ;     //Momentum   [GeV/c]
      theta = thetaVD;  //Theta
      if(phiVD<3.14159/2.){
	phiVD = phiVD;
      }
      else if (phiVD<3.14159*3.0/2.0){
	phiVD = phiVD + 3.14159;
      }
      else{
	phiVD = phiVD + 2.0*3.14159;
      }
      phi   = phiVD;    //Phi
      charge = chargeVD;//Charge
      posflag = posflagVD;//positron flag
      eleflag = eleflagVD;//electron flag
      phoflag = phoflagVD;//photon flag
      proflag = proflagVD;//proton flag
      kaoflag = kaoflagVD;//proton flag
      pioflag = pioflagVD;//pion flag
      muoflag = muoflagVD;//muon flag
      passth  = passthVD; //Beam passing through the target
      eIoni   = eIoniVD;  //electron ionization
      eBrem   = eBremVD;  //electron brems
      compt   = comptVD;  //compton scattering
      phoele  = phoeleVD; //photoelectric effect
      conv    = convVD;   //photon conversion
      
      //if( minTheta/1000.0 < theta && theta < maxTheta/1000.0 ){ //Theta cut
      //	if( minMom < p && p < maxMom ){                         //Momentum cut
      //	  if( PosiAnalFlag==0 ){
      //	    tree->Fill();  //Filling data for all particles
      //	  }// if 
      //	  else{
      //	    if(posflag == 1)tree->Fill(); //Filling data for only positrons
      //	  }
      //	}//if
      //}//if
      
    }//for
  }//if
  
  if(TargetHitFlag == true)tree->Fill();  //Filling data for all particles
  trackID++;

//G4cout << evID2 << " " << xBeam << " " << yBeam << " " << zBeam << G4endl;
//tree->Fill();  //Filling data for all particles(original)
//G4cout << pname << G4endl;
btrig = 0;
  
}
//==========================================BOTTOM//

