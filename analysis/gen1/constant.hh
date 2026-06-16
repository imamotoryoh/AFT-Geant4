// ########### //
// Constants   //
// ########### //


const double mxi = 1.32171;   //mass of xi-
const double mp  = 0.938272;  //mass of proton
const double mn  = 0.939565;  //mass of neutron
const double mk  = 0.493677;  //mass of neutron
const double mHe6  =  5.60554;
const double mLi7  =  6.53383;
const double mLi8  =  7.47136;
const double mBe9  =  8.39275;
const double mB10  =  9.32444;
const double m10B  =  9.3244;//mass of 10B
const double m11B  = 10.2525;//mass of 11B
const double m11Be = 10.2645;//mass of 11Be
const double m12C  = 11.1748;//mass of 11Be
//const double BE_12xiBe = 2.0e-3;// B.E. [GeV]

// ~~~ Binding energy ~~~~
const double BE_7xiH   = 0.0e-3;// B.E. [GeV]
const double BE_10xiLi = 0.0e-3;// B.E. [GeV]
const double BE_12xiBe = -4.5e-3;// B.E. [GeV]

// ~~~ Mass of Xi hypernuclei ~~~~~
const double m7xiH   = mxi+mHe6+BE_7xiH;
const double m10xiLi = mxi+mBe9+BE_10xiLi;
const double m12xiBe = mxi+m11B+BE_12xiBe;


