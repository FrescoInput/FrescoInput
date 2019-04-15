//------------------------------------------------------------------------------
// TNReaction.cpp
//------------------------------------------------------------------------------
// T(ype)N(uclear)R(eaction)
// C.: 2002.12.18.
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
#include <unistd.h>
#include <ctype.h>
//#include <process.h>
//#include <dir.h>
//#include <io.h>

#include "TNReaction.h"

// global data -----------------------------------------------------------------
extern TTFDBase *NDBase;
extern int GSN;
extern T2BNSystem *GEChannel;
extern T2BNSystem *GOChannel;
extern int gNoS;
extern T2BNSystem *gTChannel;
// this is bad!!!! :
extern int gNoRCI;
extern int *gRCIndex;
extern char *GFileName;
// global data -----------------------------------------------------------------


TNReaction::TNReaction()
{
  Projectile = new T1BNSystem;
  Target = new T1BNSystem;

  RString = new char[25];
  ICString = new char[15];
  BasePath = new char[2];
  Initialize();
//
  NDBase = new TTFDBase[1];
  NDBase[0].Set();
  NDBase[0].Read(); // chym .Set() vidriznjajetsja vid .Read() ?!!
//  NDBase[0].Write(); // test only!

// this is really bad, because GEChannel,GOChannel & gTChannel are global!!!
  GEChannel = new T2BNSystem[1];
  GSN = 1;
  GOChannel = new T2BNSystem[GSN];
  cPI = new int[GSN]; // this is not correct!!!
  gNoS = 0;
  gTChannel = new T2BNSystem[gNoS+1];
  cSI = new int[gNoS+1];
  
  cLogName = new char[ strlen(GFileName) + 20 ];
  strcpy( cLogName,GFileName );
  strcpy( strchrnul(cLogName,'.'),".problem\0" );
  cLogFile = fopen( cLogName,"a" );
  
}


TNReaction::~TNReaction()
{
  delete Projectile;
  delete Target;
  delete[] RString;
  delete[] ICString;
  delete[] BasePath;
//
  delete[] NDBase;

// this is really bad, because GEChannel and GOChannel are global!!!
  delete[] GEChannel;
  delete[] GOChannel;
  delete[] gTChannel;
  delete[] cPI;
  delete[] cSI;
  
  fclose( cLogFile );
  delete[] cLogName;
}

void TNReaction::Set(TRChannel *RChannel,int TR) // - ERROR !!!
{

  NC = SetInChannel(RChannel);

  if( TR == 1 ) Generate( RChannel,NC );  //- ERROR!!!

// design code
  
  SetOutChannel(RChannel,gNoRCI);

  SetTRChannel(RChannel,gNoRCI);

//  return( gNoRCI );
}

void TNReaction::Set(char *fPath) // Is it really need?
{

}


void TNReaction::Initialize() // Is it really need?
{
  TZ = 0;
  TN = 0;
  NC = 0;
  Energy = 0.;
  memset(RString,0,25);
  memset(ICString,0,15);
}


void TNReaction::SetBasePath(char *s) // ?!! or set DB path?
{
  delete[] BasePath;
  int Len;
  Len = strlen(s)+2;
  BasePath = new char[Len];
    memset(BasePath,0,Len);
  strcpy(BasePath,s);
}


int TNReaction::SetInChannel(char *p,char *t,double fEnergy)
{
//  sprintf(TString,"SetInChannel: begin"); TPrintf(TString);

  int pe = 0,pt = 0;
//  sprintf(TString,"SetInChannel: pe = %3d pt = %3d",pe,pt); TPrintf(TString);

//  Projectile->SetBasePath(BasePath);
//  Target->SetBasePath(BasePath);

  pe = Projectile->SetIsotope(p);
  pt = Target->SetIsotope(t);

//  sprintf(TString,"SetInChannel: pe = %3d pt = %3d",pe,pt); TPrintf(TString);

  Energy = fEnergy;

  if( (pe != 0) && (pt != 0) )
  {
    sprintf(ICString,"%s(%s",t,p);

    TZ = Projectile->GetZ() + Target->GetZ();
    TN = Projectile->GetN() + Target->GetN();

//    sprintf(TString,"SetInChannel: TZ = %3d TN = %3d",TZ,TN); TPrintf(TString);

    NC = SetOutReaction();

//    sprintf(TString,"SetInChannel:Number of Channel : %3d",NC); TPrintf(TString);
  }
  else{}

//  sprintf(TString,"SetInChannel: end"); TPrintf(TString);

  return(NC);
}


// SetEntranceChannel
int TNReaction::SetInChannel(TRChannel *IChannel)
{
  sprintf(TString,"TNReaction.SetInChannel - begin"); TPrintf(TString);
//  double lE = 0.;

// major thing : 
  GEChannel[0].Set( IChannel,0 );
//
  sprintf(TString,"TNReaction.SetInChannel : GEChannel.Set - Ok"); TPrintf(TString);
//
  int liv;
  double e;
  char *p,*t;

  p = new char[6];
    memset(p,0,6);
  t = new char[6];
    memset(t,0,6);

  IChannel->GetProjectile(p);
  IChannel->GetTarget(t);
  e = (double)IChannel[0].GetEnergy();

  sprintf(TString,"TNReaction.SetInChannel : e = %lf",e); TPrintf(TString);

  GEChannel[0].Set((char*)"Energy",e);
//  GEChannel[0].Get("Energy",lE); is better ???
//  lE = GEChannel[0].Get("Energy",(double)0.); // ???

//
  if( (liv = GEChannel[0].CheckNP()) == 0 ){ printf("\nNo nuclear potential for entrance channel!\n"); exit(0); } // better GetNumberOfNP & if == 0?...

  IChannel[0].CheckLE(e);
//
  int n = 0;
  n = SetInChannel(p,t,e);

  sprintf(TString,"TNReaction.SetInChannel : NC = %3d",n); TPrintf(TString);

  delete[] p;
  delete[] t;

  sprintf(TString,"TNReaction.SetInChannel - end"); TPrintf(TString);

  return(n);
}


