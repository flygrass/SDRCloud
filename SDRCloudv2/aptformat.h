#pragma once
#ifndef APTFORMAT_H
#define APTFORMAT_H
#define MAX_PEAKS_NUM 6000
#include <QString>
#include <QImage>
#include <imageviewer.h>
const int Fc = 2400;	// �����ز�Ƶ��2400Hz;
const int Fw = 4160;	// ÿ���ֵ�Ƶ����4160Hz;
const int lineLength = 2080;	// ÿ����2080��words
const int Fpulse = 1040;	// sync Aÿ�����岨��Ƶ����1040Hz
const int numPulse = 7;	// sync Aһ��ͬ����7�����岨��
const int syncLength = 39;	// sync Aͬ���ź���39��words
const double pi = 3.1415926;

typedef struct Peak {
	int index;
	double coe;
}Peak;

void loadWave(const QString fileName, qreal *&data, qint64 &len, int &Fs);
void aptToImage(int Fs, double *data, unsigned int len_in, unsigned char *&img, unsigned int &imgSize, unsigned int &width);
void generateSyncWave(qreal *&hn, int &len, const int Fs);
bool findSync(qreal *signal, const int signalLen, const qreal *hn, const int hLen, const int Fs, int *&index, int &indexLen);
bool aptDecode(ImageViewer *viewer, const QString fileName, unsigned char *&img, int &imgSize, int &width, int &height, bool isSync);
bool arrToImage(const unsigned char *imgarr, const int width, const int height, QImage &image);



#endif // !APTFORMAT_H
