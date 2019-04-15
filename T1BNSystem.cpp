//------------------------------------------------------------------------------
// T1BNSystem.cpp
//------------------------------------------------------------------------------
// T(ype)1(One)B(ody)N(uclear)System
// C.: 1997.04.22.
// V.: 2002.12.18. v. For ODESA
// M.: 2015.05.15.
//------------------------------------------------------------------------------

// for test --------------------------------------------------------------------
extern char *TString;
extern void TPrintf(char *s);
extern FILE *LogFile;
// for test --------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "T1BNSystem.h"

extern char *NDBPath; // ?
extern TTFDBase *NDBase;

extern TNDDBase *NDDBase;
extern TMySQLDB *MySQLDB;

T1BNSystem::T1BNSystem()
{
  Symbol =  new char[6];
  Isotope = new char[6];

  Initialize(); 

  States = new TNState[SN];

  Level = new double[NL+1];
  Level[0] = 0.;

  cLevIndex = new int[cNoL+1];
  cLevIndex[0] = 0;
/*/
  cNoD = 0;
  cNuclDeform = new TNDeformation[cNoD+1];
/*/
//  BasePath = new char[2];
}


T1BNSystem::~T1BNSystem()
{
  delete[] Symbol;
  delete[] Isotope;
  delete[] States;
  delete[] Level;
//  delete[] cNuclDeform;
//  delete[] BasePath;
}


void T1BNSystem::Initialize()
{
  Z = -1;
  N = -1;
  A = -1;
  MinA = 0;
  MaxA = 0;
  cAM = 0.;
  cME = 0.;
  cBE = 0.;
  memset(Symbol,0,6);
  memset(Isotope,0,6);

  SN   = 1; // S(tate) N(umber), g.s.
  cNoS = 1;
  NL   = 1; // N(umber of)L(evels)
  cNoL = 1;

  cCN = 0;
//  memset(BasePath,0,2);
//  Level[0] = 0.;
}

/*
void T1BNSystem::SetBasePath(char *s)
{
  delete[] BasePath;
  int Len;
  Len = strlen(s)+2;
  BasePath = new char[Len];
    memset(BasePath,0,Len);
  strcpy(BasePath,s);
}
*/

int T1BNSystem::Set(char *s)
{
//  sprintf(TString,"T1BNSystem.Set(char*) - begin"); TPrintf(TString);
//  sprintf(TString,"T1BNSystem.Set(char*) : s = %s",s); TPrintf(TString);

  int r = 0;

  r = SetIsotope(s);

//  sprintf(TString,"T1BNSystem.Set(char*) - end"); TPrintf(TString);

  return( r );
}


int T1BNSystem::Set(int p) // What if p == 0?
{
//  sprintf(TString,"T1BNSystem.Set(Z) - begin p = %3d",p); TPrintf(TString);

  int e = 0;

  Initialize();
  Z = p;
  if(Z == 0)
  {
    N = 1;
    A = 1;
    MinA = 1;
    MaxA = 1;
    strcpy(Symbol,"n");
  }
  else
    ReadDBFile();

  if(strlen(Symbol) != 0) e = 1;

//  sprintf(TString,"T1BNSystem.Set(Z) - end"); TPrintf(TString);

  return( e );
}


int T1BNSystem::Set(int p,int n) // vyznachytys chy Set(A,Z) chy Set(Z,N) !!!!!
{
//  sprintf(TString,"T1BNSystem.Set(Z,N) - begin"); TPrintf(TString);
//  sprintf(TString,"T1BNSystem.Set(Z,N) : Z = %3d N = %3d",p,n); TPrintf(TString);

  int e = 0;

  Initialize();

  Z = p;
  N = n;
//  A = Z + N; 
  if( Z == 0 )
  {
    A = N;
    strcpy(Symbol,"n");
    sprintf(Isotope,"%d%s",A,Symbol);
  }

  ReadDBFile();

  if(strlen(Symbol) != 0)
  {
    sprintf(Isotope,"%d%s",A,Symbol); // Is sprintf() function bad for this?
    e = 1;
  }

//  sprintf(TString,"T1BNSystem.Set(Z,N) - end"); TPrintf(TString);

  return( e );
}


