//------------------------------------------------------------------------------
// TNSEnergy.cpp
//------------------------------------------------------------------------------
// T(ype)N(ucleus)S(tate)Energy
// C.: 2006.06.05.
// M.: 2015.05.15.
//------------------------------------------------------------------------------

// for test --------------------------------------------------------------------
extern char *TString;
extern void TPrintf(char *s);
extern FILE *LogFile;
// for test --------------------------------------------------------------------

#include "TNSEnergy.h"


TNSEnergy::TNSEnergy()
{
}


TNSEnergy::~TNSEnergy()
{
}


void TNSEnergy::Set(float e,float de)
{
  Energy  = &e;
  dEnergy = &de;
}
//------------------------------------------------------------------------------

