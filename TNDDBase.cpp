//------------------------------------------------------------------------------
// TNDDBase.cpp
//------------------------------------------------------------------------------
// T(ype)N(uclear)D(ata)D(ata)Base
// C.: 2011.07.11.
// M.: 2015.07.11.
//------------------------------------------------------------------------------

// for test --------------------------------------------------------------------
extern char *TString;
extern void TPrintf(char *s);
extern FILE *LogFile;
// for test --------------------------------------------------------------------

#include <stdio.h>

#include "TNDDBase.h"


extern TMySQLDB *MySQLDB;


TNDDBase::TNDDBase()
{
  cQuery = new char[1000];
}


TNDDBase::~TNDDBase()
{
  delete[] cQuery;
}


void TNDDBase::Set()
{
  sprintf(TString,""); TPrintf(TString);
}


double TNDDBase::AtomicMass(int pZ,int pA) // pereviryty chy tse virno!!!
{
  double fValue;

  sprintf(cQuery,"SELECT Atomic_Mass FROM mass WHERE Charge=\'%d\' AND AMass=\'%d\'\0",pZ,pA);

  cRow = MySQLDB->Get((char*)"mass",cQuery,0);
  fValue = atof( cRow );
  delete[] cRow;

  *cQuery = '\0';

  fValue = pA * 1000000. + fValue; // tse nepravylno! 

  return( fValue );
}


double TNDDBase::MassExcess(int pZ,int pA)
{
  double fValue;

  sprintf(cQuery,"SELECT Mass_Excess FROM mass WHERE Charge=\'%d\' AND AMass=\'%d\'\0",pZ,pA);

  cRow = MySQLDB->Get((char*)"mass",cQuery,0);
  fValue = atof( cRow );
  delete[] cRow;

  *cQuery = '\0';

  return( fValue );
}


double TNDDBase::BindingEnergy(int pZ,int pA)
{
  double fValue;

  sprintf(cQuery,"SELECT Bind_Energy_A FROM mass WHERE Charge=\'%d\' AND AMass=\'%d\'\0",pZ,pA);

  cRow = MySQLDB->Get((char*)"mass",cQuery,0);
  fValue = atof( cRow );
  delete[] cRow;

  *cQuery = '\0';

  fValue = fValue * pA; // pererobyty...

  return( fValue );
}


//------------------------------------------------------------------------------


int TNDDBase::NuclearStateNumber( char *Symbol,int A )
{
/*
mysql> DESCRIBE levels;
+-----------------+----------------------+------+-----+---------+-------+
| Field           | Type                 | Null | Key | Default | Extra |
+-----------------+----------------------+------+-----+---------+-------+
| Mass            | tinyint(3) unsigned  | YES  |     | NULL    |       | 
| Symbol          | char(2)              | YES  |     | NULL    |       | 
| Charge          | tinyint(3) unsigned  | YES  |     | NULL    |       | 
| Energy          | decimal(12,4)        | YES  |     | NULL    |       | 
| dEnergy         | decimal(12,4)        | YES  |     | NULL    |       | 
| Parity          | varchar(20)          | YES  |     | NULL    |       | 
| HalfLifeEnergy  | decimal(12,4)        | YES  |     | NULL    |       | 
| HalfLifeScale   | varchar(10)          | YES  |     | NULL    |       | 
| HalfLifedEnergy | decimal(12,4)        | YES  |     | NULL    |       | 
| Year            | smallint(5) unsigned | YES  |     | NULL    |       | 
+-----------------+----------------------+------+-----+---------+-------+
10 rows in set (0.18 sec)
*/

  unsigned long NoR = 0; // ?!!

  sprintf(cQuery,"SELECT DISTINCT Mass,Symbol,Energy,dEnergy FROM levels WHERE "
                 "Symbol='%s' AND Mass='%d' AND Parity!='' " // Parity - Ok?
                 "ORDER BY Energy \0",
                 Symbol,A);

//  sprintf(TString,"TNDDBase.NuclearStateNumber cQuery : %s",cQuery); TPrintf(TString);

  cRow = MySQLDB->Get((char*)"mass",cQuery,0);

//  sprintf(TString,"TNDDBase.NuclearStateNumber cRow : %s",cRow); TPrintf(TString);

  if( cRow != NULL ) NoR = MySQLDB->NumberOfRows(); 
  delete[] cRow; // bad... 
  
  *cQuery = '\0';

//  sprintf(TString,"TNDDBase.NuclearStateNumber : %ld",NoR); TPrintf(TString);


  return( (int)NoR ); // ?!!
}


