//------------------------------------------------------------------------------
// TCRCScheme.cpp
//------------------------------------------------------------------------------
// TypeC(oupled)R(eaction)C(hannel)Scheme
// C.: 2005.10.13.
// M.: 2015.05.15.
//------------------------------------------------------------------------------

// for test --------------------------------------------------------------------
extern char *TString;
extern void TPrintf(char *s);
extern FILE *LogFile;
// for test --------------------------------------------------------------------

#include "TCRCScheme.h"

// global data -----------------------------------------------------------------
extern int gNoRCI;
extern int *gRCIndex;
// global data -----------------------------------------------------------------


TCRCScheme::TCRCScheme()
{
  Reaction = new TNReaction[1];

  TR = 0;
  RChannel = new TRChannel[1];

  gNoRCI = 0;
  gRCIndex = new int[1];

  cIFName = new char[1];
}


TCRCScheme::~TCRCScheme()
{
  delete[] Reaction;
  delete[] RChannel;
  delete[] gRCIndex;
  delete[] cIFName;
}

/*
void TCRCScheme::Set()
{

  delete cIFName;
  cIFName = new char[10];
  strcpy(cIFName,"test.file\0")

}
*/
void TCRCScheme::Set( char *InputFileName )
{
  sprintf(TString,"TCRCScheme.Set - begin"); TPrintf(TString);

  FILE *ifl;

  if( (ifl = fopen(InputFileName,"r")) != NULL ) // ?
  {
    Set( ifl );
    fclose(ifl);
    
    CreateFIFile( InputFileName );
  }
  else
  { 
    exit(0); // ??
  }

  sprintf(TString,"TCRCScheme.Set - end"); TPrintf(TString);
}


void TCRCScheme::Set(FILE *ifl)
{
  sprintf(TString,"TCRCScheme.Set(f) - begin"); TPrintf(TString);

  int fresco = 0,NC = 0;

//  if( fresco == 0 ) Read( ifl );
  if( fresco == 0 ) ReadNew( ifl );
//  WriteTest( (char*)"ReadNew - done" );

  sprintf(TString,"TCRCScheme.Set TR = %d\n",TR); TPrintf(TString);

  if( TR == 0 )
  {
    printf("Entrance channel is absent!\nCode stoped.\n");
    exit( 0 );
  }

  sprintf(TString,"TCRCScheme.Set gNoRCI.1 = %d\n",gNoRCI); TPrintf(TString);

  NC = Reaction[0].SetInChannel(RChannel);
//  WriteTest( (char*)"SetInChannel - done" );

  if( TR == 1 ) Generate( NC );
//  WriteTest( (char*)"Generate - done" );

  Reaction[0].SetOutChannel(RChannel,TR);
//  WriteTest( (char*)"SetOutChannel - done " );

  Reaction[0].SetTRChannel(RChannel,gNoRCI);
//  WriteTest( (char*)"SetTRChannel - done " );

  sprintf(TString,"TCRCScheme.Set.gNoRCI.2 = %d\n",gNoRCI); TPrintf(TString);

//  CreateFIFile();

  sprintf(TString,"TCRCScheme.Set(f) - end"); TPrintf(TString);
}


void TCRCScheme::Read( FILE *ifl )
{
  sprintf(TString,"TCRCScheme.Read - begin"); TPrintf(TString);

  int c,i;

  TTIFile *IFile;
  IFile = new TTIFile[1];
  IFile[0].Set(ifl);

  TR = 0;
  if( (c = RChannel[0].Read(IFile,(char*)"EntranceChannel",0)) != EOF )
  {
    TR = 1;
    rewind( ifl );
    while( (c = RChannel[0].Read(IFile,(char*)"OutgoingChannel",1)) != EOF ){ TR++; }// how to do it more easy?

    sprintf(TString,"TCRCScheme.Read.TR = %d\n",TR); TPrintf(TString);

    if( TR > 1 )
    {
      delete[] RChannel;
      RChannel = new TRChannel[TR];

// global data - bad : ---------------------------------------------------------
      gNoRCI = TR;
      delete[] gRCIndex;
      gRCIndex = new int[gNoRCI];
//------------------------------------------------------------------------------

      rewind( ifl );
      c = RChannel[0].Read(IFile,(char*)"EntranceChannel",0);

      gRCIndex[0] = 0;

      i = 1;
      rewind( ifl );
      while( (c = RChannel[i].Read(IFile,(char*)"OutgoingChannel",1)) != EOF )
      {
        gRCIndex[i] = i;
        i++;
      }
    }
  }

  delete[] IFile;

  sprintf(TString,"TCRCScheme.Read - end"); TPrintf(TString);
}