//------------------------------------------------------------------------------
void TNReaction::Generate(TRChannel *RChannel,int NC) // - ERROR!!!
{
  sprintf(TString,"TNReaction.Generate - begin"); TPrintf(TString);

  int TR = 0;

  int i; 
//  int NC = 0;
  char *RS;
  char *s,*d,*rcs;

  sprintf(TString,"ReacTest : NC = %3d",NC); TPrintf(TString);

  RS = new char[NC*12+1];
    memset(RS,0,NC*12+1); // long var = NC*12+1 - better?

//  Reaction[0].GetOutChannel(RS);
  GetOutChannel(RS);
  sprintf(TString,"Generate:RString = %s",RS); TPrintf(TString);

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

  sprintf(rcs,"%s)%s;\0",p,t); // very bad!

  delete[] p;
  delete[] t;


  TR = NC + 1; // for inelastic channel... ???
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

  i = 1;
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

//  sprintf(TString,"RChannel[%d] = ",i); TPrintf(TString);
    
    RChannel[i].Set(rcs);
    gRCIndex[i] = i;

    i++;
  }

  delete[] rcs;
  delete[] RS;

  sprintf(TString,"TNReaction.Generate - end"); TPrintf(TString);
}
//------------------------------------------------------------------------------





// SetOutgoingChannel
int TNReaction::SetOutChannel(TRChannel *IChannel,TRChannel *RChannel,int fCN) // f(unction)ChannelNumber
{
  sprintf(TString,"TNReaction.SetOutChannel - begin"); TPrintf(TString);

  int i = 0,r = 0,lI,liv;
  double ldv,lECBE,lOCBE,lQR,lMEE;
//
  char *p,*t,*test;
  p = new char[6];
    memset(p,0,6);
  t = new char[6];
    memset(t,0,6);

  test = new char[200];

//  FILE *lOF;
//  lOF = fopen("problem.log","a");
//
  GSN = fCN;
//
  GOChannel[0].Set( IChannel,0 );
  lECBE = GOChannel[0].Get((char*)"TotalBindingEnergy",ldv);
//  lECBE = GOChannel[0].Get("TotalBindingEnergy",(double)0.); // ???

//
  delete[] GOChannel;
  GOChannel = new T2BNSystem[GSN];
//
//  GOChannel[0].Set( IChannel,0 );

  GOChannel[0].Set( &RChannel[0],0); // 0 - for test only!!!
//  GOChannel[0].Set( RChannel, 0 ,0 ); // 0 - for test only!!!
  GOChannel[0].Set((char*)"Energy",Energy); // Is this correct?
  gRCIndex[0] = -1;

//  Write( RChannel,fCN,(char*)"GOChannel.Set-2 done" );

  i = 1;
  while( i < GSN )
  {
//  sprintf(TString,"TNReaction.SetOutChannel.i = %d",i); TPrintf(TString);

    GOChannel[i].Set( &RChannel[i],0 ); // 0 - for test only!!!
//    GOChannel[i].Set( RChannel,0,i );
    GOChannel[i].Set((char*)"Energy",Energy); // Is this correct?

/*/
  sprintf(test,"GOChannel.Set i=%d done",i);
  Write( RChannel,fCN,test );
/*/

    lOCBE = GOChannel[i].Get((char*)"TotalBindingEnergy",ldv);
    lQR = lECBE - lOCBE; // is this correct???
    GOChannel[i].Set((char*)"Q-Reaction",lQR);
//
    lMEE = Energy + lQR;

    if( lMEE < 0. ){ gRCIndex[i] = -1; }
/*/    else
    {
      GOChannel[i].
    }
/*/
    if( ((liv = GOChannel[i].CheckNP()) == 0) && (i != 0) )
    {
      gRCIndex[i] = -1;  // better GetNumberOfNP & if == 0?... & this is bad

      GOChannel[i].GetPIsotope(p);
      GOChannel[i].GetTIsotope(t);
      fprintf(cLogFile,"Channel : %5s + %5s - no nuclear potential\n",p,t);
    }
/*/
     sprintf(TString,"TNReaction.SetOutChannel.liv = %d",liv); TPrintf(TString);

//    if( GOChannel[i].CheckSF() == 0 ) gRCIndex[i] = -1;
/*/
    lI = gRCIndex[i];

    if( lI >= 0 )
    {
      RChannel[i].CheckLE(lMEE);
    }
//
    i++;
  }

//
  CheckRCI();

//  CheckCBE();

  SortBE();

//  CheckCBE();
//

/*/ for test :
  FILE *ofl;
  ofl = fopen("NuclDB.tst","w");

  i = 0;
  while( i < GSN )
  {
    GOChannel[i].WriteDB(ofl);
    i++;
  }

  fclose(ofl);
// for test
/*/

//  fclose(lOF);

  delete[] p;
  delete[] t;
  
  delete[] test;

  sprintf(TString,"TNReaction.SetOutChannel.gNoRCI = %d",gNoRCI); TPrintf(TString);

  sprintf(TString,"TNReaction.SetOutChannel - end"); TPrintf(TString);

  return( r ); // ?!!
}


void TNReaction::CheckRCI()
{
// to make as function? : ------------------------------------------------------
  int i,j;
  int *lRCIndex;

  lRCIndex = new int[gNoRCI];

  i = 0;
  j = 0;
  while( i < gNoRCI )
  {
    if( gRCIndex[i] >= 0 )
    {
      lRCIndex[j] = gRCIndex[i];
      j++;
    }
    i++;
  }

  gNoRCI = j;
  delete[] gRCIndex;
  gRCIndex = new int[gNoRCI];

  j = 0;
  while( j < gNoRCI )
  {
    gRCIndex[j] = lRCIndex[j];
    j++;
  }

  delete[] lRCIndex;
//------------------------------------------------------------------------------
}


