//------------------------------------------------------------------------------
// TNState.h
//------------------------------------------------------------------------------
// T(ype)N(ucleus)State
// C.: 2006.06.05.
// M.: 2015.08.17.
//------------------------------------------------------------------------------

#ifndef __TNSTATE_H
#define __TNSTATE_H 1

#include "TNSSpin.h"
#include "TNSHLife.h"

class TNState{
private:
  double Energy,dEnergy;
//  TNSEnergy *Energy;
  int SN;
  TNSSpin  *Spin;
  TNSHLife *HLife;

public:
  TNState();
  ~TNState();

  void Set( char *Symbol,int A );

  double GetEnergy(){ return( Energy ); }
  float GetSpin(){ return( Spin[0].GetSpin() ); } // is this correct?
  int GetParity(){ return( Spin[0].GetParity() ); }
  double GetHLife(){return(HLife[0].GetHLife());};

  void ReadState(char *s); // read state information from string
  void ReadStateNew(char *s);
  void WriteDB(FILE *ofl);
private:
};

#endif // __TNSTATE_H
//------------------------------------------------------------------------------

