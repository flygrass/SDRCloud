#include "rtldevice2.h"
#include "rtldriver.h"
#include <QDebug>
#include "readdataworker.h"


// �˺����������첽���ݶ�ȡ�Ļص�
static void rtlsdr_callback(unsigned char *buf, uint32_t len, void *ctx)
{
	qDebug() << "rtlsdr call back is running";
	RtlDevice *dev = (RtlDevice*)ctx;

	dev->m_data.writeChar(buf, (quint32)len);
}
const QString RtlDevice::sampleModeStrings[3] =
{
	"Quadrature sampling",
	"Direct sampling (I branch)",
	"Direct sampling (Q branch)"
};
const quint32 RtlDevice::DEFAULT_SAMPLERATES[11] = {
	3200e3,	// Hz
	2800e3,
	2560e3,
	2400e3,
	2048e3,
	1920e3,
	1800e3,
	1400e3,
	1024e3,
	900001,
	250e3};


RtlDevice::RtlDevice(QObject *parent)
	: QObject(parent),
	m_data()
{
	initRtl();

	// ��ʼ�����ݶ�ȡ���̣߳��Ժ�ֻ��Ҫ����readDataSignal�Ϳ��Զ�����
	worker = new ReadDataWorker();
	worker->moveToThread(&readDataThread);
	connect(&readDataThread, &QThread::finished, worker, &QObject::deleteLater);
	connect(this, &RtlDevice::readDataSignal, worker, &ReadDataWorker::doWork);
	connect(worker, &ReadDataWorker::done, this, &RtlDevice::readDataStopSlot);
	readDataThread.start();
}

RtlDevice::~RtlDevice()
{
}

void RtlDevice::readDataStopSlot()
{
	if (m_state != RtlDevice::RUNNING)
	{
		qDebug() << "exception when stop reading data, we assert m_state=RUNNING";
	}
	m_state = RtlDevice::OPENED;
}

QVector<quint32> RtlDevice::getDefaultSampleRateList()
{
	QVector<quint32> list;
	for (int i = 0; i < SAMPLE_RATE_COUNT; i++)
	{
		list.append(DEFAULT_SAMPLERATES[i]);
	}

	return list;
}

bool RtlDevice::getDeviceList(QVector<QString> &names, quint32 &count)
{
	QString name = "";

	names.resize(0);
	count = RTLDriver::getDeviceCount();
	for (quint32 i = 0; i < count; i++)
	{
		name = RTLDriver::getDeviceName(i);
		names.append(name);
	}
	return count == 0 ? false : true;
}

bool RtlDevice::open(const quint32 index)
{
	bool r;
	if (m_state != RtlDevice::CLOSED)
	{
		return false;	// ����Ѿ����豸����
	}
	initRtl();
	r = RTLDriver::open(index);
	if (r == false)
	{
		return false;	// �豸��ʧ��
		qDebug() << "failed to open the rtl device";
	}
	m_state = RtlDevice::OPENED;
	quint32 gainsCount = 0;

	m_name = RTLDriver::getDeviceName(index);	// ��ʼ����ǰ�豸����
	gainsCount = RTLDriver::getTunerGains(m_tunerGainList);	// ��ʼ���豸֧�ֵ������б�
	if (gainsCount <= 0)
	{
		qDebug() << "failed to get the gain list";
	}
	else
	{
		qDebug() << QString("device supports ") + QString::number(gainsCount) + " gains";
		for (int i = 0; i < gainsCount; i++) {
			qDebug() <<  QString::number(m_tunerGainList[i] / 10.0) + " dB";
		}
	}
	bool ret = false;

	// ��ȡĬ�Ͼ���Ƶ��
	ret = RTLDriver::getXtalFreq(m_rtlXtalFreq, m_tunerXtalFreq);
	if (ret == false)qDebug() << "failed to get the default Xtal freq";
	qDebug() << QString("default rtl-xtal-freq=") + QString::number(m_rtlXtalFreq) + "; default tuner-xtal-freq=" + QString::number(m_tunerXtalFreq);

	// �ر�offset tuning
	ret = RTLDriver::setOffsetTuning(m_isOffsetTuningOn);
	if (ret == false)qDebug() << "failed to close the offset tunning[just fine]";
	m_isOffsetTuningOn = RTLDriver::getOffsetTuning();
	qDebug() << QString("-1 on error, 0 means disabled, 1 enabled:") + QString::number(m_isOffsetTuningOn);

	// ����tuner������ģʽΪ�˹�
	ret = RTLDriver::setTunerGainMode((RTLDriver::GainMode)m_tunerGainMode);
	if (ret == false)qDebug() << "failed to set the tuner gain mode to manumal";

	// ����Ĭ�ϵ�tuner����Ƶ��
	ret = RTLDriver::setCenterFreq(m_tunerFreq);
	if (ret == false)qDebug() << "failed to set the default center freq of tuner";
	m_tunerFreq = RTLDriver::getCenterFreq();
	qDebug() << QString("default tuner freq is set: ") + QString::number(m_tunerFreq);

	// ����tuner������ΪĬ��ֵ
	ret = RTLDriver::setTuerGain(m_tunerGain);
	if (ret == false)qDebug() << "failed to set the default gain of tuner";
	m_tunerGain = RTLDriver::getTunerGain();
	qDebug() << QString("default tuner gain is set: ") + QString::number(m_tunerGain / 10.0) + " dB";

	// ����sample mode
	ret = RTLDriver::setSampleMode(m_sampleMode);
	if (ret == false)qDebug() << "failed to set the default sample mode";
	qint8 sampleMode = RTLDriver::getSampleMode();
	qDebug() << "[0:quadrature,1:I,2:Q]default sample mode:";

	// ����rtl��AGC�Զ��������
	ret = RTLDriver::setAgcMode(m_isRtlAgcOn);
	if (ret == false)qDebug() << "failed to open the RTL's AGC";

	// ��ȡĬ�ϵ�ʱ��Ƶ��ƫ��
	m_freqCorrection = RTLDriver::getFreqCorrection();
	qDebug() << QString("default freq correction is: ") + QString::number(m_freqCorrection) + " ppm";

	// ���ò���Ƶ��
	ret = RTLDriver::setSampleRate(m_sampleRate);
	if (ret == false)qDebug() << "failed to set the default sample rate";
	m_sampleRate = RTLDriver::getSampleRate();
	qDebug() << QString("default sample rate is set: ") + QString::number(m_sampleRate) + " SPS";

	return true;	// �豸�����ǳɹ���
	
}

