#include "rtldevice.h"
#include "rtl-sdr.h"
#include <QDebug>
#include <QTimer>

// ��̬������ʼ��
QReadWriteLock RTLDevice::buf16RWLock;
quint32 RTLDevice::buf16Len = 0;
qint16  RTLDevice::buf16[MAXIMUM_BUF_LENGTH];

RTLDevice::RTLDevice(QQueue<qreal> *queueBufX, QQueue<qreal> *queueBufY, QObject *parent)
	: SDRDevice(queueBufX, queueBufY, parent)
{
	updBufTimer = new QTimer;
	connect(updBufTimer, SIGNAL(timeout()), this, SLOT(updateBuf()));
}

RTLDevice::~RTLDevice()
{
}

quint32 RTLDevice::getDeviceCount()
{
	return quint32(rtlsdr_get_device_count());
}

QString RTLDevice::getDeviceName(quint32 index)
{
	int ind;
	ind = (uint32_t)index;
	/*qDebug() << "����RTL��getDeviceName���ұ�ִ����" << ind;*/
	return QString(rtlsdr_get_device_name(ind));
}

/*!
* ������Ӧ�豸��USB��Ϣ
* \param manufact����������������ΪNULL��
* \param product: USB�豸�ͺ���������ΪNULL��
* \param serial: USB�豸���кţ�����ΪNULL��
* \return true on success
*/
bool RTLDevice::getDeviceUSBInfo(quint32 index, QString & manufact, QString & product, QString & serial)
{
	int r = 1;
	char strm[256];
	char strp[256];
	char strs[256];
	
	r = rtlsdr_get_device_usb_strings((uint32_t)index, strm, strp, strs);
	if (r == 0)
	{
		manufact = QString(strm);
		product = QString(strp);
		serial = QString(strs);
		return true;
	}
	return false;
}

bool RTLDevice::open(quint32 index)
{
	int r = 1;

	if (m_state == CLOSED)
	{
		r = rtlsdr_open((rtlsdr_dev_t**)&m_dev, (uint32_t)index);
		if (r == 0)
		{
			m_state = OPENED;
			return true;
		}
	}
	return false;
}

bool RTLDevice::close()
{
	int r = -1;
	if (m_state != CLOSED)
	{
		rtlsdr_cancel_async((rtlsdr_dev_t*)m_dev);	// ȡ���첽���ݵĶ�ȡ
		r = rtlsdr_close((rtlsdr_dev_t*)m_dev);
		if (r == 0)
		{
			m_state = CLOSED;
		}
	}

	return r == 0 ? true : false;	// ��Ч���գ����豸�ᵼ�²���ʧ��
}
// �˺����������첽���ݶ�ȡ�Ļص�
static void rtlsdr_callback(unsigned char *buf, uint32_t len, void *ctx)
{
	qDebug() << "call back is running";
	RTLDevice::buf16RWLock.lockForWrite();
	for (int i = 0; i < len; i++)
	{
		RTLDevice::buf16[i] = ((qint16)buf[i] - 127);
	}
	RTLDevice::buf16Len = len;
	RTLDevice::buf16RWLock.unlock();
}
bool RTLDevice::readAsyncData(quint16 bufNum, quint32 bufLen)
{
	int r = -1;
	rtlsdr_reset_buffer((rtlsdr_dev_t *)m_dev);
	_sleep(1);
	r = rtlsdr_read_async((rtlsdr_dev_t *)m_dev,
		rtlsdr_callback,
		nullptr,
		(uint32_t)bufNum ,
		(uint32_t)bufLen);
	qDebug() << "rtlsdr_read_async return";
	if (r == 0)
	{	// ����Ӧ���ڼ���һЩ�����������ܿ��������ʱ��
		updBufTimer->start(100);
	}
	qDebug() << "read async ret = :" << r;
	return r == 0 ? true : false;
}

void RTLDevice::updateBuf()
{
	qint64 range = 2000;
	int resolution = 16;
	quint32 dataLen = 0;

	RTLDevice::buf16RWLock.lockForRead();
	dataLen = buf16Len;
	for (int i = 0; i < dataLen / resolution; i++)
	{
		m_QueueBufY->enqueue((qreal)(buf16[resolution * i]));
	}
	RTLDevice::buf16RWLock.unlock();

	for (int i = m_QueueBufY->count(); i > range - dataLen; i--)
	{
		if (!m_QueueBufY->empty())
			m_QueueBufY->dequeue();
	}

	// ����bufX�����ݣ�1:count2
	int count2 = m_QueueBufY->count();
	while (!m_QueueBufX->empty())m_QueueBufX->dequeue();
	for (int i = 0; i < count2; i++)
	{
		m_QueueBufX->enqueue(i);
	}
}