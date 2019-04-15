//------------------------------------------------------------------------------
// TRNucleus.cpp
//------------------------------------------------------------------------------
// TypeR(eaction)Nucleus
// C.: 2005.mm.dd.
// M.: 2015.05.15.
//------------------------------------------------------------------------------

// for test --------------------------------------------------------------------
extern char *TString;
extern void TPrintf(char *s);
extern FILE *LogFile;
// for test --------------------------------------------------------------------

#include "TRNucleus.h"

TRNucleus::TRNucleus()
{
  Nucleus = new char[6];
    memset(Nucleus,0,6);

  SN = 0;
  States = new double[SN+1];
  States[0] = 0.;

  dbNucl = new T1BNSystem[1];
}


TRNucleus::~TRNucleus()
{
  delete[] Nucleus;
  delete[] States;
  delete[] dbNucl;
}


void TRNucleus::Set(char *s)
{
//  sprintf(TString,"TRNucleus.Set s = %s",s); TPrintf(TString);

  int i = 0;

  dbNucl[0].SetIsotope(s);

  dbNucl[0].GetSymbol(Nucleus);

//  sprintf(TString,"TRNucleus.Set Nucleus = %s",Nucleus); TPrintf(TString);


  SN = dbNucl[0].GetStateNumber();

  delete[] States;
  States = new double[SN];

  while( i < SN )
  {
    States[i] = dbNucl[0].GetStateEnergy(i)/1000; // /1000 <- this is bad!!! 
    i++;
  } 
}


void TRNucleus::CheckLE(double fMEE)
{
//  sprintf(TString,"TRNucleus.CheckLE - begin"); TPrintf(TString);
//  sprintf(TString,"TRNucleus.CheckLE.fMEE = %lf",fMEE); TPrintf(TString);
//  sprintf(TString,"TRNucleus.CheckLE.Nucleus = %s",Nucleus); TPrintf(TString);
//  sprintf(TString,"TRNucleus.CheckLE.SN = %d",SN); TPrintf(TString);

  int i,j;
  double *lStates,lSEnergy;

  lStates = new double[SN];

  i = 0;
  j = 0;
  while( i < SN )
  {
    if( States[i] <= fMEE )
    {
      lStates[j] = States[i];
      j++;
    }
    i++;
  }

  SN = j;

  delete[] States;
  States = new double[SN];

  for(i=0;i<SN-1;i++)
  {
    for(j=i+1;j<SN;j++)
    {
      if( lStates[i] > lStates[j] )
      {
        lSEnergy   = lStates[i];
        lStates[i] = lStates[j];
        lStates[j] = lSEnergy;
      }
    }
    States[i] = lStates[i];
  }
  States[SN-1] = lStates[SN-1];

  delete[] lStates;

//  sprintf(TString,"TRNucleus.CheckLE.SN = %d",SN); TPrintf(TString);
//  sprintf(TString,"TRNucleus.CheckLE - end"); TPrintf(TString);
}


void TRNucleus::CheckLevels(double *fLE,int fNoLE)
{
  int i;

  delete[] States;

  SN = fNoLE;
  States = new double[SN];

  i = 0;
  while( i < SN )
  {
    States[i] = fLE[i];
    i++;
  }

}


