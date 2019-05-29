#include "dsp.h"
#include <QDebug>


void s_interp1(double *x, const unsigned int len1, double *y, const unsigned int len2)
{
	if (len1 < len2) {
		return;
	}
	qDebug() << "interp==============";
	qDebug() << "len1 = " << len1 << "len2=" << len2;
	double gap = double(len1) / double(len2);
	unsigned int gapn = ceil(double(len1) / double(len2));
	//qDebug() << "=========================";
	//qDebug() << floor(3.6) << ";" << ceil(3.1) << ";" << round(3.2) << ";" << round(3.7);

	qDebug() << "gapn=" << gapn;
	unsigned int *index = (unsigned int *)new int[len2];

	for (int i = 0; i < len2; i++) {
		index[i] = round(i*gap);
	}

	qDebug() << "index[18]=" << index[18];
	int j = 0;
	double temp = 0;

	for (int i = 0; i < len2; i++) {
		temp = 0;
		for (j = index[i]; j < (index[i] + gapn); j++) {
			if (j < len1) {
				temp += x[j];
			}
			else {
				temp += x[len1 - 1];
			}

		}
		temp /= gapn;
		y[i] = temp;
		//y[i] = 0;
	}
	delete index;
	index = nullptr;
}

// �����ݴ�sampleRate1�ز�����sampleRate2��data_out����ҪԤ�ȷ���ռ�
void resample(double *data_in, const unsigned int len_in, const int sampleRate1, const int sampleRate2, double *&data_out, unsigned int &len_out)
{
	//// ��ʱ�������򵥴���
	//data_out = data_in;
	//len_out = len_in;

	int gcd = 0;
	int L = 0;	// ��ֵ����
	int M = 0;	// ��ȡ����

	// �����ز���
	if (sampleRate1 == 48000) {
		// sampleRate2�ڱ�������Ŀǰ���̶�Ϊ12480Hz
		gcd = 960;
	}
	else if (sampleRate1 == 11025) {
		gcd = 15;
	}
	else {
		len_out = 0;
		qDebug() << "Now, the inputsampleRate" << sampleRate1 << "is not surpported";
	}

	M = sampleRate1 / gcd;
	L = sampleRate2 / gcd;
	double coeff[2];
	quint32 coeff_len;
	//make_filter(filter, coeff, coeff_len);

	// �����ز����㷨����
	quint32 interpolated_len = len_in * L;
	len_out = interpolated_len / M;	// = len_in*L/M=gcd*M*L/M=gcd*L,��������϶�������

	data_out = new double[len_out];
	quint32 t = 0;	// ����ʱ�˲�����β,��������β�������ң���Ϊβ) 
	quint32 h = 0;	// �����ͷ����
	int rem = 0;
	quint32 i = 0;
	quint32 j = 0;
	quint32 ii = 0;
	double sum = 0;
	for (t = 0; t <= interpolated_len; t += M) {	//���M���ĳ�ȡ
		if (t - coeff_len > 0) {	// t-h+1=coeff_len => h>1	// �����ڲ壬h������L��������
			h = t - coeff_len;
			rem = h % L;
			if (rem != 0) {
				h += L - rem;
			}
		}
		else {
			h = 0;
		}
		sum = 0;
		ii = h / L;
		for (i = h; i < t; i += L,ii++) {	// ii = i/L;
			sum += data_in[ii] * coeff[i-h];
		}
		data_out[j++] = sum;	// t = jM;
	}
	len_out = j;
}