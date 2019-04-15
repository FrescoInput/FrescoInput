//------------------------------------------------------------------------------
// T1BNSystem.h
//------------------------------------------------------------------------------
// T(ype)1(One)B(ody)N(uclear)System
// C.: 1997.04.22.
// V.: 2002.12.18. v. For ODESA
// M.: 2015.08.17.
//------------------------------------------------------------------------------

#ifndef __T1BNSystem_H
#define __T1BNSystem_H 1

#include "TNState.h"

class T1BNSystem{
private:
  int A; // Atomic Mass Number
  int Z; // Proton Number
  int N; // Neutron Number
  char *Symbol; // chemical Symbol
  char *Isotope; // Isotope
  int cExist; // new...

  int MinA;
  int MaxA;

//
  double cAM; // Atomic Mass?
  double cME; // Mass Excess
  double cBE; // Binding Energy
/*
  double cMassExcess;
  double cAtomicMass;
  double cBindingEnergy;*/
//

  int cNoS;
  int SN; // S(tate) N(umber)
  TNState *States;
  double cState;  // ??? where data assign?

  int cNoL;
  int *cLevIndex;
  int NL; // N(umber of)L(evels) SN == NL ?!!!
  double *Level;

  int cCN; // Channel Number
/*/
  int cNoD;
  TNDeformation *cNuclDeform;
/*/
//  char *BasePath;

public:
  T1BNSystem();
  ~T1BNSystem();

  int Set(char *s);     // why int?
  int Set(int p);       // if 0 - no exist
  int Set(int p,int n); // if 0 - no exist
  int SetCluster(int p,int n); // if 0 - no exist, bad function!!!
  int SetIsotope(int p){ Set(p); }
  int SetIsotope(int p,int n){ Set(p,n); }
  int SetIsotope(char *s); // if 0 - no exist. It is bad: if only "B" - error!!!

  void SetNoS(int n){ cNoS = n; } // ???
//  void SetLevels(double *Levels,int NoL); // ???

  void SetMaxLevEnergy(double fEV);
//
  int GetA(){ return( A ); }
  int GetZ(){ return Z;}
  int GetN(){ return N;}
  int GetMinA(){ return MinA; }
  int GetMaxA(){ return MaxA; }
  int GetStateNumber(){ return SN; }
  int GetNoS(){ return cNoS; }
  double GetState(){ sprintf(TString,"T1BNSystem.GetState cState = %lf",cState); TPrintf(TString); return cState; }
  double GetStateEnergy(int i){return States[i].GetEnergy();}
//  double GetStateEnergy(int i,char *s);// (i,MeV); (i,keV)

  double GetAtomicMass(){ return( cAM ); }
  double GetMassExcess(){ return( cME ); }
  double GetBindingEnergy(){ return( cBE/1000. ); } // ???

  void GetIsotope(char *s){ strncpy(s,Isotope,6); }
  void GetElement(char *s){ strncpy(s,Symbol,6); }
  void GetSymbol(char *s){ strncpy(s,Isotope,6); }

  float GetSpin(double e);
  int GetParity(double e);

  int SetLevels();
  void SetLevels(double *fLev,int fNoL);
  int GetNoL(){ return(cNoL); }
  double GetLevelEnergy(int i);

// for Fresco file :
  void SetCN(int fCN){ cCN = fCN; }
  int GetCN(){ return( cCN ); }

//  void SetBasePath(char *s);
//
  void WriteDB(FILE *ofl);
  void WriteDB(FILE *ofl,char *s);

private:
  void Initialize();
  void ReadDBFile();
  void SetMass();
  int Exist(FILE *OFile);
  int ReadLine(FILE *OFile,char *s,int l);
  char *RLAnalyse(char *Ls,int *fZ,int *fA,char *fE,double *fL);
// test
  char *RLAnalyseNew(char *Ls,int *fZ,int *fA,char *fE,float *fL);
};

#endif // __T1BNSystem_H
//------------------------------------------------------------------------------

