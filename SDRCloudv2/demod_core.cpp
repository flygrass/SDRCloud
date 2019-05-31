#include "demod_core.h"
#include "constant.h"
#include "math.h"
#include "aptformat.h"
#include <QDebug>
#include <QQueue>

//#include <spline3interp.h>  
//using namespace splab;

// here you can build and add Header files your self if you need.


// demod_core: do what you want
// data_in: the array data is provided by RTL USB
// len_in: tell you the length of data input
// data_out: the data you want to display on screen or play in your soundspeaker
// len_out: the length of data_out.
// Note: Be friendly to your computer, please keep data_out  is not a empty one!
// m_unDemodSignal.count() = len_in
// result1.enqueue(1.2);


void demod_core(double data_in[], unsigned int len_in, double data_out[], unsigned int *len_out)
{
	int i = 0;
	for (i = 0; i < len_in/2; i++)
	{
		// example 1:
		data_out[i] = 127.0*cos(2 * 3.14*0.003*i);


		// example 2:
		/*data_out[i] = cos(2 * 3.14*0.003*i) + data_in[i];*/

	}
	*len_out = i;
	return;
}

// ���ư������㷨,Ϊ�˽�ʡ�ڴ�,data_in��data_out����ָ��ͬһƬ�ڴ沢��data_in���ᱻ����
void demodAM(int Fs, double data_in[], unsigned int len_in, double data_out[], unsigned int & len_out)
{
	double phi = 2 * pi*Fc / double(Fs);
	double cosphi2 = 2*cos(phi);
	double sinphi = sin(phi);

	
	double xi_pre = data_in[0];
	double xi2_pre = xi_pre*xi_pre;
	
	double xi = 0.0;
	double xi2 = 0.0;
	unsigned int i = 0;
	for (i = 1; i < len_in; i++) {
		xi = data_in[i];
		xi2 = xi * xi;
		data_out[i - 1] = sqrt(xi2 + xi2_pre - cosphi2 * xi*xi_pre) / sinphi;
		xi_pre = xi;
		xi2_pre = xi2;
	}
	data_out[i - 1] = data_out[i - 2];
	len_out = i;
}


void linspace(double *v, const double startNum, const double endNum, const unsigned int N)
{
	if (N <= 0) {
		return;
	}
	else if (N == 1) {
		v[0] = startNum;

	}
	else {
		double gap = (endNum - startNum) / (N - 1);
		for (int i = 0; i < N; i++) {
			v[i] = startNum + i * gap;
		}
	}
}

void resample_fast(QQueue<qreal> &x, const quint32 len_in, const int L, 
	const int M, qreal *coeff, const quint32 coeff_len, qreal *y, quint32 &len_out)
{
	// ���������źŵĲ�����Ϊinput_rate����˺����Ὣ�źŵĲ������ز�����input_rate*L/M
	
	// ��������ͬ����Ҫ���汾��δ��������x�е������㵽��һ��
	// todo
	//static QQueue<qreal> pre;
	//QQueue<qreal> x;
	// t����ǰ����λ��
	qint64 t = 0;
	qint64 h = 0;
	qint32 rem = 0;
	qint64 i = 0;
	qint64 j = 0;
	qint64 ii = 0;
	qreal sum = 0;
	//int input_len = 0;
	quint32 interpolated_len = len_in * L;	// ��ȷ���������������2^32

	//if (pre.isEmpty() == false) {
	//	x = pre + signal;

	//}

	for (t = ceil(coeff_len/double(M))*M; t <= interpolated_len; t = t + M) {	// tҪ��M��������Ϊ���Ӧ�˼��ΪM�ĳ�ȡ
		// ���Ⱦ��ʱ�˲�����ͷλ��,����˲�����βλ��Ϊt
		if (t - coeff_len > 0) {
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
		// ����[h,t)����ΪL�ĳ˻���
		for (i = h; i < t; i = i + L,ii++) {
			sum += x[ii] * coeff[i - h];
		}
		y[j++] = sum;	// t = j*M
		//Q_ASSERT(sum != 0);
		//y[j++] = cos(2 * 3.14*500.0*j / 48000.0);
	}
	len_out = j;

	// ���汾��δ��������x��ʣ��������㵽��һ��
	// ��1
	//if (t - coeff_len > 0) {
	//	h = t - coeff_len;
	//	rem = h % L;
	//	if (rem != 0) {
	//		h += L - rem;
	//	}
	//}
	//else {
	//	h = 0;
	//}
	//ii = h / L;
	//for (i = ii; i < input_len; i++) {
	//	pre.enqueue(x[i]);
	//}

	// ��2ֱ�ӽ�ʣ������ݱ����ڶ���x�ֱ����һ�������������
	if (t - coeff_len > 0) {
		h = t - coeff_len;
		rem = h % L;
		if (rem != 0) {
			h += L - rem;
		}
	}
	else {
		h = 0;
	}
	ii = h / L;
	for (i = 0; i < len_in && i < ii; i++) {	// ����֮���Ի�Ҫ�������i<len_in����Ϊii�п��ܴ���len_in����Ϊ������h+=L-rem
		x.dequeue();
	}
}
