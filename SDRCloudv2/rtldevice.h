#pragma once
/*
	rtl�豸��
*/
#include <QObject>
#include <QVector>
#include "readwritebuffer.h"
#include <QThread>

#define SAMPLE_RATE_COUNT 11
QT_FORWARD_DECLARE_CLASS(ReadDataWorker)

class RtlDevice : public QObject
{
	Q_OBJECT

public:
	RtlDevice(QObject *parent);
	~RtlDevice();
	friend void rtlsdr_callback(unsigned char *buf, uint32_t len, void *ctx);
	static const QString sampleModeStrings[3];
public:
	typedef enum TunerType_flag
	{
		Unknown = 0,
		E4000,
		FC0012,
		FC0013,
		FC2580,
		R820T
	}TunerType;
	typedef enum GainMode_flag
	{
		Automatic = 0,
		Manual

	}GainMode;
	typedef enum SampleMode_flag
	{
		Quadrature = 0,
		DI,
		DQ

	}SampleMode;
	typedef enum State_flag
	{
		CLOSED = 0,
		OPENED,
		RUNNING,
		CANCELLING,
		LOST
	}State;
	
private:
	static const quint32 DEFAULT_SAMPLERATES[11];	// Hz
public:	// ��¶���Ľӿں���
	static QVector<quint32> getDefaultSampleRateList();
	bool getDeviceList(QVector<QString> &names, quint32 &count);
	QString getDeviceName();
	bool open(const quint32 index);	// ��rtl�豸������ȡrtl�豸�Ļ�����Ϣ
	bool close();	// �ر��Ѿ��򿪵�rtl�豸
	bool startReadData();
	bool stopReadData();
	State getState();
	SampleMode getSampleMode();
	quint32 getSampleRateIndex();
	bool getOffsetTuningState();
	bool getTunerAgcState();
	bool getRtlAgcState();
	void getTunerGainRange(quint32 &min, quint32 &max);
	quint32 getTunerGain();
	void getData(QVector<qreal>& data);


public:
	bool m_readData();	// �������Ӧ����readDataSignal���е���
signals:
	void readDataSignal(RtlDevice *dev);	// �˷�����������Ӧ�÷ŵ�worker��ִ��
public slots:
	void readDataStopSlot();
private:
	void initRtl();
	bool stopRunning();	// ֹͣ���ݵĶ�ȡ
private:
	// �豸��״̬����
	//bool m_isOpen;	// �豸�Ƿ����
	//bool m_isRunning;	// �豸�������ж�ȡ����
	State m_state;	// �豸��״̬
	bool m_isBufferReset;	// �������Ƿ�λ
	QString m_name;	// �豸������

	// tuner���
	TunerType m_tunerType;	// tuner����
	quint32 m_tunerFreq;	// tuner��Ƶ��
	QVector<qint32> m_tunerGainList;	// �豸֧�ֵ������б�
	qint32 m_tunerGain;	// tuner������
	GainMode m_tunerGainMode;	// tuner������ģʽ
	bool m_isOffsetTuningOn;

	// RTL
	quint32 m_sampleRate;	// ������
	qint32 m_freqCorrection;	// ʱ��Ƶ�����У��
	bool m_isRtlAgcOn;	// �Ƿ�����RTL��AGC�Զ��������
	SampleMode m_sampleMode;

	// ����
	quint32 m_downSampleRate1;	// �����ݽ��е��²�����
	quint32 m_rtlXtalFreq;
	quint32 m_tunerXtalFreq;

	// ������
	ReadWriteBuffer<qreal> m_data;
	QThread readDataThread;
	ReadDataWorker *worker;
};
