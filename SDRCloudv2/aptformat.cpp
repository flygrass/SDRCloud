#include "aptformat.h"
#include "dsp.h"
#include "wavfile.h"
#include "demod_core.h"
#include <QAudioFormat>
#include <QDebug>
#include "fir_12480_4160.h"


// 1.loadWave���˺���ͨ��ָ���ļ������ض�Ӧ�����ݵ�data����������ݵĲ���Ƶ��inputSampleRate
// 2.resample���˺������ź�data�����ز�����ʹ���ܹ���4160�������Ӷ�ʹ��һ��word��Ӧ��������������
// 3.demodAM�� �˺���������źŽ��а������õ�����signal�����ȣ�����Ƶ�ʱ��ֲ���
// 4.generateSyncWave�� �˺�����������sync A��ͬ������
// 5.findSync���˺������ݸ����Ĳ���hn����signal��Ѱ��peak��
// 6.s_interp1���˺�����һ�����ȵ����ݽ��������ڲ�
// 7.aptDecode���˺�������ȫ�֣����������������

bool arrToImage(const unsigned char *imgarr, const int width, const int height, QImage &image)
{
	QImage img(width, height, QImage::Format_Grayscale8);
	//for (int i = 0; i < 10; i++) {
	//	qDebug() << data[i];
	//}
	//RGB����ֵ
	int b = 0;
	int g = 0;
	int r = 0;

	//��������
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			b = (int)imgarr[i * width + j];
			g = b;
			r = g;
			img.setPixel(j, i, qRgb(r, g, b));
		}
	}
	image = img;
	return true;
}

const double* fir_12480_4160 = FIR_12480_TO_4160;
const int fir_12480_4160_len = FIR_12480_TO_4160_LEN;
bool aptDecode(ImageViewer *viewer, const QString fileName, unsigned char *&img, 
	unsigned int &imgSize, int &width, int &height, bool isSync)
{
	qint64 len = 0;
	unsigned int signalLen = 0;
	qreal *data;
	qreal *signal;
	int inputSampleRate;
	int work_rate = 12480;	// ʵ�ʴ���ʱ���õĲ���Ƶ�ʣ���������4160Hz��������

	// ���ز�������
	viewer->updateStatusInfo("����wav������", 0);
	loadWave(fileName, data, len, inputSampleRate);
	// �ز���
	viewer->updateStatusInfo("�ز�����", 1);
	resample(data, len, inputSampleRate, work_rate, signal, signalLen);
	qDebug() << "before AM demod signalLen=" << signalLen;
	for (int i = 0; i < 100; i++) {
		qDebug() << signal[i];
	}

	// AM���
	viewer->updateStatusInfo("AM�����", 2);
	demodAM(work_rate, signal, signalLen, signal, signalLen);

	qDebug() << "before conv filter";
	for (int i = 0; i < 10; i++) {
		qDebug() << signal[i];
	}
	// �ٴ��˲��������ٳ�ȡҲ�Ͳ����˲���
	conv_filter(signal, signalLen, fir_12480_4160, fir_12480_4160_len);

	qDebug() << "after conv filter";
	for (int i = 1000; i < 1010; i++) {
		qDebug() << signal[i];
	}
	bool ret = false;
	unsigned int *index = nullptr;
	int indexLen = 0;
	if (isSync) {
		// ���ɴ�����źŲ���
		qreal *hn;
		int hLen = 0;
		viewer->updateStatusInfo("���ɴ�����źŲ�����", 3);
		generateSyncWave(hn, hLen, work_rate);
		// Ѱ��ͬ����
		
		viewer->updateStatusInfo("sync Aͬ��λ��Ѱ����", 4);
		ret = findSync(signal, signalLen, hn, hLen, work_rate, index, indexLen);
		if (ret == false) {
			viewer->updateStatusInfo("�ź�̫�̵���ͬ��ʧ��", 4);
		}
		else {
			// ���źŽ��ж���
			align_signal(signal, signalLen, index, indexLen);
		}
	}



	//int samplesPerLine = round(double(work_rate) / Fw * lineLength);
	width = lineLength;
	height = 0;
	//double *imgData = nullptr;
	//int imgLen = 0;

	for (int i = 0; i < 10; i++) {
		qDebug() << signal[i];
	}
	qDebug() << "Fs=" << inputSampleRate;

	viewer->updateStatusInfo("��ͼ���²�����", 5);

	// ���źŽ��г�ȡ
	decimate(signal, signalLen, 3);

	//if (ret == false || isSync == false) {
	//	// ͼƬû�н���ͬ��
	//	imgLen = round(double(signalLen) / Fs * Fw);
	//	height = ceil(double(imgLen) / width); // imgSize = width*height; width = 2080; height = floor(imgLen/width)
	//	imgData = new double[imgLen];
	//	s_interp1(signal, signalLen, imgData, imgLen);
	//	qDebug() << "==================";
	//	for (int i = 0; i < 10; i++) {
	//		qDebug() << imgData[i];
	//	}
	//}
	//else {

	//	int i = 0;
	//	qreal *x;
	//	height = indexLen;
	//	imgLen = width * height;
	//	imgData = new double[imgLen];
	//	for (i = 0; i < height; i++) {
	//		x = signal+index[i];

	//		s_interp1(x, samplesPerLine, imgData+i*width, width);
	//	}
	//}


	// ͼ���һ����contrast����
	height = ceil(double(signalLen) / width);
	imgSize = width * height;
	img = (unsigned char*)new char[imgSize];

	normalize_contrast(signal, signalLen, img, imgSize);

	//int i = 0;
	//for (i = 0; i < imgLen; i++) {
	//	if (imgData[i] > 1) {
	//		imgData[i] = 1;
	//	}
	//	else if (imgData[i] < 0) {
	//		imgData[i] = 0;
	//	}
	//	else {
	//		;
	//	}
	//	img[i] = (unsigned char)(imgData[i] * 255);
	//	//img[i] = 0;
	//}
	//for (; i < imgSize; i++) {
	//	img[i] = 0;
	//}
		// ��ʱ����ռ�
	delete signal;
	signal = nullptr;

	return true;
	
}