int T1BNSystem::SetCluster(int p,int n)
{
  int lrv = 0;

  Initialize();

  Z = p;
  N = n;
//  A = Z + N; 
  if( Z == 0 ) // for neutron
  {
    A = N;
    strcpy(Symbol,"n");
    sprintf(Isotope,"%d%s",A,Symbol);
  }
  else // for proton
  {
    A = Z;
    strcpy(Symbol,"H");
    sprintf(Isotope,"%d%s",A,Symbol);
  }

  SetLevels(); // read excited state information

//  ReadDBFile();
/*
  if(strlen(Symbol) != 0)
  {
    sprintf(Isotope,"%d%s",A,Symbol); // Is sprintf() function bad for this?
    lrv = 1;
  }
*/
  return( lrv );
}


void T1BNSystem::SetLevels(double *fLev,int fNoL)
{
//  sprintf(TString,"T1BNSystem.SetLevels - begin"); TPrintf(TString);

  int i = 0,j = 0,li = 0;
  double lLE,lSE;

  cNoL = fNoL;

  if( cNoL > cNoS ){ cNoL = cNoS; } 

  delete[] cLevIndex;
  cLevIndex = new int[cNoL];

//  sprintf(TString,"T1BNSystem.SetLevels: cNoL = %d",cNoL); TPrintf(TString);

  li = 0;
  i = 0;
  while( i < cNoL )
  {
    lLE = fLev[i];

//  sprintf(TString,"T1BNSystem.SetLevels: i = %d lLE = %lf",i,lLE); TPrintf(TString);
//  sprintf(TString,"T1BNSystem.SetLevels: cNoS = %d",cNoS); TPrintf(TString);

    j = 0;
    while( (j < cNoS) && ( lLE != ( (lSE = States[j].GetEnergy())/1000. ) ) )
//  while( j < cNoS )
    {
      j++;
//  sprintf(TString,"T1BNSystem.SetLevels: j = %d",j); TPrintf(TString);
    }

    if( j != cNoS ){ cLevIndex[li] = j; li++; }

    i++;
  }

/*// test only :
  i = 0;
  while( i < cNoL )
  {
    sprintf(TString,"T1BNSystem.SetLevels: i = %d Ind = %d Energy = %lf",i,cLevIndex[i],States[cLevIndex[i]].GetEnergy()/1000); TPrintf(TString);
    i++;
  }
*///

//  sprintf(TString,"T1BNSystem.SetLevels - end"); TPrintf(TString);
}


void T1BNSystem::SetMaxLevEnergy(double fEV)
{

}


double T1BNSystem::GetLevelEnergy(int fI)
{
  double lrv = 0.;

//  sprintf(TString,"T1BNSystem.GetLevelEnergy: fI = %d",fI); TPrintf(TString);

//  sprintf(TString,"T1BNSystem.GetLevelEnergy: Index = %d",cLevIndex[fI]); TPrintf(TString);

  if( fI < cNoL ) lrv = (States[cLevIndex[fI]].GetEnergy())/1000.; 

  return( lrv );
}


