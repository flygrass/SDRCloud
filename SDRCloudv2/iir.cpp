#include "iir.h"
#include <qmath.h>
#include "fir_2048k_256k.h"
#include "fir_2048k_128k.h"
#include <QDebug>



const int IIR_A_LEN = 3;
const int IIR_B_LEN = 3;
// 50Hz�ݲ��˲���
//const double IIR_A[] = { 1, -1.99884767026, 0.9988904831675 };
//const double IIR_B[] = { 0.9994452415838, -1.99884767026, 0.9994452415838 };

const double IIR_A[] = { 1,  -0.3075663597922,    0.188345160884 };
const double IIR_B[] = { 0.220194700273,   0.4403894005459,    0.220194700273 };

//********************* FM || APT �źŽ��ղ���switch***************************\\

//======================= ��Ƶ�㲥����============================================
//const int FIR_LEN = FIR_2048K_TO_256K_LEN;
//qreal *coeff = (qreal *)FIR_2048K_TO_256K;
//int D = 8;	// �²�������2048K->256K

//========================APT�źŽ���=============================================
int D = 16;	// �²�������2048K->128K
const int FIR_LEN = FIR_2048K_TO_128K_LEN;
qreal *coeff = (qreal *)FIR_2048K_TO_128K;


//****************************************************************************//

#define signalI(i) ((2*i+last < 2*FIR_LEN) ? pre[2*i+last]:(qint16(data_in[2*(i-FIR_LEN)+last])-128))
#define signalQ(i) ((2 * i+1+last < 2*FIR_LEN) ? pre[2 * i+1+last] : (qint16(data_in[2*(i-FIR_LEN)+1+last])-128))
#define len_I ceil((len_pre+len_in)/2.0)
#define len_Q (len_pre+len_in)/2

// ����ȡsignalQ(i)�����ڲ�ȷ����
// FM��===========================================================================================================
void downsampleWithFir(const unsigned char *data_in, const quint32 len_in, qreal data_out[], quint32 &len_out)
{
	// ������źŽ���D�����²�������ͬʱʹ�ÿ�����˲���
	// ʹ�þ���Ϊint16��FIR�˲�����-128->127��Χ�ڵ��źŽ����˲����������ֶ�Ӧ�����ֵ��
	// ������ֵ�;����FIR�˲���ϵ��
	// ���˻�������double�͵�ϵ����

	static qreal pre[FIR_LEN*2];	// ����Ҫ����2*FIR_LEN������
	static quint32 len_pre = 0;	// ��һ��ʣ�����ݵĳ���
	static quint32 last = 2*FIR_LEN;	// ������ס�źŵĿ�ͷ
	//static int I = 0;
	//static int Q = 0;	// I�� Q�ź��˲����㵱ǰ����ʼλ��
	quint32 leni = len_I;
	quint32 lenq = len_Q;

	qint64 i = 0;
	qint64 jI = 0;
	qint64 jQ = 0;
	quint32 r = 0;
	quint32 s = 0;
	qreal sum = 0;

	//qDebug() << "downsampling with filter:" << len_in;
	//qDebug() << "len_pre=" << len_pre;
	//qDebug() << "leni=" << leni;
	//qDebug() << "lenq=" << lenq;
	// ����len_preΪ0
	len_pre = 0;

	// �ȶ�I·�źſ�����˲�������²���
	qint64 h = 0;	// Ϊ�����͵�ͷλ��
	qint64 t = 0;	// Ϊ��ǰ����λ�ã�Ҳ�������͵�β
	for (t = ceil(FIR_LEN / double(D))*D; t < leni; t = t + D) {
		if (t > FIR_LEN) {					//signal:	x(0) x(1) x(2) x(3)...
			// ����˲�����β���Ѿ����뵽�ź���//coeff:			 h(0) h(1) h(2)...
			h = t - FIR_LEN;
		}
		else {								//signal:		 x(0) x(1) x(2) x(3) ...
			// �˲���β����û�н��뵽�ź���	//coeff:	h(0) h(1) h(2) h(3)...
			h = 0;
		}

		sum = 0;
		for (i = h; i < t; i++) {	// ������
			sum += signalI(i)*coeff[i-h];
		}
		data_out[r] = sum;
		r = r + 2;
	}


	h = t - FIR_LEN;
	if (h < leni && h >= 0) {
		// data_in����ʣ���I·������Ҫ���Ƶ�pre�л���
		//if (len_in % 2 == 0) {	// ˵��ĩβ����I·�ź�

		//}
		jI = 2*FIR_LEN - 2;	 // ��Ϊ�������ĳ�����2*FIR_LEN
		for (i = leni - 1; i >= h; i--) {
			pre[jI] = signalI(i);
			jI = jI - 2;
			len_pre++;
		}
	}
	
	// �ٶ�Q·�źſ�����˲�������²���
	s = 1;
	for (t = ceil(FIR_LEN/double(D))*D; t < lenq; t = t + D) {
		if (t > FIR_LEN) {					//signal:	x(0) x(1) x(2) x(3)...
			// ����˲�����β���Ѿ����뵽�ź���//coeff:			 h(0) h(1) h(2)...
			h = t - FIR_LEN;
		}
		else {								//signal:		 x(0) x(1) x(2) x(3) ...
			// �˲���β����û�н��뵽�ź���	//coeff:	h(0) h(1) h(2) h(3)...
			h = 0;
		}

		sum = 0;
		for (i = h; i < t; i++) {	// ������
			sum += signalQ(i)*coeff[i - h];
		}
		data_out[s] = sum;
		s = s + 2;
	}

	h = t - FIR_LEN;
	if (h < lenq && h >= 0) {
		// data_in����ʣ���Q·������Ҫ���Ƶ�pre�л���
		//if (len_in % 2 == 0) {	// ˵��ĩβ����I·�ź�

		//}
		jQ = 2*FIR_LEN - 1;
		for (i = lenq - 1; i >= h; i--) {
			pre[jQ] = signalQ(i);
			jQ = jQ - 2;
			len_pre++;
		}
	}
	if (jI > jQ) {
		// ���Ǳ�֤��һ����I·��
		pre[jI] = pre[jI + 2];
		jI = jI - 2;
		len_pre++;
	}
	Q_ASSERT(r - s == 1 || r - s == -1);
	Q_ASSERT(jI < jQ);
	len_out = r > s ? s : r;	// len_out = min(r,s)
	//qDebug() << "len_out=" << len_out;
	last = jQ + 1;	// ��¼pre�����ݵĿ�ʼλ��
	Q_ASSERT(len_pre == 2 * FIR_LEN - last);
	
	
	
}
//============================================================================================================

