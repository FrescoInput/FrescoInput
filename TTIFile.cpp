//------------------------------------------------------------------------------
// TTIFile.cpp
//------------------------------------------------------------------------------
// TypeT(ext)I(nput)File
// C.: 2008.02.28.
// M.: 2015.05.15.
//------------------------------------------------------------------------------

// for test
extern void TPrintf(char *s);
extern char *TString;
// for test

#include <stdlib.h>

#include "TTIFile.h"

TTIFile::TTIFile()
{
  cDRN = 0;
  cFileDataRow = (struct cDRow*)malloc( sizeof(struct cDRow) );
  
  cCDN = 0;
  cCData = (struct cCD*)malloc( sizeof(struct cCD) );
}

TTIFile::~TTIFile()
{
  free( cFileDataRow );
  free( cCData );
}

/*/
void TTIFile::Set(FILE *fl)
{
  ifl = fl;
}
/*/

int TTIFile::Read(FILE *fl)
{
  int i = 0;
  
  return( i ); 
}

int TTIFile::Write(FILE *fl)
{
  int i = 0;
  
  return( i );
}


int TTIFile::FileDataRead(char *str,int max)
{
  int r,l;

  do
  {
    r = FileRowRead(str,max);
    if( r != EOF )
    {
      l = StrComntDel(str);
      l = StrSpaceDel(str);
    }
  }
  while( (l == 0) && (r != EOF) );

  if( r == EOF ){ l = EOF; }

  return( l );
}


int TTIFile::StrnGetData(char *str,char *ds)
{
//  fprintf(LogFile,"TRNucleus.StrnGetData - begin\n");

  char *s,*d;

  s = str;

  while( (isalnum(*s) == 0) && (*s != '\0') ) s++;

  if( *s != '\0' )
  {
    d = ds;
    *d = '\0';
    while( (isalnum(*s) != 0) || (*s == '.') )
    {
      *d = *s;
      d++;
      s++;
    }
    *d = '\0';
  }

//  fprintf(LogFile,"TRNucleus.StrnGetData - end\n");

  return( strlen(ds) );
}


int TTIFile::StrnGetKind(char *kind,char *str)
{
  int i = 0,l,rv = 1; // r(eturn)v(alue)
  char *s;

  l = strlen(kind);

  s = str;

  while( (strlen(s) >= l) && ((rv = strncasecmp(s,kind,l)) != 0) )
  {
    s++;
    i++;
  }

  if( rv == 0 ) rv = i + l;
  else rv = 0;

  return( rv );
}


int TTIFile::StrComntDel(char *str)
{
  int l;
  char *s;

  if( (s = strchr(str,'#')) != NULL) *s = '\0';

  l = strlen(str);

  return( l );
}


int TTIFile::StrSpaceDel(char *str)
{
  int rv;
  char *s,*d;

  s = str;
  d = str;

  while( *s != '\0' )
  {
    if( *s != ' ' )
    {
      *d = *s;
      d++;
    }
    s++;
  }
  *d = '\0';

  rv = strlen(str);

  return( rv );
}


int TTIFile::FileRowRead(char *str,int max)
{
//  sprintf(TString,"TFFile::RowRead : begin"); TPrintf(TString);

// int max;
// max = strlen(str);

  int l = 0,c;
  char *s;

  s = str;

  *s = '\0';

  while( ((c = fgetc(ifl)) != 10) && (c != EOF) && (l < max) ) // ???
  {
    *s = c;
    s++;
    l++;
  }
  *s = '\0';

  l = strlen(str);

  if( (l == 0) && (c == EOF) ) l = EOF;

//  sprintf(TString,"TFFile::RowRead : end"); TPrintf(TString);

  return( l );
}


//*/----------------------------------------------------------------------------


