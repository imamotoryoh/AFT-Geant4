/*
  nuclmass.cc
  Calculation for nuclear mass from atomic mass.

  Toshi Gogami , 9July2014
*/

const int n = 7;
// ~~~~~ G. Audi, NPA 729, 337-676 (2003) ~~~~~
const double am[n]= {5.012220, 6.0188891,
		     10.013533.8,
		     11.021658, 10.012937,
		     11.0093054, 12.0};//[u]
const double N[n] = {5, 6, 6, 7, 5, 6, 6};
const double Z[n] = {2, 2, 4, 4, 5, 5, 6};

double calcBel(double z){
  double A = 14.4381 * pow(z,2.39);
  double B = 1.55468e-6 * pow(z,5.35);
  double Bel = A + B;
  return Bel * 1.0e-6; // [MeV]
}

void nuclmass(){
  const double mele = 0.51099;// [MeV]
  double nm;
  double Bel;
  double change = 931.49;// [u]-->[MeV]
  for(int i=0 ; i<n ; i++){
    nm = am[i]*change - Z[i]*mele + calcBel(Z[i]);
    //cout << calcBel(Z[i]) << endl;
    cout << "N:"<< N[i] << " "
	 << "Z:"<< Z[i] << "  "
	 << "-->  M_{nucl.} = " << nm 
	 << " [MeV]"<< endl;
  }
}

