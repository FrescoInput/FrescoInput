//------------------------------------------------------------------------------
// TSFactor.cpp
//------------------------------------------------------------------------------
// T(ype)S(pectroscopic)Factor
// C.: 2008.03.21.
// M.: 2015.08.17.
//------------------------------------------------------------------------------

// for test --------------------------------------------------------------------
extern char *TString;
extern void TPrintf(char *s);
extern FILE *LogFile;
// for test --------------------------------------------------------------------

#include <stdio.h>

#include "TSFactor.h"

extern TNDDBase *NDDBase;

extern TTFDBase *NDBase;

TSFactor::TSFactor() // is this Ok?
{
  cN =  0;
  cL =  0;
  cJ =  0.;
  cSF = 0.;
  cSI = 0;
  cProV = 0;
  cEI = -1;
}


TSFactor::~TSFactor()
{
}


void TSFactor::Set()
{
//  sprintf(TString,""); TPrintf(TString);
}


void TSFactor::Set( int Zcp,int Acp,int Ecp,int Zcr,int Acr,int Ecr,int Zcl,int Acl,int Ecl )
{
//  sprintf(TString,"TSFactor.Set - begin"); TPrintf(TString);
  char *fRow,*s;
  
  fRow = NDDBase->SpectroscopicFactor( Zcp,Acp,Ecp,Zcr,Acr,Ecr,Zcl,Acl,Ecl );

//  sprintf(TString,"TSFactor.Set fRow : %s",fRow); TPrintf(TString);

  s = fRow;
  cN  = atoi( s );
    s = strchr(++s,' ');
  cL  = atoi( s );
    s = strchr(++s,' ');
  cJ  = atof( s );
    s = strchr(++s,' ');
  cSF = atof( s );

  delete[] fRow;
  
//  sprintf(TString,"TSFactor.Set cN=%d cL=%d cJ=%lf cSF=%lf ",cN,cL,cJ,cSF); TPrintf(TString);
  
//  sprintf(TString,"TSFactor.Set - end"); TPrintf(TString);
}


void TSFactor::Set(int RI,int fSI,int fProV,int fEI)
{
//  sprintf(TString,""); TPrintf(TString);

  cN  = NDBase[0].SF_GetN(RI);
  cL  = NDBase[0].SF_GetL(RI);
  cJ  = NDBase[0].SF_GetJ(RI);
  cSF = NDBase[0].SF_GetSF(RI);
  cSI = fSI;
  cProV = fProV;
  cEI = fEI + 1; // correction for 0 index...
}

/*
int TSFactor::ReadDB()
{
//  sprintf(TString,""); TPrintf(TString);
}
*/
//------------------------------------------------------------------------------