void TTIFile::Set( FILE *fl )
{
  ifl = fl;
    
  char *str;
  int i,max = 200;
  str = new char[max];

  int DatRowNumber = 0;

  DatRowNumber = DataRowNumber( fl );
  
  printf("DataRowNumber = %d\n",DatRowNumber);

  free( cFileDataRow );  
  cFileDataRow = (struct cDRow*)malloc( sizeof(struct cDRow) * DatRowNumber );
  
  for( i=0;i<DatRowNumber;i++ )
  {
    *(cFileDataRow[i].p) = '\0';
      cFileDataRow[i].pe =   0.;
    *(cFileDataRow[i].t) = '\0';
      cFileDataRow[i].te =   0.;
      cFileDataRow[i].e  =   0.;
/*/
    printf("i = %d %5s %f %5s %f\n",i,
            cFileDataRow[i].p,
            cFileDataRow[i].pe,
            cFileDataRow[i].t,
            cFileDataRow[i].te );
/*/
  }
  
  cDRN = DatRowNumber;
  
  FileRead( fl );
  
  if( cDRN > 1 ) ChannelData(); // else - ???
  
  delete[] str;
}
      
int TTIFile::DataRowNumber(FILE *fl)
{
  sprintf(TString,"TTIFile.DataRowNumber - begin"); TPrintf(TString);

  int r,l;

  int DRN = 0;

  char *str;
  int max = 200; // ???
  str = new char[max];

  do
  {
    r = FileRowRead(str,max);
    if( r != EOF )
    {
      l = StrComntDel(str);
      l = StrSpaceDel(str);
    }
    if( l ) DRN++;
  }
  while( r != EOF );

  delete[] str;

  if( r == EOF ){ l = EOF; }

  sprintf(TString,"TTIFile.DataRowNumber - end"); TPrintf(TString);

  rewind( fl );

  return( DRN );
}
        
        
void TTIFile::FileRead( FILE *fl )
{
  sprintf(TString,"TTIFile.FileRead - begin"); TPrintf(TString);

  int r,l;

  char *str;
  int max = 200;
  str = new char[max];

  int i;
  
  char *s,*n,*e;
  
  for(i=0;i<cDRN;i++)
  {

   do
    {
    r = FileRowRead(str,max);
    if( r != EOF )
      {
        l = StrComntDel(str);
//        l = StrSpaceDel(str);
      }
    }
    while( (l == 0) && (r != EOF) );

//  sprintf(TString,"TTIFile.FileRead str - %s",str); TPrintf(TString);

//*/

  s = str;
  e = s + strlen( s );

  if( (n = Next( s,e )) != NULL )
  {
    sprintf( cFileDataRow[i].p,"%s\0",n );
    s = n + strlen(n);
    s++;
  }

  if( (n = Next( s,e )) != NULL )
  {
    if( n < e )
    {
      cFileDataRow[i].pe = atof(n);
      s = n + strlen(n);
      s++;
    }
  }

  if( (n = Next( s,e )) != NULL )
  {
    sprintf( cFileDataRow[i].t,"%s\0",n );
    s = n + strlen(n);
    s++;
  }
  
  if( (n = Next( s,e )) != NULL )
  {
    cFileDataRow[i].te = atof(n);
    s = n + strlen(n);
    s++;
  }

  if( (n = Next( s,e )) != NULL )
  {
    cFileDataRow[i].e = atof(n);
    s = n + strlen(n);
    s++;
  }

/*/
   sprintf(TString,"i = %d %5s %lf %5s %lf %lf\n",i,
            cFileDataRow[i].p,
            cFileDataRow[i].pe,
            cFileDataRow[i].t,
            cFileDataRow[i].te,
            cFileDataRow[i].e
    ); TPrintf(TString);
/*/
  
//*/

  }


//  rewind( fl );

  delete[] str;

  sprintf(TString,"TTIFile.FileRead - end"); TPrintf(TString);

//  return( l );
}


char* TTIFile::Next( char *s,char *e )
{
//  sprintf(TString,"TTIFile.Next - begin"); TPrintf(TString);

//  sprintf(TString,"TTIFile.Next s - %s",s); TPrintf(TString);

  char *n;
  n = NULL;

  if( s < e )
  {
    while( (*s == ' ') && ( s < e ) ) s++;

    n = s;

//  sprintf(TString,"TTIFile.Next n - %s",n); TPrintf(TString);

    s = strchr( n,' ' );
  
    if( (s != NULL) && (s < e) ) *s = '\0';
  }
  
//  sprintf(TString,"TTIFile.Next s - %s",s); TPrintf(TString);


//  sprintf(TString,"TTIFile.Next - end"); TPrintf(TString);

  return( n );
}


