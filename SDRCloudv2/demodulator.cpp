#include "demodulator.h"
#include "rtldevice.h"
#include "qmath.h"
#include "demod_core.h"

Demodulator::Demodulator(RtlDevice *dongle, QObject *parent)
	: QObject(parent),
	m_preQdivI(0.0),
	m_signalLen(0),
	m_dongle(dongle)
{
	demodWorker = new DemodWorker();	// ������ᱻ�ظ�ִ�У����Բ����ж��Ƿ��ظ�ʵ����
	demodWorker->moveToThread(&m_demodThread);
	connect(this, &Demodulator::m_demodFMSignal, demodWorker, &DemodWorker::doWork);
	m_demodThread.start();
}

Demodulator::~Demodulator()
{
	m_demodThread.quit();
	m_demodThread.wait();
}

void Demodulator::startDemodFM()
{
	m_isRunning = true;
	emit m_demodFMSignal(this);
}

void Demodulator::stopDemodFM()
{
	m_isRunning = false;
}

bool Demodulator::isRunning()
{
	return m_isRunning;
}

void Demodulator::getData(QVector<qreal>& data)
{
	// ���������Ƿ���Ҫһ���������ȴ��������ظ���ȡ����
	m_signalLock.lockForRead();
	data.resize(0);
	for (int i = 0; i < m_signalLen; i++)
	{
		data.append(m_signal[i]);
	}
	m_signalLock.unlock();
}

void Demodulator::getDataByChar(char data[], quint32 &len)
{
	// ���������Ƿ���Ҫһ���������ȴ��������ظ���ȡ����
	m_signalLock.lockForRead();
	int i = 0;
	for (i = 0; i < m_signalLen; i++)
	{
		data[i] = char(m_signal[i]);
	}
	len = i;
	m_signalLock.unlock();
}

void Demodulator::m_demodFM()
{
	qreal QdivIn_1 = 0.0;
	qreal QdivIn = 0.0;
	const qreal numZero = 1e-4;
	static double data_in[DEFAULT_MAX_BUFFER_SIZE];
	static double data_out[DEFAULT_MAX_BUFFER_SIZE];
	unsigned int len_in = 0;
	unsigned int len_out = 0;

	qreal max = 0.1;

	while (true)
	{
		//qDebug() << "is demoding";
		// ���������ֹͣ���������̵߳�����
		if (m_dongle->getState() == RtlDevice::CANCELING)
		{
			m_isRunning = false;
			qDebug() << "jump out demoding";
			break;
		}

		m_dongle->getData(m_unDemodSignal);	//	������ȡӲ�����ԭʼIQ���ݣ�ע�������п�������
											// m_unDemodSignal�е�����-128-127֮��
		// ע:���ֽ���㷨�������󣬾��ȸ�

		for (int i = 0; i < m_unDemodSignal.count(); i++)
		{
			data_in[i] = m_unDemodSignal[i];
		}
		len_in = m_unDemodSignal.count();
		demod_core(data_in, len_in, data_out, &len_out);

		for (int i = 0; i < m_unDemodSignal.count(); i++)
		{
			data_in[i] = m_unDemodSignal[i];
		}

		m_signalLock.lockForWrite();
		for (int i = 0; i < len_out; i++)
		{
			m_signal[i] = data_out[i];
		}
		m_signalLen = len_out;
		m_signalLock.unlock();
	}
}

//void Demodulator::m_demodFM()
//{
//	qreal QdivIn_1 = 0.0;
//	qreal QdivIn = 0.0;
//	const qreal numZero = 1e-4;
//	
//	qreal max = 0.1;
//
//	while (true)
//	{
//		//qDebug() << "is demoding";
//		// ���������ֹͣ���������̵߳�����
//		if (m_dongle->getState() == RtlDevice::CANCELING)
//		{
//			m_isRunning = false;
//			qDebug() << "jump out demoding";
//			break;
//		}
//
//		m_dongle->getData(m_unDemodSignal);	//	������ȡӲ�����ԭʼIQ���ݣ�ע�������п�������
//											// m_unDemodSignal�е�����-128-127֮��
//		// ע:���ֽ���㷨�������󣬾��ȸ�
//
//		QdivIn_1 = m_preQdivI;
//		quint32 j = 0;
//		m_signalLock.lockForWrite();
//		for (int i = 0; i < m_unDemodSignal.count() - 1; i = i + 2)
//		{
//			QdivIn = m_unDemodSignal[i + 1] / (m_unDemodSignal[i] + numZero);	// = Q(n) / I(n)
//			m_signal[j] = (QdivIn - QdivIn_1) / (1.0 + QdivIn * QdivIn);
//			if (m_signal[j] > max)
//			{
//				max = m_signal[j];	// ������һ���Ǵֱ���
//			}
//			m_signal[j] = 127.0*m_signal[j] / max;	// ��С100��
//			j++;
//			QdivIn_1 = QdivIn;	// Q(n) / I(n) => Q(n-1) / I(n-1)
//		}
//
//		m_signalLen = j;
//		m_signalLock.unlock();
//		m_preQdivI = QdivIn_1;
//	}
//}


	//for (int i = 0; i < m_unDemodSignal.count() - 1; i = i + 2)
	//{
	//	//QdivIn = m_unDemodSignal[i + 1] / (m_unDemodSignal[i] + numZero);	// = Q(n) / I(n)
	//	//m_signal[j] = (QdivIn - QdivIn_1) / (1.0 + QdivIn * QdivIn);
	//	//if (m_signal[j] > max)
	//	//{
	//	//	max = m_signal[j];	// ������һ���Ǵֱ���
	//	//}
	//	//m_signal[j] = 127.0*m_signal[j] / max;	// ��С100��
	//	m_signal[j] = 50.0*qCos(2.0*3.14*0.003*j)+ m_unDemodSignal[1];
	//	j++;
	//	//QdivIn_1 = QdivIn;	// Q(n) / I(n) => Q(n-1) / I(n-1)
	//}

	// ����Ҫ�����Ƿ�Ӹ�������������������ظ���ȡһ��������



void DemodWorker::doWork(Demodulator *demod)
{
	demod->m_demodFM();
}