//=========================================APT��==============================================================
//const int FIR_LEN_APT = FIR_2048K_TO_128K_LEN;
//qreal *coeff_APT = (qreal *)FIR_2048K_TO_128K;
//#define signalI_APT(i) ((2*i+last < 2*FIR_LEN) ? pre[2*i+last]:(qint16(data_in[2*i-2*FIR_LEN])-128))
//#define signalQ(i) ((2 * i+1+last < 2*FIR_LEN) ? pre[2 * i+1+last] : (qint16(data_in[2*i-2*FIR_LEN+1]-128)))
//#define len_I ceil((len_pre+len_in)/2.0)
//#define len_Q (len_pre+len_in)/2
//
//void downsampleWithFir_APT(const unsigned char *data_in, const quint32 len_in, qreal data_out[], quint32 &len_out)
//{
//	// ������źŽ���D�����²�������ͬʱʹ�ÿ�����˲���
//	// ʹ�þ���Ϊint16��FIR�˲�����-128->127��Χ�ڵ��źŽ����˲����������ֶ�Ӧ�����ֵ��
//	// ������ֵ�;����FIR�˲���ϵ��
//	// ���˻�������double�͵�ϵ����
//
//	int D = 16;	// �²�������2048K->256K
//	static qreal pre[FIR_LEN * 2];	// ����Ҫ����2*FIR_LEN������
//	static int len_pre = 0;
//	static int last = 2 * FIR_LEN;	// ������ס�źŵĿ�ͷ
//	//static int I = 0;
//	//static int Q = 0;	// I�� Q�ź��˲����㵱ǰ����ʼλ��
//	quint32 leni = len_I;
//	quint32 lenq = len_Q;
//
//	int i = 0;
//	int jI = 0;
//	int jQ = 0;
//	int r = 0;
//	int s = 0;
//	qreal sum = 0;
//
//	//qDebug() << "downsampling with filter:" << len_in;
//
//	// �ȶ�I·�źſ�����˲�������²���
//	int h = 0;	// Ϊ�����͵�ͷλ��
//	int t = 0;	// Ϊ��ǰ����λ�ã�Ҳ�������͵�β
//	for (t = 0; t < leni; t = t + D) {
//		if (t - FIR_LEN > 0) {					//signal:	x(0) x(1) x(2) x(3)...
//			// ����˲�����β���Ѿ����뵽�ź���//coeff:			 h(0) h(1) h(2)...
//			h = t - FIR_LEN;
//		}
//		else {								//signal:		 x(0) x(1) x(2) x(3) ...
//			// �˲���β����û�н��뵽�ź���	//coeff:	h(0) h(1) h(2) h(3)...
//			h = 0;
//		}
//
//		sum = 0;
//		for (i = h; i < t; i++) {	// ������
//			sum += signalI(i)*coeff[i - h];
//		}
//		data_out[r] = sum;
//		r = r + 2;
//	}
//
//	h = t - FIR_LEN;
//	if (h < leni && h >= 0) {
//		// data_in����ʣ���I·������Ҫ���Ƶ�pre�л���
//		//if (len_in % 2 == 0) {	// ˵��ĩβ����I·�ź�
//
//		//}
//		jI = 2*FIR_LEN - 2;
//		for (i = leni - 1; i >= h; i--) {
//			pre[jI] = signalI(i);
//			jI = jI - 2;
//		}
//	}
//
//	// �ٶ�Q·�źſ�����˲�������²���
//	s = 1;
//	for (t = 0; t < lenq; t = t + D) {
//		if (t - FIR_LEN > 0) {					//signal:	x(0) x(1) x(2) x(3)...
//			// ����˲�����β���Ѿ����뵽�ź���//coeff:			 h(0) h(1) h(2)...
//			h = t - FIR_LEN;
//		}
//		else {								//signal:		 x(0) x(1) x(2) x(3) ...
//			// �˲���β����û�н��뵽�ź���	//coeff:	h(0) h(1) h(2) h(3)...
//			h = 0;
//		}
//
//		sum = 0;
//		for (i = h; i < t; i++) {	// ������
//			sum += signalQ(i)*coeff[i - h];
//		}
//		data_out[s] = sum;
//		s = s + 2;
//	}
//
//	h = t - FIR_LEN;
//	if (h < leni && h >= 0) {
//		// data_in����ʣ���Q·������Ҫ���Ƶ�pre�л���
//		//if (len_in % 2 == 0) {	// ˵��ĩβ����I·�ź�
//
//		//}
//		jQ = 2*FIR_LEN - 1;
//		for (i = leni - 1; i >= h; i--) {
//			pre[jI] = signalQ(i);
//			jQ = jQ - 2;
//		}
//	}
//	Q_ASSERT(r - s == 1 || r - s == -1);
//	Q_ASSERT(jI - jQ == 1 || jI - jQ == -1);
//	len_out = r > s ? s : r;	// len_out = min(r,s)
//	last = ((jI > jQ ? jI : jQ) - 1);	// ��¼pre�����ݵĿ�ʼλ��
//}
void iir_downsample(const unsigned char *data_in, const quint32 len_in, qreal data_out[], quint32 &len_out)
{
	static const qreal IIR_A[3] = { 1,   -1.895957017836,    0.901108152734 };
	static const qreal IIR_B[3] = { 0.001287783724582, 0.002575567449164, 0.001287783724582 };
	static qreal ix_n_1 = 0;
	static qreal ix_n_2 = 0;
	static qreal iy_n_1 = 0;
	static qreal iy_n_2 = 0;

	qreal ix_n = 0;
	qreal iy_n = 0;
	// y(n) = 
	int i = 0;
	int count = len_in;
	for (i = 0; i < count; i=i+2)
	{
		ix_n = qreal(qint16(data_in[i])-128.0);
		iy_n = IIR_B[0] * ix_n + IIR_B[1] * ix_n_1 + IIR_B[2] * ix_n_2 - IIR_A[1] * iy_n_1 - IIR_A[2] * iy_n_2;

		iy_n_2 = iy_n_1;
		iy_n_1 = iy_n;

		ix_n_2 = ix_n_1;
		ix_n_1 = ix_n;

		data_out[i] = iy_n;
		//if (qAbs(data_out[i]) < 5)
		//{
		//	data_out[i] = 0.0;
		//}

	}

	static qreal qx_n_1 = 0;
	static qreal qx_n_2 = 0;
	static qreal qy_n_1 = 0;
	static qreal qy_n_2 = 0;

	qreal qx_n = 0;
	qreal qy_n = 0;
	for (i = 1; i < count; i = i + 2)
	{
		qx_n = qreal(qint16(data_in[i]) - 128.0);
		qy_n = IIR_B[0] * qx_n + IIR_B[1] * qx_n_1 + IIR_B[2] * qx_n_2 - IIR_A[1] * qy_n_1 - IIR_A[2] * qy_n_2;

		qy_n_2 = qy_n_1;
		qy_n_1 = qy_n;

		qx_n_2 = qx_n_1;
		qx_n_1 = qx_n;

		data_out[i] = qy_n;
		//if (qAbs(data_out[i]) < 5)
		//{
		//	data_out[i] = 0.0;
		//}
	}
	len_out = len_in;
}

void iir(QQueue<qreal> &data_in, qreal data_out[], quint32 &len)
{
	static qreal x_n_1 = 0;
	static qreal x_n_2 = 0;
	static qreal y_n_1 = 0;
	static qreal y_n_2 = 0;

	qreal x_n = 0;
	qreal y_n = 0;
	// y(n) = 
	int i = 0;
	int count = data_in.count();
	for (i = 0; i < count; i++)
	{
		x_n = data_in.dequeue();
		y_n = IIR_B[0] * x_n + IIR_B[1] * x_n_1 + IIR_B[2] * x_n_2 - IIR_A[1] * y_n_1 - IIR_A[2] * y_n_2;

		y_n_2 = y_n_1;
		y_n_1 = y_n;

		x_n_2 = x_n_1;
		x_n_1 = x_n;

		data_out[i] = 30.0*y_n;
		//if (qAbs(data_out[i]) < 5)
		//{
		//	data_out[i] = 0.0;
		//}

	}

	len = quint32(i);
}