void TTIFile::ChannelData()
{
  sprintf(TString,"TTIFile.ChannelData - begin"); TPrintf(TString);

  char *fName;
  fName = new char[6];
  *fName = '\0';

  int i = 1;
  
  cCDN = 0; // ?
  
  while( i < cDRN )
  {
    if( strcmp( fName,cFileDataRow[i].p ) )
    {
      cCDN++;
      strcpy( fName,cFileDataRow[i].p );
    }

    i++;
  }
  
//  sprintf(TString,"TTIFile.ChannelData cCDN = %d",cCDN); TPrintf(TString);

  if( cCDN )
  {
    free( cCData );
    cCData = (struct cCD*)malloc( sizeof(struct cCD)*cCDN );
    
    *fName = '\0';
    i = 1;
    
    int cn = 0,pn = 1,tn = 0;
    
    while( i < cDRN )
    {
      if( strcmp( fName,cFileDataRow[i].p ) )
      {
        if( cn )
        {
          cCData[cn-1].First  = i - pn - tn;
          cCData[cn-1].PNumber = pn;
          cCData[cn-1].TNumber = tn;
        }
        cn++;
        pn = 1;
        tn = 0;
        
        strcpy( fName,cFileDataRow[i].p );
      }
      else
      {
        if( cFileDataRow[i].pe )
          pn++;
        else
          tn++;
      }
      
      i++;
    }
    
    cCData[cn-1].First  = i - pn - tn;
    cCData[cn-1].PNumber = pn;
    cCData[cn-1].TNumber = tn;

/*/ test
    i = 0;
    while( i < cCDN )
    {
  sprintf(TString,"cCData[%d] : %d %d %d",i,cCData[i].First,cCData[i].PNumber,cCData[i].TNumber); TPrintf(TString);
      i++;
    }
/*/
  }

  delete[] fName;

  sprintf(TString,"TTIFile.ChannelData - end"); TPrintf(TString);
}


char* TTIFile::Projectile( int N )
{
//  sprintf(TString,"TTIFile.Projectile - begin"); TPrintf(TString);
  
  if( N )
  {
    return( cFileDataRow[ cCData[N-1].First ].p );
  }
  else
  {
    return( cFileDataRow[0].p );
  }
  
//  sprintf(TString,"TTIFile.Projectile - end"); TPrintf(TString);
}


int TTIFile::ProjectileStateNumber( int N )
{
//  sprintf(TString,"TTIFile.ProjectileSN - begin"); TPrintf(TString);
  
  if( N )
  {
    return( cCData[N-1].PNumber ); // ??? g.s.?
  }
  else
  {
    return( 0 ); // ???
  }
  
//  sprintf(TString,"TTIFile.ProjectileSN - end"); TPrintf(TString);
}


double TTIFile::ProjectileState( int N,int i )
{
//  sprintf(TString,"TTIFile.ProjectileS - begin"); TPrintf(TString);
  
  if( N )
  {
    return( cFileDataRow[ cCData[N-1].First + i ].pe );
  }
  else
  {
    return( 0. );
  }
  
//  sprintf(TString,"TTIFile.ProjectileS - end"); TPrintf(TString);
}


char* TTIFile::Target( int N )
{
//  sprintf(TString,"TTIFile.Target - begin"); TPrintf(TString);
  
  if( N )
  {
    return( cFileDataRow[ cCData[N-1].First ].t );
  }
  else
  {
    return( cFileDataRow[0].t );
  }
  
//  sprintf(TString,"TTIFile.Target - end"); TPrintf(TString);
}


int TTIFile::TargetStateNumber( int N )
{
//  sprintf(TString,"TTIFile.TargetSN - begin"); TPrintf(TString);
  
  if( N )
  {
    return( cCData[N-1].TNumber ); // ??? g.s.?
  }
  else
  {
    return( 0 ); // ???
  }
  
//  sprintf(TString,"TTIFile.TargetSN - end"); TPrintf(TString);
}


double TTIFile::TargetState( int N,int i )
{
//  sprintf(TString,"TTIFile.TargetS - begin"); TPrintf(TString);
  
  if( N )
  {
    return( cFileDataRow[ cCData[N-1].First + cCData[N-1].PNumber + i - 1].te );
  }
  else
  {
    return( 0. );
  }
  
//  sprintf(TString,"TTIFile.TargetS - end"); TPrintf(TString);
}
//*/----------------------------------------------------------------------------
//------------------------------------------------------------------------------