char* TNDDBase::NuclearState( char *Symbol,int A )
{
  char *fRow;

  sprintf(cQuery,"SELECT DISTINCT Mass,Symbol,Energy,dEnergy,Parity FROM levels WHERE "
                 "Symbol='%s' AND Mass='%d' AND Parity!='' " // Parity - Ok?
                 "ORDER BY Energy \0",
                 Symbol,A);

  fRow = MySQLDB->Get((char*)"mass",cQuery,0);

//  sprintf(TString,"TNDDBase.NuclearState fRow : %s",fRow); TPrintf(TString);
  
  *cQuery = '\0';

//  sprintf(TString,"TNDDBase.NuclearState : %ld",NoR); TPrintf(TString);


  return( fRow );
}


int TNDDBase::NuclearPotentialNumber( int Zp,int Ap,int Ep,int Zt,int At,int Et )
{
  unsigned long NoR = 0;

/*
  sprintf(cQuery,"SELECT * FROM nucl_pot WHERE "
                 "PCharge=\'%d\' AND PMass=\'%d\' AND PEnergy=\'%f\' AND "
                 "TCharge=\'%d\' AND TMass=\'%d\' AND TEnergy=\'%f\'\0",
                 Zp,Ap,Ep,Zt,At,Et );
*/

  sprintf(cQuery,"SELECT * FROM nucl_pot WHERE "
                 "PCharge=\'%d\' AND PMass=\'%d\' AND "
                 "TCharge=\'%d\' AND TMass=\'%d\'\0",
                 Zp,Ap,Zt,At );


  cRow = MySQLDB->Get((char*)"mass",cQuery,0);

//  sprintf(TString,"TNDDBase.NuclearPotentialNumber cRow : %s",cRow); TPrintf(TString);

  if( cRow != NULL ) NoR = MySQLDB->NumberOfRows(); 
  delete[] cRow;
  
  *cQuery = '\0';

//  sprintf(TString,"TNDDBase.NuclearPotentialNumber : %ld",NoR); TPrintf(TString);


  return( (int)NoR );
}


char* TNDDBase::NuclearPotential( int Zp,int Ap,int Ep,int Zt,int At,int Et )
{
/*
| VR        | float               | YES  |     | NULL    |       | 
| rR        | float               | YES  |     | NULL    |       | 
| aR        | float               | YES  |     | NULL    |       | 
| WI        | float               | YES  |     | NULL    |       | 
| rI        | float               | YES  |     | NULL    |       | 
| aI        | float               | YES  |     | NULL    |       | 
| rC        | float               | YES  |     | NULL    |       | 
*/
  char *fRow;

  sprintf(cQuery,"SELECT VR,rR,aR,WI,rI,aI FROM nucl_pot WHERE "
                 "PCharge=\'%d\' AND PMass=\'%d\' AND "
                 "TCharge=\'%d\' AND TMass=\'%d\'\0",
                 Zp,Ap,Zt,At );

  fRow = MySQLDB->Get((char*)"mass",cQuery,0);

//  sprintf(TString,"TNDDBase.NuclearPotential fRow : %s",fRow); TPrintf(TString);
  
  *cQuery = '\0';

//  sprintf(TString,"TNDDBase.NuclearPotential : %ld",NoR); TPrintf(TString);


  return( fRow );
}


// Nuclear Deformation :
int TNDDBase::NuclearDeformationNumber( int Zp,int Ap,int Ep )
{
  unsigned long NoR = 0;

  sprintf(cQuery,"SELECT Def_1 FROM nucl_def WHERE "
                 "Charge=\'%d\' AND Mass=\'%d\' AND Energy=\'%lf\'",
                  Zp,Ap,Ep );

  cRow = MySQLDB->Get((char*)"mass",cQuery,0);
  if( cRow != NULL ) NoR = MySQLDB->NumberOfRows(); 

//  sprintf(TString,"TNDDBase.NuclearDeformationNumber cRow : %s",cRow); TPrintf(TString);

  delete[] cRow;
  
  *cQuery = '\0';

//  sprintf(TString,"TNDDBase.NuclearDeformationNumber : %ld",NoR); TPrintf(TString);

  return( (int)NoR );
}


