//------------------------------------------------------------------------------
// TNDeformation.h
//------------------------------------------------------------------------------
// T(ype)NuclearDeformation
// C.: 2008.04.28.
// M.: 2015.05.15.
//------------------------------------------------------------------------------

#ifndef __TNDeformation_H
#define __TNDeformation_H

class TNDeformation{
private:
  double cRadius;
  int cNoDL;
  double *cDefLength;

public:
  TNDeformation();
  ~TNDeformation();
 
  void Set();
  void Set(int fRI);
  void Set( int Zp,int Ap,int Ep );

  double Get_Def(int i){ return( cDefLength[i] ); }
};

#endif // __TNDeformation_H
//------------------------------------------------------------------------------