bool RtlDevice::close()
{
	if (m_state == RtlDevice::CLOSED)
	{
		return true;	// ����豸�Ѿ����ر�
	}
	if (m_state == RtlDevice::RUNNING)
	{
		// ����豸�������ж�ȡ�����Ǿ�ֹͣ��
		stopRunning();
	}

	bool ret = false;

	ret = RTLDriver::close();
	if (ret)
	{
		m_state = RtlDevice::CLOSED;
	}
	else
	{
		qDebug() << "failed to close the RTL device";
	}
	return ret;
}

bool RtlDevice::startReadData()
{
	if (m_state != RtlDevice::OPENED)
	{
		return false;
		qDebug() << "the device is unreadable";
	}
	if (m_isBufferReset == false)
	{
		bool ret = RTLDriver::resetBuffer();
		if (ret)qDebug() << "buffer is reset";
		m_isBufferReset = true;
	}
	m_state = RtlDevice::RUNNING;
	emit readDataSignal(this);
	return true;
}

bool RtlDevice::stopReadData()
{
	bool ret = stopRunning();
	m_isBufferReset = false;
	return ret;
}

RtlDevice::State RtlDevice::getState()
{
	return m_state;
}

RtlDevice::SampleMode RtlDevice::getSampleMode()
{
	qint8 ret = 0;
	ret = RTLDriver::getSampleMode();
	if (ret < 0)
	{
		qDebug() << "failed to get the sample mode";
		return SampleMode(0);
	}

	return SampleMode(ret);
}

quint32 RtlDevice::getSampleRateIndex()
{
	for (int i = 0; i < SAMPLE_RATE_COUNT; i++)
	{
		if (m_sampleRate == RtlDevice::DEFAULT_SAMPLERATES[i])
		{
			return i;
		}
	}
	return 0;
}

bool RtlDevice::getOffsetTuningState()
{
	return m_isOffsetTuningOn;
}

bool RtlDevice::getTunerAgcState()
{
	return !(bool)m_tunerGainMode;
}

bool RtlDevice::getRtlAgcState()
{
	return m_isRtlAgcOn;
}

void RtlDevice::getTunerGainRange(quint32 & min, quint32 & max)
{
	//m_tunerGainList
	min = 0;
	max = 1;
	for (int i = 0; i < m_tunerGainList.count(); i++)
	{
		if (m_tunerGainList[i] < min)min = m_tunerGainList[i];
		if (m_tunerGainList[i] > max)max = m_tunerGainList[i];
	}
}

quint32 RtlDevice::getTunerGain()
{
	return m_tunerGain;
}

void RtlDevice::getData(QVector<qreal>& data)
{
	m_data.read(data);
}

bool RtlDevice::m_readData()
{
	qDebug() << "the start of m_readData";
	if (m_state != RtlDevice::RUNNING && m_isBufferReset)
	{
		qDebug() << "failed to start read the data for the unreadable device";
		return false;	// ��ǰ�豸���ɶ�
	}

	bool ret = false;
	qDebug() << "be ready to read async";
	ret = RTLDriver::readAsync(rtlsdr_callback, this);
	if (ret == false)
	{
		qDebug() << "error when read the data";
	}
	return ret;
}

// ��ʼ��Rtl�豸
void RtlDevice::initRtl()
{
	m_state = RtlDevice::CLOSED;
	m_isBufferReset = false;
	m_name = "";
	m_tunerType = RtlDevice::Unknown;
	m_tunerFreq = 90e6;	// 90MHz
	m_tunerGain = 100;	// 10dB
	m_tunerGainMode = RtlDevice::Manual;
	m_isOffsetTuningOn = false;
	m_sampleRate = DEFAULT_SAMPLERATES[4];	// 2.048MSPS
	m_sampleMode = RtlDevice::Quadrature;
	m_freqCorrection = 0;	// ppm
	m_isRtlAgcOn = true;
	m_downSampleRate1 = 1;
}

bool RtlDevice::stopRunning()
{
	if (m_state != RtlDevice::RUNNING)
	{
		return true;
	}
	bool ret = false;
	
	ret = RTLDriver::cancelAsync();

	if (ret)
	{
		m_state = RtlDevice::OPENED;
		return true;
	}
	else
	{
		qDebug() << "failed to cancel read async";
		return false;
	}
}