int T1BNSystem::SetIsotope(char *fs)
{
//  sprintf(TString,"T1BNSystem.SetIsotope(char) - begin"); TPrintf(TString);
//  sprintf(TString,"T1BNSystem.SetIsotope(char) s = %5s",fs); TPrintf(TString);

  int e = 0;
  char *s;

  s = new char[6];
    memset(s,0,6);

  strcpy(s,fs);

//*/ 
  if( strcmp(fs,"n") == 0 ){ strcpy(s,"1N"); Z = 0; }
  if( strcmp(fs,"1n") == 0 ){ strcpy(s,"1N"); Z = 0; } // ???
   
  if( strcmp(fs,"p") == 0 ){ strcpy(s,"1H"); } 
  if( strcmp(fs,"d") == 0 ){ strcpy(s,"2H"); } 
  if( strcmp(fs,"t") == 0 ){ strcpy(s,"3H"); } 

  if( strcmp(fs,"2p") == 0 ){ strcpy(s,"2He"); } 
  if( strcmp(fs,"2n") == 0 ){ strcpy(s,"2N"); Z = 0; } 
//*/

  Initialize();

//  sprintf(TString,"T1BNSystem.SetIsotope(char) Initialize - Ok"); TPrintf(TString);

  if( strlen(s) <= 5 )
  {
    strcpy(Isotope,s);
    char *c,*sZ;
    sZ = new char[5];
      memset(sZ,0,5);
    c = Isotope;
//    int i = 0,a = 0;//,d = 0;

    char *e,*a,*d;
    
    a = Symbol;
    d = sZ;
    
    e = c + strlen( Isotope ); 
    
    while( c < e )
    {

      while( (isalpha(*c) != 0) && (c < e) )
      {
        *a = *c;
        a++;
        c++;
      }

      while( (isdigit(*c) != 0) && (c < e) )
      {
        *d = *c;
        d++;
        c++;
      }

      if( isalnum(*c) == 0 ) c++;
      
//      printf("c = %c\n",*c);

    }
    
    *a = '\0';
    *d = '\0';
  
    A = atoi(sZ);
/* */
//  sprintf(TString,"T1BNSystem.SetIsotope(char) Symbol = %s",Symbol); TPrintf(TString);

    if( strlen(Symbol) != 0 ) 
    {
      char *query,*fRow;
      query = new char[1000];

      if( !strcmp(Symbol,"N") )
      {
        if( Z == -1 ) Z = 7;
      }
      else
      {
        sprintf(query,"SELECT Charge FROM nucleus WHERE Symbol=\'%s\'\0",Symbol); // u vypadku N pomylka !!!
        fRow = MySQLDB->Get((char*)"nucleus",query,0);

//        sprintf(TString,"T1BNSystem.SetIsotope : MySQL DB rows: %s",fRow); TPrintf(TString);

        Z = atoi( fRow );

        delete[] fRow;
      }
//      if( fRow != NULL ) delete[] fRow;


//  printf("MySQL DB rows: %ld\n",MySQLDB->Get((char*)"nucleus",(char*)"*",0) );
//  printf("row #%5d : %s\n",0,SString);
      delete[] query;
    }
    else{} 
    
    N = A - Z;
    
/* */
//    sprintf(TString,"T1BNSystem.SetIsotope(char) Symbol = %s; A = %3d",Symbol,A); TPrintf(TString);

    if( strcasecmp(s,"2He") == 0 ) 
    {
      A = 2;
      Z = 2;
      N = 0;
      strcpy(Symbol,"He");
      strcpy(Isotope,"2He");
    }
    else if( !strcasecmp(s,"1n") ) 
    {
      A = 1;
      Z = 0;
      N = 1;
      strcpy(Symbol,"N");
      strcpy(Isotope,"1N");
    }
    else if( strcasecmp(s,"2n") == 0 ) 
    {
      A = 2;
      Z = 0;
      N = 2;
      strcpy(Symbol,"n");
      strcpy(Isotope,"2n");
    }
    else
    {
      Set(Z,N); 
//      ReadDBFile();
    }
//    sprintf(TString,"T1BNSystem.SetIsotope(char).Symbol = %s; Z = %3d N = %3d A = %3d",Symbol,Z,N,A); TPrintf(TString);

    delete[] sZ;
  }
  else{}

  if(strlen(Symbol) != 0) e = 1;

//  sprintf(TString,"T1BNSystem.SetIsotope(char) - end"); TPrintf(TString);

  delete[] s;

  return( e );
}

/*
void T1BNSystem::SetLevels(double *Levels,int NoL)
{
  int i = 0;
}
*/

void T1BNSystem::ReadDBFile()
{
//  sprintf(TString,"ReadDBFile: begin"); TPrintf(TString);

  char *FileName;
  FILE *DataFile;
  int Len;
  Len = strlen(NDBPath)+20;
  FileName = new char[Len];
    memset(FileName,0,Len);
  strcpy(FileName,NDBPath);
//  strcat(FileName,"/home/momotyuk/User/Data/Physics/NuclDB/nucleus.ndb");
  strcat(FileName,"nucleus.ndb");

//  sprintf(TString,"ReadDBFile: FileName: %s",FileName); TPrintf(TString);
/*//
  if( access(FileName,F_OK) == 0 ) // Ok!
  {
//    DataFile = fopen(FileName,"r");
  }
  else
  {
    printf("Nuclear DataBase file %s is absent!\nCode stoped.\n",FileName); // only absent?  
    exit( 0 );
  }
//  DataFile = fopen("/home/momotyuk/User/Data/Physics/NuclDB/nucleus.ndb","r");
*///

//  Z = Exist(DataFile);
  Z = Exist( NULL ); 

  if(Z == -1)
  {
    Z = -1;
    N = -1;
    A = -1;
    MinA = -1;
    MaxA = -1;
    memset(Symbol,0,6);
    memset(Isotope,0,6);
  }
  else
  {
    if( Z == 0 ){ strcpy(Symbol,"n"); }

    sprintf(Isotope,"%d%s",A,Symbol); // Is sprintf() function bad for this?
 
    SetLevels(); // read excited state information
//
    SetMass();
//
  }
//  fclose(DataFile);
  delete[] FileName;

//  sprintf(TString,"ReadDBFile: end"); TPrintf(TString);
}