char* TNDDBase::NuclearDeformation( int Zp,int Ap,int Ep )
{
/*
| Charge    | tinyint(3) unsigned | YES  |     | NULL    |       | 
| Mass      | tinyint(3) unsigned | YES  |     | NULL    |       | 
| Nucleus   | char(5)             | YES  |     | NULL    |       | 
| Energy    | decimal(10,6)       | YES  |     | NULL    |       | 
| Def_1     | decimal(7,3)        | YES  |     | NULL    |       | 
| Def_2     | decimal(7,3)        | YES  |     | NULL    |       | 
| Def_3     | decimal(7,3)        | YES  |     | NULL    |       | 
| Def_4     | decimal(7,3)        | YES  |     | NULL    |       | 
| Def_5     | decimal(7,3)        | YES  |     | NULL    |       | 
| Def_6     | decimal(7,3)        | YES  |     | NULL    |       | 
| Reference | varchar(100)        | YES  |     | NULL    |       | 
*/
  char *fRow;

  sprintf(cQuery,"SELECT Def_1,Def_2,Def_3,Def_4,Def_5,Def_6 FROM nucl_def WHERE "
                 "Charge=\'%d\' AND Mass=\'%d\' AND Energy=\'%lf\'",
                  Zp,Ap,Ep );

  fRow = MySQLDB->Get((char*)"mass",cQuery,0);

//  sprintf(TString,"TNDDBase.NuclearDeformation fRow : %s",fRow); TPrintf(TString);
  
  *cQuery = '\0';

  return( fRow );
}

// SpectroscopicFactor :
int TNDDBase::SpectroscopicFactorNumber( int Zcp,int Acp,double Ecp,int Zcr,int Acr,double Ecr,int Zcl,int Acl,double Ecl )
{
  unsigned long NoR = 0;

  sprintf(cQuery,"SELECT DISTINCT n,l,j,Sx FROM nucl_spc WHERE "
                 "Com_Charge=\'%d\' AND Com_Mass=\'%d\' AND Com_Energy=\'%6.3lf\' AND "
                 "Cor_Charge=\'%d\' AND Cor_Mass=\'%d\' AND Cor_Energy=\'%6.3lf\' AND "
                 "Cls_Charge=\'%d\' AND Cls_Mass=\'%d\' AND Cls_Energy=\'%6.3lf\'",
                  Zcp, Acp, Ecp, 
                  Zcr, Acr, Ecr, 
                  Zcl, Acl, Ecl );

  cRow = MySQLDB->Get((char*)"nucl_spc",cQuery,0);
  if( cRow != NULL ) NoR = MySQLDB->NumberOfRows(); 

//  sprintf(TString,"TNDDBase.SpectroscopicFactorNumber cRow : %s",cRow); TPrintf(TString);

  delete[] cRow;
  
  *cQuery = '\0';

//  sprintf(TString,"TNDDBase.SpectroscopicFactorNumber : %ld",NoR); TPrintf(TString);

  return( (int)NoR );
}


/*
mysql> DESCRIBE nucl_spc;
+------------+---------------------+------+-----+---------+-------+
| Field      | Type                | Null | Key | Default | Extra |
+------------+---------------------+------+-----+---------+-------+
| Com_Charge | tinyint(3) unsigned | YES  |     | NULL    |       | 
| Com_Mass   | tinyint(3) unsigned | YES  |     | NULL    |       | 
| Com_Symbol | char(5)             | YES  |     | NULL    |       | 
| Com_Energy | decimal(12,6)       | YES  |     | NULL    |       | 
| Cor_Charge | tinyint(3) unsigned | YES  |     | NULL    |       | 
| Cor_Mass   | tinyint(3) unsigned | YES  |     | NULL    |       | 
| Cor_Symbol | char(5)             | YES  |     | NULL    |       | 
| Cor_Energy | decimal(12,6)       | YES  |     | NULL    |       | 
| Cls_Charge | tinyint(3) unsigned | YES  |     | NULL    |       | 
| Cls_Mass   | tinyint(3) unsigned | YES  |     | NULL    |       | 
| Cls_Symbol | char(5)             | YES  |     | NULL    |       | 
| Cls_Energy | decimal(12,6)       | YES  |     | NULL    |       | 
| n          | tinyint(3) unsigned | YES  |     | NULL    |       | 
| l          | tinyint(3) unsigned | YES  |     | NULL    |       | 
| j          | float               | YES  |     | NULL    |       | 
| Sx         | float               | YES  |     | NULL    |       | 
| Reference  | varchar(200)        | YES  |     | NULL    |       | 
*/
char* TNDDBase::SpectroscopicFactor( int Zcp,int Acp,double Ecp,int Zcr,int Acr,double Ecr,int Zcl,int Acl,double Ecl )
{
  char *fRow;

  sprintf(cQuery,"SELECT DISTINCT n,l,j,Sx FROM nucl_spc WHERE "
                 "Com_Charge=\'%d\' AND Com_Mass=\'%d\' AND Com_Energy=\'%lf\' AND "
                 "Cor_Charge=\'%d\' AND Cor_Mass=\'%d\' AND Cor_Energy=\'%lf\' AND "
                 "Cls_Charge=\'%d\' AND Cls_Mass=\'%d\' AND Cls_Energy=\'%lf\' "
                 "ORDER BY Com_Energy,Cor_Energy,Cls_Energy",
                  Zcp, Acp, Ecp, 
                  Zcr, Acr, Ecr, 
                  Zcl, Acl, Ecl );

  fRow = MySQLDB->Get((char*)"mass",cQuery,0);

//  sprintf(TString,"TNDDBase.SpectroscopicFactor fRow : %s",fRow); TPrintf(TString);
  
  *cQuery = '\0';

  return( fRow );
}