void TNReaction::CheckCBE()
{
//  sprintf(TString,"TNReaction.CheckCBE - begin"); TPrintf(TString);

  int i,lI;
  double ldv,lICBE,lOCBE;
  char *lP,*lT;

  lP = new char[6];
    memset(lP,0,6);
  lT = new char[6];
    memset(lT,0,6);

  i = 0;
  
  while( i < gNoRCI )
  {
    lI = gRCIndex[i];

    GOChannel[lI].GetPIsotope( lP );
    GOChannel[lI].GetTIsotope( lT );
    lOCBE = GOChannel[lI].Get((char*)"TotalBindingEnergy",ldv);

    if( !i )
    {
      lICBE = lOCBE;
//      sprintf(TString,"TNReaction.CheckCBE : P : %5s T : %5s BE = %lf",lP,lT,lICBE ); TPrintf(TString);
    }

//    sprintf(TString,"TNReaction.CheckCBE : P : %5s T : %5s BE = %lf QR = %lf",lP,lT,lOCBE,lICBE-lOCBE); TPrintf(TString);
    
    i++;
  }

  delete[] lP;
  delete[] lT;

//  sprintf(TString,"TNReaction.CheckCBE - end"); TPrintf(TString);
}


void TNReaction::SortBE()
{
//  sprintf(TString,"TNReaction.CheckSBE - begin"); TPrintf(TString);

  int i,j,lI;
  double ldv,*lCBE,lTBE;

  lCBE = new double[ gNoRCI ];

  i = 1;
  while( i < gNoRCI )
  {
    lI = gRCIndex[i];
    lCBE[i] = GOChannel[lI].Get((char*)"TotalBindingEnergy",ldv);
    i++;
  }

  for(i = 1; i < (gNoRCI-1); i++)
  {
    for(j = (i+1); j < gNoRCI; j++)
    {
      if( lCBE[i] < lCBE[j] )
      {
        lTBE = lCBE[i];
        lCBE[i] = lCBE[j];
        lCBE[j] = lTBE;
        
        lI = gRCIndex[i];
        gRCIndex[i] = gRCIndex[j];
        gRCIndex[j] = lI;
      }
    }
  }

  delete[] lCBE;

//  sprintf(TString,"TNReaction.CheckSBE - end"); TPrintf(TString);
}


// SetTransferChannel
int TNReaction::SetTRChannel(TRChannel *IChannel,TRChannel *RChannel,int fCN) // f(unction)ChannelNumber
{
  sprintf(TString,"TNReaction.SetTRChannel - begin"); TPrintf(TString);

  int i,j,lNoS,lrv = 0,lSI,lSIo,lI,lSIp,lSIt,lPI,lPIo;
  char *lep,*let,*lop,*lot;

  lep = new char[6];
    memset(lep,0,6);
  let = new char[6];
    memset(let,0,6);
  lop = new char[6];
    memset(lop,0,6);
  lot = new char[6];
    memset(lot,0,6);

// bad code:
  lNoS = fCN * 2; // Number of Core-Cluster Systems, what if fCN == 0 ???

  gNoS = lNoS;

  delete[] gTChannel;
  gTChannel = new T2BNSystem[gNoS];
//
  delete[] cSI;
  cSI = new int[gNoS];
//
    IChannel[0].GetProjectile(lep);
    IChannel[0].GetTarget(let);
//
  i = 1; // ???!!!
  j = 0;
  lSI = 0;
  lPI = gNoRCI + 1; // ???
  while( i < fCN )
  {
//
    lI = gRCIndex[i];

    if( lI >= 0 )
    {
      RChannel[lI].GetProjectile(lop);
      RChannel[lI].GetTarget(lot);
//
//  sprintf(TString,"\n\nTNReaction.SetTRChannel : %s(%s,%s)%s",let,lep,lop,lot); TPrintf(TString);

      lSIo = lSI;
      lPIo = lPI;

      if( (lrv = OneStepTransfer(IChannel,&RChannel[lI])) != 0 )
      {
//        if( ( lSIp = gTChannel[j*2].Set(IChannel,1,&RChannel[lI],lI+1,"Projectile",lSI,lPI) ) != 0 )
        if( ( lSIp = gTChannel[j*2].Set(IChannel,1,&RChannel[lI],j+2,(char*)"Projectile",lSI,lPI) ) != 0 )
        {
          lSI = lSI + lSIp;
          lPI++;

//  sprintf(TString,"TNReaction.SetTRChannel.lSI_pr = %d",lSI); TPrintf(TString);

//          if( (lSIt = gTChannel[j*2+1].Set(IChannel,1,&RChannel[lI],lI+1,"Target",lSI,lPI) ) != 0 )
          if( (lSIt = gTChannel[j*2+1].Set(IChannel,1,&RChannel[lI],j+2,(char*)"Target",lSI,lPI) ) != 0 )
          {
            lSI = lSI + lSIt;
            lPI++;
            j++;
//  sprintf(TString,"TNReaction.SetTRChannel.lSI_tr = %d",lSI); TPrintf(TString);
          }
          else{ gRCIndex[i] = -1; lSI = lSIo; lPI = lPIo; }
        }
        else{ gRCIndex[i] = -1; lSI = lSIo; lPI = lPIo; }
      }
      else // for first (0.1) version only!!!
      {
        gRCIndex[i] = -1;
      }
//
//  sprintf(TString,"TNReaction.SetTRChannel.OneStepTransfer : %d",lrv); TPrintf(TString);
//
    }
    i++;
  }

  gNoS = j * 2;

//
  CheckRCI();
//

/*// for test :
  FILE *ofl;
  ofl = fopen("CC-NuclDB.out","w");

  i = 0;
  while( i < fCN )
  {
    gTChannel[i*2  ].WriteDB_CC(ofl);
    gTChannel[i*2+1].WriteDB_CC(ofl);
    i++;
  }

  fclose(ofl);
*/// for test

  delete[] lep;
  delete[] let;
  delete[] lop;
  delete[] lot;

  sprintf(TString,"TNReaction.SetTRChannel - end"); TPrintf(TString);

  return( i ); // ?!!
}


