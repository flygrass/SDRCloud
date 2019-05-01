#pragma once
/*
	������дbuffer�࣬�Դ���
*/
#ifndef READWRITEBUFFER_H
#define READWRITEBUFFER_H

#include <QObject>
#include <QVector>
#include "rwturnlock.h"
#include <QDebug>
#include "constant.h"

#define DEFAULT_BUFFERSIZE 16*16384
static quint32 _DEFAULT_SAMPLERATES[11] = {
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
	250e3 };

static quint32 downSampleRate = (quint32)(2*CONFIG_DOWNSAMPLE_RATE_BEFORE_DEMOD);
template <class T>
class ReadWriteBuffer
{

public:
	ReadWriteBuffer();
public:
	void write(const T data[], quint32 len);	// ����buffer������
	void writeChar(const unsigned char *data, quint32 len);	// ����buffer������
	//void write(const QVector<T> &data, quint32 len);
	void read(QVector<T> &data);	// ��ȡbuffer������

private:
	RWTurnLock m_lock;	// ��д������
	T m_buf[DEFAULT_BUFFERSIZE];	// ������
	quint32 m_len;	// ���������ݳ���
};



template<class T>
ReadWriteBuffer<T>::ReadWriteBuffer()
{
	m_len = 0;
	for (int i = 0; i < DEFAULT_BUFFERSIZE; i++)
	{
		m_buf[i] = (T)0;
	}
}

template<class T>
void ReadWriteBuffer<T>::write(const T data[], quint32 len)
{
	quint32 j = 0;
	m_lock.lockForWrite();
	for (int i = 0; i < len-1; i = i+downSampleRate)
	{
		m_buf[j] = data[i];	// �²���ʱ��ȷ��I,Q����
		j++;
		m_buf[j] = data[i + 1];
		j++;
	}
	m_len = j;
	m_lock.unlock();
}

template<class T>
void ReadWriteBuffer<T>::writeChar(const unsigned char *data, quint32 len)
{
	quint32 j = 0;
	m_lock.lockForWrite();
	//qDebug() << "write char";
	for (int i = 0; i < len-1; i = i + downSampleRate)	// �������һ�����²���
	{
		/*m_buf[i] = (T)(((qint16)data[i] - 127.0) / 128.0);*/
		m_buf[j] = (T)((qint16)data[i] - 127.0);
		j++;
		m_buf[j] = (T)((qint16)data[i + 1] - 127.0);	// ȷ��i+1��ҪԽ��
		j++;
	}
	m_len = j;
	m_lock.unlock();
}

template<class T>
void ReadWriteBuffer<T>::read(QVector<T>& data)
{
	m_lock.lockForRead();
	//qDebug() << "ReadWrite buffer read ";
	data.resize(0);
	for (int i = 0; i < m_len; i++)
	{
		data.append(m_buf[i]);
	}
	m_lock.unlock();
}

#endif // !READWRITEBUFFER_H