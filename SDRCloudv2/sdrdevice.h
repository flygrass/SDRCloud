#pragma once
#ifndef SDRDEVICE_H
#define	SDRDEVICE_H

#include <QObject>
//QT_FORWARD_DECLARE_CLASS(QQueue<qreal>)
#include <QQueue>
class SDRDevice : public QObject
{
	Q_OBJECT
public:
	typedef enum DEV_STATE_FLAG{CLOSED, OPENED, INACTIVE, ACTIVE}DEV_STATE;
	typedef struct sdrdev dev_t;
public:
	SDRDevice(QQueue<qreal> *queueBufX, QQueue<qreal> *queueBufY, QObject *parent);
	~SDRDevice();
public:
	DEV_STATE getState();	// ��ȡ�豸��ǰ״̬
	virtual quint32 getDeviceCount() = 0;	// ��ȡ����Ѱ�����豸������
	virtual QString getDeviceName(quint32 index) = 0;	// ��ȡ�豸�������
	virtual bool getDeviceUSBInfo(quint32 index,
		QString &manufact, QString &product, QString &serial) = 0;	// ��ȡ�豸��USB��Ϣ
	virtual bool open(quint32 index) = 0;	// ����������һ��SDR�豸
	virtual bool close() = 0;	// �ر�һ��SDR�豸
	virtual bool readAsyncData(quint16 bufNum = 0, quint32 bufLen = 0) = 0;	// �첽��ȡ����

protected:
	dev_t *m_dev;	// SDR�豸�ľ�����������ڲ�������Ϣ�ã����ǲ����˽⣩
	DEV_STATE m_state;
	QQueue<qreal> *m_QueueBufX;
	QQueue<qreal> *m_QueueBufY;
};
#endif // !SDRDEVICE_H