int TNReaction::OneStepTransfer(char *fep,char *fop)
{
  int lrv = 0,leA,loA;
  T1BNSystem *lEN,*lON;

  lEN = new T1BNSystem[1];
  lON = new T1BNSystem[1];

  lEN[0].Set(fep);
  lON[0].Set(fop);

  leA = lEN[0].GetA();
  loA = lON[0].GetA();

  if( leA == loA )
  {
    lrv = 0;
  }
  else
  {
    if( leA > loA )
    {
      if( ( lEN[0].GetZ() - lON[0].GetZ() < 0 ) || ( lEN[0].GetN() - lON[0].GetN() < 0 ) ){ lrv = 0;}
      else{ lrv = 1; }
    }
    else
    {
      if( ( lON[0].GetZ() - lEN[0].GetZ() < 0 ) || ( lON[0].GetN() - lEN[0].GetN() < 0 ) ){ lrv = 0;}
      else{ lrv = 1; }
    }
  }

  delete[] lEN;
  delete[] lON;

  return( lrv );
}


int TNReaction::OneStepTransfer(TRChannel *EChannel,TRChannel *OChannel)
{
  int lrv = 0;
  char *lei,*loi;

  lei = new char[6];
    memset(lei,0,6);
  loi = new char[6];
    memset(loi,0,6);

  EChannel->GetProjectile( lei );
  OChannel->GetProjectile( loi );

  lrv = OneStepTransfer(lei,loi);

  delete[] lei;
  delete[] loi;

  return( lrv );
}

//int TNReaction::GetNumberChannel()
//{
//}


// int TNReaction::GetNumberChannel(char *n);


//void TNReaction::GetOutReaction(char *s)
//{
//}


// void TNReaction::GetOutReaction(char *s,char *n);


int TNReaction::SetOutReaction()
{
//  sprintf(TString,"SetOutReaction: begin"); TPrintf(TString);

//  sprintf(TString,"SetOutReaction: TZ = %3d TN = %3d",TZ,TN); TPrintf(TString);
  char *lep;
  lep = new char[6];
  Projectile->GetIsotope(lep);
 

  int *OutReaction;
  OutReaction = new int[4*TZ*TN+2];
//  if(OutReaction == 0) return 0; exit(1);
//  sprintf(TString,"SetOutReaction: OuT2BNSystem = %u",OutReaction); TPrintf(TString);
  char *WString;
  WString = new char[12*TZ*TN+2];
  memset(WString,0,12*TZ*TN+2);

  T1BNSystem *Nucl3,*Nucl4;
  Nucl3 = new T1BNSystem;
  Nucl4 = new T1BNSystem;

//  Nucl3->SetBasePath(BasePath);
//  Nucl4->SetBasePath(BasePath);

  int Z3,N3,A3,Z4,N4,A4;

  int i = 0,p,n,sn,en,MinN3,MaxN3,MinN4,MaxN4,v;
  char *sE3,*sE4,*RSp,*RSw; // RS(tring)p(osition)
  sE3 = new char[5];
  sE4 = new char[5];
  RSw = new char[13];
    memset(sE3,0,5);
    memset(sE4,0,5);
    memset(RSw,0,13);
  RSp = WString;
  for(p=0;p<=TZ;p++)
  {
//    sprintf(TString,"SetOutReaction p : %3d",p); TPrintf(TString);

    v = Nucl3->SetIsotope(p);
    MinN3 = Nucl3->GetMinA() - p;
    MaxN3 = Nucl3->GetMaxA() - p;
//    sprintf(TString,"SetOutReaction MinN3 = %3d MaxN3 = %3d TZ = %3d",MinN3,MaxN3,TZ); TPrintf(TString);

    v = Nucl4->SetIsotope(TZ-p);
    MinN4 = Nucl4->GetMinA() - (TZ-p);
    MaxN4 = Nucl4->GetMaxA() - (TZ-p);
//    sprintf(TString,"SetOutReaction MinN4 = %3d MaxN4 = %3d",MinN4,MaxN4); TPrintf(TString);

    Nucl3->GetElement(sE3);
    Nucl4->GetElement(sE4);

    if(MaxN3 > TN-MinN4) MaxN3 = TN-MinN4;
    if(MaxN4 > TN-MinN3) MaxN4 = TN-MinN3;

    sn = MinN3;
    if( (TN - MinN3) > MaxN4 ) sn = TN - MaxN4;
    en = MaxN3;
    if( (TN - MaxN3) < MinN4 ) sn = TN - MinN4;

//    sprintf(TString,"SetOutReaction sn : %3d en : %3d",sn,en); TPrintf(TString);

    for(n=sn;n<=en;n++)
    {
      Z3 = p;
      N3 = n;
      A3 = Z3 + N3;
      Z4 = TZ - p;
      N4 = TN - n;
      A4 = Z4 + N4;
      OutReaction[4*i]   = Z3;
      OutReaction[4*i+1] = N3;
      OutReaction[4*i+2] = Z4;
      OutReaction[4*i+3] = N4;

// new code ! only one transfer reactions!
      sprintf(RSw,"%d%s\0",A3,sE3);

      if( OneStepTransfer(lep,RSw) != 0 )
      {
        i++;
        sprintf(RSw,"%d%s)%d%s;",A3,sE3,A4,sE4);
        RSp = stpcpy(RSp,RSw); // it is bad!
//      if(RSp-WString - strlen(RSw) < 12*TZ*TN) RSp = stpcpy(RSp,RSw);
      }
//
    }
    memset(sE3,0,5);
    memset(sE4,0,5);
    memset(RSw,0,13);
  }

  int ChN;
  ChN = i;

  delete[] RString;
  RString = new char[strlen(WString)+5];
  memset(RString,0,strlen(WString)+5);
  strcpy(RString,WString);
  delete[] WString;

//  sprintf(TString,"OutReaction     :  Z3+ N3) Z4+ N4"); TPrintf(TString);
  char *c,*cRSw;
  c = RString;
  cRSw = RSw;

  for(i=0;i<ChN;i++)
  {
    while( *c != ';' )
    {
      *cRSw = *c;
      c++;
      cRSw++;
    }
    c++;

/* */
 // temp
    sprintf(TString,"OutReaction %3d : %3d+%3d)%3d+%3d : %s",
                    i,
                    OutReaction[4*i],
                    OutReaction[4*i+1],
                    OutReaction[4*i+2],
                    OutReaction[4*i+3],
                    RSw);
    TPrintf(TString);
//
/* */   

    memset(RSw,0,13);
    cRSw = RSw;
  }

  delete[] sE3;
  delete[] sE4;
  delete[] RSw;
  delete Nucl3;
  delete Nucl4;
  delete[] OutReaction;

  delete[] lep;

//  sprintf(TString,"SetOutReaction: end"); TPrintf(TString);

  return(ChN);
}

