/*
  ParamMan.cc
  Parameter manager
  
  Toshi Gogami , 9July2014
*/

#include "constant.hh"
#include "ParamMan.hh"
#include <TVector3.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

ParamMan::ParamMan()
  :kpmom_center(1.3), kpmom_bite(0.1), 
   theta_min(0.0), theta_max(0.26),
   PID(12),//seedname("dragon.seed"),
   EID(0)//eid(0)
   //kptheta(0.0), kpphi(0.0),
   //kp_x(0.0), kp_y(0.0), kp_z(0.0)
{
  for(int i=0 ; i<3 ; i++){
    pkm[i] = -2222.0;
    pkp[i] = -2222.0;
  }
}
ParamMan::~ParamMan()
{}

//void ParamMan::ReadInputFile(char* file){
void ParamMan::ReadInputFile(string file){
  //ifstream* ifs = new ifstream(file);
  ifstream* ifs = new ifstream(file.c_str());
  //ifstream* ifs = new ifstream("input.dat");
  if(*ifs){
    cout << " ~~~ParamMan::ReadInputFile()~~~" << endl;
    cout << " Found input file: " << file << endl;
    *ifs >> kpmom_center >> kpmom_bite;
    *ifs >> theta_min >> theta_max;
    *ifs >> PID;
    *ifs >> seedname;
    cout << "      p_{k+} (center) = " 
	 << kpmom_center << " GeV/c " << endl;
    cout << "      p_{k+} (bite)   = " 
	 << kpmom_bite << " GeV/c " << endl;
    cout << "      theta_{k+} (min)= " 
	 << theta_min << " rad " << endl;
    cout << "      theta_{k+} (max)= " 
	 << theta_max << " rad " << endl;
    cout << "      PID             = " 
	 << PID << endl;
    // ============== PID ========================
    if(PID==1){
      cout << "                       (Xi^{-})" << endl;
      mcore = 0.0;
      mtar  = mp;
      mhyp  = mxi;
    }
    else if (PID==7){
      cout << "                       (7XiH)" << endl;
      mcore = mHe6;
      mtar  = mLi7;
      mhyp  = m7xiH;
    }
    else if (PID==10){
      cout << "                       (10XiLi)" << endl;
      mcore = mBe9;
      mtar  = mB10;
      mhyp  = m10xiLi;
    }
    else if (PID==12){
      cout << "                       (12XiBe)" << endl;
      mcore = m11B;
      mtar  = m12C;
      mhyp  = m12xiBe;
    }
    else{
      cout << " I do not know the PID, " << PID << endl;
      cout << " So, I just Set PID=12.   (12XiBe)" << endl;
      mcore = m11B;
      mtar  = m12C;
      mhyp  = m12xiBe;
    }
    cout << "      SeedFile        = " 
	 << seedname << endl;
   
  }
  else{
    cout << " There is no input file.... " << endl;
    cout << " Set default value --> " << endl;
    cout << "      p_{k+} (center) = 1.3 GeV/c" << endl;
    cout << "      p_{k+} (bite)   = 0.1 GeV/c" << endl;
    cout << "      theta_{k+} (min)= 0.0  rad" << endl;
    cout << "      theta_{k+} (max)= 0.26 rad" << endl;
    cout << "      PID             = 12" << endl;
    cout << "      SeedFile        = dragon.seed" << endl;
    kpmom_center = 1.3;
    kpmom_bite   = 0.1;
    theta_min = 0.0;
    theta_max = 0.26;
    PID = 12;
  }
  ifs->close();
}

void ParamMan::SeedOutStart(){
  cout << " ~~~ParamMan::SeedOutStart()~~~" << endl;
  cout << "    start filling data to " 
       << seedname << endl;
  //ofstream* dragon_seed = new ofstream(seedname);
  dragon_seed = new ofstream(seedname.c_str());
  dragon_seed->precision(8);
  *dragon_seed << EID << " " 
    //*dragon_seed 
	       << pp[0] << " " << pp[1] << " " << pp[2] << " "
	       << pkm0[0] << " " << pkm0[1] << " " << pkm0[2] << " "
	       << pkm[0] << " " << pkm[1] << " " << pkm[2] << " "
	       << pkp[0] << " " << pkp[1] << " " << pkp[2] << endl; 
  //dragon_seed->close();
}
void ParamMan::SeedOutStart_Fiber(){
  cout << " ~~~ParamMan::SeedOutStart()~~~" << endl;
  cout << "    start filling data to " 
       << seedname << endl;
  //ofstream* dragon_seed = new ofstream(seedname);
  dragon_seed = new ofstream(seedname.c_str());
  dragon_seed->precision(8);
  *dragon_seed << EID << " " 
    //*dragon_seed 
	       << pp[0] << " " << pp[1] << " " << pp[2] << " "
	       << pkm0[0] << " " << pkm0[1] << " " << pkm0[2] << " "
	       << pkm[0] << " " << pkm[1] << " " << pkm[2] << " "
	       << pkp[0] << " " << pkp[1] << " " << pkp[2] << " ";
  *dragon_seed << nhitfibers_x << " ";
  for(int i=0 ; i<nhitfibers_x ; i++){
    *dragon_seed << hitx[i] << " " << dex[i] << " ";
  }
  *dragon_seed << nhitfibers_y << " ";
  for(int i=0 ; i<nhitfibers_y ; i++){
    *dragon_seed << hity[i] << " " << dey[i] << " ";
  }
  *dragon_seed << endl;
  
  //dragon_seed->close();
}

void ParamMan::SeedOut(){
  //ofstream* dragon_seed = new ofstream(seedname,ios_base::app);
  *dragon_seed << EID << " " 
    //*dragon_seed 
	       << pp[0] << " " << pp[1] << " " << pp[2] << " "
	       << pkm0[0] << " " << pkm0[1] << " " << pkm0[2] << " "
	       << pkm[0] << " " << pkm[1] << " " << pkm[2] << " "
	       << pkp[0] << " " << pkp[1] << " " << pkp[2] << endl;
  //dragon_seed->close();
}
void ParamMan::SeedOut_Fiber(){
  //ofstream* dragon_seed = new ofstream(seedname,ios_base::app);
  *dragon_seed << EID << " " 
    //*dragon_seed 
	       << pp[0] << " " << pp[1] << " " << pp[2] << " "
	       << pkm0[0] << " " << pkm0[1] << " " << pkm0[2] << " "
	       << pkm[0] << " " << pkm[1] << " " << pkm[2] << " "
	       << pkp[0] << " " << pkp[1] << " " << pkp[2] << " ";
  *dragon_seed << nhitfibers_x << " ";
  for(int i=0 ; i<nhitfibers_x ; i++){
    *dragon_seed << hitx[i] << " " << dex[i] << " ";
  }
  *dragon_seed << nhitfibers_y << " ";
  for(int i=0 ; i<nhitfibers_y ; i++){
    *dragon_seed << hity[i] << " " << dey[i] << " ";
  }
  *dragon_seed << endl;
  
  //dragon_seed->close();
}
void ParamMan::SeedOutEnd(){
  dragon_seed -> close();
}
