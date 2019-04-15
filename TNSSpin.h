//------------------------------------------------------------------------------
// TNSSpin.h
//------------------------------------------------------------------------------
// T(ype)N(ucleus)S(tate)Spin
// C.: 2006.06.06.
// M.: 2015.05.15.
//------------------------------------------------------------------------------

#ifndef __TNSSPIN_H
#define __TNSSPIN_H 1

class TNSSpin{
private:
  int data; // 0 - no data, 1 - there is data;
  int SN;
  float *Spin;

public:
  TNSSpin();
  ~TNSSpin();

  float GetSpin(){ return( Spin[0] ); }
  float GetSpin(int i){ if( i < SN ) return( Spin[i] ); }
  int GetParity();

  int ReadSpin(char *s); // read spin information from string
  int GetSpinNumber(){ return( SN ); }
  int GetSpinData(){ return( data ); }
//  float GetSpin(int i){ if( mod?(i) < SN ){ return(Spin[i]); } else { return(error); } }

private:
  int ReadValue(char *s,char *d,char *c);
  float GetValue(char *s);
};

#endif // __TNSSPIN_H
//------------------------------------------------------------------------------

