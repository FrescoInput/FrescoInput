//------------------------------------------------------------------------------
// TNSSpin.cpp
//------------------------------------------------------------------------------
// T(ype)N(ucleus)S(tate)Spin
// C.: 2006.06.06.
// M.: 2015.05.15.
//------------------------------------------------------------------------------

// for test --------------------------------------------------------------------
extern char *TString;
extern void TPrintf(char *s);
extern FILE *LogFile;
// for test --------------------------------------------------------------------

#include <ctype.h>

#include "TNSSpin.h"

TNSSpin::TNSSpin()
{
  data = 0;
  SN = 1; // maybe it is better SN = 0 ?
  Spin = new float[SN];
  Spin[0] = 0.; // ???
}


TNSSpin::~TNSSpin()
{
  delete[] Spin;
}


int TNSSpin::GetParity() // for test only !
{
  int r;

  if( Spin[0] >= 0 ) r = 1;
  else r = -1;

  return( r );
}


int TNSSpin::ReadSpin(char *ss)
{
//  sprintf(TString,"TNSSpin.ReadSpin - begin"); TPrintf(TString);
//  sprintf(TString,"TNSSpin.ReadSpin ss = %s",ss); TPrintf(TString);

//  FILE *ofl;
//  ofl = fopen("spin-test.tmp","a");

  int Len,i;
  Len = strlen(ss);

  char *ds,*cs;
  ds = new char[Len];
    memset(ds,0,Len);

  cs = new char[7]; // 000/2-
    memset(cs,0,7);

  char *s,*d;  

  i = 0;
  s = ss;
  d = ds;
  while( i < Len )
  {
    if( *s != ' ' )
    {
      *d = *s;
      d++;
    }
    s++;
    i++;
  }
  *d = 0; // ?

//  sprintf(TString,"TNSSpin::ReadSpin: ss : %s",ss); TPrintf(TString);
//  sprintf(TString,"TNSSpin::ReadSpin: ds : %s",ds); TPrintf(TString);

//  fprintf(ofl,"fS = %s\n",ss);
//  fprintf(ofl,"CS = %s\n",ds);

  s = ds;  

  int l;

  char *zs;
  zs = new char[16];
  sprintf(zs,"0123456789/+-()\0");

  SN = 0;

  while( (l = ReadValue(s,cs,zs)) != 0 ){ SN++; s+=l; }

  delete[] Spin;
  Spin = new float[SN+1];
//*/
  Spin[0] = 0.; // ???
//*/
  s = ds;
  i = 0;
  while( (l = ReadValue(s,cs,zs)) != 0 )
  {
    Spin[i] = GetValue(cs);
    i++;
    s+=l;
//    fprintf(ofl,"spin = %f\n",Spin[i-1]);
  }

//  sprintf(TString,"TNSSpin::ReadSpin: SN = %d",SN); TPrintf(TString);

  if( (SN != 1) || (strchr(ds,'(') != NULL) ){ data = 0; }
  else{ data = 1; }

  delete[] ds;
  delete[] cs;
  delete[] zs;

//  fclose(ofl);

//  sprintf(TString,"TNSSpin.ReadSpin Spin[0] = %f",Spin[0]); TPrintf(TString);
//  sprintf(TString,"TNSSpin.ReadSpin: end"); TPrintf(TString);

  return( 0 ); // ???
}


int TNSSpin::ReadValue(char *ss,char *ds,char *cs)
{
//  sprintf(TString,"TNSSpin::ReadValue: begin"); TPrintf(TString);

  int l = 0,c = 0,i = 0;
  char *s,*d;

  s = ss;

  while( (strchr(cs,c = *s) == NULL) && (*s != 0) ){ s++; i++; }

  if( *s == 0)
  {
    l = 0;
  }
  else
  {
    d = ds;
    while( (strchr(cs,c = *s) != NULL) && (*s != 0) )
    {
      *d = *s;
      d++;
      s++;
    }
    *d = '\0';
    l = i + strlen(ds);
  }

//  sprintf(TString,"TNSSpin::ReadValue: end"); TPrintf(TString);

  return( l );
}


float TNSSpin::GetValue(char *ss)
{
//  sprintf(TString,"TNSSpin::GetValue - begin"); TPrintf(TString);

  int c;
  float v;
  char *ds,*s,*d;
  c = strlen(ss);
  ds = new char[c];
    memset(ds,0,c);

  s = ss;
  d = ds;

  while( (isdigit(c = *s) == 0) && (c != '\0') ) s++; // ?

  while( isdigit(c = *s) != 0 )
  {
    *d = *s;
    d++;
    s++;
  }
  *d = 0;

  v = atof(ds); 

  if( strchr(ss,'/') != NULL ) v = v/2.;
  if( strchr(ss,'-') != NULL ) v = (-1)*v;

  delete[] ds;

//  sprintf(TString,"TNSSpin::GetValue - end"); TPrintf(TString);

  return( v );
}
//------------------------------------------------------------------------------

