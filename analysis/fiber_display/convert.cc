/*
  convert.cc
  Toshiyuki Gogami, 2015/10/9
*/

#include<TView3D.h>
#include<TGeoManager.h>
#include<TGeoMaterial.h>
#include<TGeoMedium.h>
#include<TGeoVolume.h>
#include<TGeoMatrix.h>
#include<TGeoTube.h>
#include<TVirtualGeoTrack.h>

//void display_kminus(){
void convert(){
  // ========= General conditions =============
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  
  //
  //gSystem->Load("libGeom");
  //TGeoManager* man = new TGeoManager("Fiber_target","Fiber_target");
  //TGeoMaterial *mat=new TGeoMaterial("Vacuum",0,0,0);
  //TGeoMedium *med =new TGeoMedium("Vacumm",1,mat);
  //TGeoVolume *top =man->MakeBox("TOP",med,150./2.,50./2.,100./2.);
  //TGeoVolume *top =man->MakeBox("TOP",med,140./2.,50./2.,30./2.);
  
  // ======= Hit fibers =============
  //TGeoTube *tube_w =new TGeoTube("TUBE_hitwire",0,0.05,50/2.);
  //TGeoVolume *hitF =new TGeoVolume("HITF" ,tube_w,med);
  //TGeoVolume *hitWS=new TGeoVolume("HITFS",tube_w,med);

  const int nfibers = 1000;
  //TGeoRotation r1,r2;
  //r1.SetAngles(  0,90,0);
  //r2.SetAngles( 90,90,0);

  //TFile* f1 = new TFile("test.root");
  //TFile* f1 = new TFile("../fiber_12Xi-1.root");
  TFile* f1 = new TFile("../kbeam.root");
  TTree* t1 = (TTree*)f1->Get("tree");
  //Double_t de[nfibers];
  //double ok[nfibers];
  Double_t dex[1000], dex_new[1000];
  Double_t dey[1000], dey_new[1000];
  double okx[1000];
  double oky[1000];
  //double genz;
  const double de_th = 0.000001; // [MeV]
  Int_t evID, trackID;
  Double_t beamx, beamy, beamz;
  Double_t beamz_recon;
  Double_t beampx, beampy, beampz;;
  Double_t beamp;
  Double_t beam_theta;
  Double_t beam_phi;
  Double_t x, y, z, p;
  Double_t px, py, pz;
  Double_t theta, phi;
  Double_t charge;
  Int_t posflag, eleflag, phoflag, proflag;
  Int_t kaoflag, neuflag, pioflag, muoflag;
  Double_t ent;
  Int_t count = 0;
  ent = t1->GetEntries();
  t1->SetBranchAddress("eventid" ,  &evID );
  t1->SetBranchAddress("trackID",&trackID);
  t1->SetBranchAddress("xBeam",  &beamx);
  t1->SetBranchAddress("yBeam",  &beamy);
  t1->SetBranchAddress("zBeam",  &beamz);
  t1->SetBranchAddress("zBeamR", &beamz_recon);
  t1->SetBranchAddress("pxBeam", &beampx);
  t1->SetBranchAddress("pyBeam", &beampy);
  t1->SetBranchAddress("pzBeam", &beampz);
  t1->SetBranchAddress("pBeam",  &beamp );
  t1->SetBranchAddress("thetaBeam", &beam_theta);
  t1->SetBranchAddress("phiBeam", &beam_phi);
  t1->SetBranchAddress("xVD"  ,  &x );
  t1->SetBranchAddress("yVD"  ,  &y );
  t1->SetBranchAddress("zVD"  ,  &z );
  t1->SetBranchAddress("pxVD" ,  &px);
  t1->SetBranchAddress("pyVD" ,  &py);
  t1->SetBranchAddress("pzVD" ,  &pz);
  t1->SetBranchAddress("pVD"  ,  &p );
  t1->SetBranchAddress("thetaVD",  &theta );
  t1->SetBranchAddress("phiVD"  ,  &phi   );
  t1->SetBranchAddress("charge" , &charge );
  t1->SetBranchAddress("posflag", &posflag);
  t1->SetBranchAddress("eleflag", &eleflag);
  t1->SetBranchAddress("phoflag", &phoflag);
  t1->SetBranchAddress("proflag", &proflag);
  t1->SetBranchAddress("kaoflag", &kaoflag);
  t1->SetBranchAddress("neuflag", &neuflag);
  t1->SetBranchAddress("pioflag", &pioflag);
  t1->SetBranchAddress("muoflag", &muoflag);
  t1->SetBranchAddress("dE_fiberx",&dex);
  t1->SetBranchAddress("dE_fibery",&dey);
  //t1->SetBranchAddress("zBeam",&genz);
  //double ent = t1->GetEntries();
  
  // ====== Create new ROOT file ===============
  TFile* fnew = new TFile("fiber_12Xi-2.root","recreate");
  TTree* tnew = new TTree("tree","K- beam");
  tnew -> Branch("eventid" ,  &evID   , "eventid/I" );
  tnew -> Branch("trackID",&trackID   , "trackID/I");
  tnew -> Branch("xBeam",  &beamx     , "xBeam/D");
  tnew -> Branch("yBeam",  &beamy     , "yBeam/D");
  tnew -> Branch("zBeam",  &beamz     , "zBeam/D");
  tnew -> Branch("zBeamR", &beamz_recon, "zBeamR/D");
  tnew -> Branch("pxBeam", &beampx    , "pxBeam/D");
  tnew -> Branch("pyBeam", &beampy    , "pyBeam/D");
  tnew -> Branch("pzBeam", &beampz    , "pzBeam/D");
  tnew -> Branch("pBeam",  &beamp     , "pBeam/D");
  tnew -> Branch("thetaBeam", &beam_theta , "thetaBeam/D");
  tnew -> Branch("phiBeam", &beam_phi , "phiBeam/D");
  tnew -> Branch("xVD"  ,  &x         , "xVD/D");
  tnew -> Branch("yVD"  ,  &y         , "yVD/D");
  tnew -> Branch("zVD"  ,  &z         , "zVD/D");
  tnew -> Branch("pxVD" ,  &px        , "pxVD/D");
  tnew -> Branch("pyVD" ,  &py        , "pyVD/D");
  tnew -> Branch("pzVD" ,  &pz        , "pzVD/D");
  tnew -> Branch("pVD"  ,  &p         , "pVD/D");
  tnew -> Branch("thetaVD",  &theta   , "thetaVD/D");
  tnew -> Branch("phiVD"  ,  &phi     , "phiVD/D");
  tnew -> Branch("charge" , &charge   , "charge/D");
  tnew -> Branch("posflag", &posflag  , "posflag/I");
  tnew -> Branch("eleflag", &eleflag  , "eleflag/I");
  tnew -> Branch("phoflag", &phoflag  , "phoflag/I");
  tnew -> Branch("proflag", &proflag  , "proflag/I");
  tnew -> Branch("kaoflag", &kaoflag  , "kaoflag/I");
  tnew -> Branch("neuflag", &neuflag  , "neuflag/I");
  tnew -> Branch("pioflag", &pioflag  , "pioflag/I");
  tnew -> Branch("muoflag", &muoflag  , "muoflag/I");
  tnew -> Branch("dE_fiberx",&dex     , "dE_fiberx[1000]/D");
  tnew -> Branch("dE_fibery",&dey     , "dE_fibery[1000]/D");
  tnew -> Branch("dE_fiberx_kminus",&dex_new,"dE_fiberx_kminus[1000]/D");
  tnew -> Branch("dE_fibery_kminus",&dey_new,"dE_fibery_kminus[1000]/D");
  
  double nx = 50;
  double ny = 17;
  double fiber_phi = 3.0; // [mm]
  //double x,y,z;
  //double fiber_startx  = -1.0 * 3.0 * (50.0 -1.0)/2.0;
  double fiber_startx  = -1.0 * fiber_phi * (nx -1)/2.0;
  double fiber_starty  = -1.0 * fiber_phi * (ny -1)/2.0;
  double nset = 9.0;
  //double startxp = -1.0 * (50.0 * 3.0)/2.0 + 3/2.0;
  double min=0.0,max=0.0;
  double min2=0.0,max2=0.0;
  double dz1 = fiber_phi/2.0 * sqrt(3.0);
  double dz2 = fiber_phi;
  //double dz_oneset = fiber_phi + fiber_phi/2.0*sqrt(3.0);
  //double fiber_startz = 2.0 * (fiber_phi+fiber_phi/2.0*sqrt(3.0)) * nset;
  double dz_oneset = 2.0 * (dz1 + dz2);
  double fiber_startz = dz_oneset * nset;
  fiber_startz = -1.0 * fiber_startz / 2.0 + fiber_phi/2.0;
  int gen_block=0;
  
  //TCanvas* c1 = new TCanvas("c1","c1",800,700);
  //c1->Divide(2,1);
  TCanvas* c1; // for xz plane
  //TCanvas* c2; // for yz plane
  TH2F* h_empx = new TH2F("h_empx","",100,-100.0,100.0,100,-100.0,100.0);
  TH2F* h_empy = new TH2F("h_empy","",100,-100.0,100.0,100,-100.0,100.0);
  TEllipse* hitfiber[1000];
  TEllipse* hitfiber2[1000];
  TEllipse* hitfiber_converted[1000];
  //TEllipse* hitfiber2[1000];
  //TBox* box_xz = new TBox(-150.0/2.0,-100.8/2.0,150.0/2.0,100.8/2.0);
  TBox* box_xz = new TBox(-75.0,-50.4,75.0,50.4);
  //box_xz->SetFillColor(0);
  box_xz->SetFillStyle(0);
  box_xz->SetLineColor(1);
  box_xz->SetLineStyle(7);
  //h_empx->Draw();
  TBox* box_yz = new TBox(-25.5,-50.4,25.5,50.4);
  //box_xz->SetFillColor(0);
  box_yz->SetFillStyle(0);
  box_yz->SetLineColor(1);
  box_yz->SetLineStyle(7);
  //h_empx->Draw();
  
  int fiber_conv_x  = 0;
  int fiber_conv_xp = 0;
  int fiber_conv_y  = 0;
  int fiber_conv_yp = 0;
  bool fillflag = false;

  cout << "Opened an input ROOT file: " << f1->GetName() << endl;
  cout << "--> Will create new ROOT file: " << fnew->GetName() << endl;
  cout << "  Wait.... (loop:" << ent << ")" << endl;
  for(int i=0 ; i<ent ; i++){

    t1->GetEntry(i);
    beamz = beamz * 10.0; // [cm] --> [mm]
    
    //cout << " Generation point (z) = " << beamz << " mm" << endl;
    //fiber_startz = -1.0 * fiber_startz + fiber_phi/2.0;
    fillflag=false;
    for(int k=0 ; k<nset ; k++){
      if( (dz_oneset*(k)+fiber_startz-fiber_phi/2.0<beamz 
	   && beamz<dz_oneset*(k+1)+ fiber_startz - fiber_phi/2.0
	   || dz_oneset*(k)+fiber_startz-fiber_phi/2.0==beamz 
	   || beamz==dz_oneset*(k+1)+ fiber_startz - fiber_phi/2.0)
	  && kaoflag
	  ){
	//      if(dz_oneset*(k)+fiber_startz-fiber_phi/2.0<beamz 
//	 && beamz<dz_oneset*(k+0.5)+ fiber_startz - fiber_phi/2.0 ){
	// <-- Choose a particle generation in xx' fibers
	
	//cout << " Layer: " << k << endl;
	gen_block=k;
	fiber_conv_x = gen_block * 2 * nx;
	fiber_conv_y = gen_block * 2 * ny;
	fillflag = true;
	//fiber_conv_x = gen_block * 
//	cout << " Layer: " << k 
//	     << " " << fiber_conv_x << " " 
//	     << " " << fiber_conv_y << endl;
	
      }
      //else fillflag = false;
    }
    
    
    //if(c1) c1->Clear();
    //c1 = new TCanvas("c1","c1",850,600);
    //c1 -> Divide(2,1);
    //c1->Clear();
    
    //c1->cd(1);h_empx->Draw();
    //c1->cd(1);box_xz->Draw();
    //c1->cd(2);h_empy->Draw();
    //c1->cd(2);box_yz->Draw();
    
    //h_empx->Clear();
    //h_empx->Draw();
    //cout << "----"  << i << "-----" << endl;
    for(int nf=0 ; nf<nfibers ; nf++){
      dex_new[nf] = 0.0;
      dey_new[nf] = 0.0;
      
      if(dex[nf]>de_th) okx[nf] = 1.0;
      else okx[nf] = 0.0;
      
      if(dey[nf]>de_th) oky[nf] = 1.0;
      else oky[nf] = 0.0;
      
      if( okx[nf]==1.0 ){
	for(int j=0 ; j<nset ; j++){
	  // ----- x --------
	  //min = 2.0*(nx+ny)*j;
	  min = 2.0*(nx)*j;
	  max = min + nx;
	  if(min-1<nf && nf<max){
	    //x = fiber_startx + fiber_phi * nf;
	    x = fiber_startx + fiber_phi*(nf-min);
	    y = 0.0;
	    z = fiber_startz + dz_oneset*j;
//	    cout << "x-"<< j << "-" << nf
//		 <<": (min,max=" << min << "," << max << ")"
//		 <<" (" << x << "," << y << "," << z
//		 << ")" << endl;

	    hitfiber[nf] = new TEllipse(x,z,fiber_phi/2.0,fiber_phi/2.0);
	    //c1->cd(1);  hitfiber[nf] ->Draw();
	    
	    // --- Fill energy loss information into converted number of fibers
	    //cout << " "<< nf << " " << fiber_conv_x << endl;
	    if(nf-fiber_conv_x>0 && fillflag==true){
	      dex_new[nf-fiber_conv_x] = dex[nf];
	    }
	    //else  dex_new[nf-fiber_conv_x] = 0.0;
	    //dey_new[nf-min] = dey[nf];
	  }
	  // ----- x' --------
	  min = max;
	  max = min + nx;
	  //cout << "x': " << min << " " << max << endl;
	  //if(nx-1.0<nf && nf<2.0*nx){
	  if(min-1<nf && nf<max){
	    //x = fiber_phi/2.0 + fiber_startx + fiber_phi * nf;
	    x = fiber_phi/2.0 + fiber_startx + fiber_phi*(nf-min);
	    y = 0;
	    z = fiber_startz + dz_oneset*j + dz1;
//	    cout << "x'-"<< j << "-" << nf
//		 <<": (min,max=" << min << "," << max << ")"
//		 <<" (" << x << "," << y << "," << z
//		 << ")" << endl;
	    hitfiber[nf] = new TEllipse(x,z,fiber_phi/2.0,fiber_phi/2.0);
	    //c1->cd(1);  hitfiber[nf] ->Draw();
	    // --- Fill energy loss information into converted number of fibers
	    if(nf-fiber_conv_x>0 && fillflag==true){
	      dex_new[nf-fiber_conv_x] = dex[nf];
	    }
	    //dex_new[nf-fiber_conv_x] = dex[nf];
	    //dey_new[nf-min] = dey[nf];
	  }
	} 
      } // ok for x hit
	
      //c1->cd(2);
      if(oky[nf]==1.0){
	for(int j=0 ; j<nset ; j++){
	  // ----- y --------
	  min = 2.0*(ny)*j;
	  max = min + ny;
	  if(min-1<nf && nf<max){
	    x = 0.0;
	    y = fiber_starty + fiber_phi*(nf-min);
	    z = fiber_startz + dz_oneset*(j+0.5);
//	    cout << "y-"<< j << "-" << nf
//		 <<": (min,max=" << min << "," << max << ")"
//		 <<" (" << x << "," << y << "," << z
//		 << ")" << endl;

	    hitfiber2[nf] = new TEllipse(y,z,fiber_phi/2.0,fiber_phi/2.0);
	    //c1->cd(2);hitfiber2[nf] ->Draw();
	    // --- Fill energy loss information into converted number of fibers
	    if(nf-fiber_conv_y>0 && fillflag==true){
	      dey_new[nf-fiber_conv_y] = dey[nf];
	    }
	    //dex_new[nf-min] = dex[nf];
	    //dey_new[nf-fiber_conv_y] = dey[nf];
	  }
	  // ----- y' --------
	  min = max;
	  max = min + ny;
	  if(min-1<nf && nf<max){
	    x = 0.0;
	    y = fiber_phi/2.0 + fiber_starty + fiber_phi*(nf-min);
	    z = fiber_startz + dz_oneset*(j+0.5) + dz1;
//	    cout << "y'-"<< j << "-" << nf
//		 <<": (min,max=" << min << "," << max << ")"
//		 <<" (" << x << "," << y << "," << z
//		 << ")" << endl;
	    hitfiber2[nf] = new TEllipse(y,z,fiber_phi/2.0,fiber_phi/2.0);
	    //c1->cd(2);hitfiber2[nf] ->Draw();
	    // --- Fill energy loss information into converted number of fibers
	    if(nf-fiber_conv_y>0 && fillflag==true){
	      dey_new[nf-fiber_conv_y] = dey[nf];
	    }
	    //dex_new[nf-min] = dex[nf];
	    //dey_new[nf-fiber_conv_y] = dey[nf];
	  } // y'
	} // loop for nset times 
      } // ok for y hit
    } // loop for (Number of fibers)
    
    //tnew->Fill();
    if(fillflag==true) tnew->Fill();
    //c1->Update();
    //c1->WaitPrimitive();
  } // loop for (Total entries in ROOT file)
  
  tnew->Write();
  fnew->Close();
  
}
