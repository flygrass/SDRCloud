#pragma once
/*
	��Ƶ�����������
*/
#ifndef PCMIODEVICE_H
#define PCMIODEVICE_H
#include "constant.h"

#include <QObject>
#include <QIODevice>
#include <QByteArray>
#include <QMutex>
#include <QWaitCondition>

class PCMIODevice : public QIODevice
{
	Q_OBJECT

public:
	
	PCMIODevice();
	~PCMIODevice();
	qint64 readData(char *data, qint64 maxlen);
	qint64 writeData(const char *data, qint64 len); //���Ǹ����麯���� ���ò�ʵ��
private:
	/*QByteArray m_buffer;*/
	char m_buffer[DEFAULT_MAX_AUDIO_BUFFER_SIZE];
	qint64 curRead;	// ��buffer�е�ǰӦ��д��λ��
	qint64 curWrite;

	// ����Ҫ����

	QMutex mutex;
	QWaitCondition notForRead;
	QWaitCondition notForWrite;
	bool isForWrite;
};

#endif // !PCMIODEVICE_H