// ��fileName�ļ�������ݶ���data�����ҪԤ�ȷ���ռ�
void loadWave(const QString fileName, qreal *&data, qint64 &len, int &Fs)
{
	WavFile wav;
	int sampleSize;
	quint64 len2;
	QAudioFormat format;
	
	// ��ȡ�ļ��ĳ��Ⱥ��������С
	wav.getWaveDataSize(fileName, len2, sampleSize);	// len / sampleSize = ��������
	data = (qreal*)(new char[ceil(double(len2) / (sampleSize / 8.0)) * sizeof(qreal)]);

	// �������
	wav.readWave(fileName, data, len, format);
	qDebug() << "wav datasize = " << QString::number(len);
	Fs = format.sampleRate();
}

// ����һ����ƥ���źŵĲ��Σ�����Ԥ�ȷ���ռ�
void generateSyncWave(qreal *&hn, int &len, const int Fs)
{
	// Ϊ�����ܣ��ڴ˻���hn
	static int _Fs = 0;
	static int _len = 0;

	if (Fs == _Fs) {	// ��ȷ��hn��Ҫ������޸�
		len = _len;
		return;
	}
	else {
		_Fs = Fs;
	}
	
	int samplesPerWord = round(double(Fs) / Fw);
	int samplesPerLine = round(double(Fs) / Fw * lineLength);

	int samplesPerPulse = 4 * samplesPerWord;

	len = syncLength * samplesPerWord;
	_len = len;	// ����
	hn = new qreal[len];
	for (int i = 0; i < len; i++) {
		hn[i] = -1;
	}
	
	int startPos = samplesPerPulse;
	int endPos = startPos + (numPulse - 1)*samplesPerPulse;

	int i = 0;
	for (i = startPos; i <= endPos; i=i+samplesPerPulse) {
		for (int j = i; j <= (i + samplesPerPulse / 2); j++) {
			hn[j] = 1;
		}
	}
}

// ����һ����ƥ���źŵĲ��Σ�����Ԥ�ȷ���ռ�
void generateSyncWave_s(qreal *&hn, int &len, const int Fs)
{
	// Ϊ�����ܣ��ڴ˻���hn
	static int _Fs = 0;
	static int _len = 0;

	if (Fs == _Fs) {	// ��ȷ��hn��Ҫ������޸�
		len = _len;
		return;
	}
	else {
		_Fs = Fs;
	}

	int samplesPerWord = round(double(Fs) / Fw);
	int samplesPerLine = round(double(Fs) / Fw * lineLength);

	int samplesPerPulse = 4 * samplesPerWord;

	len = syncLength * samplesPerWord;
	_len = len;	// ����
	hn = new qreal[len];
	for (int i = 0; i < len; i++) {
		hn[i] = -1;
	}

	int startPos = samplesPerPulse;
	int endPos = startPos + (numPulse - 1)*samplesPerPulse;

	int i = 0;
	for (i = startPos; i <= endPos; i = i + samplesPerPulse) {
		for (int j = i; j < (i + samplesPerPulse / 2); j++) {	// ����϶�������
			hn[j] = 1;
		}
	}
}

