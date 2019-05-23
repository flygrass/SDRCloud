#pragma once
/*
	�������
*/
#ifndef DEMODULATOR_H
#define DEMODULATOR_H

#include <QObject>
#include "constant.h"
#include <QVector>
#include <QReadWriteLock>
#include <QThread>

QT_FORWARD_DECLARE_CLASS(RtlDevice)

// ������࣬������źŵ����ֽ��
class Demodulator : public QObject
{
	Q_OBJECT

public:
	Demodulator(RtlDevice *dongle, QObject *parent = nullptr);
	~Demodulator();
	friend class DemodWorker;
public:	// ��¶��ȥ�Ľӿ�
	void getData(QVector<qreal> &data);	// ��ȡ����������
	void getDataByChar(char data[], quint32 &len);
	void startDemodFM();	// ��ȷ��Ӳ����������ʱ�ٵ����ҽ��н����������ܳ����߳�һֱ������ȥ�Ĵ���
	void stopDemodFM();
	bool isRunning();

signals:
	void m_demodFMSignal(Demodulator *demod);
private:
	void m_demodFM();
private:
	RtlDevice *m_dongle;
	QVector<qreal> m_unDemodSignal;
	qreal m_signal[DEFAULT_MAX_BUFFER_SIZE];	// ��Ž����Ľ��
	quint32 m_signalLen;	// ��ǰ�źų���
	QReadWriteLock m_signalLock;	// ��д��
	qreal m_preQdivI;	// Q(n-1)/I(n-1)
	QThread m_demodThread;
	DemodWorker *demodWorker;
	bool m_isRunning;
};


class DemodWorker :public QObject
{
	Q_OBJECT
public slots:
	void doWork(Demodulator *demod);
signals:
	void done();
};

#endif // !DEMODULATOR_H