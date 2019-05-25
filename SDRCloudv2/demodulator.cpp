#include "demodulator.h"
#include "rtldevice.h"
#include "qmath.h"
#include "demod_core.h"
#include "firimport.h"
#include "iir.h"
#include <QQueue>
#include <QFile>
#include <QDataStream>


Demodulator::Demodulator(RtlDevice *dongle, QObject *parent)
	: QObject(parent),
	m_preQdivI(0.0),
	m_signalLen(0),
	m_dongle(dongle),
	scaleFactor(20)
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

int Demodulator::getScaleFactor()
{
	return scaleFactor;
}

int Demodulator::setScaleFactor(int factor)
{
	scaleFactor = factor;
	return factor;
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
		if (m_signal[i] > 127) {
			data[i] = char(127);
		}
		else if (m_signal[i] < -128) {
			data[i] = char(-128);
		}
		else {
			data[i] = char(m_signal[i]);
		}
		
	}
	len = i;
	m_signalLock.unlock();
}


void Demodulator::m_demodFM()
{
	quint32 result1Count = 0;
	QQueue<qreal> result1;

	qreal preI = 0.0;
	qreal preQ = 0.0;

	qreal I = 0.0;
	qreal Q = 0.0;
	qreal reM = 0.0;
	qreal imM = 0.0;

	//QFile audioFile("E:/workspace/Qt/SDRCloud/waves/wave01.dat");
	//audioFile.open(QIODevice::WriteOnly);
	//QDataStream out(&audioFile);


	

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
		
		for (int i = 0; i < m_unDemodSignal.count() - 1; i = i + 2)
		{
			// FM���
			I = m_unDemodSignal[i];
			Q = m_unDemodSignal[i + 1];

			reM = I * preI + Q * preQ;
			imM = Q * preI - I * preQ;

			result1.enqueue(qAtan2(imM, reM));

			preI = I;
			preQ = Q;
		}
		result1Count = result1.count();


		// �Խ����Ľ������FIR�˲�(hFilterLen)============================
		//int i = 0;
		//int j = 0;
		//qreal sum = 0;
		//m_signalLock.lockForWrite();
		//for (i = 0; i < result1Count - FIR_LEN; i++)
		//{
		//	// ����hFileterLen��Ԫ��Ҫ�ȴ���һ�����ݲ��ܱ��˲�
		//	sum = 0;
	
		//	for (j = 0; j < HALF_FIR_LEN; j++)
		//	{
		//		sum += hFir[j] * (result1[j]+result1[FIR_LEN -1-j]);

		//	}

		//	if (hFirLen % 2 != 0)
		//	{
		//		sum += hFir[j] * result1[j];
		//	}
		//	/*qDebug() << QString::number(sum);*/
		//	m_signal[i] = sum;
		//	//qDebug() << QString::number(hFir[2]);

		//	if (result1.isEmpty() == false)
		//	{
		//		result1.dequeue();
		//	}
		//}

		//m_signalLen = i;
		////qDebug() << QString::number(m_signal[2]);
		//m_signalLock.unlock();
		////=============================================

		// ���˲�============================
		m_signalLock.lockForWrite();
		int i = 0;
		for (i = 0; i < result1Count; i++)
		{
			m_signal[i] = scaleFactor*result1.dequeue();
			if (m_signal[i] >= 128.0 || m_signal[i] < -128) {
				qDebug() << "*" << m_signal[i];
			}
			/*m_signal[i] = 5.0*qCos(2*3.14*50*i*(1/48000.0));*/
			/*out << m_signal[i];*/
		}
		m_signalLen = i;

		m_signalLock.unlock();
		//===================================

		// IIR�˲�============================
		//m_signalLock.lockForWrite();
		//iir(result1, m_signal, m_signalLen);
		//m_signalLock.unlock();
		//===================================
	}
}


void DemodWorker::doWork(Demodulator *demod)
{
	demod->m_demodFM();
}