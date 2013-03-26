#ifdef SWIG
%module LibSample
%{
#include "libsample.h"
%}
#endif

#ifndef _LIBSAMPLE_H_
#define _LIBSAMPLE_H_

extern int smpl_sum(int a, int b);

#endif