int TNReaction::SetOutReaction(char *s)
{
//  sprintf(TString,"SetOutReaction(char): begin"); TPrintf(TString);

  NC = SetOutReaction(); // if SetInChannel is Ok!
  T1BNSystem *Nucl3;
  Nucl3 = new T1BNSystem;
//  Nucl3->SetBasePath(BasePath);
  int v = 0;
  v = Nucl3->SetIsotope(s);
  char *c;
  int i = 0;

//  sprintf(TString,"SetOutReaction(char): v = %d",v); TPrintf(TString);

  if(v != 0)
  {
    char *WString,*WSp,*sE,*WSw,*cWSw,*Sw,*bSw;
    WString = new char[strlen(RString+5)]; // it is bad!
      memset(WString,0,strlen(RString+5));
    WSp = WString;
    sE = new char[5];
      memset(sE,0,5);
    WSw = new char[13];
      memset(WSw,0,13);
    Sw = new char[5];
      memset(Sw,0,5);
    bSw = Sw;

    Nucl3->GetElement(sE);

    c = RString;
    cWSw = WSw;

// Âèçíà÷åííÿ ê³ëüêîñò³ âèõ³äíèõ êàíàë³â
    int ChN = 0;
    while( *c != 0 )
    {
      if(*c == ';') i++;
      c++;
    }
    ChN = i; // ê-òü êàíàë³â
    c = RString;

    for(i=0;i<ChN;i++)
    {
      while(*c != ';')
      {
        *cWSw = *c;
        c++;
        cWSw++;
      }
      *cWSw = *c;
      c++;
      cWSw = WSw;
      while(isalpha(*cWSw) == 0) cWSw++; // ïðîïóñêàºìî öèôðè
      while(isalpha(*cWSw) != 0)
      {
        *bSw = *cWSw;
        cWSw++;
        bSw++;
      }
      bSw = Sw;
      cWSw = WSw;

// Win32:      if(strnicmp(sE,Sw,2) == 0)
      if(strncasecmp(sE,Sw,2) == 0)
      {
//        sprintf(TString,"SetOutReaction : WSw : %s",WSw); TPrintf(TString);
        WSp = stpcpy(WSp,WSw); // it is bad!
      }
      memset(WSw,0,13);
      memset(Sw,0,5);
    }
    delete[] sE;
    delete[] WSw;
    delete[] Sw;

    delete[] RString;
    RString = new char[strlen(WString)+5];
    memset(RString,0,strlen(WString)+5);
    strcpy(RString,WString);
    delete[] WString;
  }
  else{}

  c = RString;
  i = 0;
  while( *c != 0 )
  {
    if(*c == ';') i++;
    c++;
  }

  NC = i;

  delete Nucl3;

//  sprintf(TString,"SetOutReaction(char): end"); TPrintf(TString);

  return(NC);
}

