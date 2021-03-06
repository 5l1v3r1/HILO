/*
 * util_ss.h - Math helper fuctions for SS
 *
 * Code taken from QD 2.3.11, Copyright (c) 2000-2001 DoE
 */

#ifndef _UTIL_SS_H
#define _UTIL_SS_H

#include "ss.h"

/* ss */

/* Computes fl(a+b) and err(a+b).  Assumes |a| >= |b|. */
inline ss ss_quick_two_sum(float a, float b){
  float s;
  s = a + b;
  return num_num_to_ss(s, b - (s - a));
}

/* Computes fl(a-b) and err(a-b).  Assumes |a| >= |b| */
inline ss ss_quick_two_diff(float a, float b){
  float s;
  s = a - b;
  return num_num_to_ss(s, (a - s) - b);
}

/* Computes fl(a+b) and err(a+b).  */
inline ss ss_two_sum(float a, float b){
  ss retval;
  float bb;

  retval.num = a + b;
  bb = retval.num - a;
  retval.err = (a - (retval.num - bb)) + (b - bb);
  
  return retval;
}

/* Computes fl(a-b) and err(a-b).  */
inline ss ss_two_diff(float a, float b){
  ss retval;
  float bb;

  retval.num = a - b;
  bb = retval.num - a;
  retval.err = (a - (retval.num - bb)) - (b + bb);
  
  return retval;
}

#define _SS_SPLITTER 9.0                      // = 2^3 + 1
#define _SS_SPLIT_THRESH 2.12676479325587e+37 // = 2^124

/* Computes high word and lo word of a        */
/* uses ss.num as hi word, ss.err as low word */
inline ss ss_split(double a){
  ss retval;
  double temp;

  if (a > _SS_SPLIT_THRESH || a < -_SS_SPLIT_THRESH){
    a *= 0.0625;                 // 2^-4
    temp = _SS_SPLITTER * a;
    retval.num = temp - (temp - a);
    retval.err = a - retval.num;
    retval.num *= 16.0;          // 2^4
    retval.err *= 16.0;          // 2^4
  } else {
    temp = _SS_SPLITTER * a;
    retval.num = temp - (temp - a);
    retval.err = a - retval.num;
  }

  return retval;
}

/* Computes fl(a*b) and err(a*b). */
inline ss ss_two_prod(float a, float b){
  ss retval, split_a, split_b;

  retval.num = a * b;
  split_a = ss_split(a);
  split_b = ss_split(b);
  retval.err = ((split_a.num * split_b.num - retval.num) + split_a.num * split_b.err + split_a.err * split_b.num) + split_a.err * split_b.err;

  return retval;
}

#endif /* _UTIL_SS_H */
