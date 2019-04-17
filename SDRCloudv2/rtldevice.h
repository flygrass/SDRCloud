#pragma once
#ifndef RTLDEVICE_H
#define RTLDEVICE_H

#include "sdrdevice.h"
#include <QReadWriteLock>
#include <QWaitCondition>
#include <QMutex>

#define DEFAULT_SAMPLE_RATE		24000
#define DEFAULT_BUF_LENGTH		(1 * 16384)
#define MAXIMUM_OVERSAMPLE		16
#define MAXIMUM_BUF_LENGTH		(MAXIMUM_OVERSAMPLE * DEFAULT_BUF_LENGTH)
#define AUTO_GAIN				-100
#define DEFAULT_BUFFER_DUMP		4096

#define FREQUENCIES_LIMIT		1024

QT_FORWARD_DECLARE_CLASS(QTimer)
QT_FORWARD_DECLARE_CLASS(QThread)
class RTLDevice : public SDRDevice
{
	Q_OBJECT

public:
	RTLDevice(QQueue<qreal> *queueBufX, QQueue<qreal> *queueBufY, QVector<qreal> *bufX, QVector<qreal> *bufY, QObject *parent);
	~RTLDevice();
private:
	static QReadWriteLock buf16RWLock;
	static quint32 buf16Len;
	static qint16  buf16[MAXIMUM_BUF_LENGTH];	// �˿黺������Ҫ���buf16RWLock��д��ʹ���Է���ͻ
	// Ϊ��ֹ��д��һֱ��д��ռ������������������
	static QWaitCondition buf16NotFull;
	static QWaitCondition buf16NotEmpty;
	static QMutex buf16Mutex;
	static bool buf16Full;
	friend static void rtlsdr_callback(unsigned char *buf, uint32_t len, void *ctx);

public slots:
	void updateBuf();

signals:
	void readAsyncDataSignal(quint16 bufNum, quint32 bufLen);
	void updBufSignal();
public:
	quint32 getDeviceCount()override;	// ��ȡ����Ѱ�����豸������
	QString getDeviceName(quint32 index)override;	// ��ȡ�豸�������
	bool getDeviceUSBInfo(quint32 index,
		QString &manufact, QString &product, QString &serial)override;	// ��ȡ�豸��USB��Ϣ
	bool open(quint32 index)override;	// ����������һ��SDR�豸
	bool close()override;	// �ر�һ��SDR�豸
	bool readAsyncData(quint16 bufNum = 0, quint32 bufLen = 0)override;
	void readAsyncDataSlot(quint16 bufNum, quint32 bufLen);

private:
	QTimer *updBufTimer;
	QThread *workerThread;
	QVector<qreal> *m_bufX;
	QVector<qreal> *m_bufY;
};

#endif // !RTLDEVICE_H