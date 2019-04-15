//------------------------------------------------------------------------------
// TSFactor.h
//------------------------------------------------------------------------------
// T(ype)S(pectroscopic)Factor
// C.: 2008.03.21.
// M.: 2015.08.17.
//------------------------------------------------------------------------------

#ifndef __TSFactor_H
#define __TSFactor_H

class TSFactor{
private:
  int cN;
  int cL;
  float cJ;
  double cSF;
  int cSI;
  int cProV; // 1 - pro., 2 - tar.
  int cEI;

public:
  TSFactor();
  ~TSFactor();

  void Set();
  void Set(int RI,int fSI,int fProV,int fEI);
  void Set( int Zcp,int Acp,int Ecp,int Zcr,int Acr,int Ecr,int Zcl,int Acl,int Ecl );

  int GetN(){ return( cN ); }
  int GetL(){ return( cL ); }
  float GetJ(){ return( cJ ); }
  double GetSF(){ return( cSF ); }
  int GetSI(){ return( cSI ); }
  int GetProV(){ return( cProV ); }
  int GetEI(){ return( cEI ); }

//  int ReadDB();
};

#endif // __TSFactor_H
//------------------------------------------------------------------------------

