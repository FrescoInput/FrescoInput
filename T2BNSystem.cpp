//------------------------------------------------------------------------------
// T2BNSystem.cpp
//------------------------------------------------------------------------------
// T(ype)2(Two)B(ody)N(uclear)System
// C.: 200y.mm.dd.
// M.: 2015.08.17.
//------------------------------------------------------------------------------

// for test --------------------------------------------------------------------
extern char *TString;
extern void TPrintf(char *s);
extern FILE *LogFile;
// for test --------------------------------------------------------------------

#include "T2BNSystem.h"

extern TNDDBase *NDDBase;
extern TTFDBase *NDBase;
extern char *GFileName;

T2BNSystem::T2BNSystem()
{
  cProjectile = new T1BNSystem[1];
  cTarget     = new T1BNSystem[1];
  cEnergy = 0.;

  cNoNP = 0;
  cPotential  = new TNPotential[cNoNP+1];
  cPIndex = 0;
//
  cNoPD = 0;
  cProDeform = new TNDeformation[cNoPD+1];
  cNoTD = 0;
  cTarDeform = new TNDeformation[cNoTD+1];
//
  cCore     = new T1BNSystem[1];
  cCluster  = new T1BNSystem[1];
  cCompound = new T1BNSystem[1];
  cCCBEnergy = 0.;
  cNoSF = 1;
  cSFactor  = new TSFactor[cNoSF];
//
  Projectile = new TRNucleus[1];
  Target     = new TRNucleus[1];
// 
  cLogName = new char[ strlen( GFileName ) + 20 ];
  strcpy( cLogName,GFileName );
  strcpy( strchrnul(cLogName,'.'),".problem\0" );
  cLogFile = fopen( cLogName,"a" );
  
}


T2BNSystem::~T2BNSystem()
{
  delete[] cProjectile;
  delete[] cTarget;
  delete[] cPotential;
//
  delete[] cCore;
  delete[] cCluster;
  delete[] cCompound;
  delete[] cSFactor;
//
  delete[] Projectile;
  delete[] Target;
  
  fclose( cLogFile );
  delete[] cLogName;
}


void T2BNSystem::Set()
{
}


int T2BNSystem::Set(char *fs,int fiv)
{
  int lI;
  int liv;
  float lfv;
  double ldv;
  char *lsv;

  lI = GetIndex( fs );

  SetValue(lI,&fiv,&lfv,&ldv,lsv);
}


float T2BNSystem::Set(char *fs,float ffv)
{
  int lI;
  int liv;
  float lfv;
  double ldv;
  char *lsv;

  lI = GetIndex( fs );

  SetValue(lI,&liv,&ffv,&ldv,lsv);
}


double T2BNSystem::Set(char *fs,double fdv)
{
  int lI;
  int liv;
  float lfv;
  double ldv;
  char *lsv;

  lI = GetIndex( fs );

  SetValue(lI,&liv,&lfv,&fdv,lsv);
}


int T2BNSystem::Get(char *fs,int fiv)
{
  int lI;
  int liv;
  float lfv;
  double ldv;
  char *lsv;

  lI = GetIndex( fs );

  GetValue(lI,&liv,&lfv,&ldv,lsv);

  return( liv );
}


float T2BNSystem::Get(char *fs,float ffv)
{
  int lI;
  int liv;
  float lfv;
  double ldv;
  char *lsv;

  lI = GetIndex( fs );

  GetValue(lI,&liv,&lfv,&ldv,lsv);

  return( lfv );
}


double T2BNSystem::Get(char *fs,double fdv)
{
  int lI;
  int liv;
  float lfv;
  double ldv;
  char *lsv;

  lI = GetIndex( fs );

  GetValue(lI,&liv,&lfv,&ldv,lsv);

  return( ldv );
}


int T2BNSystem::GetIndex(char *fs) // this is slow function...
{
  if( strcasecmp("TotalBindingEnergy",fs) == 0 ) return( 1 );
  if( strcasecmp("Q-Reaction",fs)         == 0 ) return( 2 );
  if( strcasecmp("Energy",fs)             == 0 ) return( 3 );
//
  if( strcasecmp("NumberOfNuclearPotentials",fs) == 0 ) return( 4 );
  if( strcasecmp("NumberOfSpectroscopicFactors",fs) == 0 ) return( 5 );
//
  return( -1 );
}


