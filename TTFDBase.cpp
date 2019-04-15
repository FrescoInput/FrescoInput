//------------------------------------------------------------------------------
// TTFDBase.cpp
//------------------------------------------------------------------------------
// T(ype)T(ext)F(ile)D(ata)Base
// C.: 2008.03.28.
// M.: 2015.05.15.
//------------------------------------------------------------------------------

// for test --------------------------------------------------------------------
extern char *TString;
extern void TPrintf(char *s);
extern FILE *LogFile;
// for test --------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>

#include "TTFDBase.h"

extern char *NDBPath; // ?

TTFDBase::TTFDBase()
{
  NM_DBase = new TPart[1];
  NP_DBase = new TPart[1];
  ND_DBase = new TPart[1];
  SF_DBase = new TPart[1];
}


TTFDBase::~TTFDBase()
{
  delete[] NM_DBase;
  delete[] NP_DBase;
  delete[] ND_DBase;
  delete[] SF_DBase;
}


int TTFDBase::Set()
{
  sprintf(TString,"TTFDBase.Set - begin"); TPrintf(TString);

  int i;

  Initialize();

  sprintf(TString,"TTFDBase.Set - end"); TPrintf(TString);

  return( i );
}


void TTFDBase::Initialize()
{
  NM_DBase[0].Set((char*)"MassEnergy");
  NP_DBase[0].Set((char*)"NuclearPotential");
  ND_DBase[0].Set((char*)"NuclearDeformation");
  SF_DBase[0].Set((char*)"SpectroscopicFactor");
}


void TTFDBase::Read() // brad !!!
{
//  sprintf(TString,"TTFDBase.Read - begin"); TPrintf(TString);

  char *NDBFileName;

  NDBFileName = new char[strlen(NDBPath)+20];

/*
  OpenFile(NM_DBase,NDBFileName,(char*)"mass.mas03.ndb"); // Mass 
  OpenFile(NP_DBase,NDBFileName,(char*)"nucl_pot.ndb"); // Potential
  OpenFile(ND_DBase,NDBFileName,(char*)"nucl_def.ndb"); // Deformation
*/  //OpenFile(SF_DBase,NDBFileName,(char*)"nucl_spc.ndb"); // Spectroscopic


/* old :
  ReadFile(NP_DBase,"/home/momotyuk/User/Data/Physics/NuclDB/nucl_pot.ndb"); // Potential
  ReadFile(ND_DBase,"/home/momotyuk/User/Data/Physics/NuclDB/nucl_def.ndb"); // Deformation
  ReadFile(SF_DBase,"/home/momotyuk/User/Data/Physics/NuclDB/nucl_spc.ndb"); // Spectroscopic
*/

  delete[] NDBFileName;
//  sprintf(TString,"TTFDBase.Read.NM_DBase - Ok"); TPrintf(TString);
//  sprintf(TString,"TTFDBase.Read.NP_DBase - Ok"); TPrintf(TString);
//  sprintf(TString,"TTFDBase.Read - end"); TPrintf(TString);
}


void TTFDBase::OpenFile(TPart *DBase,char *ffn,char *fn)
{

  strcpy(ffn,NDBPath);
  strcat(ffn,fn);

  if( access(ffn,F_OK) == 0 ) // Ok!
  {
    ReadFile(DBase,ffn);
  }
  else
  {
    printf("Nuclear DataBase file %s is absent!\nCode stoped.\n",ffn); // only absent?  
    exit( 0 );
  }

}


void TTFDBase::Write()
{
/*/
  WriteFile(NM_DBase,"nuclear_masses.test"); // Mass
  WriteFile(NP_DBase,"nuclear_potential.test"); // Potential
  WriteFile(ND_DBase,"nuclear_deformation.test"); // Deformation
  WriteFile(SF_DBase,"nuclear_spectroscopic.test"); // Spectroscopic
/*/
}


void TTFDBase::ReadFile(TPart *DBase,char *FName)
{
  FILE *fl;
  fl = fopen(FName,"r");

  DBase[0].Read(fl);

  fclose(fl);
}


void TTFDBase::WriteFile(TPart *DBase,char *FName)
{
  FILE *fl;
  fl = fopen(FName,"w");

  DBase[0].Write(fl);

  fclose(fl);
}
//------------------------------------------------------------------------------

