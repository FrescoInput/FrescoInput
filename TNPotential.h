//------------------------------------------------------------------------------
// TNPotential.h
//------------------------------------------------------------------------------
// T(ype)N(uclear)P(otential) 
// C.: 2008.04.04.
// M.: 2015.05.15.
//------------------------------------------------------------------------------

#ifndef __TNPotential_H
#define __TNPotential_H

class TNPotential{
private:
  int cType,cIt,cShape;
  double cV,crV,caV;
  double cW,crW,caW;
  double cP0;
  int cPI;
  int cEI;

public:
  TNPotential();
  ~TNPotential();

  void Set();
  void Set( int Zp,int Ap,int Ep,int Zt,int At,int Et );


  void Set(int fRI,int fPI,int fEI);
  void SetR(int fAp,int fAt,int fPI); // ?

  int Get_Type(){  return( cType ); }
  int Get_It(){    return( cIt ); }
  int Get_Shape(){ return( cShape ); }

  double Get_V(){  return( cV );  }
  double Get_rV(){ return( crV ); }
  double Get_aV(){ return( caV ); }

  double Get_W(){  return( cW );  }
  double Get_rW(){ return( crW ); }
  double Get_aW(){ return( caW ); }

  double Get_P0(){ return( cP0 ); }

};

#endif // __TNPotential_H
//------------------------------------------------------------------------------