// ���ݴ�����ź�hn��ȥsignal����ͬ���㣬���ͨ��index����������index����ҪԤ�ȷ���ռ�
bool findSync(qreal *signal, const unsigned int signalLen, const qreal *hn, const int hLen, const int Fs, unsigned int *&index, int &indexLen)
{
	// ����peaks����̾���
	unsigned int maxPos = signalLen - hLen;
	int samplesPerLine = round(double(Fs) / Fw * lineLength);
	if (maxPos <= 0) {
		return false;	// �ź�̫�̲���sync
	}
	int minDistance = round(double(Fs) / Fw * lineLength*0.8);
	Peak *peaks;
	Peak peak;
	
	// Ϊ����������
	peaks = new Peak[MAX_PEAKS_NUM];
	peak.index = 0;
	peak.coe = 0;
	peaks[0] = peak;

	unsigned int i = 0;
	unsigned int p = 0;
	double coe = 0;
	for (i = 0; i <= maxPos && (i + samplesPerLine) <= signalLen; i++) {
		coe = 0;
		for (unsigned int j = 0; j < hLen; j++) {
			coe += hn[j] * signal[i + j];
		}
		if (i - peaks[p].index > minDistance) {
			// �������������㹻
			p++;
			peaks[p].index = i;
			peaks[p].coe = coe;
		}
		if (coe > peaks[p].coe) {
			peaks[p].index = i;	// ѡ��ߵķ�
			peaks[p].coe = coe;
		}
	}

	if (peaks[p].index + samplesPerLine <= signalLen)
	{
		p++;
	}

	index = new unsigned int[p];
	for (i = 0; i < p; i++) {
		index[i] = peaks[i].index;
	}
	indexLen = p;
	delete peaks;
	peaks = nullptr;
	return true;
}



void align_signal(double *signal, unsigned int &signal_len, unsigned int *index, const int indexLen)
{
	int row = 0;
	int work_rate = 12480;
	int samplesPerLine = round(double(work_rate) / Fw * lineLength);
	unsigned int i = 0;
	unsigned int j = 0;
	for (row = 0; row < indexLen; row++) {
		for (i = index[row]; i < samplesPerLine+ index[row] && i < signal_len; i++) {
			signal[j++] = signal[i];
		}
	}
	signal_len = j;
}


// img����ҪԤ�ȷ���ռ�
void aptToImage(int Fs, double *data, unsigned int len_in, unsigned char *&img, unsigned int &imgSize, unsigned int &width)
{
	int samplesPerWord = round(double(Fs) / Fw);
	int samplesPerPulse = round(double(Fs) / Fpulse);
	unsigned int len2 = round(double(len_in) / Fs * Fw);
	double temp = 0;
	double tMax = double(len_in) / double(Fs);

	// ��ʼ��ͼ��ߴ�
	width = lineLength;
	imgSize = ceil(double(len2) / double(width))*width;	// len*Fw/Fs
	img = (unsigned char*)new char[imgSize];


	// �����ڲ�
	double *imgData = new double[len2];
	s_interp1(data, len_in, imgData, len2);

	for (int i = 0; i < 10; i++) {
		qDebug() << "=" << imgData[i];
	}

	int i = 0;
	for (i = 0; i < len2; i++) {
		img[i] = (unsigned char)(imgData[i] * 255);
	}

	for (; i < imgSize; i++) {
		img[i] = 0;	// ��ʣ�ಿ�ֽ������
	}

	delete imgData;
	imgData = nullptr;
	//// ��ʼ���ڲ��õ���һЩ����
	//double *tApt = new double[len_in];
	//double *tImg = new double[len2];
	//
	//linspace(tApt, 0, tMax, len_in);
	//linspace(tImg, 0, tMax, len2);

	//// �ڲ����
	//double Ml = 0.2105;
	//double Mr = 0.2105;
	//Vector<double> x(len_in, tApt);
	//Vector<double> y(len_in, data);
	//Spline3Interp<double> poly(x, y, Ml, Mr);
	//poly.calcCoefs();

	//int i = 0;
	//int k = 0;
	//for (i = 0; i < len2; i++) {
	//	img[i] = (unsigned char)255.0*poly.evaluate(tImg[i]);
	//}

	//for (; i < imgSize; i++) {
	//	img[i] = 0;	// ��ʣ�ಿ�ֽ������
	//}

	//delete tApt;
	//tApt = nullptr;
	//delete tImg;
	//tImg = nullptr;

	//int i = 0;
	//int j = 0;
	//int k = 0;

	//for (i = 0; i < len_in; i += samplesPerWord) {
	//	temp = 0;
	//	for (j = i; j < i + samplesPerWord; j++) {
	//		temp += data[i];
	//	}
	//	temp = temp * 255.0 / double(samplesPerWord);
	//	img[k++] = (unsigned char)temp;
	//}
	//for (; k < imgSize; k++) {
	//	img[k++] = 0;	// ��ʣ�ಿ�ֽ������
	//}
}

void normalize_contrast(double *signal, const unsigned int signalLen, unsigned char *img, const unsigned int imgSize)
{
	double maxValue = 0;
	double minValue = 0;
	double range = 0;
	unsigned int i = 0;
	for (i = 0; i < signalLen; i++) {
		if (signal[i] > maxValue) {
			maxValue = signal[i];
		}

		if (signal[i] < minValue) {
			minValue = signal[i];
		}
	}
	range = maxValue - minValue;
	if (range == 0) {
		return;
	}
	for (i = 0; i < signalLen; i++) {
		img[i] = (unsigned char)((signal[i]-minValue) * 255.0 / range);
	}
	for (; i < imgSize; i++) {
		img[i] = (unsigned char)0;
	}
}