#pragma once
#ifndef DEMOD_CORE_H
#define DEMOD_CORE_H
// don not edit this file!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// don not edit this file!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// don not edit this file!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void demod_core(double data_in[], unsigned int len_in, double data_out[], unsigned int *len_out);
void demodAM(int Fs, double data_in[], unsigned int len_in, double data_out[], unsigned int &len_out);
void aptToImage(int Fs, double *data, unsigned int len_in, unsigned char *&img, unsigned int &imgSize, unsigned int &width);

// don not edit this file!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// don not edit this file!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// don not edit this file!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#endif // !DEMOD_CORE_H