/*
void TNReaction::Kinematics(char *Path,float *Range)
{
  sprintf(TString,"Kinematics: begin");
  TPrintf(TString);

  T1BNSystem *Nucl3,*Nucl4;
  Nucl3 = new T1BNSystem;
  Nucl4 = new T1BNSystem;
  Nucl3->SetBasePath(BasePath);
  Nucl4->SetBasePath(BasePath);
  char *CPath;
  CPath = new char[strlen(Path)+10]; // it is bad?
  memset(CPath,0,strlen(Path)+10);
  strcpy(CPath,Path);

  char *FileName,*FileNameE;
  FileName = new char[15];
    memset(FileName,0,15);
  FileNameE = new char[15];
    memset(FileNameE,0,15);
  FILE *OutFile;

  char *sE3,*sE4,*CReaction;
  sE3 = new char[5];
    memset(sE3,0,5);
  sE4 = new char[5];
    memset(sE4,0,5);
  CReaction = new char[25];
    memset(CReaction,0,25);

  char *E3,*E4;
  E3 = new char[5];
    memset(E3,0,5);
  E4 = new char[5];
    memset(E4,0,5);

  char *c,*b,*p;
  c = RString;
  int A1,A3,A4;

  int i = 0,nL3 = 0,nL4 = 0,oL3 = 1,oL4 = 1,v;
  float *L3,*L4;
  L3 = new float[oL3+2];
    LevelInitialise(L3,oL3+2);
  L4 = new float[oL4+2];
    LevelInitialise(L4,oL4+2);

//  sprintf(TString,"Kinematics: NC = %d",NC);
//  TPrintf(TString);

  A1 = Target->GetZ() + Target->GetN();

  for(i=0;i<NC;i++) // this cycle is bad!!! go subroutine!!!
  {
    memset(sE3,0,5);
    b = sE3;
    while(*c != ')')
    {
      *b = *c;
      c++;
      b++;
    }
    c++;

    memset(sE4,0,5);
    b = sE4;
    while(*c != ';')
    {
      *b = *c;
      c++;
      b++;
    }
    c++;

    memset(CReaction,0,25);
    sprintf(CReaction,"%s,%s)%s",ICString,sE3,sE4);

    sprintf(TString,"Kinematics: CReaction : %s",CReaction);
    TPrintf(TString);

    v = Nucl3->SetIsotope(sE3);
    nL3 = Nucl3->SetLevels(); // n(umber of)L(evel)
    if(oL3 < nL3)
    {
      delete[] L3;
      oL3 = nL3;
      L3 = new float[oL3+2];
    }
    LevelInitialise(L3,oL3+2);
    Nucl3->GetLevels(L3);
    memset(E3,0,5);
    Nucl3->GetElement(E3);
    A3 = Nucl3->GetZ() + Nucl3->GetN();

    v = Nucl4->SetIsotope(sE4);
    nL4 = Nucl4->SetLevels();
    if(oL4 < nL4)
    {
      delete[] L4;
      oL4 = nL4;
      L4 = new float[oL4+2];
    }
    LevelInitialise(L4,oL4+2);
    Nucl4->GetLevels(L4);
    memset(E4,0,5);
    Nucl4->GetElement(E4);
    A4 = Nucl4->GetZ() + Nucl4->GetN();

    p = CPath;
    p+=strlen(Path);
    memset(p,0,3);
    memset(p,'\\',1);
    strcat(CPath,E3);
    if( (v = chdir(CPath)) == -1 )
    {
      v = mkdir(CPath);
      v = chdir(CPath);
    }
    sprintf(TString,"Kinematics: spawnlp(): begin");
    TPrintf(TString);

    memset(FileName,0,13);
    sprintf(FileName,"%s%d)%s%d",E3,A3,E4,A4);
    memset(FileNameE,0,13);
    sprintf(FileNameE,"%s%d)%s%d.in",E3,A3,E4,A4);
    OutFile = fopen(FileNameE,"w");

    KinematicsInFile(OutFile,CReaction,Range,nL3,L3,nL4,L4);

    fclose(OutFile);

    spawnlp(P_WAIT,"km.bat","km.bat",FileName,NULL);

    memset(FileNameE,0,13);
    sprintf(FileNameE,"%s%d)%s%d.out",E3,A3,E4,A4);
    ToLevels(FileNameE,A1,A3);

    v = chdir(Path);
    sprintf(TString,"Kinematics: Path: %s",Path);
    TPrintf(TString);

    sprintf(TString,"Kinematics: spawnlp(): end");
    TPrintf(TString);
  }

  delete[] E3;
  delete[] E4;

  delete[] L3;
  delete[] L4;

  delete[] sE3;
  delete[] sE4;

  delete[] CReaction;

  delete[] FileName;
  delete[] FileNameE;
  delete Nucl3;
  delete Nucl4;

  sprintf(TString,"Kinematics: end");
  TPrintf(TString);
}
*/

void TNReaction::KinematicsInFile(FILE *oFile,char *RS,float *R,int n3,float *L3,int n4,float *L4)
{
//  sprintf(TString,"KinematicsInFile: begin"); TPrintf(TString);

//7LI(11B,10B)8LI                40.00      0.5 45.5  0.5    8    0
//  0.0       0.7184    1.7402    2.1543    3.5871    0.9808    2.255     3.210     5.400     6.1       6.530     7.1
//END

/*  strupr(RS); */

//    1 FORMAT(30A1,F10.0,3F5.0,2I5)
  fprintf(oFile,"%-30s",RS);
// for test only!!!
  int tn;
  tn = n3 + n4;
  if( tn > 45 ) // it is bad!
  {
    if( n3 > 45 )
    {
      n3 = 45;
      n4 = 0;
    }
    else
    {
      n4 = n4 - (tn - 45);
    }
    tn = n3 + n4;
  }
  fprintf(oFile,"%10.3f%5.1f%5.1f%5.1f%5d%5d\n",Energy,R[0],R[1],R[2],tn,0);
//    2 FORMAT(8F10.0)
  int d = 0,i = 0;
  for(i=0;i<n3;i++)
  {
//    sprintf(TString,"KinematicsInFile: L3[%d] = %10.6f",i,L3[i]); TPrintf(TString);

    fprintf(oFile,"%10.6f",L3[i]);
    d++;
    if(d == 8)
    {
      fprintf(oFile,"\n");
      d = 0;
    }
  }

  for(i=0;i<n4;i++)
  {
    fprintf(oFile,"%10.6f",L4[i]);
    d++;
    if(d == 8)
    {
      fprintf(oFile,"\n");
      d = 0;
    }
  }

  if(d != 0) fprintf(oFile,"\n");
  fprintf(oFile,"END\n\n");

//  sprintf(TString,"KinematicsInFile: end"); TPrintf(TString);
}


void TNReaction::LevelInitialise(float *l,int n)
{
  int i = 0;
  for(i=0;i<n;i++) l[i] = 0.;
}