int T1BNSystem::Exist(FILE *DataFile)
{
//  sprintf(TString,"Exist: begin"); TPrintf(TString);

  int S = 0;
  if( (Z != -1) && (N == -1) && (A == -1) && (strlen(Symbol) == 0) ) S = 1;
  if( (Z != -1) && (N != -1) && (A == -1) && (strlen(Symbol) == 0) ) S = 2;
  if( (Z == -1) && (N == -1) && (A != -1) && (strlen(Symbol) != 0) ) S = 3;
  if( (Z == -1) && (N == -1) && (A == -1) && (strlen(Symbol) != 0) ) S = 4;

//  FILE *OutFile;
//  OutFile = fopen("nucleus.tst","w");

  int e = -1; // No exist A Symbol
  int c;
  char *SString,*b;
  SString = new char[1061];
    memset(SString,0,1061);

  int fZ,fN,fA; // f(ile)
  char *fE;
  double fL;
  fE = new char[5];
    memset(fE,0,5);

  char *fQuery,*fRow;
  fQuery = new char[80];

  if( N == -1 )
    sprintf(fQuery,"SELECT * FROM nucleus WHERE Charge='%d'\0",Z);
  else
    sprintf(fQuery,"SELECT * FROM nucleus WHERE Charge='%d' AND Neutron='%d'\0",Z,N);

//  printf("MySQL DB rows: %ld\n",fRow = MySQLDB->Get((char*)"nucleus",fQuery,0) );
//  strcpy(SString,fRow);
  
//  if( fRow != NULL ) delete[] fRow;


//  while( ((c=ReadLine(DataFile,SString,1060)) != EOF) &&

  while( (e == -1) && 
         ( (fRow = MySQLDB->Get((char*)"nucleus",fQuery,0)) != NULL ) )
  {
    strcpy(SString,fRow);
    if( fRow != NULL ) delete[] fRow;

//    sprintf(TString,"T1BNSystem.Exist : MySQL DB row : %s",SString ); TPrintf(TString);
  
    b = RLAnalyse(SString,&fZ,&fA,fE,&fL);

    fN = fA - fZ;

    switch( S )
    {
      case 1 :
      {
        if( Z == fZ )
        {
          e = fZ;
          N = fA - fZ;
          A = fA;
          strcpy(Symbol,fE);
        }
      }
      break;
      case 2 :
      {
        if( (Z == fZ) && (N == fN) )
        {
          e = fZ;
          A = Z + N;
          strcpy(Symbol,fE);
        }
      }
      break;
      case 3 :
      {
// Win32:       if( (A == fA) && (strnicmp(Symbol,fE,2) == 0) ) 
        if( (A == fA) && (strncasecmp(Symbol,fE,2) == 0) ) 
        {
          e = fZ;
          N = fA - fZ;
        }
      }
      break;
      case 4 :
      {
// Win32        if( (A == 0) && (strnicmp(Symbol,fE,2) == 0) )
        if( (A == 0) && (strncasecmp(Symbol,fE,2) == 0) ) 
        {
          e = fZ;
          Z = fZ;
          A = fA;
          N = fA - fZ;
        }
      }
      break;
      default : {} break;
    }

//    sprintf(TString,"Isotope: fA = %3d fE = %-3s fZ = %3d",fA,fE,fZ); TPrintf(TString);

//    fprintf(OutFile,"%03d     %-2s      %03d %s\n",fZ,fE,fA,b);

    memset(SString,0,161);
    memset(fE,0,5);
  }

// for MinA,MaxA - block begin
  MinA = A;
  MaxA = A;

  int End = 0;
//  while( ((c=ReadLine(DataFile,SString,160)) != EOF) &&

  sprintf(fQuery,"SELECT * FROM nucleus WHERE Charge='%d'\0",Z);

  while( (End == 0) &&
         ( (fRow = MySQLDB->Get((char*)"nucleus",fQuery,0)) != NULL ) )
  {
    strcpy(SString,fRow);
    if( fRow != NULL ) delete[] fRow;

    b = RLAnalyse(SString,&fZ,&fA,fE,&fL);
// Win32    if( strnicmp(Symbol,fE,2) == 0 )
    if( strncasecmp(Symbol,fE,2) == 0 )
    {
      if( MinA > fA ) MinA = fA;
      if( MaxA < fA ) MaxA = fA;
    }
    else
      End = 1;

//    fprintf(OutFile,"%03d     %-2s      %03d %s\n",fZ,fE,fA,b);
//    sprintf(TString,"%03d     %-2s      %03d %s\n",fZ,fE,fA,b); TPrintf(TString);

    memset(SString,'\0',161);
    memset(fE,'\0',5);
  }
//  sprintf(TString,"Exist: MinA = %3d MaxA = %3d",MinA,MaxA); TPrintf(TString);

// for MinA,MaxA - block end

  sprintf(fQuery,"SELECT * FROM nucleus WHERE Charge=' '\0");
  fRow = MySQLDB->Get((char*)"nucleus",fQuery,0);
  if( fRow != NULL ) delete[] fRow;

  delete[] fQuery;
  delete[] fE;
  delete[] SString;

//  fclose(OutFile);

//  sprintf(TString,"Exist: end"); TPrintf(TString);

  return( e );
}


