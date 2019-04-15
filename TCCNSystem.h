//------------------------------------------------------------------------------
// TCCNSystem.h
//------------------------------------------------------------------------------
// T(ype)C(luster)C(ore)N(uclear)S(ystem)
// C.: 2008.03.20.
// M.: 2015.03.dd.
//------------------------------------------------------------------------------

#ifndef __TCCNSystem_H
#define __TCCNSystem_H

#include "TRChannel.h"
#include "T1BNSystem.h"
//#include "T2BNSystem.h"

class TCCNSystem{
private:
  T1BNSystem *cCore;
  T1BNSystem *cCluster;
  T1BNSystem *cCompound;

public:
  TCCNSystem();
  ~TCCNSystem();

  void Set();
  void Set(TRChannel *EChannel,TRChannel *OChannel,char *s);
//  void Set(T2BNSystem *EChannel,T2BNSystem *TChannel);

  void WriteDB(FILE *ofl);
};

#endif // __TCCNSystem_H
//------------------------------------------------------------------------------

