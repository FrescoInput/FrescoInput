//------------------------------------------------------------------------------
// TRChannel.h
//------------------------------------------------------------------------------
// TypeR(action)Channel
// C.: 2005.mm.dd.
// M.: 2015.05.15.
//------------------------------------------------------------------------------

#ifndef __TRChannel_H
#define __TRChannel_H

#include "TRNucleus.h"
#include "TTIFile.h"

class TRChannel{
private:
  TRNucleus *Projectile,*Target;
  double Energy;

public:
  TRChannel();
  ~TRChannel();

  void Set(char *s);
  int Read(FILE *f,char *s);
  int Read(TTIFile *f,char *s,int c); // c = 0 if input channel, c = 1 if exit channel
  void Write(FILE *f,char *s,int c);

  void GetProjectile(char *s){ Projectile[0].GetSymbol(s); } // GetProjectileElement ?
  void GetTarget(char *s){ Target[0].GetSymbol(s); }
  double GetEnergy(){ return( Energy ); } 

  int GetProNumberOfStates(){ return( Projectile[0].GetNumberOfStates() ); }
  int GetTarNumberOfStates(){ return( Target[0].GetNumberOfStates() ); }
  int GetTotNumberOfStates()
  { return(Projectile[0].GetNumberOfStates() + Target[0].GetNumberOfStates()); }

  int GetProLevels(double *Lev,int fSN);
  int GetTarLevels(double *Lev,int fSN);

  double GetPStateEnergy(int i){ return( Projectile[0].GetStateEnergy( i ) ); }
  double GetTStateEnergy(int i){ return( Target[0].GetStateEnergy( i ) ); }

  void CheckLE(double fMEE);
  void CheckLevels(int fNT,double *fLE,int fNoLE);

// new :
  int ReadNew(TTIFile *f,char *s,int N); // c = 0 if input channel, c = 1 if exit channel

private:
  char* DelSpace(char *str);
};

#endif // __TRChannel_H
//------------------------------------------------------------------------------

