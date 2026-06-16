/*
  display2.cc
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

void display3(){
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
  
  // ####### Do you want to print out ?? ###########
  bool print=false;
  //bool print=true;
  // ################################################

  const int nfibers = 1000;
  //TGeoRotation r1,r2;
  //r1.SetAngles(  0,90,0);
  //r2.SetAngles( 90,90,0);

  ////bool kminusFlag = true;
  //TFile* f1 = new TFile("test.root");
  //TFile* f1 = new TFile("../test_kk_xi.root");
  //TFile* f1 = new TFile("../fiber_12Xi-4.root");
  TFile* f1 = new TFile("fiber_12Xi-4.root"); // Sample ROOT file
  //TFile* f1 = new TFile("../fiber_12Xi-4_.root");
  //TFile* f1 = new TFile("test_new.root"); // Created ROOT file
  TTree* t1 = (TTree*)f1->Get("tree");
  //Double_t de[nfibers];
  //double ok[nfibers];
  Double_t dex[1000];
  Double_t dey[1000];
  Double_t dex_km[1000];
  Double_t dey_km[1000];
  Double_t dex_kp[1000];
  Double_t dey_kp[1000];
  Double_t dex_xi[1000];
  Double_t dey_xi[1000];
  double okx[1000];
  double oky[1000];
  double x,y,z;
  const double de_th = 0.1; // [MeV]
  t1->SetBranchAddress("zBeam",&z);
  t1->SetBranchAddress("xBeam",&x);
  t1->SetBranchAddress("yBeam",&y);
  t1->SetBranchAddress("dE_fiberx",&dex);
  t1->SetBranchAddress("dE_fibery",&dey);
  t1->SetBranchAddress("dE_fiberx_xi",&dex_xi);
  t1->SetBranchAddress("dE_fibery_xi",&dey_xi);
  t1->SetBranchAddress("dE_fiberx_kminus",&dex_km);
  t1->SetBranchAddress("dE_fibery_kminus",&dey_km);
  t1->SetBranchAddress("dE_fiberx_kplus",&dex_kp);
  t1->SetBranchAddress("dE_fibery_kplus",&dey_kp);
  //t1->SetBranchAddress("dE_fiberx_kplus",&dex_xi); // for test
  //t1->SetBranchAddress("dE_fibery_kplus",&dey_xi); // for test

  double ent = t1->GetEntries();
  if(print==true) ent = 100;
  ent = 100;
  
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
  
  //TCanvas* c1 = new TCanvas("c1","c1",800,700);
  //c1->Divide(2,1);
  TCanvas* c1; // for xz plane
  //TCanvas* c2; // for yz plane
  TH2F* h_empx = new TH2F("h_empx","",100,-100.0,100.0,100,-100.0,100.0);
  TH2F* h_empy = new TH2F("h_empy","",100,-100.0,100.0,100,-100.0,100.0);
  TEllipse* hitfiber[1000];
  TEllipse* hitfiber2[1000];
  TEllipse* hitfiber_km[1000];
  TEllipse* hitfiber2_km[1000];
  TEllipse* hitfiber_kp[1000];
  TEllipse* hitfiber2_kp[1000];
  TEllipse* hitfiber_xi[1000];
  TEllipse* hitfiber2_xi[1000]; 
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

  TMarker* genpoint;
  TMarker* genpoint2;
  bool first_xxp_check = false;
  //bool print=true;
  
  int countp=0;
  //ent = 3;
  for(int i=0 ; i<ent ; i++){

    t1->GetEntry(i);
    
    //if(c1) c1->Clear();
    c1 = new TCanvas("c1","c1",850,600);
    c1 -> Divide(2,1);
    //c1->Clear();
    
    genpoint  = new TMarker(x*10.,z*10.,25);
    genpoint2 = new TMarker(y*10.,z*10.,25);
    
    c1->cd(1);h_empx->Draw();
    c1->cd(1);box_xz->Draw();
    c1->cd(1);genpoint->Draw();
    c1->cd(2);h_empy->Draw();
    c1->cd(2);box_yz->Draw();
    c1->cd(2);genpoint->Draw();
    
    //h_empx->Clear();
    //h_empx->Draw();
    //cout << "----"  << i << "-----" << endl;
    
    first_xxp_check = false;
    for(int j=0 ; j<1000 ; j++){
      if(dex[j]>de_th){
	if(-1<j && j<100){
	  first_xxp_check = true;
	}
      }
    }
    
    if(first_xxp_check==true){
      
      for(int nf=0 ; nf<nfibers ; nf++){
	
	// ~~~~~~~~~~~ ALL ~~~~~~~~~~~~~~~~~~~~
	//if(dex[nf]>de_th) okx[nf] = 1.0;
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
	      cout << "x-"<< j << "-" << nf
		   <<": (min,max=" << min << "," << max << ")"
		   <<" (" << x << "," << y << "," << z
		   << ")" << endl;
	      
	      hitfiber[nf] = new TEllipse(x,z,fiber_phi/2.0,fiber_phi/2.0);
	      c1->cd(1);  hitfiber[nf] ->Draw();
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
	      cout << "x'-"<< j << "-" << nf
		   <<": (min,max=" << min << "," << max << ")"
		   <<" (" << x << "," << y << "," << z
		   << ")" << endl;
	      hitfiber[nf] = new TEllipse(x,z,fiber_phi/2.0,fiber_phi/2.0);
	      c1->cd(1);  hitfiber[nf] ->Draw();
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
	      cout << "y-"<< j << "-" << nf
		   <<": (min,max=" << min << "," << max << ")"
		   <<" (" << x << "," << y << "," << z
		   << ")" << endl;
	      
	      hitfiber2[nf] = new TEllipse(y,z,fiber_phi/2.0,fiber_phi/2.0);
	      c1->cd(2);hitfiber2[nf] ->Draw();
	    }
	    // ----- y' --------
	    min = max;
	    max = min + ny;
	    if(min-1<nf && nf<max){
	      x = 0.0;
	      y = fiber_phi/2.0 + fiber_starty + fiber_phi*(nf-min);
	      z = fiber_startz + dz_oneset*(j+0.5) + dz1;
	      cout << "y'-"<< j << "-" << nf
		   <<": (min,max=" << min << "," << max << ")"
		   <<" (" << x << "," << y << "," << z
		   << ")" << endl;
	      hitfiber2[nf] = new TEllipse(y,z,fiber_phi/2.0,fiber_phi/2.0);
	      c1->cd(2);hitfiber2[nf] ->Draw();
	    } // y'
	  } // loop for nset times 
	} // ok for y hit
	// ~~~~~~~~~~~ K- ~~~~~~~~~~~~~~~~~~~~
	//if(dex[nf]>de_th) okx[nf] = 1.0;
	if(dex_km[nf]>de_th) okx[nf] = 1.0;
	else okx[nf] = 0.0;
	
	if(dey_km[nf]>de_th) oky[nf] = 1.0;
	else oky[nf] = 0.0;
	
	if( okx[nf]==1.0 ){
	  for(int j=0 ; j<nset ; j++){
	    // ----- x --------
	    min = 2.0*(nx)*j;
	    max = min + nx;
	    if(min-1<nf && nf<max){
	      x = fiber_startx + fiber_phi*(nf-min);
	      y = 0.0;
	      z = fiber_startz + dz_oneset*j;
	      //	      cout << "x-"<< j << "-" << nf
	      //		   <<": (min,max=" << min << "," << max << ")"
	      //		   <<" (" << x << "," << y << "," << z
	      //		   << ")" << endl;
	      hitfiber_km[nf] = new TEllipse(x,z,fiber_phi/2.0,fiber_phi/2.0);
	      hitfiber_km[nf]->SetLineColor(9);
	      c1->cd(1);  hitfiber_km[nf] ->Draw();
	    }
	    // ----- x' --------
	    min = max;
	    max = min + nx;
	    if(min-1<nf && nf<max){
	      x = fiber_phi/2.0 + fiber_startx + fiber_phi*(nf-min);
	      y = 0;
	      z = fiber_startz + dz_oneset*j + dz1;
	      //	      cout << "x'-"<< j << "-" << nf
	      //		   <<": (min,max=" << min << "," << max << ")"
	      //		   <<" (" << x << "," << y << "," << z
	      //		   << ")" << endl;
	      hitfiber_km[nf] = new TEllipse(x,z,fiber_phi/2.0,fiber_phi/2.0);
	      hitfiber_km[nf]->SetLineColor(9);
	      c1->cd(1);  hitfiber_km[nf] ->Draw();
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
	      //	      cout << "y-"<< j << "-" << nf
	      //		   <<": (min,max=" << min << "," << max << ")"
	      //		   <<" (" << x << "," << y << "," << z
	      //		   << ")" << endl;
	      hitfiber2_km[nf] = new TEllipse(y,z,fiber_phi/2.0,fiber_phi/2.0);
	      hitfiber2_km[nf]->SetLineColor(9);
	      c1->cd(2);hitfiber2_km[nf] ->Draw();
	    }
	    // ----- y' --------
	    min = max;
	    max = min + ny;
	    if(min-1<nf && nf<max){
	      x = 0.0;
	      y = fiber_phi/2.0 + fiber_starty + fiber_phi*(nf-min);
	      z = fiber_startz + dz_oneset*(j+0.5) + dz1;
	      //	      cout << "y'-"<< j << "-" << nf
	      //		   <<": (min,max=" << min << "," << max << ")"
	      //		   <<" (" << x << "," << y << "," << z
	      //		   << ")" << endl;
	      hitfiber2_km[nf] = new TEllipse(y,z,fiber_phi/2.0,fiber_phi/2.0);
	      hitfiber2_km[nf]->SetLineColor(9);
	      c1->cd(2);hitfiber2_km[nf] ->Draw();
	    } // y'
	  } // loop for nset times 
	} // ok for y hit
	
	// ~~~~~~~~~~~ K+ ~~~~~~~~~~~~~~~~~~~~
	//if(dex[nf]>de_th) okx[nf] = 1.0;
	if(dex_kp[nf]>de_th) okx[nf] = 1.0;
	else okx[nf] = 0.0;
	
	if(dey_kp[nf]>de_th) oky[nf] = 1.0;
	else oky[nf] = 0.0;
	
	if( okx[nf]==1.0 ){
	  for(int j=0 ; j<nset ; j++){
	    // ----- x --------
	    min = 2.0*(nx)*j;
	    max = min + nx;
	    if(min-1<nf && nf<max){
	      x = fiber_startx + fiber_phi*(nf-min);
	      y = 0.0;
	      z = fiber_startz + dz_oneset*j;
	      //	      cout << "x-"<< j << "-" << nf
	      //		   <<": (min,max=" << min << "," << max << ")"
	      //		   <<" (" << x << "," << y << "," << z
	      //		   << ")" << endl;
	      hitfiber_kp[nf] = new TEllipse(x,z,fiber_phi/2.0,fiber_phi/2.0);
	      hitfiber_kp[nf]->SetLineColor(2);
	      c1->cd(1);  hitfiber_kp[nf] ->Draw();
	    }
	    // ----- x' --------
	    min = max;
	    max = min + nx;
	    if(min-1<nf && nf<max){
	      x = fiber_phi/2.0 + fiber_startx + fiber_phi*(nf-min);
	      y = 0;
	      z = fiber_startz + dz_oneset*j + dz1;
	      //	      cout << "x'-"<< j << "-" << nf
	      //		   <<": (min,max=" << min << "," << max << ")"
	      //		   <<" (" << x << "," << y << "," << z
	      //		   << ")" << endl;
	      hitfiber_kp[nf] = new TEllipse(x,z,fiber_phi/2.0,fiber_phi/2.0);
	      hitfiber_kp[nf]->SetLineColor(2);
	      c1->cd(1);  hitfiber_kp[nf] ->Draw();
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
	      //	      cout << "y-"<< j << "-" << nf
	      //		   <<": (min,max=" << min << "," << max << ")"
	      //		   <<" (" << x << "," << y << "," << z
	      //		   << ")" << endl;
	      hitfiber2_kp[nf] = new TEllipse(y,z,fiber_phi/2.0,fiber_phi/2.0);
	      hitfiber2_kp[nf]->SetLineColor(2);
	      c1->cd(2);hitfiber2_kp[nf] ->Draw();
	    }
	    // ----- y' --------
	    min = max;
	    max = min + ny;
	    if(min-1<nf && nf<max){
	      x = 0.0;
	      y = fiber_phi/2.0 + fiber_starty + fiber_phi*(nf-min);
	      z = fiber_startz + dz_oneset*(j+0.5) + dz1;
	      //	      cout << "y'-"<< j << "-" << nf
	      //		   <<": (min,max=" << min << "," << max << ")"
	      //		   <<" (" << x << "," << y << "," << z
	      //		   << ")" << endl;
	      hitfiber2_kp[nf] = new TEllipse(y,z,fiber_phi/2.0,fiber_phi/2.0);
	      hitfiber2_kp[nf]->SetLineColor(2);
	      c1->cd(2);hitfiber2_kp[nf] ->Draw();
	    } // y'
	  } // loop for nset times 
	} // ok for y hit
	
	// ~~~~~~~~~~~ Xi- ~~~~~~~~~~~~~~~~~~~~
	//if(dex[nf]>de_th) okx[nf] = 1.0;
	if(dex_xi[nf]>de_th) okx[nf] = 1.0;
	else okx[nf] = 0.0;
	
	if(dey_xi[nf]>de_th) oky[nf] = 1.0;
	else oky[nf] = 0.0;
	
	if( okx[nf]==1.0 ){
	  for(int j=0 ; j<nset ; j++){
	    // ----- x --------
	    min = 2.0*(nx)*j;
	    max = min + nx;
	    if(min-1<nf && nf<max){
	      x = fiber_startx + fiber_phi*(nf-min);
	      y = 0.0;
	      z = fiber_startz + dz_oneset*j;
	      //	      cout << "x-"<< j << "-" << nf
	      //		   <<": (min,max=" << min << "," << max << ")"
	      //		   <<" (" << x << "," << y << "," << z
	      //		   << ")" << endl;
	      //hitfiber_xi[nf] = new TEllipse(x,z,fiber_phi/2.0,fiber_phi/2.0);
	      hitfiber_xi[nf] = new TEllipse(x,z,fiber_phi/3.0,fiber_phi/3.0);
	      hitfiber_xi[nf]->SetLineColor(8);
	      c1->cd(1);  hitfiber_xi[nf] ->Draw();
	    }
	    // ----- x' --------
	    min = max;
	    max = min + nx;
	    if(min-1<nf && nf<max){
	      x = fiber_phi/2.0 + fiber_startx + fiber_phi*(nf-min);
	      y = 0;
	      z = fiber_startz + dz_oneset*j + dz1;
	      //	      cout << "x'-"<< j << "-" << nf
	      //		   <<": (min,max=" << min << "," << max << ")"
	      //		   <<" (" << x << "," << y << "," << z
	      //		   << ")" << endl;
	      //hitfiber_kp[nf] = new TEllipse(x,z,fiber_phi/2.0,fiber_phi/2.0);
	      hitfiber_xi[nf] = new TEllipse(x,z,fiber_phi/3.0,fiber_phi/3.0);
	      hitfiber_xi[nf]->SetLineColor(8);
	      c1->cd(1);  hitfiber_xi[nf] ->Draw();
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
	      //	      cout << "y-"<< j << "-" << nf
	      //		   <<": (min,max=" << min << "," << max << ")"
	      //		   <<" (" << x << "," << y << "," << z
	      //		   << ")" << endl;
	      hitfiber2_xi[nf] = new TEllipse(y,z,fiber_phi/3.0,fiber_phi/3.0);
	      hitfiber2_xi[nf]->SetLineColor(8);
	      c1->cd(2);hitfiber2_xi[nf] ->Draw();
	    }
	    // ----- y' --------
	    min = max;
	    max = min + ny;
	    if(min-1<nf && nf<max){
	      x = 0.0;
	      y = fiber_phi/2.0 + fiber_starty + fiber_phi*(nf-min);
	      z = fiber_startz + dz_oneset*(j+0.5) + dz1;
	      //	      cout << "y'-"<< j << "-" << nf
	      //		   <<": (min,max=" << min << "," << max << ")"
	      //		   <<" (" << x << "," << y << "," << z
	      //		   << ")" << endl;
	      hitfiber2_xi[nf] = new TEllipse(y,z,fiber_phi/3.0,fiber_phi/3.0);
	      hitfiber2_xi[nf]->SetLineColor(8);
	      c1->cd(2);hitfiber2_xi[nf] ->Draw();
	    } // y'
	  } // loop for nset times 
	} // ok for y hit
	
	//  top->AddNode(hitW,geonum,M[hit]);
	//} // loop for (Number of fibers)
	

      } // loop for (Number of fibers)
    
    
      
      c1->Update();
      
      if(print==true){
	if(countp==0) c1->Print("display3_print.pdf(","pdf");
	else c1->Print("display3_print.pdf","pdf");
	countp++;
      }
      else c1->WaitPrimitive();
	
    } // first_xxp_check
  } // loop for (Total entries in ROOT file)
  
  if(print==true) c1->Print("display3_print.pdf)","pdf");
}
