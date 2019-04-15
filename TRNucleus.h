//------------------------------------------------------------------------------
// TRNucleus.h
//------------------------------------------------------------------------------
// TypeR(eaction)Nucleus
// C.: 2005.mm.dd.
// M.: 2015.05.15.
//------------------------------------------------------------------------------

#ifndef __TRNUCLEUS_H
#define __TRNUCLEUS_H

#include "T1BNSystem.h"
#include "TTIFile.h"

class TRNucleus{
private:
  char *Nucleus;
  int SN;
  double *States;

  T1BNSystem *dbNucl;

public:
  TRNucleus();
  ~TRNucleus();

  void Set(){};
  void Set(char *s);
  int Read(FILE *f,char *s){};
  int Read(TTIFile *fl,char *kind);
  void Write(FILE *f,char *s);

  void GetSymbol(char *s){ strncpy(s,Nucleus,6); }
  int GetNumberOfStates(){ return( SN ); }
  double GetStateEnergy(int i){ if(i < SN) return( States[i] ); }

  void CheckLE(double fMEE);
  void CheckLevels(double *fLE,int fNoLE);

// new :
  int ReadNew(TTIFile *fl,char *kind,int N);
//*/
  char*  Symbol(){ return( Nucleus ); }
  int    StateNumber( void ){ return( SN ); }
  double StateEnergy(int i){ if(i < SN) return( States[i] ); }
//*/
//private:
};

#endif // __TRNUCLEUS_H
//------------------------------------------------------------------------------