char *T1BNSystem::RLAnalyse(char *fS,int *fZ,int *fA,char *fE,double *fL)
{
  int End = 0,Len = 0,lwS = 0,i = 0;
  char *b,*wS,*bwS;
  bwS = new char[20];
    memset(bwS,0,20);
  wS = bwS;

  b = fS;
  Len = strlen(fS);

// ReadLine analyse begin
  while( End < 3 )
  {
    while( (isalnum((int)*b) == 0) && (i <= Len) )
    {
      b++;
      i++;
    }
    while( (isalnum((int)*b) != 0) && (i <= Len) && (lwS < 4) )
    {
      *wS = *b;
      b++;
      i++;
      wS++;
      lwS++;
    }

    switch(End)
    {
      case 0 : { *fZ = atoi(bwS); End = 1;} break;
      case 1 : { strcpy(fE,bwS);  End = 2;} break;
      case 2 : { *fA = atoi(bwS); End = 3;} break;
      default : {} break;
    }
    memset(bwS,0,20);
    wS = bwS;
    lwS = 0;
  }

// read float (for levels)
  while(*b == ' ') // it is bad cycle!!!
  {
    b++;
    i++;
  }

// it is bad!!! design : fL = ReadFloat(char *s); !!!
// or s = ReadFloat(char *s,float *fL) !!!
// or is better ReadCurrent() !!! class
  while( ((isdigit((int)*b) != 0) || (*b == '.')) && (i <= Len) && (lwS < 20) )
  {
    *wS = *b;
    b++;
    i++;
    wS++;
    lwS++;
  }

  *fL = atof(bwS);

// ReadLine analyse end

  delete[] bwS;
  return( b );
}


int T1BNSystem::ReadLine(FILE *DataFile,char *Str,int Len)
{
  int c,i = 1;
  char *b;
  b = Str;

  while( ((c=fgetc(DataFile)) != 10) &&
         (c != EOF) &&
         (i <= Len) )
  {
    *b = (char)c;
    b++;
    i++;
  }

  if( (i > Len) && (c != 10) && (c != EOF))
    while( ((c=fgetc(DataFile)) != 10) && (c != EOF) ){}

  return( c );
}