int TRNucleus::Read(TTIFile *IFile,char *kind)
{
//  sprintf(TString,"TRNucleus.newRead - begin"); TPrintf(TString);
//  sprintf(TString,"TRNucleus.newRead.kind = %s",kind); TPrintf(TString);

  int rv,i,l,d; // ReturnValue
  long pos;
  char *ws,*ds; // Work,DataString

  ws = new char[80]; // why 80? it is bad!
    memset(ws,0,80);

  ds = new char[80];
    memset(ds,0,80);

  do
  {
    rv = IFile[0].FileDataRead(ws,80);

    if( rv != EOF ) 
    {
      if( (l = IFile[0].StrnGetKind(kind,ws)) != 0 )
        d = IFile[0].StrnGetData(ws+l,ds);
    }
  }
  while( (l == 0) && (rv != EOF) );

  strncpy(Nucleus,ds,6);
//  sprintf(TString,"TRNucleus.Nucleus = %s",Nucleus); TPrintf(TString);

  SN = 1;

  pos = IFile[0].GetPos();

  do
  {
    rv = IFile[0].FileDataRead(ws,80);

    if( rv != EOF ) 
    {
      if( (l = IFile[0].StrnGetKind((char*)"State",ws)) != 0 )
      {
        d = IFile[0].StrnGetData(ws+l,ds);
        SN++;
      }
    }
  }
  while( (l != 0) && (rv != EOF) );

  l = IFile[0].SetPos(pos);

//  sprintf(TString,"TRNucleus.SN = %d",SN); TPrintf(TString);

  delete[] States;
  States = new double[SN];
  States[0] = 0.; // g.s.

  i = 1;

  do
  {
    pos = IFile[0].GetPos();
    rv = IFile[0].FileDataRead(ws,80);

    if( rv != EOF ) 
    {
      if( (l = IFile[0].StrnGetKind((char*)"State",ws)) != 0 )
        d = IFile[0].StrnGetData(ws+l,ds);

//  sprintf(TString,"TRNucleus.ds = %s",ds); TPrintf(TString);

      if( atof(ds) != 0. )
      { 
        States[i] = atof(ds);

//  sprintf(TString,"TRNucleus.States[%d] = %lf",i,States[i]); TPrintf(TString);

        i++;
      }
      else
      {
        if( (strlen(ds) != 0) && (SN > 1) ){ SN--; }
      }

      *ds = '\0';
    }
  }
  while( (l != 0) && (rv != EOF) );

//  sprintf(TString,"TRNucleus.SN (last) = %d",SN); TPrintf(TString); // to delete!

  l = IFile[0].SetPos(pos);

  delete[] ds;
  delete[] ws;

//  sprintf(TString,"TRNucleus.newRead - end"); TPrintf(TString);

  return( rv );
}


int TRNucleus::ReadNew(TTIFile *IFile,char *kind,int N)
{
//  sprintf(TString,"TRNucleus.ReadNew - begin"); TPrintf(TString);
//  sprintf(TString,"TRNucleus.newRead.kind = %s",kind); TPrintf(TString);

  int i,p,sn;

  if( strcmp(kind,"Target") )
    p = 1;
  else
    p = 0;

  if( p )
  {
    strncpy( Nucleus,IFile[0].Projectile( N ),5 );

//  sprintf(TString,"TRNucleus.ReadNew N = %d Nucleus = %s",N,Nucleus); TPrintf(TString);

    sn = IFile[0].ProjectileStateNumber( N );

    SN = sn;
    if( !SN ) SN = 1; // if SN == 0

//  sprintf(TString,"TRNucleus.ReadNew SN = %d",SN); TPrintf(TString);

    delete[] States;
    States = new double[SN];
    States[0] = 0.; // g.s.
    
    i = 1;
    
    while( i < SN )
    {
      States[i] = IFile[0].ProjectileState( N,i );

//  sprintf(TString,"TRNucleus.ReadNew States[%d] = %f",i,States[i]); TPrintf(TString);
  
      i++;
    }
  }
  else
  {
    strncpy( Nucleus,IFile[0].Target( N ),5 );

//  sprintf(TString,"TRNucleus.ReadNew N = %d Nucleus = %s",N,Nucleus); TPrintf(TString);

    sn = IFile[0].TargetStateNumber( N );

    SN = sn + 1; // + g.s.

//  sprintf(TString,"TRNucleus.ReadNew SN = %d",SN); TPrintf(TString);

    delete[] States;
    States = new double[SN];
    States[0] = 0.; // g.s.
    
    i = 1;
    
    while( i < SN )
    {
      States[i] = IFile[0].TargetState( N,i );

//  sprintf(TString,"TRNucleus.ReadNew States[%d] = %f",i,States[i]); TPrintf(TString);

      i++;
    }
  }

//  sprintf(TString,"TRNucleus.ReadNew - end"); TPrintf(TString);

  return( 0 ); // ???
}


void TRNucleus::Write(FILE *ofl,char *w)
{
  int i = 0;

  fprintf(ofl,"#%-16s : %s\n",w,Nucleus);

  sprintf(TString,"TRNucleus.Write %s SN = %d",w,SN); TPrintf(TString);

  while( i < SN ){fprintf(ofl,"#%-16s : %7.4lf\n","State",States[i]); i++;}
}
//------------------------------------------------------------------------------

