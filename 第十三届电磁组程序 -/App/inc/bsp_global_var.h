#ifndef _bsp_global_var_
#define _bsp_global_var_

#include "common.h"

/******************小车调整参数结构体***********/
struct PARAM
{
  int16 int01;
  int16 int02;
  int16 int03;
  int16 int04;
  int16 int05;
  int16 int06;
  int16 int07;
  int16 int08;
  int16 int09;
  int16 int10;
  int16 int11;
  int16 int12;
  
  int16 float01;
  int16 float02;
  int16 float03;
  int16 float04;
  int16 float05;
  int16 float06;
  int16 float07;
  int16 float08;
  int16 float09;
  int16 float10;
  int16 float11;
  int16 float12;  
  
  int16 param_flag;
};

/******************小车调整参数结构体***********/  
extern struct PARAM parameter;

extern int16 int01;
extern int16 int02;
extern int16 int03;
extern int16 int04;
extern int16 int05;
extern int16 int06;
extern int16 int07;
extern int16 int08;
extern int16 int09;
extern int16 int10;
extern int16 int11;
extern int16 int12;

extern float float01;
extern float float02;
extern float float03;
extern float float04;
extern float float05;
extern float float06;
extern float float07;
extern float float08;
extern float float09;
extern float float10;
extern float float11;
extern float float12;


#endif /*_bsp_global_var_*/