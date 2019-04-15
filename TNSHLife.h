//------------------------------------------------------------------------------
// TNSHLife.h
//------------------------------------------------------------------------------
// T(ype)N(ucleus)S(tate)H(alf)Life
// C.: 2006.06.06.
// M.: 2015.05.15.
//------------------------------------------------------------------------------

#ifndef __TNSHLIFE_H
#define __TNSHLIFE_H 1

class TNSHLife{
private:
  int data; // 0 - no data, 1 - there is data;
  double HLife,dHLife;
  char *Scale;

public:
  TNSHLife();
  ~TNSHLife();

  int ReadHLife(char *s); // read spin information from string
  int GetHLifeData(){ return( data ); }
  double GetHLife(){ return( HLife ); }

private:
  int ReadValue(char *s,char *d);
  int CountDigitNumber(char *s,char *c);
  float GetValue(char *s);
};

#endif // __TNSHLIFE_H
//------------------------------------------------------------------------------

