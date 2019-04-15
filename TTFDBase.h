//------------------------------------------------------------------------------
// TTFDBase.h
//------------------------------------------------------------------------------
// T(ype)T(ext)F(ile)D(ata)Base
// C.: 2008.03.28.
// M.: 2015.05.15.
//------------------------------------------------------------------------------

#ifndef __TTFDBase_H
#define __TTFDBase_H

#include "TFIFile/TPart.h"
#include "T1BNSystem.h"

class TTFDBase{
private:
  TPart *NM_DBase;
  TPart *NP_DBase;
  TPart *ND_DBase;
  TPart *SF_DBase;

public:
  TTFDBase();
  ~TTFDBase();

  int Set(); // return 0 if Ok?
  void Read();
  void Write();

/*/ ...slow way?
  int Get(char *fvn,int fiv){ return( Get(fvn,fiv,0) ); }
  float Get(char *fvn,float ffv){ return( Get(fvn,ffv,0) ); }
  double Get(char *fvn,double fdv){ return( Get(fvn,fdv,0) ); }
//  char *Get(char *fvn,char *fsv){ return( Get(fvn,fsv,0) ); } // ???

  int Get(char *fvn,int fiv,int fri);
  float Get(char *fvn,float ffv,int fri);
  double Get(char *fvn,double fdv,int fri);
/*/

// db NM :
  double NM_GetAtomicMass(int fZ,int fA){ return( NM_DBase[0].NM_GetAtomicMass(fZ,fA) ); }
  double NM_GetMassExcess(int fZ,int fA){ return( NM_DBase[0].NM_GetMassExcess(fZ,fA) ); }
  double NM_GetBindingEnergy(int fZ,int fA){ return( NM_DBase[0].NM_GetBindingEnergy(fZ,fA) ); }

// db NP :
  int NP_GetNoNP(T1BNSystem *fProjectile,T1BNSystem *fTarget,int *fRI,int *fEI)
  { return( NP_DBase[0].NP_GetNoNP(fProjectile,fTarget,fRI,fEI) ); }
// ...fast way?
  double NP_Get_V(int RI){  return( NP_DBase[0].NP_Get_V(RI)  ); }
  double NP_Get_rV(int RI){ return( NP_DBase[0].NP_Get_rV(RI) ); }
  double NP_Get_aV(int RI){ return( NP_DBase[0].NP_Get_aV(RI) ); }

  double NP_Get_W(int RI){  return( NP_DBase[0].NP_Get_W(RI)  ); }
  double NP_Get_rW(int RI){ return( NP_DBase[0].NP_Get_rW(RI) ); }
  double NP_Get_aW(int RI){ return( NP_DBase[0].NP_Get_aW(RI) ); }

// db ND :
  int ND_GetNoND(T1BNSystem *fNucleus,int *fRI)
  { return( ND_DBase[0].ND_GetNoND(fNucleus,fRI) ); }

  double ND_Get_Def(int RI,int DI){ return( ND_DBase[0].ND_Get_Def(RI,DI) ); }

// db SF :
  int SF_GetNoSF(T1BNSystem *fCompound,T1BNSystem *fCore,T1BNSystem *fCluster,double *fLE,int *fRI,int *fEI)
  { return( SF_DBase[0].SF_GetNoSF(fCompound,fCore,fCluster,fLE,fRI,fEI) ); }

  int SF_GetN(int RI){ int lrv; return( SF_DBase[0].SF_GetN(RI) ); }
  int SF_GetL(int RI){ int lrv; return( SF_DBase[0].SF_GetL(RI) ); }
  float SF_GetJ(int RI){ int lrv; return( SF_DBase[0].SF_GetJ(RI) ); }
  double SF_GetSF(int RI){ double lrv; return( SF_DBase[0].SF_GetSF(RI) ); }
//

private:
  void OpenFile(TPart *DBase,char *ffn,char *fn);
  void ReadFile(TPart *DBase,char *FName);
  void WriteFile(TPart *DBase,char *FName);
  void Initialize();
};

#endif // __TTFDBase_H
//------------------------------------------------------------------------------

