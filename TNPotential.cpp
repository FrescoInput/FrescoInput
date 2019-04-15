//------------------------------------------------------------------------------
// TNPotential.cpp
//------------------------------------------------------------------------------
// T(ype)N(uclear)P(otential) 
// C.: 2008.04.04.
// M.: 2015.05.15.
//------------------------------------------------------------------------------

// for test --------------------------------------------------------------------
extern char *TString;
extern void TPrintf(char *s);
extern FILE *LogFile;
// for test --------------------------------------------------------------------

#include <stdio.h>
#include <math.h>

#include "TNPotential.h"

extern TNDDBase *NDDBase;

extern TTFDBase *NDBase;

TNPotential::TNPotential()
{
  cType  = 0;
  cIt    = 0;
  cShape = 0;

  cV  = 0.;
  crV = 0.;
  caV = 0.;
  cW  = 0.;
  crW = 0.;
  caW = 0.;

  cP0 = 0.;

  cPI = 0;
  cEI = 0;
}


TNPotential::~TNPotential()
{
}


void TNPotential::Set()
{
//  sprintf(TString,""); TPrintf(TString);
}


void TNPotential::Set( int Zp,int Ap,int Ep,int Zt,int At,int Et )
{
//  sprintf(TString,"TNPotential.Set - begin"); TPrintf(TString);
  char *fRow,*s;
  
  fRow = NDDBase->NuclearPotential( Zp,Ap,Ep,Zt,At,Et );

  s = fRow;
  cV  = atof( s );
    s = strchr(++s,' ');
  crV = atof( s );
    s = strchr(++s,' ');
  caV = atof( s );
    s = strchr(++s,' ');

  cW  = atof( s );
    s = strchr(++s,' ');
  crW = atof( s );
    s = strchr(++s,' ');
  caW = atof( s );
//  s = strchr(s,' ');

  delete[] fRow;
  
//  sprintf(TString,"TNPotential.Set V=%lf rR=%lf aR=%lf W=%lf rI=%lf aI=%lf ",cV,crV,caV,cW,crW,caW); TPrintf(TString);
  
//  sprintf(TString,"TNPotential.Set - end"); TPrintf(TString);
}


void TNPotential::Set(int fRI,int fPI,int fEI)
{
//  sprintf(TString,"TNPotential.Set - begin"); TPrintf(TString);

  cV  = NDBase[0].NP_Get_V(fRI);
  crV = NDBase[0].NP_Get_rV(fRI);
  caV = NDBase[0].NP_Get_aV(fRI);

  cW  = NDBase[0].NP_Get_W(fRI);
  crW = NDBase[0].NP_Get_rW(fRI);
  caW = NDBase[0].NP_Get_aW(fRI);

//  sprintf(TString,"TNPotential.Set.caW = %lf",caW); TPrintf(TString);

  cPI = fPI;
  cEI = fEI + 1; // correction for 0 index...

//  sprintf(TString,"TNPotential.Set - end"); TPrintf(TString);
}


void TNPotential::SetR(int fAp,int fAt,int fPI)
{
//  sprintf(TString,"TNPotential.SetR - begin"); TPrintf(TString);
  double ldv;

  ldv = 1./3.;

  cV  = 60.0;
  crV = (1.25 * pow(fAp + fAt,ldv)) / ( pow(fAp,ldv) + pow(fAt,ldv) );
  caV = 0.65;

  cW  = 0.;
  crW = 0.;
  caW = 0.;

//  sprintf(TString,"TNPotential.SetR.caW = %lf",caW); TPrintf(TString);

  cPI = fPI;
  cEI = 1; // correction for 0 index...

//  sprintf(TString,"TNPotential.SetR - end"); TPrintf(TString);
}
//------------------------------------------------------------------------------