int T1BNSystem::SetLevels()
{
//  sprintf(TString,"SetLevels: begin");  TPrintf(TString);
// 11   BE     4    320.04   10           1/2-            115 FS 10           90

  int i = 0;

/*
  int fZ,fA;
  double fL = 0.;

  int i = 0,r = 0,End = 0;
  double es = 0.;

  char *fE,*b;
  fE = new char[5];
    memset(fE,0,5);

  char *query,*fRow;
  query = new char[1000];
*/


  SN = 0;

/*//
//  -----------------------------------------------------------------
  sprintf(query,"SELECT * FROM levels WHERE Symbol='%s' AND Mass='%d'\0",Symbol,A);

  while( (fRow = MySQLDB->Get((char*)"nucleus",query,0)) != NULL )
  {
    SN++;
//    sprintf(TString,"T1BNSystem.SetLevels : MySQL DB row : %s",fRow ); TPrintf(TString);
    delete[] fRow;
  }
  fRow = MySQLDB->Get((char*)"nucleus",(char*)"SELECT * FROM nucleus WHERE Charge=' '\0",0);
  if( fRow != NULL ) delete[] fRow;

  sprintf(TString,"SetLevels: SN = %d",SN); TPrintf(TString);
//------------------------------------------------------------------------------
//*/

  SN = NDDBase->NuclearStateNumber( Symbol,A );

  sprintf(TString,"SetLevels: SN = %d", SN ); TPrintf(TString);

  if( SN )
  {
    delete[] States;
    States = new TNState[SN];

    i = 0;
    while( i < SN )
    {
      States[i].Set( Symbol,A );
      i++;
    }
  }


/*
  r = 0;
  End = 0;

  sprintf(query,"SELECT * FROM levels WHERE Symbol='%s' AND Mass='%d'\0",Symbol,A);

  while( (fRow = MySQLDB->Get((char*)"nucleus",query,0)) != NULL )
  {
    sprintf(TString,"T1BNSystem.SetLevels fRow = %s",fRow); TPrintf(TString);


    b = RLAnalyse(fRow,&fA,&fZ,fE,&fL); // it is bad!!!

    if( (strncasecmp(Symbol,fE,2) == 0) && (A == fA) )
    {
//      States[i].ReadState(SString);
      States[i].ReadStateNew(fRow);
      i++;
      es = States[i-1].GetEnergy();
      es = States[i-1].GetHLife();
//      sprintf(TString,"SetLevels: State[%d] = %lf",i-1,es); TPrintf(TString);
//      sprintf(TString,"SetLevels: HLife[%d] = %lf",i-1,es); TPrintf(TString);
      r = 1;
    }
    else
    {
      if(r != 0) End = 1;
    }

    delete[] fRow;
    memset(fE,0,5);
  }

*/

/*
  delete[] query;
  delete[] fE;
*/
  NL = SN;

//  sprintf(TString,"SetLevels: end"); TPrintf(TString);

  return( SN );
}


void T1BNSystem::SetMass()
{
//  sprintf(TString,"T1BNSystem.SetMass: begin");  TPrintf(TString);

//  cAM = NDBase[0].NM_GetAtomicMass(Z,A) / 1000000.; // is this Ok?
//  cME = NDBase[0].NM_GetMassExcess(Z,A);
//  cBE = NDBase[0].NM_GetBindingEnergy(Z,A);

  cAM = NDDBase->AtomicMass( Z,A ) / 1000000.;
  cME = NDDBase->MassExcess( Z,A );
  cBE = NDDBase->BindingEnergy( Z,A );
  
//  sprintf(TString,"T1BNSystem.SetMass.cAM = %lf Z=%d A=%d",cAM,Z,A);  TPrintf(TString);
//  sprintf(TString,"T1BNSystem.SetMass.cAM mysql = %lf Z=%d A=%d",NDDBase->AtomicMass(Z,A)/1000000.,Z,A);  TPrintf(TString);


//  sprintf(TString,"T1BNSystem.SetMass: end");  TPrintf(TString);
}


char *T1BNSystem::RLAnalyseNew(char *fS,int *fZ,int *fA,char *fE,float *fL)
{
/*
//  11   BE     4   3410      20      (1/2,3/2,5/2+)      125 KEV 20           90
*/
  FILE *ofl;
  ofl = fopen("str-test.tmp","a");

  char *CS;
  CS = new char[80];
    memset(CS,0,80);

  strncpy(CS, fS+ 0, 6); // A
  *(CS+6) = 0;
  fprintf(ofl,"%s\n",CS);

  strncpy(CS, fS+ 6, 2); // symbol
  *(CS+2) = 0;
  fprintf(ofl,"%s\n",CS);

  strncpy(CS, fS+ 8, 6); // Z
  *(CS+6) = 0;
  fprintf(ofl,"%s\n",CS);

  strncpy(CS, fS+14,11); // LEnergy
  *(CS+11) = 0;
  fprintf(ofl,"%s\n",CS);

  strncpy(CS, fS+25, 4); // LdE
  *(CS+4) = 0;
  fprintf(ofl,"%s\n",CS);

  strncpy(CS, fS+30,23); // Jp
  *(CS+23) = 0;
  fprintf(ofl,"%s\n",CS);

  strncpy(CS, fS+53,22); // Half-Life
  *(CS+22) = 0;
  fprintf(ofl,"%s\n",CS);

  strncpy(CS, fS+75, 3); // Year
  *(CS+3) = 0;
  fprintf(ofl,"%s\n",CS);

  fclose(ofl);

  delete[] CS;

  return( fS ); // ???
}

