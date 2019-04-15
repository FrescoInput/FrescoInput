//------------------------------------------------------------------------------
// TRChannel.cpp
//------------------------------------------------------------------------------
// TypeR(action)Channel
// C.: 2005.mm.dd.
// M.: 2015.05.15.
//------------------------------------------------------------------------------

// for test --------------------------------------------------------------------
extern char *TString;
extern void TPrintf(char *s);
extern FILE *LogFile;
// for test --------------------------------------------------------------------

#include "TRChannel.h"


TRChannel::TRChannel()
{
  Projectile = new TRNucleus[1];
  Target     = new TRNucleus[1];
}


TRChannel::~TRChannel()
{
  delete[] Projectile;
  delete[] Target;
}


void TRChannel::Set(char *rcs)
{
//  sprintf(TString,"%s",rcs); TPrintf(TString);

  char *s,*d,*n;

  n = new char[6];
    memset(n,0,6);

  s = rcs;
  d = n;
  while( *s != ')' )
  {
    *d = *s;
    d++;
    s++;
  }
  *d = '\0';
  s++;

//  sprintf(TString,"TRChannel.Set projectile = %s",n); TPrintf(TString);

  Projectile[0].Set(n);

  d = n;
  while( *s != '\0' )
  {
    *d = *s;
    d++;
    s++;
  }
  *d = '\0';

  Target[0].Set(n);

  delete[] n;
}


void TRChannel::CheckLE(double fMEE)
{
  Projectile[0].CheckLE(fMEE);
  Target[0].CheckLE(fMEE);
}


void TRChannel::CheckLevels(int fNT,double *fLE,int fNoLE)
{
  switch( fNT )
  {
    case  1 : Projectile[0].CheckLevels(fLE,fNoLE); break;
    case  2 :     Target[0].CheckLevels(fLE,fNoLE); break;
    default : {} break;
  }
}


int TRChannel::GetProLevels(double *Lev,int fSN) // very,very bad!!!
{
//  sprintf(TString,"TRChannel.GetProLevels - begin"); TPrintf(TString);

  int i,lrv = 0;

  lrv = Projectile[0].GetNumberOfStates();

  if( lrv > fSN ) lrv = fSN; // full brad!!!

//  sprintf(TString,"TRChannel.GetProLevels.lrv = %d",lrv); TPrintf(TString);

  i = 0;
  while( i < lrv )
  {
    Lev[i] = Projectile[0].GetStateEnergy(i);
//  sprintf(TString,"TRChannel.GetProLevels.i = %d Energy = %lf",i,Lev[i]); TPrintf(TString);
    i++;
  } 
 
//  sprintf(TString,"TRChannel.GetProLevels - end"); TPrintf(TString);

  return(lrv);
} 


int TRChannel::GetTarLevels(double *Lev,int fSN)
{
  int i,lrv = 0;

  lrv = Target[0].GetNumberOfStates();

  if( lrv > fSN ) lrv = fSN;

//  sprintf(TString,"TRChannel.GetTarLevels.lrv = %d",lrv); TPrintf(TString);

  i = 0;
  while( i < lrv )
  {
    Lev[i] = Target[0].GetStateEnergy(i);
//  sprintf(TString,"TRChannel.GetTarLevels.i = %d Energy = %lf",i,Lev[i]); TPrintf(TString);
    i++;
  } 

  return(lrv);
} 


int TRChannel::Read(TTIFile *IFile,char *kind,int c)
{
  sprintf(TString,"TRChannel.Read - begin"); TPrintf(TString);

  int rv,r,l = 0,d; // return value
  long cpos,ppos,tpos;
  char *ws,*ds;

  ws = new char[80]; // why 80? it is bad!
    memset(ws,0,80);
  ds = new char[80];
    memset(ds,0,80);

  do
  {
    rv = IFile[0].FileDataRead(ws,80);

    if( rv != EOF ) 
    {
      l = IFile[0].StrnGetKind(kind,ws);
    }
  }
  while( (l == 0) && (rv != EOF) );

  if( l != 0 )
  {
    cpos = IFile[0].GetPos();

    r = Projectile[0].Read(IFile,(char*)"Projectile");
    ppos = IFile[0].GetPos(); 

    r = IFile[0].SetPos(cpos);
    r = Target[0].Read(IFile,(char*)"Target");
    tpos = IFile[0].GetPos(); 

    if( c == 0 ) // read input channel information
    {
      r = IFile[0].SetPos(cpos);
      do
      {
        r = IFile[0].FileDataRead(ws,80);

        if( r != EOF ) 
        {
          if( (l = IFile[0].StrnGetKind((char*)"Energy",ws)) != 0 )
          {
            d = IFile[0].StrnGetData(ws+l,ds);
            Energy = atof(ds);
          }
        }
      }
      while( (l == 0) && (r != EOF) );
    }

    if( tpos > ppos ){ r = IFile[0].SetPos(tpos); }
    else{ r = IFile[0].SetPos(ppos); }
  }
  else
  {
  }

  delete[] ds;
  delete[] ws;

  sprintf(TString,"TRChannel.Read - end"); TPrintf(TString);
  sprintf(TString,"\n"); TPrintf(TString);

  return( rv );
}