void TNReaction::ToLevels(char *NameInFile,int A1,int A3)
{
  sprintf(TString,"ToLevels : begin"); TPrintf(TString);

//  void CrntStrn(char *s,int l);

  int NextChar = 0,OldChar,Exit = 0;
  int NExcEng = 0,MaxAng = 0,ang = 0,eng = 0;
  char *a;
  float ExEng = 0.;

  FILE *InFile,*OutFile;

  InFile = fopen(NameInFile,"r");

  int i = 0,j = 0;
  float Digit[10];

// Finding of reaction string
  do
  {
    OldChar = NextChar;
    NextChar = fgetc(InFile);
    if( ( (OldChar == 32) && (isdigit(NextChar) != 0) ) ||
          (NextChar == EOF) )
      Exit = 1;
  }
  while(Exit == 0);

// Loading of reaction string
  char *SReaction;
  SReaction = new char[25];
    memset(SReaction,0,25);
  a = SReaction;
  do
  {
    *SReaction = (char)NextChar;
    SReaction++;
    NextChar = fgetc(InFile);
  }
  while( ( (NextChar != 10) && ((char)NextChar != ' ') ) &&
           (NextChar != EOF) );

  *SReaction = '\0';
  SReaction = a;
  sprintf(TString,"SReaction : %s",SReaction);
  TPrintf(TString);

// Set of outgoing channel
  a = strrchr(SReaction,',');
  if( a == NULL ) a = SReaction;
  char *React;
  React = new char[15];
    memset(React,0,15);
  strcpy(React,++a);
  a = strrchr(React,')');
  if( a == NULL ) a = React;
  memset(a,'+',1);
  sprintf(TString,"React : %s",React);
  TPrintf(TString);

// Finding of energy
  char *WString,*DString,*BDString;
  WString = new char[10];
    memset(WString,0,10);
  BDString = new char[10];
    memset(BDString,0,10);
  DString = BDString;
  do
  {
    NextChar = fgetc(InFile);
    *WString = (char)NextChar;
    CrntStrn(WString,6);
  }
// Win32:  while( (strnicmp(WString,"EBEAM=",6) != 0) && (NextChar != EOF) );
  while( (strncasecmp(WString,"EBEAM=",6) != 0) && (NextChar != EOF) );

  while((isdigit(NextChar) == 0) && ((char)NextChar != '.') &&
        (NextChar != EOF) )
    NextChar = fgetc(InFile);

// Loading of energy
  DString = BDString;
  while( ((isdigit(NextChar) != 0) || ((char)NextChar == '.')) &&
         (NextChar != EOF) )
  {
    *DString = (char)NextChar;
    DString++;
    NextChar = fgetc(InFile);
  }
  memset(DString,0,1);
  if(strlen(BDString) != 0) ExEng = atof(BDString);
  sprintf(TString,"ExEng = %10.4lf",ExEng);
  TPrintf(TString);

// Finding and loading of kinematic energy
  while( (NextChar=fgetc(InFile)) != EOF )
  {
    *WString = (char)NextChar;
    CrntStrn(WString,3);
// Win32:    if( strnicmp(WString,"EXC",3) == 0 )
    if( strncasecmp(WString,"EXC",3) == 0 )
    {
      while( (isdigit(NextChar) == 0) && ((char)NextChar != '.') &&
             (NextChar != EOF) )
      {
        NextChar = fgetc(InFile);
      }
// Loading of Exc. energy
      DString = BDString;
      while( (isdigit(NextChar) != 0) || ((char)NextChar == '.') &&
             (NextChar != EOF) )
      {
        *DString = (char)NextChar;
        DString++;
        NextChar = fgetc(InFile);
      }
      memset(DString,0,1);
      eng++;
//      Data[0][eng] = atof(BDString);
      NExcEng++;

// Cancelling of three rows
      i = 0;
      while( (i < 4) && (NextChar != EOF) )
      {
        if( (int)NextChar == 10 ) i++;
        NextChar = fgetc(InFile);
      }

      while( ((char)NextChar == ' ') && (NextChar != EOF) )
      {
        NextChar = fgetc(InFile);
      }

      ang = 0;

// Loading of kinematic angle and energy
      while( (isdigit(NextChar) != 0) || ((char)NextChar == '.') &&
             (NextChar != EOF) )
      {
        i = 0;
        while(i < 3)
        {
          DString = BDString;
          *DString = (char)NextChar;
          while( (isdigit(NextChar) != 0) || ((char)NextChar == '.') &&
                 (NextChar != EOF) )
          {
            *DString = (char)NextChar;
            DString++;
            NextChar = fgetc(InFile);
          }
          memset(DString,0,1);
          Digit[i] = atof(BDString);
          i++;
          while( ((char)NextChar == ' ') && (NextChar != EOF) )
            NextChar = fgetc(InFile);
        }
        ang++;
//        Data[ang][0] = Digit[0];
//        Data[ang][eng] = Digit[2];
        if( ang > MaxAng ) MaxAng = ang;

        while( ((int)NextChar != 10) && (NextChar != EOF) )
          NextChar = fgetc(InFile);

        do
        {
          NextChar = fgetc(InFile);
        }
        while( ((char)NextChar == ' ') && (NextChar != EOF) );
      }
    }
    else
    {
    }
  }
  MaxAng++;


// New code

  ang = 0,eng = 0;

  float *Data;
  Data = new float[(NExcEng+2)*(MaxAng+2)];
  for(i=0;i<=NExcEng;i++)
  {
    for(j=0;j<MaxAng;j++) Data[MaxAng*i+j] = 0.;
  }
  i = 0;
  j = 0;

  rewind(InFile);
// Finding and loading of kinematic energy
  while( (NextChar=fgetc(InFile)) != EOF )
  {
    *WString = (char)NextChar;
    CrntStrn(WString,3);
// Win32:    if( strnicmp(WString,"EXC",3) == 0 )
    if( strncasecmp(WString,"EXC",3) == 0 )
    {
      while( (isdigit(NextChar) == 0) && ((char)NextChar != '.') &&
             (NextChar != EOF) )
      {
        NextChar = fgetc(InFile);
      }
// Loading of Exc. energy
      DString = BDString;
      while( (isdigit(NextChar) != 0) || ((char)NextChar == '.') &&
             (NextChar != EOF) )
      {
        *DString = (char)NextChar;
        DString++;
        NextChar = fgetc(InFile);
      }
      memset(DString,0,1);
      eng++;
      Data[MaxAng*eng] = atof(BDString);

// Cancelling of three rows
      i = 0;
      while( (i < 4) && (NextChar != EOF) )
      {
        if( (int)NextChar == 10 ) i++;
        NextChar = fgetc(InFile);
      }

      while( ((char)NextChar == ' ') && (NextChar != EOF) )
      {
        NextChar = fgetc(InFile);
      }

      ang = 0;
// Loading of kinematic angle and energy
      while( (isdigit(NextChar) != 0) || ((char)NextChar == '.') &&
             (NextChar != EOF) )
      {
        i = 0;
        while(i < 3)
        {
          DString = BDString;
          *DString = (char)NextChar;
          while( (isdigit(NextChar) != 0) || ((char)NextChar == '.') &&
                 (NextChar != EOF) )
          {
            *DString = (char)NextChar;
            DString++;
            NextChar = fgetc(InFile);
          }
          memset(DString,0,1);
          Digit[i] = atof(BDString);
          i++;
          while( ((char)NextChar == ' ') && (NextChar != EOF) )
            NextChar = fgetc(InFile);
        }
        ang++;
        Data[ang] = Digit[0];
        Data[MaxAng*eng+ang] = Digit[2];

        while( ((int)NextChar != 10) && (NextChar != EOF) )
          NextChar = fgetc(InFile);

        do
        {
          NextChar = fgetc(InFile);
        }
        while( ((char)NextChar == ' ') && (NextChar != EOF) );
      }
    }
    else
    {
    }
  }
  fclose(InFile);

  if(MaxAng > 1)
  {
    char *NameOutFile;
    NameOutFile = new char[30];

    char *TDir,*CDir,*ODir;
//    TDir = getcwd(NULL,MAXPATH); // DOS & Win32 code and... what is it?
    CDir = new char[256];
      memset(CDir,0,256);
//  CDir = getcwd( CDir,100 );  it is bad!!!
    if( TDir != NULL )
    {
      strcpy(CDir,TDir);
      free(TDir);
    }
    else
    {
      strcpy(CDir,"C:\\Temp\\Test");
    }
//  strcpy(CDir,"C:\\Temp\\Test");
    sprintf(TString,"CDir : %s",CDir); TPrintf(TString);

    ODir = new char[strlen(CDir)+20];
      memset(ODir,0,strlen(CDir)+20);
    strcpy(ODir,CDir);

    int av;

    if(CDir != NULL)
    {
      strcat(CDir,"\\Lev");
//    sprintf(TString,"%s",CDir);
//    TPrintf(TString);
      if( (av = chdir(CDir)) == -1 )
      {
// for linux       av = mkdir(CDir);
        av = chdir(CDir);
      }

      memset(NameOutFile,0,30);
      sprintf(NameOutFile,"\\%02d",A3);
      strcat(CDir,NameOutFile);
      sprintf(TString,"Path+File : %s",CDir); TPrintf(TString);
      if( (av = chdir(CDir)) == -1 )
      {
// - for linux        av = mkdir(CDir);
        av = chdir(CDir);
      }
    }

    char *EngStr;
    EngStr = new char[10];

//  sprintf(TString,"Begin of cycle"); TPrintf(TString);

    sprintf(TString,"MaxAng = %d",MaxAng); TPrintf(TString);
    
    for(i=1;i<MaxAng;i++)
    {
//      sprintf(TString,"Data[%3d] = %10.4f",i,Data[i]); TPrintf(TString);

      memset(EngStr,0,10);
      sprintf(EngStr,"%04.1f",Data[i]);

      a = strrchr( EngStr,46);
      memset(a,'\'',1);

      memset(NameOutFile,0,30);
      sprintf(NameOutFile,"%02d%02d%4s.dat",A1,A3,EngStr);

      OutFile=fopen(NameOutFile,"w");

      fprintf(OutFile,"\"Reaction: %s; Energy: %6.2f MeV; Angle: %6.2f;\"\n",
                    SReaction,ExEng,Data[i]);
      Exit = 0;
      for(j=1;j<=NExcEng;j++)
      {
        if(Data[MaxAng*j+i] != 0.0)
        {
          fprintf(OutFile,"%7.3f 1 0 0 \"%6.3f\"",Data[MaxAng*j+i],Data[MaxAng*j]);
          if(Exit == 0)
          {
            fprintf(OutFile," \"%s\"",React);
            Exit = 1;
          }
          fprintf(OutFile,"\n");
        }
      }
      fclose(OutFile);
    }

    //  if(CDir != NULL)
//  {
    av = chdir(ODir);
    delete[] ODir;
//  }
    delete[] CDir;
    delete[] NameOutFile;
    delete[] EngStr;
  }

//  sprintf(TString,"End of cycle"); TPrintf(TString);

  delete[] Data;

  delete[] SReaction;
  delete[] React;
  delete[] WString;
  delete[] BDString;
  sprintf(TString,"ToLevels : end"); TPrintf(TString);
}


