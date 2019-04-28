#include "demodulator.h"
#include "rtldevice2.h"
Demodulator::Demodulator(RtlDevice *dongle, QObject *parent)
	: QObject(parent),
	m_preQdivI(0.0),
	m_signalLen(0),
	m_dongle(dongle)
{
	worker = new DemodWorker();	// ������ᱻ�ظ�ִ�У����Բ����ж��Ƿ��ظ�ʵ����
	worker->moveToThread(&m_demodThread);
	connect(this, &Demodulator::m_demodFMSignal, worker, &DemodWorker::doWork);
	m_demodThread.start();
}

Demodulator::~Demodulator()
{
	m_demodThread.wait();
}

void Demodulator::startDemodFM()
{
	emit m_demodFMSignal(this);
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
void Demodulator::getDataByChar(char data[], quint32 len)
{
	// ���������Ƿ���Ҫһ���������ȴ��������ظ���ȡ����
	m_signalLock.lockForRead();
	for (int i = 0; i < m_signalLen; i++)
	{
		data[i] = (char)m_signal[i];
	}
	m_signalLock.unlock();
}

void Demodulator::m_demodFM()
{
	qreal QdivIn_1 = 0.0;
	qreal QdivIn = 0.0;
	qreal numZero = 1e-4;
	quint32 j = 0;
	qreal max = 0.1;
	while (true)
	{
		// ���������ֹͣ���������̵߳�����
		if (m_dongle->getState() == RtlDevice::CANCELLING)
		{
			break;
		}
		
		m_dongle->getData(m_unDemodSignal);	//	������ȡӲ�����ԭʼIQ���ݣ�ע�������п�������
											// m_unDemodSignal�е�����-128-127֮��

		// ע:���ֽ���㷨�������󣬾��ȸ�

		QdivIn_1 = m_preQdivI;
		j = 0;
		
		m_signalLock.lockForWrite();
		for (int i = 0; i < m_unDemodSignal.count()-1; i = i + 2)
		{
			QdivIn = m_unDemodSignal[i+1] / (m_unDemodSignal[i] + numZero);	// = Q(n) / I(n)
			m_signal[j] = (QdivIn - QdivIn_1) / (1.0 + QdivIn * QdivIn);
			if (m_signal[j] > max)
			{
				max = m_signal[j];	// ������һ���Ǵֱ���
			}
			m_signal[j] = 127.0*m_signal[j] / max;	// ��С100��
			j++;
			QdivIn_1 = QdivIn;	// Q(n) / I(n) => Q(n-1) / I(n-1)
		}
		m_signalLen = j;
		m_signalLock.unlock();
		// ����Ҫ�����Ƿ�Ӹ�������������������ظ���ȡһ��������

		m_preQdivI = QdivIn_1;
	}
}


void DemodWorker::doWork(Demodulator *demod)
{
	demod->m_demodFM();
}