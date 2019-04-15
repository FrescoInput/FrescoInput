//------------------------------------------------------------------------------
// TNDeformation.cpp
//------------------------------------------------------------------------------
// T(ype)NuclearDeformation
// C.: 2008.04.28.
// M.: 2015.05.15.
//------------------------------------------------------------------------------

// for test --------------------------------------------------------------------
extern char *TString;
extern void TPrintf(char *s);
extern FILE *LogFile;
// for test --------------------------------------------------------------------

#include <stdio.h>

#include "TNDeformation.h"

extern TNDDBase *NDDBase;


TNDeformation::TNDeformation()
{
  cRadius = 0.;
  cNoDL = 6;
  cDefLength = new double[cNoDL+1];
}


TNDeformation::~TNDeformation()
{
  delete[] cDefLength;
}


void TNDeformation::Set()
{
  sprintf(TString,""); TPrintf(TString);
}

/* */
void TNDeformation::Set( int Zp,int Ap,int Ep )
{
//  sprintf(TString,"TNDeformation.Set - begin"); TPrintf(TString);
  char *fRow,*s;
  
  fRow = NDDBase->NuclearDeformation( Zp,Ap,Ep );

  int i;

  s = fRow;
  i = 0;
  while( i < cNoDL )
  {
//    cDefLength[i] = NDBase[0].ND_Get_Def(fRI,i);

    cDefLength[i] = atof( s );
      s = strchr(++s,' '); // nepravylno !!! mozhe buty pomylka!!!

//  sprintf(TString,"TNDeformation.Set cDefLength[%d] = %lf",i,cDefLength[i]); TPrintf(TString);


    i++;
  }

  delete[] fRow;
  
//  sprintf(TString,"TNDeformation.Set D1=%lf D2=%lf D3=%lf D4=%lf D5=%lf D6=%lf",
//                  cDefLength[0],cDefLength[1],cDefLength[2],
//                  cDefLength[3],cDefLength[4],cDefLength[5] ); TPrintf(TString);
  
//  sprintf(TString,"TNDeformation.Set - end"); TPrintf(TString);
}
/* */

void TNDeformation::Set(int fRI)
{
//  sprintf(TString,"TNPotential.Set - begin"); TPrintf(TString);

  int i;

  i = 0;
  while( i < cNoDL )
  {
    cDefLength[i] = NDBase[0].ND_Get_Def(fRI,i);
    i++;
  }
/*/
  cV  = NDBase[0].NP_Get_V(fRI);
  crV = NDBase[0].NP_Get_rV(fRI);
  caV = NDBase[0].NP_Get_aV(fRI);

  cW  = NDBase[0].NP_Get_W(fRI);
  crW = NDBase[0].NP_Get_rW(fRI);
  caW = NDBase[0].NP_Get_aW(fRI);

//  sprintf(TString,"TNPotential.Set.caW = %lf",caW); TPrintf(TString);

  cPI = fPI;
  cEI = fEI + 1; // correction for 0 index...
/*/
//  sprintf(TString,"TNPotential.Set - end"); TPrintf(TString);
}
//------------------------------------------------------------------------------