void TNReaction::CrntStrn(char *String,int Len)
{
  char TChar;
  int i;

  TChar = *String;
  for(i=0;i<=Len-2;i++)
  {
    *(String+i) = *(String+i+1);
  }
  *(String+Len-1) = TChar;
}



void TNReaction::Write(TRChannel *RChannel,int TR,char *str)
{
  sprintf(TString,"TNReaction.Write - begin"); TPrintf(TString);

  char *fn = (char*)"RChannel.test";
  FILE *ofile;
  ofile = fopen(fn,"a");

  int i;

  fprintf(ofile,"\n%s\n\n",str);

  RChannel[0].Write(ofile,(char*)"Entrance Channel",0);

  i = 1;
  while( i < TR){ RChannel[i].Write(ofile,(char*)"Outgoing Channel",1); i++;}

  fclose(ofile);

  sprintf(TString,"TNReaction.Write - end"); TPrintf(TString);
}

// Old version !!!
/*
class::Chanell
{
  private:
    Nuclei Projectile;
    Nuclei Target;
    float Energy;
  public:
    getProjectile();
    putProgectile();
    getTarget();
    putTarget();
    getEnergy();
    putEnergy();
};

main()
{
  Chanell InChanell,OutChanell[];
  SetChanell(InChanell);
  SetChanell(OutChanell[1]); // Elastic scatering
  readInFile(Number of chanell, chanell) // InFile - only .txt file!

 7Li(11B // InChanell or in LaTeX form ^{7}Li(^{11}B ?
 50.0 MeV // Energy of ions
 11B)7Li // first OutChanell
 11B2.15 second OutChanell (inelastic scatering)
 (Full form : ^{11}B^{*}_{2.15} )
 12B and other...

 or?

 7Li(11B
 50.0 MeV
 11B)7Li
 11B2.15)7Li
 11B)7Lix.xx
 12B)6Li
 13B)5Li

  SetChanell(InChanell) -> Set Projectile & Target -> Set Nucleus

  SeT1BNSystem -> read data from Nuclei DB
  Set Z,N,A, binding energy, exited levels & characteristic &
}
*/
//------------------------------------------------------------------------------