void TCRCScheme::ReadNew( FILE *ifl )
{
  sprintf(TString,"TCRCScheme.ReadNew - begin"); TPrintf(TString);

  int c,i;

  TTIFile *IFile;
  IFile = new TTIFile[1];

  IFile[0].Set( ifl );

  if( IFile[0].DataRowNumber() )
  {
//    c = RChannel[0].ReadNew( IFile,(char*)"EntranceChannel",0 ); // ???

    TR = 1;
    
    if( c = IFile[0].ChannelNumber() ) 
    {
      TR+=c;
    
      delete[] RChannel;
      RChannel = new TRChannel[TR];

// global data  : ---------------------------------------------------------
      gNoRCI = TR;
      delete[] gRCIndex;
      gRCIndex = new int[gNoRCI];
//------------------------------------------------------------------------------
      gRCIndex[0] = 0;

      i = 1;
      while( i < TR )
      {
        c = RChannel[i].ReadNew( IFile,(char*)"OutgoingChannel",i );
        gRCIndex[i] = i;
        i++;
      }
    }
    
    c = RChannel[0].ReadNew( IFile,(char*)"EntranceChannel",0 );
  }
  else
  {
    // ???
  }

  delete[] IFile;

  sprintf(TString,"TCRCScheme.ReadNew - end"); TPrintf(TString);
}


void TCRCScheme::Write( FILE *ofl )
{
  sprintf(TString,"TCRCScheme.Write - begin"); TPrintf(TString);

  int i;

  fprintf(ofl,"#Projectile Target Energy");

  RChannel[0].Write(ofl,(char*)"Entrance Channel",0);

  i = 1;
  while( i < TR){ RChannel[i].Write(ofl,(char*)"Outgoing Channel",1); i++;}

  fprintf(ofl,"\n");

  sprintf(TString,"TCRCScheme.Write - end"); TPrintf(TString);
}


void TCRCScheme::WriteTest( char *str )
{
  FILE *ofile;
  ofile = fopen("RChannel.test","a");

  fprintf(ofile,"\n%s\n\n",str);

  Write( ofile );

  fclose(ofile);
}


void TCRCScheme::Generate(int NC)
{
  sprintf(TString,"TCRCScheme.Generate - begin"); TPrintf(TString);
  sprintf(TString,"TCRCScheme.Generate ReacTest : NC = %3d",NC); TPrintf(TString);


  int i; 
  char *RS;
  char *s,*d,*rcs;

  RS = new char[NC*12+1];
    memset(RS,0,NC*12+1); // long var = NC*12+1 - better?

  Reaction[0].GetOutChannel(RS);
  sprintf(TString,"TCRCScheme.Generate RString = %s",RS); TPrintf(TString);

  s = RS;

  rcs = new char[13];
    memset(rcs,0,13);

  char *p,*t;
  p = new char[6];
    memset(p,0,6);
  t = new char[6];
    memset(t,0,6);

  RChannel[0].GetProjectile(p);
  RChannel[0].GetTarget(t);

  sprintf(rcs,"%s)%s;\0",p,t); 

//  delete[] p;
//  delete[] t;


  TR = NC + 2; // for entrance and inelastic channel... ???
  delete[] RChannel;
  RChannel = new TRChannel[TR];

// bad :
  gNoRCI = TR;
  delete[] gRCIndex;
  gRCIndex = new int[gNoRCI];
//

//------------------------------------------------------------------------------
  RChannel[0].Set(rcs);
  gRCIndex[0] = 0;
//------------------------------------------------------------------------------
  RChannel[1].Set(rcs); // inelastic scattering...
  gRCIndex[1] = 1;
//------------------------------------------------------------------------------

  i = 2;
  while(i < TR)
  {
    d = rcs;
    while( *s != ';' )
    {
      *d = *s;
      d++;
      s++;
    }
    *d = '\0';
    s++;

//  sprintf(TString,"TCRCScheme.Generate RChannel[%d] = %s",i,rcs); TPrintf(TString);
    
    RChannel[i].Set(rcs);
    gRCIndex[i] = i;

/*/
    RChannel[i].GetProjectile( p );
    RChannel[i].GetTarget( t );
  sprintf(TString,"TCRCScheme.Generate RChannel[%d].Projectile = %s",i,p); TPrintf(TString);
  sprintf(TString,"TCRCScheme.Generate RChannel[%d].Target = %s",i,t); TPrintf(TString);
/*/

    i++;
  }

  delete[] rcs;
  delete[] RS;


  delete[] p;
  delete[] t;


  sprintf(TString,"TCRCScheme.Generate - end"); TPrintf(TString);
}


void TCRCScheme::CreateFIFile( char *InputFileName )
{
  sprintf(TString,"TCRCScheme.CreateFIFile - begin"); TPrintf(TString);

  char *FrescoFileName;
  TFFile *FrescoInFile;
  
  FrescoFileName = new char[ strlen(InputFileName) + 10 ];
  FrescoInFile = new TFFile[1];

  strcpy( FrescoFileName,InputFileName );
  strcpy( strchrnul(FrescoFileName,'.'),".fresco\0" );

  FrescoInFile[0].Create(RChannel,gNoRCI);
//  FrescoInFile[0].Create(RChannel,TR);
//  FrescoInFile[0].WriteFile((char*)"fresco.in");
  FrescoInFile[0].WriteFile( FrescoFileName );

  delete[] FrescoInFile;
  delete[] FrescoFileName;

  sprintf(TString,"TCRCScheme.CreateFIFile - end"); TPrintf(TString);
}
//------------------------------------------------------------------------------