/*

mysql> DESCRIBE nucl_pot;
+-----------+---------------------+------+-----+---------+-------+
| Field     | Type                | Null | Key | Default | Extra |
+-----------+---------------------+------+-----+---------+-------+
| PCharge   | tinyint(3) unsigned | YES  |     | NULL    |       | 
| PMass     | tinyint(3) unsigned | YES  |     | NULL    |       | 
| PSymbol   | char(5)             | YES  |     | NULL    |       | 
| PEnergy   | decimal(12,6)       | YES  |     | NULL    |       | 
| TCharge   | tinyint(3) unsigned | YES  |     | NULL    |       | 
| TMass     | tinyint(3) unsigned | YES  |     | NULL    |       | 
| TSymbol   | char(5)             | YES  |     | NULL    |       | 
| TEnergy   | decimal(12,6)       | YES  |     | NULL    |       | 
| Type      | char(5)             | YES  |     | NULL    |       | 
| Energy    | double              | YES  |     | NULL    |       | 
| VR        | float               | YES  |     | NULL    |       | 
| rR        | float               | YES  |     | NULL    |       | 
| aR        | float               | YES  |     | NULL    |       | 
| WI        | float               | YES  |     | NULL    |       | 
| rI        | float               | YES  |     | NULL    |       | 
| aI        | float               | YES  |     | NULL    |       | 
| rC        | float               | YES  |     | NULL    |       | 
| Reference | varchar(100)        | YES  |     | NULL    |       | 
+-----------+---------------------+------+-----+---------+-------+
18 rows in set (0.09 sec)


*/


/* 
double TNDDBase::(int pZ,int pA)
{
  double ;

  sprintf(cQuery,"SELECT  FROM mass WHERE Charge=\'%d\' AND AMass=\'%d\'\0",pZ,pA);
  cRow = MySQLDB->Get((char*)"mass",cQuery,0);
  *cQuery = '\0';

  return();
}
*/

/* 
mysql> DESCRIBE mass;
+---------------+----------------------+------+-----+---------+-------+
| Field         | Type                 | Null | Key | Default | Extra |
+---------------+----------------------+------+-----+---------+-------+
| ToDel1        | char(1)              | YES  |     | NULL    |       | 
| N_Z           | tinyint(4)           | YES  |     | NULL    |       | 
| Neutron       | tinyint(3) unsigned  | YES  |     | NULL    |       | 
| Charge        | tinyint(3) unsigned  | YES  |     | NULL    |       | 
| AMass         | smallint(5) unsigned | YES  |     | NULL    |       | 
| Symbol        | char(2)              | YES  |     | NULL    |       | 
| o_data        | char(4)              | YES  |     | NULL    |       | 
| Mass_Excess   | decimal(13,5)        | YES  |     | NULL    |       | 
| dMass_Excess  | decimal(11,5)        | YES  |     | NULL    |       | 
| Bind_Energy_A | decimal(11,3)        | YES  |     | NULL    |       | 
| dBnd_Energy_A | decimal(9,3)         | YES  |     | NULL    |       | 
| ToDel2        | char(2)              | YES  |     | NULL    |       | 
| BetaD_Energy  | decimal(11,3)        | YES  |     | NULL    |       | 
| dBetaD_Energy | decimal(9,3)         | YES  |     | NULL    |       | 
| ToDel3        | char(3)              | YES  |     | NULL    |       | 
| Atomic_Mass   | decimal(12,5)        | YES  |     | NULL    |       | 
| dAtomic_Mass  | decimal(12,6)        | YES  |     | NULL    |       | 
+---------------+----------------------+------+-----+---------+-------+

*/




//------------------------------------------------------------------------------

