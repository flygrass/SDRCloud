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

QT_FORWARD_DECLARE_CLASS(WaveRecorder);

class PCMIODevice : public QIODevice
{
	Q_OBJECT

public:
	
	PCMIODevice(WaveRecorder *recorder);
	~PCMIODevice();
	qint64 readData(char *data, qint64 maxlen);
	qint64 writeData(const char *data, qint64 len); //���Ǹ����麯���� ���ò�ʵ��
	void stopUpdate();
	void setRecordState(bool on);
private:
	/*QByteArray m_buffer;*/
	char m_buffer[DEFAULT_MAX_AUDIO_BUFFER_SIZE];
	qint64 curRead;	// ��buffer�е�ǰӦ��д��λ��
	qint64 curWrite;
	quint64 validDataLength;
	WaveRecorder *m_recorder;
	bool isRecordOn;

	// ����Ҫ����
	QMutex mutex;
	QWaitCondition notForRead;
	QWaitCondition notForWrite;
	bool isForWrite;
};

#endif // !PCMIODEVICE_H