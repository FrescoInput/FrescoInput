//------------------------------------------------------------------------------
// TNReaction.h
//------------------------------------------------------------------------------
// T(ype)N(uclear)R(eaction)
// C.: 2002.12.18.
// M.: 2015.05.15.
//------------------------------------------------------------------------------

#ifndef __TREACTIO_H
#define __TREACTIO_H 1

#include "TRChannel.h"
#include "T1BNSystem.h"
#include "T2BNSystem.h"
//------
#include "TFIFile/TFFile.h"

class TNReaction{
private:
//  T2BNSystem *GEChannel; // classEntrance Channel or global?
//  T2BNSystem *GOChannel; // classOutgoing Channel
//  int GSN; // classSystemNumber
//  T2BNSystem *cRChannel; // Reaction Channel, 0 - entrance

  T1BNSystem *cCluster;

  float cQValue;
  int *cPI; // Potential Index
  int *cSI; // Spectroscopic Index

  T1BNSystem *Projectile;
  T1BNSystem *Target;
  double Energy;
  char *RString;
  int TZ; // T(otal) Z
  int TN; // T(otal) N
  int NC; // N(umber of)C(hannel)
  char *ICString; // I(n)C(hannel)String
  char *BasePath;

  char *cLogName;
  FILE *cLogFile;

public:
  TNReaction();
  ~TNReaction();

  void Set(char *fPath);
  void SetBasePath(char *s);
  int SetInChannel(char *p,char *t,double e);
/*
  int SetEntranceChannel();
  int SetOutgoingChannel();
  int SetTransferSystems();
*/
  int SetInChannel( TRChannel *RChannel); // Entrance
// new : 
  int SetOutChannel(TRChannel *RChannel,int cn)
      { SetOutChannel( RChannel,RChannel,cn); } // Outgoing
  int SetTRChannel( TRChannel *RChannel,int cn)
      { SetTRChannel(  RChannel,RChannel,cn); } // Transfer reactions
//


  int SetOutReaction(char *s); // for one elements for example B :)
  void GetOutChannel(char *s){strcpy(s,RString);} // Get RString only !!!
//  void Kinematics(char *s,float *r); // s - path, r(ange)

//  int GetNumberChannel(); // all
//  int GetNumberChannel(char *n);
//  void GetOutReaction(char *s); // all exist channel?!!
//  void GetOutReaction(char *s,char *n);

private:

  int OneStepTransfer(char *ep,char *op); // 0 if impossible
  int OneStepTransfer(TRChannel *EChannel,TRChannel *OChannel); // 0 if impossible

  void Initialize();
  int SetOutReaction(); // all channel, return number of out channel
  void LevelInitialise(float *l,int n);
  void KinematicsInFile(FILE *oFile,char *RS,float *R,int n3,float *L3,int n4,float *L4);
  void ToLevels(char *FileName,int A1,int A3);
  void CrntStrn(char *n,int v); // for ToLevels()
//  int SetOutReaction(char *s); // for one elements for example B :)

  void CheckRCI();
  void CheckCBE();
  void SortBE();

// to delete?

  void Set(TRChannel *RChannel,int TR);
  void Generate(TRChannel *RChannel,int NC);

  int SetOutChannel(TRChannel *IChannel,TRChannel *RChannel,int cn); // Outgoing
  int SetTRChannel( TRChannel *IChannel,TRChannel *RChannel,int cn); // Transfer reactions


  void Write(TRChannel *IChannel,int TR,char *str);
};

#endif // __TREACTIO_H
//------------------------------------------------------------------------------