/* - new

  int End = 0,Len = 0,lwS = 0,i = 0;
  char *b,*wS,*bwS;
  bwS = new char[20];
    memset(bwS,0,20);
  wS = bwS;

  b = fS;
  Len = strlen(fS);

// ReadLine analyse begin
  while( End < 3 )
  {
    while( (isalnum((int)*b) == 0) && (i <= Len) )
    {
      b++;
      i++;
    }
    while( (isalnum((int)*b) != 0) && (i <= Len) && (lwS < 4) )
    {
      *wS = *b;
      b++;
      i++;
      wS++;
      lwS++;
    }

    switch(End)
    {
      case 0 : { *fZ = atoi(bwS); End = 1;} break;
      case 1 : { strcpy(fE,bwS);  End = 2;} break;
      case 2 : { *fA = atoi(bwS); End = 3;} break;
      default : {} break;
    }
    memset(bwS,0,20);
    wS = bwS;
    lwS = 0;
  }

// read float (for levels)
  while(*b == ' ') // it is bad cycle!!!
  {
    b++;
    i++;
  }

// it is bad!!! design : fL = ReadFloat(char *s); !!!
// or s = ReadFloat(char *s,float *fL) !!!
// or is better ReadCurrent() !!! class
  while( ((isdigit((int)*b) != 0) || (*b == '.')) && (i <= Len) && (lwS < 20) )
  {
    *wS = *b;
    b++;
    i++;
    wS++;
    lwS++;
  }

  *fL = atof(bwS);

// ReadLine analyse end

  delete[] bwS;
  return( b );

*/

//}

// -----------------------------------------------------------------------------
/*
void T1BNSystem::GetLevels(float *l)
{
  int i = 0;
  for(i=0;i<NL;i++) l[i] = Level[i]/1000.; // keV - > MeV - it is bad!!!

//  l[i] = 0.; - initialise?
}
*/

float T1BNSystem::GetSpin(double e) // for test only!!!
{
//  sprintf(TString,"T1BNSystem.GetSpin - begin"); TPrintf(TString);
//  sprintf(TString,"T1BNSystem.GetSpin: e = %lf",e); TPrintf(TString);

  int i = 0;
  float r = 0.;

//  sprintf(TString,"T1BNSystem.GetSpin SN = %d",SN); TPrintf(TString);

  while( ( i < SN ) && 
         ( (States[i].GetEnergy() - e) < 1. ) )
  {
    i++;

//  sprintf(TString,"T1BNSystem.GetSpin GetEnergy = %lf",States[i-1].GetEnergy()); TPrintf(TString);
  }

//  sprintf(TString,"T1BNSystem.GetSpin i = %d",i); TPrintf(TString);

  if( i < SN )
  {
    r = States[i].GetSpin();
  }
  else
  {
    r = 0.0; // ??? - nepravylno !!!
  }

//  sprintf(TString,"T1BNSystem.GetSpin - end"); TPrintf(TString);

  return( r ); // is this correct? 
}


int T1BNSystem::GetParity(double e) // for test only!!!
{
  int i = 0;
  int r = 0;

  while( (i < SN) && (States[i].GetEnergy() != e) ) i++;

  if( i < SN ) r = States[i].GetParity();

  return( r ); // is this correct? 
}


void T1BNSystem::WriteDB(FILE *ofl) // for test only!!!
{
  int i = 0;

  fprintf(ofl,
    "Isotope              = %5s\n"
    "Chemical Symbol      = %5s\n"
    "Atomic Mass Number   = %5d\n"
    "Proton Number        = %5d\n"
    "Neutron Number       = %5d\n"
    "Mass                 = %14.3lf\n"
    "Total Binding Energy = %14.3lf\n"
    "Number of States     = %5d\n",
    Isotope,Symbol,A,Z,N,cAM,cBE,SN);

  while( i < SN ){
    States[i].WriteDB(ofl);
    i++; }
}


void T1BNSystem::WriteDB(FILE *ofl,char *s) 
{
  if( strlen(s) != 0 )
    fprintf(ofl,
"--------------------------------------------------------------------------------\n"
"%s : \n",s);
//"                     = %s\n",s);

  WriteDB(ofl);
}
//------------------------------------------------------------------------------

