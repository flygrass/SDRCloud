#pragma once
#ifndef READWRITEBUFFER_H
#define READWRITEBUFFER_H

#include <QObject>
#include <QVector>
#include "rwturnlock.h"
#include <QDebug>

#define DEFAULT_BUFFERSIZE 16*16384

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
	m_lock.lockForWrite();
	for (int i = 0; i < len; i++)
	{
		m_buf[i] = data[i];
	}
	m_len = len;
	m_lock.unlock();
}

template<class T>
void ReadWriteBuffer<T>::writeChar(const unsigned char *data, quint32 len)
{
	m_lock.lockForWrite();
	qDebug() << "write char";
	for (int i = 0; i < len; i++)
	{
		m_buf[i] = (T)(((qint16)data[i] - 127.0) / 128.0);
	}
	m_len = len;
	m_lock.unlock();
}

template<class T>
void ReadWriteBuffer<T>::read(QVector<T>& data)
{
	m_lock.lockForRead();
	qDebug() << "ReadWrite buffer read ";
	data.resize(0);
	for (int i = 0; i < m_len; i++)
	{
		data.append(m_buf[i]);
	}
	m_lock.unlock();
}

#endif // !READWRITEBUFFER_H