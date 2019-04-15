//------------------------------------------------------------------------------
// TCRCScheme.h
//------------------------------------------------------------------------------
// TypeC(oupled)R(eaction)C(hannel)Scheme
// C.: 2005.10.13.
// M.: 2015.05.15.
//------------------------------------------------------------------------------

#ifndef __TCRCScheme_H
#define __TCRCScheme_H

#include "TNReaction.h"
#include "TRChannel.h"
#include "TTIFile.h"
#include "TFIFile/TFFile.h"

class TCRCScheme{
private:
  TNReaction *Reaction;

  TRChannel *RChannel; // R(eaction) Channels
  char *cIFName;

  int TR;

public:
  TCRCScheme();
  ~TCRCScheme();

//  void Set();
  void Set( char *ifn );

  void Write(FILE *f); // <- should make it private?

private:
  void Set( FILE *ifl ); // dummy function!!!
  void Read(FILE *f);
  void Generate(int fNoC);
  void CreateFIFile( char *ifn );
//
  void ReadNew( FILE *f );
  
  void WriteTest( char *str );

};

#endif // __TCRCScheme_H
//------------------------------------------------------------------------------

