//------------------------------------------------------------------------------
// TNSHLife.cpp
//------------------------------------------------------------------------------
// T(ype)N(ucleus)S(tate)H(alf)Life
// C.: 2006.06.06.
// M.: 2015.05.15.
//------------------------------------------------------------------------------

// for test --------------------------------------------------------------------
extern char *TString;
extern void TPrintf(char *s);
extern FILE *LogFile;
// for test --------------------------------------------------------------------

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "TNSHLife.h"


TNSHLife::TNSHLife()
{
  data = 0;
  HLife = 0.;
  Scale = new char[10];
}


TNSHLife::~TNSHLife()
{
  delete[] Scale;
}


int TNSHLife::ReadHLife(char *ss)
{
//  FILE *ofl;
//  ofl = fopen("hlife-test.tmp","a");

  int Len,i = 0,dn = 0,l = 0;
  Len = strlen(ss);

  char *ds,*cs,*s,*d;
  ds = new char[Len];
    memset(ds,0,Len);

  s = ss;

//  while( (l = ReadValue(s,ds)) != 0 ){ i++; s+=l; }

  cs = new char[16];
  sprintf(cs,"0123456789.+-Ee\0");

  l = ReadValue(s,ds);

  while( l != 0 )
  {
    dn = CountDigitNumber(ds,cs);

    Len = strlen(ds);

    if( (dn != Len) && (HLife == 0.) ) HLife = atof(ds);
    else{
      if( (dn != Len) && (HLife != 0.) ) dHLife = atof(ds);
      else
      {
        delete[] Scale;
        Scale = new char[Len];
        strcpy(Scale,ds);
//        fprintf(ofl,"Scale = %s\n",Scale);
      }
    }
    s+=l;
    l = ReadValue(s,ds);
  }  

//  fprintf(ofl,"HLife = %lf\n",HLife);

//  fclose(ofl);

  delete[] ds;
  delete[] cs;

  return( 0 ); // ???
}


int TNSHLife::ReadValue(char *ss,char *ds)
{
  int l = 0,i = 0;
  char *s,*d;

  s = ss;
  while( (*s == ' ') && (*s != 0) ){ s++; i++; }

  if( *s == 0)
  {
    l = 0;
  }
  else
  {
    d = ds;
    while( (*s != ' ') && (*s != 0) )
    {
      *d = *s;
      d++;
      s++;
    }
    *d = 0;
    l = i + strlen(ds);
  }

  return( l );
}


int TNSHLife::CountDigitNumber(char *ss,char *cs)
{
  int c,dn = 0;
  char *s,*d;

  s = ss;
  d = ss;

  while( *s != 0 )
  {
    if( (strchr(cs,c = *s) != NULL) || (isalpha(c) != 0) )
    {
      *d = c;
      d++;
      s++;
      if( isalpha(c) != 0 ) dn++;
    }
    else
    {
      s++;
    }
  }
  *d = 0;

  return( dn );
}


float TNSHLife::GetValue(char *ss)
{
  float v;
  char *ds,*s,*d;
  ds = new char[4]; // why 4 ? 000\0
    memset(ds,0,4);

  s = ss;
  d = ds;

  while( isdigit(*s) != 1 ) s++;

  while( isdigit(*s) == 1 )
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

  return( v );
}
//------------------------------------------------------------------------------