void T2BNSystem::SetValue(int fI,int *fiv,float *ffv,double *fdv,char *fsv)
{

  switch( fI )
  {
    case  2 : { cQReaction = *fdv; CheckLevels(); } break;
    case  3 : { cEnergy = *fdv;    } break;

    default : {} break; // vyvesty shchos typu "Value is absent!" ?
  }
}


void T2BNSystem::GetValue(int fI,int *fiv,float *ffv,double *fdv,char *fsv)
{

  switch( fI )
  {
    case  1 :
    {
      *fdv = cProjectile[0].GetBindingEnergy() + cTarget[0].GetBindingEnergy();
    }
    break;

    case  2 : { *fdv = cQReaction; } break;
    case  3 : { *fdv = cEnergy; } break;

    default : {} break;
  }
}


//------------------------------------------------------------------------------
void T2BNSystem::CheckLevels() // ???
{
  double lMLE; // MaxLevEnergy...

  lMLE = cEnergy + cQReaction;

  cProjectile[0].SetMaxLevEnergy(lMLE);
  cTarget[0].SetMaxLevEnergy(lMLE);
}
//------------------------------------------------------------------------------


void T2BNSystem::Set(char *rcs)
{
  sprintf(TString,"%s",rcs); TPrintf(TString);

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


int T2BNSystem::Set(TRChannel *Channel,int fPI)
{
//  sprintf(TString,"T2BNSystem.Set(TRChannel*,int) - begin"); TPrintf(TString);

  int lrv = 0,ldv; // local deformation value
  char *fE; // functionElement

  fE = new char[6];
    memset(fE,0,6);

  Channel->GetProjectile(fE);
//  Channel[fI].GetProjectile(fE);
//  sprintf(TString,"T2BNSystem.Set.fE : %s",fE); TPrintf(TString);
// Projectile.Set( Channel->GetProjectile ) - is better ?
  cProjectile[0].Set(fE);
  cProjectile[0].SetNoS(Channel->GetProNumberOfStates());

//  sprintf(TString,"T2BNSystem.Set.fE : %s",fE); TPrintf(TString);



  Channel->GetTarget(fE);
//  Channel[fI].GetTarget(fE);
//  sprintf(TString,"T2BNSystem.Set.fE : %s",fE); TPrintf(TString);
  cTarget[0].Set(fE);
  cTarget[0].SetNoS(Channel->GetTarNumberOfStates());

// Nuclear potential setup :
  lrv = SetNP(cProjectile,cTarget,fPI); // potential index

  if( lrv == 0 ){ lrv = SetNP(cTarget,cProjectile,fPI); } // try to find other NP...
//

// Nuclear deformation setup :
  int fDI = 0; // - temp !!!
  ldv = SetND(cProjectile,cTarget,fDI); // deformation index
//

  delete[] fE;

//  sprintf(TString,"T2BNSystem.Set(TRChannel*,int) - end"); TPrintf(TString);

  return( lrv );
}

// pry najavnosti zminnyh fECN ta fOCN zminni typu TRChannel zajvi? : 
int T2BNSystem::Set(TRChannel *EChannel,int fECN,TRChannel *OChannel,int fOCN,char *s,int fSI,int fPI)
{
//  sprintf(TString,"T2BNSystem.Set(CCS) - begin"); TPrintf(TString);
  int lrv = 0;

  int lZc,lNc,lio;
  int lez,lea,loz,loa,exist = 0,lNoL = 0,lProV = 0;
  int lNoLE;
  double *Levels;
  char *lep,*let,*lop,*lot;

  T1BNSystem *lEN,*lON;
  TRNucleus *lip; // this is brad!!!

  Levels = new double[1];

  lep = new char[6];
    memset(lep,0,6);
  let = new char[6];
    memset(let,0,6);
  lop = new char[6];
    memset(lop,0,6);
  lot = new char[6];
    memset(lot,0,6);

  lEN = new T1BNSystem[1];
  lON = new T1BNSystem[1];

  EChannel->GetProjectile( lep );
  EChannel->GetTarget( let );
  OChannel->GetProjectile( lop );
  OChannel->GetTarget( lot );

//  sprintf(TString,"\nT2BNSystem.Set(CCS)    : %s(%s,%s)%s",let,lep,lop,lot); TPrintf(TString);

  if( strcmp("Projectile",s) == 0 )
  {
    lProV = 1; // projectile
    EChannel->GetProjectile( lep );
    OChannel->GetProjectile( lop );
    lio = OChannel->GetProNumberOfStates();   

//  sprintf(TString,"T2BNSystem.Set(CCS) Pro : lio = %d",lio); TPrintf(TString);

    delete[] Levels;
    Levels = new double[lio];

    lNoL = OChannel->GetProLevels(Levels,lio); // this is brad!!! 

//  sprintf(TString,"T2BNSystem.Set(CCS) Pro : lNoL = %d",lNoL); TPrintf(TString);
  }

  if( strcmp("Target",s) == 0 )
  {
    lProV = 2;
    EChannel->GetTarget( lep );
    OChannel->GetTarget( lop );
    lio = OChannel->GetTarNumberOfStates(); // or Levels?

//  sprintf(TString,"T2BNSystem.Set(CCS) Tar : lio = %d",lio); TPrintf(TString);

    delete[] Levels;
    Levels = new double[lio];

    lNoL = OChannel->GetTarLevels(Levels,lio); // this is brad!!! 

//  sprintf(TString,"T2BNSystem.Set(CCS) Tar : lNoL = %d",lNoL); TPrintf(TString);
  }

  lNoLE = lio;
//  sprintf(TString,"T2BNSystem.Set(CCS)    : lEN : %s lON : %s",lep,lop); TPrintf(TString);

  lEN[0].Set(lep);
  lON[0].Set(lop);

  lez = lEN[0].GetZ();
  lea = lEN[0].GetN();
  loz = lON[0].GetZ();
  loa = lON[0].GetN();

//  sprintf(TString,"T2BNSystem.Set(CCS)    : EZ = %d EN = %d",lez,lea); TPrintf(TString);
//  sprintf(TString,"T2BNSystem.Set(CCS)    : OZ = %d ON = %d",loz,loa); TPrintf(TString);

  lZc = abs( lEN[0].GetZ() - lON[0].GetZ() );
  lNc = abs( lEN[0].GetN() - lON[0].GetN() );

//  sprintf(TString,"T2BNSystem.Set(CCS)    : CZ = %d CN = %d",lZc,lNc); TPrintf(TString);

// this is bad :
  if( (lZc != 0) && (lNc != 0) )
  {
    exist = cCluster[0].Set(lZc,lNc);
  }
  else
  {
    exist = cCluster[0].SetCluster(lZc,lNc); // this is bad! It is REALLY BAD! - June 9
  }
//

//  sprintf(TString,"T2BNSystem.Set(CCS)    : exist = %d",exist); TPrintf(TString);

  cCluster[0].GetSymbol(let);

//  sprintf(TString,"T2BNSystem.Set(CCS)    : Cluster : %s",let); TPrintf(TString);
//  sprintf(TString,"T2BNSystem.Set(CCS)    : NoS     : %d",lio); TPrintf(TString);


  if( lEN[0].GetA() > lON[0].GetA() )
  {
    cCore[0].Set(lop);
    cCore[0].SetCN(fOCN);
    cCore[0].SetNoS(lio);
    cCore[0].SetLevels(Levels,lNoL);
    cCompound[0].Set(lep);
    cCompound[0].SetCN(fECN);
  }
  else
  {
    cCore[0].Set(lep);
    cCore[0].SetCN(fECN);
    cCompound[0].Set(lop);
    cCompound[0].SetCN(fOCN);
    cCompound[0].SetNoS(lio);
    cCompound[0].SetLevels(Levels,lNoL);
  }

/*// test only :
  int i = 0,tNoS;
  double tSE;
  tNoS = cCore[0].GetStateNumber();

  sprintf(TString,"T2BNSystem.Set.Core : tNoS = %d",tNoS); TPrintf(TString);

  while( i < tNoS )
  {
    tSE = cCore[0].GetStateEnergy(i);
  sprintf(TString,"T2BNSystem.Set.Core : i = %d, Energy = %lf",i,tSE); TPrintf(TString);
    i++;
  }
*///

  cCCBEnergy = cCompound[0].GetBindingEnergy() - cCore[0].GetBindingEnergy() - cCluster[0].GetBindingEnergy();

// Nuclear potential setup :
//  int fPI = 0; // test only!!!
  lrv = SetNP(cCore,cCluster,fPI); // potential index

  if( lrv == 0 ){ lrv = SetNP(cCluster,cCore,fPI); } // try to find other NP...

  if( lrv == 0 )
  {
    lrv = SetNPR(cCore,cCluster,fPI); // set NP Rudchik's :)
//
//    FILE *lOF;
//    lOF = fopen("problem.log","a");

    cCompound[0].GetIsotope(lep);
        cCore[0].GetIsotope(lop);
     cCluster[0].GetIsotope(lot);

    fprintf(cLogFile,"System : %5s -> %5s + %5s - no nuclear potential, \"universal\" are used\n",lep,lop,lot);

//    fclose(lOF);
//
  }

  lrv = SetSF(fSI,lProV,Levels,&lNoLE);

  if( lrv == 0 )
  {
//
//    FILE *lOF;
//    lOF = fopen("problem.log","a"); // 

    cCompound[0].GetIsotope(lep);
        cCore[0].GetIsotope(lop);
     cCluster[0].GetIsotope(lot);

    fprintf(cLogFile,"System : %5s -> %5s + %5s - no spectroscopic factor\n",lep,lop,lot);

//    fclose(lOF);
//
  }

  OChannel->CheckLevels(lProV,Levels,lNoLE);  // 1 - Pr, 2 - Tr

  delete[] Levels;

  delete[] lep;
  delete[] let;
  delete[] lop;
  delete[] lot;

  delete[] lEN;
  delete[] lON;

//  sprintf(TString,"T2BNSystem.Set(CCS) - end"); TPrintf(TString);
  return( lrv );
}


int T2BNSystem::SetNP(T1BNSystem *fProjectile,T1BNSystem *fTarget,int fPI)
{
//  sprintf(TString,"T2BNSystem.SetNP - begin"); TPrintf(TString);

  int i = 0,lrv = 0;
  int lNoNP = 0,lPI;
  int *lRI,*lEI;

  lRI = new int[20]; 
  lEI = new int[20]; 

  cPIndex = fPI;

  cNoNP = NDBase[0].NP_GetNoNP(fProjectile,fTarget,lRI,lEI);
// MySQL
  int Zp,Ap,Zt,At;
  double Ep = 0.,Et = 0.;

  Zp = fProjectile->GetZ();
  Ap = fProjectile->GetA();
//  Ep = fProjectile->GetState();
  Zt =     fTarget->GetZ();
  At =     fTarget->GetA();
//  Et =     fTarget->GetState();

  int mNoNP = 0;
  mNoNP = NDDBase->NuclearPotentialNumber( Zp,Ap,Ep,Zt,At,Et );
//
  lrv = cNoNP; // ??? Does lrv really need?

//  sprintf(TString,"T2BNSystem.SetNP.cNoNP       : %d",cNoNP); TPrintf(TString);
//  sprintf(TString,"T2BNSystem.SetNP.MySQL cNoNP : %d",mNoNP); TPrintf(TString);

  cNoNP = mNoNP;

  if( cNoNP != 0 )
  {
    delete[] cPotential;
    cPotential = new TNPotential[cNoNP];
    lPI = fPI + 1;

    i = 0;
    while( i < cNoNP )
    {
//      cPotential[i].Set(lRI[i],lPI,lEI[i]);
      cPotential[i].Set( Zp,Ap,Ep,Zt,At,Et );
      i++;
      lPI++;
    }
  }

  delete[] lRI;
  delete[] lEI;

//  sprintf(TString,"T2BNSystem.SetNP - end"); TPrintf(TString);

  return( lrv );
}


int T2BNSystem::SetNPR(T1BNSystem *fProjectile,T1BNSystem *fTarget,int fPI)
{
//  sprintf(TString,"T2BNSystem.SetNPR - begin"); TPrintf(TString);

  int i = 0,lrv = 0;
  int lNoNP = 0,lPI;
  int lAp,lAt;


  cPIndex = fPI;

//  cNoNP = NDBase[0].NP_GetNoNP(fProjectile,fTarget,lRI,lEI);

  cNoNP = 1;

  lrv = cNoNP; // ??? Does lrv really need?

//  sprintf(TString,"T2BNSystem.SetNPR.cNoNP : %d",cNoNP); TPrintf(TString);

  lAp = fProjectile[0].GetA();
  lAt =     fTarget[0].GetA();

  if( cNoNP != 0 )
  {
    delete[] cPotential;
    cPotential = new TNPotential[cNoNP];
    lPI = fPI + 1;

    cPotential[0].SetR(lAp,lAt,lPI);
/*/
    i = 0;
    while( i < cNoNP )
    {
      cPotential[i].SetR(lPI);
      i++;
      lPI++;
    }
/*/
  }

//  delete[] lRI;
//  delete[] lEI;

//  sprintf(TString,"T2BNSystem.SetNPR - end"); TPrintf(TString);

  return( lrv );
}


int T2BNSystem::SetND(T1BNSystem *fProjectile,T1BNSystem *fTarget,int fDI)
{
//  sprintf(TString,"T2BNSystem.SetNP - begin"); TPrintf(TString);

  int i = 0,lrv = 0;
  int lNoND = 0,lDI;
  int *lRI,*lEI;

  lRI = new int[20]; 
  lEI = new int[20]; 

// Projectile deformation setup : ---------------------------------------------- 

//  cNoPD = NDBase[0].ND_GetNoND(fProjectile,lRI);

// MySQL
  int Zp,Ap,Zt,At;
  double Ep = 0.,Et = 0.;

  Zp = fProjectile->GetZ();
  Ap = fProjectile->GetA();
//  Ep = fProjectile->GetState();

  int mNoPD = 0;
  mNoPD = NDDBase->NuclearDeformationNumber( Zp,Ap,Ep );
//
  cNoPD = mNoPD;

  lrv = cNoPD; // ??? Does lrv really need?

//  sprintf(TString,"T2BNSystem.SetND.cNoPD : %d",cNoPD); TPrintf(TString);
//  sprintf(TString,"T2BNSystem.SetND.MySQL mNoPD : %d",mNoPD); TPrintf(TString);

  if( cNoPD != 0 )
  {
    delete[] cProDeform;
    cProDeform = new TNDeformation[cNoPD];
    lDI = fDI + 1;

    i = 0;
    while( i < cNoPD )
    {
////      cProDeform[i].Set(lRI[i],lDI,lEI[i]);
//      cProDeform[i].Set(lRI[i]);
      cProDeform[i].Set( Zp,Ap,Ep );
      i++;
      lDI++;
    }
  }

// Target deformation setup : --------------------------------------------------
 
//  cNoTD = NDBase[0].ND_GetNoND(fTarget,lRI);

// MySQL
  Zt =     fTarget->GetZ();
  At =     fTarget->GetA();
//  Et =     fTarget->GetState();

  int mNoTD = 0;
  mNoTD = NDDBase->NuclearDeformationNumber( Zt,At,Et );
//
  cNoTD = mNoTD;

  lrv = cNoTD; // ??? Does lrv really need?

//  sprintf(TString,"T2BNSystem.SetND.cNoTD : %d",cNoTD); TPrintf(TString);
//  sprintf(TString,"T2BNSystem.SetND.MySQL mNoTD : %d",mNoTD); TPrintf(TString);

  if( cNoTD != 0 )
  {
    delete[] cTarDeform;
    cTarDeform = new TNDeformation[cNoTD];
    lDI = fDI + 1;

    i = 0;
    while( i < cNoTD )
    {
////      cTarDeform[i].Set(lRI[i],lDI,lEI[i]);
//      cTarDeform[i].Set(lRI[i]);
      cTarDeform[i].Set( Zt,At,Et );
      i++;
      lDI++;
    }
  }

//------------------------------------------------------------------------------

  delete[] lRI;
  delete[] lEI;

//  sprintf(TString,"T2BNSystem.SetNP - end"); TPrintf(TString);

  return( lrv );
}


int T2BNSystem::SetSF(int fSI,int fProV,double *fLE,int *fNoLE)
{
//  sprintf(TString,"T2BNSystem.SetSF - begin"); TPrintf(TString);

  int i = 0;
/*
  char *lcp,*lcr,*lcl;

  lcp = new char[6];
    memset(lcp,0,6);
  lcr = new char[6];
    memset(lcr,0,6);
  lcl = new char[6];
    memset(lcl,0,6);

  cCompound[0].GetSymbol(lcp);
  cCore[0].GetSymbol(lcr);
  cCluster[0].GetSymbol(lcl);

//  sprintf(TString,"T2BNSystem.SetSF       : %6s -> %6s + %6s",lcp,lcr,lcl); TPrintf(TString);
*/

  int Zcp,Acp,Zcr,Acr,Zcl,Acl;
  double Ecp = 0.,Ecr = 0.,Ecl = 0.;

  Zcp = cCompound->GetZ();
  Acp = cCompound->GetA();
//  Ecp = cCompound->GetState();

  Zcr = cCore[0].GetZ();
  Acr = cCore[0].GetA();
//  Ecr = cCore[0].GetStateEnergy( 0 ); // 0 - ground level

  Zcl = cCluster->GetZ();
  Acl = cCluster->GetA();
//  Ecl = cCluster->GetState();

  cNoSF = NDDBase->SpectroscopicFactorNumber( Zcp,Acp,Ecp, Zcr,Acr,Ecr, Zcl,Acl,Ecl );

  sprintf(TString,"T2BNSystem.SetSF.MySQL cNoSF = %d",cNoSF); TPrintf(TString);

  if( cNoSF != 0 )
  {
    delete[] cSFactor;
    cSFactor = new TSFactor[cNoSF];

    i = 0;
    while( i < cNoSF )
    {
      cSFactor[i].Set( Zcp,Acp,Ecp, Zcr,Acr,Ecr, Zcl,Acl,Ecl );
//  sprintf(TString,"T2BNSystem.SetSF i = %d \n",i); TPrintf(TString);
      i++;
    }
  }

/*
  delete[] lcp;
  delete[] lcr;
  delete[] lcl;
*/

//  sprintf(TString,"T2BNSystem.SetSF - end"); TPrintf(TString);

  return( cNoSF );
}


int T2BNSystem::Read(FILE *ifl,char *w,int c)
{
//  sprintf(TString,"T2BNSystem.Read - begin"); TPrintf(TString);

  int rv; // return value
  char *cs;

  cs = new char[20]; 
    memset(cs,0,20);

  if( (rv = StringRead(ifl,w,cs)) != 0)
  {
    rv = Projectile[0].Read(ifl,(char*)"Projectile");
    rv = Target[0].Read(ifl,(char*)"Target");

    if( c == 0 ) // read input channel information
    {
      rv = StringRead(ifl,(char*)"Energy",cs);
      cEnergy = atof(cs);
//  sprintf(TString,"Read:Energy: %lf",cEnergy); TPrintf(TString);
    }

    rv = 1;
  }
  else
  {
    rv = 0;
  }

  delete[] cs;

//  sprintf(TString,"T2BNSystem.Read - end"); TPrintf(TString);

  return( rv );
}


void T2BNSystem::Write(FILE *ofl,char *w,int c)
{
//  sprintf(TString,"T2BNSystem.Write - begin"); TPrintf(TString);

  if( strcasecmp(w,"#Input Channel") != 0 )
    fprintf(ofl,"#Exit channels : ");

  fprintf(ofl,"%s :\n",w);

  Projectile[0].Write(ofl,(char*)"Projectile");
  Target[0].Write(ofl,(char*)"Target");

//  if(cEnergy != 0.) fprintf(ofl,"%-13s : %lf\n","Energy",cEnergy);

//  sprintf(TString,"T2BNSystem.Write - end"); TPrintf(TString);
}


void T2BNSystem::WriteDB(FILE *ofl)
{
  fprintf(ofl,"Energy    = %14.3lf (MeV)\n",cEnergy);
  fprintf(ofl,"QReaction = %14.3lf (keV)\n",cQReaction);
  cProjectile[0].WriteDB(ofl); 
  cTarget[0].WriteDB(ofl); 
}


void T2BNSystem::WriteDB_CC(FILE *ofl)
{
  cCompound[0].WriteDB(ofl,(char*)"Compound");
      cCore[0].WriteDB(ofl,(char*)"Core");
   cCluster[0].WriteDB(ofl,(char*)"Cluster");
   fprintf(ofl,"\n");
}
//------------------------------------------------------------------------------

