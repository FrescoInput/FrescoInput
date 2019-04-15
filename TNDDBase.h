//------------------------------------------------------------------------------
// TNDDBase.h
//------------------------------------------------------------------------------
// T(ype)N(uclear)D(ata)D(ata)Base
// C.: 2011.07.11.
// M.: 2015.07.11.
//------------------------------------------------------------------------------

#ifndef __TNDDBase_H
#define __TNDDBase_H

class TNDDBase{
private:
  char *cQuery;
  char *cRow;


public:
  TNDDBase();
  ~TNDDBase();

  void Set();


  double AtomicMass(    int Z,int A );
  double MassExcess   ( int Z,int A );
  double BindingEnergy( int Z,int A );
//------------------------------------------------------------------------------


  int NuclearStateNumber( char *Symbol,int A );
  char* NuclearState( char *Symbol,int A );

  int NuclearPotentialNumber( int Zp,int Ap,int Ep,int Zt,int At,int Et );
  char* NuclearPotential( int Zp,int Ap,int Ep,int Zt,int At,int Et );

  int NuclearDeformationNumber( int Zt,int At,int Et );
  char* NuclearDeformation( int Zp,int Ap,int Ep );

//  // - povnyj bred!!!
  int SpectroscopicFactorNumber( int Zcp,int Acp,double Ecp,int Zcr,int Acr,double Ecr,int Zcl,int Acl,double Ecl );
  char* SpectroscopicFactor( int Zcp,int Acp,double Ecp,int Zcr,int Acr,double Ecr,int Zcl,int Acl,double Ecl );
//  // - povnyj bred!!!


/*
// db NP :
  int GetNoNP(T1BNSystem *fProjectile,T1BNSystem *fTarget,int *fRI,int *fEI)
  { return( NP_DBase[0].NP_GetNoNP(fProjectile,fTarget,fRI,fEI) ); }
// ...fast way?
  double NP_V(int RI){  return( NP_DBase[0].NP_Get_V(RI)  ); }
  double NP_rV(int RI){ return( NP_DBase[0].NP_Get_rV(RI) ); }
  double NP_aV(int RI){ return( NP_DBase[0].NP_Get_aV(RI) ); }

  double NP_W(int RI){  return( NP_DBase[0].NP_Get_W(RI)  ); }
  double NP_rW(int RI){ return( NP_DBase[0].NP_Get_rW(RI) ); }
  double NP_aW(int RI){ return( NP_DBase[0].NP_Get_aW(RI) ); }

// db ND :
  int GetNoND(T1BNSystem *fNucleus,int *fRI)
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
//------------------------------------------------------------------------------
*/

};

#endif // __TNDDBase_H
//------------------------------------------------------------------------------

