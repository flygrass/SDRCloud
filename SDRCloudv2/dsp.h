#pragma once
#ifndef DSP_H
#define DSP_H
#include "math.h"


void s_interp1(double *x, const unsigned int len1, double *y, const unsigned int len2);
void resample(double *data_in, const unsigned int len_in, const int sampleRate1, const int sampleRate2, double *&data_out, unsigned int &len_out);
#endif // !DSP_H