int TRChannel::ReadNew(TTIFile *IFile,char *kind,int N) // ,int c)
{
  sprintf(TString,"TRChannel.ReadNew - begin"); TPrintf(TString);

  int r;

  r = Projectile[0].ReadNew( IFile,(char*)"Projectile",N );
  r =     Target[0].ReadNew( IFile,(char*)"Target",N );

  if( !N )
  {
    Energy = IFile[0].Energy();
  }


/*/
  int rv,r,l = 0,d; // return value
  long cpos,ppos,tpos;
  char *ws,*ds;

  ws = new char[80]; // why 80? it is bad!
    memset(ws,0,80);
  ds = new char[80];
    memset(ds,0,80);

  do
  {
    rv = IFile[0].FileDataRead(ws,80);

    if( rv != EOF ) 
    {
      l = IFile[0].StrnGetKind(kind,ws);
    }
  }
  while( (l == 0) && (rv != EOF) );

  if( l != 0 )
  {
    cpos = IFile[0].GetPos();

    r = Projectile[0].Read(IFile,(char*)"Projectile");
    ppos = IFile[0].GetPos(); 

    r = IFile[0].SetPos(cpos);
    r = Target[0].Read(IFile,(char*)"Target");
    tpos = IFile[0].GetPos(); 

    if( c == 0 ) // read input channel information
    {
      r = IFile[0].SetPos(cpos);
      do
      {
        r = IFile[0].FileDataRead(ws,80);

        if( r != EOF ) 
        {
          if( (l = IFile[0].StrnGetKind((char*)"Energy",ws)) != 0 )
          {
            d = IFile[0].StrnGetData(ws+l,ds);
            Energy = atof(ds);
          }
        }
      }
      while( (l == 0) && (r != EOF) );
    }

    if( tpos > ppos ){ r = IFile[0].SetPos(tpos); }
    else{ r = IFile[0].SetPos(ppos); }
  }
  else
  {
  }

  delete[] ds;
  delete[] ws;
  
/*/
  sprintf(TString,"TRChannel.ReadNew - end"); TPrintf(TString);
  sprintf(TString,"\n"); TPrintf(TString);

  return( 0 ); // ???
}


void TRChannel::Write(FILE *ofl,char *w,int c)
{
//  sprintf(TString,"TRChannel.Write - begin"); TPrintf(TString);

//  fprintf(ofl,"#%s :\n",w);

//  Projectile[0].Write(ofl,(char*)"Projectile");
//  Target[0].Write(ofl,(char*)"Target");

  int i,sn;
  char *p,*t,*s;

  if( !strcasecmp(w,"Entrance Channel") )
  {
    p = Projectile[0].Symbol();
    t = Target[0].Symbol();

    sprintf(TString,"%*s %lf %*s %lf",strlen(p),p,Projectile[0].StateEnergy(0),
                                      strlen(t),t,    Target[0].StateEnergy(0) );
    DelSpace( TString );
    fprintf(ofl,"\n%s",TString);
//                                       TPrintf(TString);

    fprintf(ofl," %lf",Energy);
//    fprintf(ofl,"%-17s : %7.3lf\n","#Energy",Energy);
  }
  else
  {

//  sn = Projectile[0].StateNumber();
  sn = Projectile[0].GetNumberOfStates();
//  sprintf(TString,"TRChannel.Write p sn = %d",sn); TPrintf(TString);

    p = Projectile[0].Symbol();
    t = Target[0].Symbol();

//  sprintf(TString,"TRChannel.Write p len = %d symbol = %s",strlen(p),p); TPrintf(TString);
//  sprintf(TString,"TRChannel.Write t len = %d symbol = %s",strlen(t),t); TPrintf(TString);
  
  i = 0;
  while( i < sn )
  {
    sprintf(TString,"%*s %lf %*s %lf",strlen(p),p,Projectile[0].StateEnergy(i),
                                      strlen(t),t,    Target[0].StateEnergy(0) );
    DelSpace( TString );
    fprintf(ofl,"\n%s",TString);
//                                       TPrintf(TString);
    i++;
  }


  sn = Target[0].GetNumberOfStates();
//  sprintf(TString,"TRChannel.Write t sn = %d",sn); TPrintf(TString);

    p = Projectile[0].Symbol();
    t = Target[0].Symbol();

//  sprintf(TString,"TRChannel.Write p len = %d",strlen(p)); TPrintf(TString);
//  sprintf(TString,"TRChannel.Write t len = %d",strlen(t)); TPrintf(TString);

  i = 1;
  while( i < sn )
  {
    sprintf(TString,"%*s %lf %*s %lf",strlen(p),p,Projectile[0].StateEnergy(0),
                                      strlen(t),t,    Target[0].StateEnergy(i) );
    DelSpace( TString );
    fprintf(ofl,"\n%s",TString);
//                                       TPrintf(TString);
    i++;
  }

  }  


//  fprintf(ofl,"\n",w);

//  sprintf(TString,"TRChannel.Write - end"); TPrintf(TString);
}


char* TRChannel::DelSpace(char *str)
{
  char *s,*d,*e;

  s = str;
  d = str;
  e = str + strlen(str);
  
  while( s < e )
  {
    if( *s != ' ' )
    {
      *d = *s;
      d++;
    }
    else
    {
      if( *(d - 1) != ' ' )
      {
        *d = *s;
        d++;
      }
    }
    s++;
  }
  *d = '\0';

  return( str );
}
//------------------------------------------------------------------------------

