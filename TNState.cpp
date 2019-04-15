//------------------------------------------------------------------------------
// TNState.cpp
//------------------------------------------------------------------------------
// T(ype)N(ucleus)State
// C.: 2006.06.05.
// M.: 2015.05.15.
//------------------------------------------------------------------------------

// for test --------------------------------------------------------------------
extern char *TString;
extern void TPrintf(char *s);
extern FILE *LogFile;
// for test --------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "TNState.h"

extern TNDDBase *NDDBase;


TNState::TNState()
{
  Energy  = 0.;
  dEnergy = 0.;
  SN = 1;
  Spin = new TNSSpin[SN];
  HLife = new TNSHLife[SN];
}


TNState::~TNState()
{
  delete[] Spin;
  delete[] HLife;
}

/*
double TNState::GetHLife()
{
  double hl;
  hl = HLife[0].GetHLife();
  return( hl );
}
*/


void TNState::Set( char *Symbol,int A )
{
//  sprintf(TString,"TNState.Set - begin"); TPrintf(TString);
  int i,n;
  char *fRow,*s;
  
  fRow = NDDBase->NuclearState( Symbol,A );

//  sprintf(TString,"TNState.Set fRow : %s", fRow ); TPrintf(TString);

  s = fRow;
  
   i = atoi( s );
   s = strchr(++s,' ');
   s = strchr(++s,' '); // to skip Symbol
  Energy = atof( s );
   s = strchr(++s,' ');
  dEnergy = atof( s );
   s = strchr(++s,' ');

  i = Spin[0].ReadSpin( s );
  i = Spin[0].GetSpinData();
  n = Spin[0].GetSpinNumber();

  delete[] fRow;
  
//  sprintf(TString,"TNState.Set  ",); TPrintf(TString);
  
//  sprintf(TString,"TNState.Set - end"); TPrintf(TString);
}





void TNState::ReadStateNew(char *fS)
{
//  sprintf(TString,"TNState.ReadStateNew - begin"); TPrintf(TString);

// 6 LI 3 5650.0000 0.0000 1+              1.5000 MEV        2.0000 88

  int i,n;
  char *CS;
  CS = new char[80];
    memset(CS,0,80);

  sscanf(fS,"%d %2s %d %lf %lf %s",&i,CS,&n,&Energy,&dEnergy,CS);

//  sprintf(TString,"TNState.ReadStateNew : A = %d Symbol = %s Z = %d",i,CS,n); TPrintf(TString);
//  sprintf(TString,"TNState.ReadStateNew : Energy = %lf dEnergy = %lf",Energy,dEnergy); TPrintf(TString);

/*/
  strncpy(CS, fS+30,23); // Jp
  *(CS+23) = 0;
/*/

  i = Spin[0].ReadSpin(CS);
  i = Spin[0].GetSpinData();
  n = Spin[0].GetSpinNumber();

  delete[] CS;

//  sprintf(TString,"TNState.ReadStateNew - end"); TPrintf(TString);
}


void TNState::ReadState(char *fS)
{
//  sprintf(TString,"TNState::ReadState - begin"); TPrintf(TString);

  int i,n; // ?

//  FILE *ofl;
//  ofl = fopen("str-test.tmp","a");
  
  char *CS;
  CS = new char[80];
    memset(CS,0,80);

  strncpy(CS, fS+ 0, 6); // A
  *(CS+6) = 0;
//  fprintf(ofl,"%s\n",CS);
//  sprintf(TString,"TNState::ReadState: CS : %s",CS); TPrintf(TString);

  strncpy(CS, fS+ 6, 2); // symbol
  *(CS+2) = 0;
//  fprintf(ofl,"%s\n",CS);

  strncpy(CS, fS+ 8, 6); // Z
  *(CS+6) = 0;
//  fprintf(ofl,"%s\n",CS);

  strncpy(CS, fS+14,11); // LEnergy
  *(CS+11) = 0;
  Energy = atof(CS);
//  fprintf(ofl,"Energy = %lf\n",Energy);

//  fprintf(ofl,"%s\n",CS);

  strncpy(CS, fS+25, 4); // LdE
  *(CS+4) = 0;
  dEnergy = atof(CS);
//  fprintf(ofl,"dEnergy = %lf\n",dEnergy);

//  fprintf(ofl,"%s\n",CS);

  strncpy(CS, fS+30,23); // Jp
  *(CS+23) = 0;
  i = Spin[0].ReadSpin(CS);

  i = Spin[0].GetSpinData();
//  fprintf(ofl,"SData = %d\n",i);

  n = Spin[0].GetSpinNumber();
//  fprintf(ofl,"SNumber = %d\n",i);

  i = 0;
//  while(i < n){ fprintf(ofl,"spin[%d] = %f\n",i+1,Spin[0].GetSpin(i)); i++; }

//  fprintf(ofl,"%s\n",CS);

  strncpy(CS, fS+53,22); // Half-Life
  *(CS+22) = 0;
  i = HLife[0].ReadHLife(CS);

//  fprintf(ofl,"%s\n",CS);

  strncpy(CS, fS+75, 3); // Year
  *(CS+3) = 0;
//  fprintf(ofl,"%s\n",CS);

//  fclose(ofl);

  delete[] CS;

//  sprintf(TString,"TNState::ReadState - end"); TPrintf(TString);

//  return( fS ); // ???
}


void TNState::WriteDB(FILE *ofl)
{
  fprintf(ofl,
    "Energy               = %10.4lf "
    "Spin   = %10.1f "
    "Parity = %5d\n",
    Energy/1000,Spin[0].GetSpin(),Spin[0].GetParity());
}
//------------------------------------------------------------------------------

