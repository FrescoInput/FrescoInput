//------------------------------------------------------------------------------
// T2BNSystem.h
//------------------------------------------------------------------------------
// T(ype)2(Two)B(ody)N(uclear)System
// C.: 2008.02.dd.
// M.: 2015.08.17.
//------------------------------------------------------------------------------

#ifndef __T2BNSystem_H
#define __T2BNSystem_H

#include "TRNucleus.h"
#include "TRChannel.h"
//#include "TPotential.h"
#include "TNDeformation.h"
#include "TSFactor.h"
#include "T1BNSystem.h"

class T2BNSystem{
private:
  T1BNSystem *cProjectile; // & cCluster?
  T1BNSystem *cTarget; // & cCore?
  double cEnergy;
  double cQReaction;

  int cNoNP; // ???
  TNPotential *cPotential;
  int cPIndex;
//
  int cNoPD;
  TNDeformation *cProDeform;
  int cNoTD;
  TNDeformation *cTarDeform;
//

// Core-Cluster System
  T1BNSystem *cCore;
  T1BNSystem *cCluster;
  T1BNSystem *cCompound;
  double cCCBEnergy;
  int cNoES; // ???

  int cNoSF;
  TSFactor *cSFactor;

//
  TRNucleus *Projectile,*Target;

//
  char *cLogName;
  FILE *cLogFile;

public:
  T2BNSystem();
  ~T2BNSystem();

  void Set();
  void Set(char *s);
  int Set(TRChannel *Channel,int fPI);
  int Set(TRChannel *EChannel,int fECN,TRChannel *OChannel,int fOCN,char *s,int fSI,int fPI); // C-C System setup

// new functions : -------------------------------------------------------------
  int Set(char *s,int fiv);
  float Set(char *s,float ffv);
  double Set(char *s,double fdv);
  char *Set(char*s,char *fsv){};

/*  int Get(char *s);
  float Get(char *s);
  double Get(char *s);
  char *Get(char*s){}; - error */

  int Get(char *s,int fiv);
  float Get(char *s,float ffv);
  double Get(char *s,double fdv);
  char *Get(char*s,char *fsv){};
//------------------------------------------------------------------------------

  int Read(FILE *f,char *s,int c); // c = 0 if input channel, c = 1 if exit channel
  void Write(FILE *f,char *s,int c);
//
  void WriteDB(FILE *f);
  void WriteDB_CC(FILE *f);

  void GetProjectile(char *s){ Projectile[0].GetSymbol(s); }
  void GetTarget(char *s){ Target[0].GetSymbol(s); }

  double GetEnergy(){ return( cEnergy ); }

  int GetPA(){ return(cProjectile[0].GetA()); }
  int GetTA(){ return(cTarget[0].GetA()); }

  int GetClA(){ return(cCluster[0].GetA()); }
  int GetCrA(){ return(cCore[0].GetA()); }
  double GetCCBindingEnergy(){ return( cCCBEnergy ); }

  int GetPZ(){ return(cProjectile[0].GetZ()); }
  int GetTZ(){ return(cTarget[0].GetZ()); }

  void GetPIsotope(char *s){ cProjectile[0].GetIsotope(s); }// { Get(ProjectileIsotope",s); }
//char Get("ProjectileIsotope",s) - is better?
  void GetTIsotope(char *s){ cTarget[0].GetIsotope(s); }
//
  int GetProjectileNoS(){ return( cProjectile[0].GetNoS() ); } 
  int GetTargetNoS(){     return(     cTarget[0].GetNoS() ); }
//
  int GetProjectileNoL(){ return( cProjectile[0].GetNoL() ); } 
  int GetTargetNoL(){     return(     cTarget[0].GetNoL() ); }
//
  float GetPSpin(double e){ return( cProjectile[0].GetSpin(e) ); }
  float GetTSpin(double e){ return( cTarget[0].GetSpin(e) ); }
  int GetPParity(double e){ return( cProjectile[0].GetParity(e) ); }
  int GetTParity(double e){ return( cTarget[0].GetParity(e) ); }

  int StringRead(FILE *f,char *w,char *s){};

// NM :
  double GetPMass(){ return(cProjectile[0].GetAtomicMass()); }
  double GetTMass(){ return(cTarget[0].GetAtomicMass()); }
// NP :
  int CheckNP(){ return( cNoNP ); }

  int NP_Get_PIndex(){ return( cPIndex ); }
  int NP_Get_Type(){  return( cPotential[0].Get_Type()  ); }
  int NP_Get_It(){    return( cPotential[0].Get_It()    ); }
  int NP_Get_Shape(){ return( cPotential[0].Get_Shape() ); }

  double NP_Get_V(){  return( cPotential[0].Get_V()  ); }
  double NP_Get_rV(){ return( cPotential[0].Get_rV() ); }
  double NP_Get_aV(){ return( cPotential[0].Get_aV() ); }

  double NP_Get_W(){  return( cPotential[0].Get_W()  ); }
  double NP_Get_rW(){ return( cPotential[0].Get_rW() ); }
  double NP_Get_aW(){ return( cPotential[0].Get_aW() ); }

  double NP_Get_P0(){ return( cPotential[0].Get_P0() ); }

// ND :
  double ND_Get_Def(int n,int i){
    if( n == 0 ){ return( cProDeform[0].Get_Def(i) );}
    else{         return( cTarDeform[0].Get_Def(i) );} }

// SF :
  int CheckSF(){ return( cNoSF ); } // -???!!!
  int GetNoSF(){ return( cNoSF ); }
  int SF_GetN(int i){ return( cSFactor[i].GetN() ); }
  int SF_GetL(int i){ return( cSFactor[i].GetL() ); }
  float SF_GetJ(int i){ return( cSFactor[i].GetJ() ); }
  double SF_GetSF(int i){ return( cSFactor[i].GetSF() ); }
  int SF_GetSI(int i){ return( cSFactor[i].GetSI() ); }
  int SF_GetProV(int i){ return( cSFactor[i].GetProV() ); }
  int SF_GetEI(int i){ return( cSFactor[i].GetEI() ); }
//
  void SetNoES(int fN){ cNoES = fN; }
  int SF_GetNoES(){ return(cNoES); } // bad...
// Fresco file :
  int GetCompoundCN(){ return( cCompound[0].GetCN() ); }
  int GetCoreCN(){ return( cCore[0].GetCN() ); }
  int GetClusterParity(){ return( cCluster[0].GetParity( 0. ) ); } // this is for g.s. only!!!
  float GetClusterSpin(){ return( cCluster[0].GetSpin( 0. ) ); } // this is for g.s. only!!!

private:
  int GetIndex(char *fs);
  void SetValue(int fI,int *i,float *f,double *d,char *s);
  void GetValue(int fI,int *i,float *f,double *d,char *s);
//
  void CheckLevels();
// db :
  int SetNP(T1BNSystem *fProjectile,T1BNSystem *fTarget,int fPI);
 int SetNPR(T1BNSystem *fProjectile,T1BNSystem *fTarget,int fPI); // ???
  int SetND(T1BNSystem *fProjectile,T1BNSystem *fTarget,int fDI);
  int SetSF(int fSI,int fProV,double *fLE,int *fNoLE);
};

#endif // __T2BNSystem_H
//------------------------------------------------------------------------------

