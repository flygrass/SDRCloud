#include "pcmiodevice.h"
#include "constant.h"
#include <QDebug>
#include "waverecorder.h"


PCMIODevice::PCMIODevice(WaveRecorder *recorder) :
	curRead(0),
	curWrite(0),
	isForWrite(true),
	validDataLength(0),
	m_recorder(recorder),
	isRecordOn(false)
{
	for (int i = 0; i < DEFAULT_MAX_AUDIO_BUFFER_SIZE; i++)
	{
		m_buffer[i] = 2;
	}
	this->open(QIODevice::ReadOnly);
}

PCMIODevice::~PCMIODevice()
{
	this->close();
}

qint64 PCMIODevice::readData(char * data, qint64 maxlen)
{
	qint64 chunk = 0;
	qint64 pos = 0;	// data�ĵ�ǰλ��ָ��
	/*qDebug() << "audio read data" + QString::number(maxlen);*/
	
	mutex.lock();
	if (isForWrite == true)
	{
		notForWrite.wait(&mutex);	// �����������Ҫ��Ϊ������ִ�е�̫���writeData����
									// ͬʱ�������п��ܳ��ֶ��ߣ�д�ߣ���ȥ������һ���������ġ�
	}
	mutex.unlock();

	if (maxlen > validDataLength)
	{
		maxlen = validDataLength;	// ���ʣ����Ч���ݲ���
	}
	for (qint64 len = maxlen; len > 0; pos += chunk, len -= chunk)
	{
		chunk = len > (DEFAULT_MAX_AUDIO_BUFFER_SIZE - curRead) ? (DEFAULT_MAX_AUDIO_BUFFER_SIZE - curRead) : len;
		memcpy(data+pos, m_buffer+curRead, chunk);
		curRead = (curRead + chunk) % DEFAULT_MAX_AUDIO_BUFFER_SIZE;	
	}

	validDataLength -= (quint64)pos;
	// ��ȡ������Ƶ���ݶ���data����䳤��Ϊpos
	if (isRecordOn) {
		m_recorder->cacheWave(data, pos);	// ����Ƶ���ݽ��л���
	}
	mutex.lock();
	isForWrite = true;
	notForRead.wakeAll();
	mutex.unlock();

	if (pos == 0)
	{
		data[0] = (char)0;
		pos = 1;	// ��������������������״̬����Ȼд�߻ᱻ�¶�����ס
	}
	//qDebug() << QString::number(pos) + data[0];
	return pos;
}

qint64 PCMIODevice::writeData(const char * data, qint64 len)
{
	qint64 chunk = 0;
	qint64 pos = 0;	// data�ĵ�ǰλ��ָ��

	mutex.lock();
	if (isForWrite == false)
	{
		notForRead.wait(&mutex);
	}
	mutex.unlock();

	for (qint64 ilen = len; ilen > 0; pos += chunk, ilen -= chunk)
	{
		chunk = ilen > (DEFAULT_MAX_AUDIO_BUFFER_SIZE - curWrite) ? (DEFAULT_MAX_AUDIO_BUFFER_SIZE - curWrite) : ilen;
		memcpy(m_buffer + curWrite, data + pos, chunk);
		curWrite = (curWrite + chunk) % DEFAULT_MAX_AUDIO_BUFFER_SIZE;
	}
	validDataLength += quint64(pos);

	mutex.lock();
	isForWrite = false;
	notForWrite.wakeAll();
	mutex.unlock();

	return pos;
}

void PCMIODevice::stopUpdate()
{
	notForRead.wakeAll();
	notForWrite.wakeAll();
}

void PCMIODevice::setRecordState(bool on)
{
	isRecordOn = on;
}
