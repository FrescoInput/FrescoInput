//------------------------------------------------------------------------------
// TCCNSystem.cpp
//------------------------------------------------------------------------------
// T(ype)C(luster)C(ore)N(uclear)S(ystem)
// C.: 2008.03.20.
// M.: 2015.03.dd.
//------------------------------------------------------------------------------

// for test --------------------------------------------------------------------
extern char *TString;
extern void TPrintf(char *s);
extern FILE *LogFile;
// for test --------------------------------------------------------------------

#include <stdio.h>

#include "TCCNSystem.h"

TCCNSystem::TCCNSystem()
{
  cCore     = new T1BNSystem[1];
  cCluster  = new T1BNSystem[1];
  cCompound = new T1BNSystem[1];
}


TCCNSystem::~TCCNSystem()
{
  delete[] cCore;
  delete[] cCluster;
  delete[] cCompound;
}


void TCCNSystem::Set()
{
//  sprintf(TString,""); TPrintf(TString);
}


void TCCNSystem::Set(TRChannel *EChannel,TRChannel *OChannel,char *s)
{
//  sprintf(TString,""); TPrintf(TString);

  int lZc,lNc;
  char *lce,*lco;

  T1BNSystem *lEN,*lON;

  lce = new char[6];
    memset(lce,0,6);
  lco = new char[6];
    memset(lco,0,6);

  lEN = new T1BNSystem[1];
  lON = new T1BNSystem[1];

  if( strcmp("Projectile",s) == 0 )
  {
    EChannel->GetProjectile( lce );
    OChannel->GetProjectile( lco );
  }

  if( strcmp("Target",s) == 0 )
  {
    EChannel->GetTarget( lce );
    OChannel->GetTarget( lco );
  }

  lEN[0].Set(lce);
  lON[0].Set(lco);

  if( lEN[0].GetA() > lON[0].GetA() )
  {
    cCore[0].Set(lco);
    cCompound[0].Set(lce);
  }
  else
  {
    cCore[0].Set(lce);
    cCompound[0].Set(lco);
  }

  lZc = abs( lEN[0].GetZ() - lON[0].GetZ() );
  lNc = abs( lEN[0].GetN() - lON[0].GetN() );

  cCluster[0].Set(lZc,lNc);

  delete[] lce;
  delete[] lco;
  delete[] lEN;
  delete[] lON;
}


void TCCNSystem::WriteDB(FILE *ofl)
{
  cCore[0].WriteDB(ofl,"Core");
  cCluster[0].WriteDB(ofl,"Cluster");
  cCompound[0].WriteDB(ofl,"Compound");
}


//------------------------------------------------------------------------------

