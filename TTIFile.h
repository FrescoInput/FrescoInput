//------------------------------------------------------------------------------
// TTIFile.h
//------------------------------------------------------------------------------
// TypeT(ext)I(nput)File
// C.: 2008.02.28.
// M.: 2015.05.15.
//------------------------------------------------------------------------------

#ifndef __TTIFile_H
#define __TTIFile_H

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// for test
extern void TPrintf(char *s);
extern char *TString;
// for test

class TTIFile{
private:
  FILE *ifl;
  
//*/
  struct cDRow{
    char p[5];
    double pe;
    char t[5];
    double te;
    double e;
  };
  struct cDRow *cFileDataRow;
  int cDRN;
  
  struct cCD{
    int First;
    int PNumber;
    int TNumber;
  };
  struct cCD *cCData;
  int cCDN;
//*/  

public:
  TTIFile();
  ~TTIFile();

  void Set(FILE *fl);
  int Read(FILE *fl);
  int Write(FILE *fl);

  long GetPos(){ return( ftell(ifl) ); }
  long SetPos(long pos){ return( fseek(ifl,pos,0) ); }

  int FileDataRead(char *str,int max);
  int StrnGetKind(char *kind,char *str);
  int StrnGetData(char *str,char *ds);

// new :
  int DataRowNumber( void ){ return( cDRN ); }
  int ChannelNumber( void ){ return( cCDN ); }
//
  float Energy(){ return( cFileDataRow[0].e ); }
  char *Projectile( int N );
  int ProjectileStateNumber( int N );
  double ProjectileState( int N,int i );
  char *Target( int N );
  int TargetStateNumber( int N );
  double TargetState( int N,int i );
  
  
private:
  int FileRowRead(char *str,int max);
  int StrSpaceDel(char *str);
  int StrComntDel(char *str);
  
// new:
  int DataRowNumber( FILE *f );
  void FileRead( FILE *fl );
  char* Next( char *s,char *e );
  void ChannelData();
  
  
};
#endif // __TTIFile_H
