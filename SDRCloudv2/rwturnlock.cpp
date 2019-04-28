#include "rwturnlock.h"
#include <QMutex>
#include <QWaitCondition>

RWTurnLock::RWTurnLock()
{
	isFull = false;
	bufMutex = new QMutex;
	bufNotFull = new QWaitCondition;
	bufNotEmpty = new QWaitCondition;
}

RWTurnLock::~RWTurnLock()
{
	delete bufMutex;
	bufMutex = NULL;
	delete bufNotFull;
	bufNotFull = NULL;
	delete bufNotEmpty;
	bufNotEmpty = NULL;
}

void RWTurnLock::lockForWrite()
{
	bufMutex->lock();
	if (isFull == true)	// ע�������д��Ӧ��ʹ��while�����������жϣ���������ȷ���̱߳�����ʱ���͸�ִ��һ��
	{					// ͬʱҲ��Ϊ�˷�ֹ����һ���߳��˳�ʱ������һ���̱߳�����������ȥ
		bufNotFull->wait(bufMutex);
	}
	bufMutex->unlock();
}

void RWTurnLock::lockForRead()
{
	bufMutex->lock();
	if (isFull == false)
	{
		bufNotEmpty->wait(bufMutex);
	}
	bufMutex->unlock();
}

void RWTurnLock::unlock()
{
	bufMutex->lock();
	if (isFull == false)
	{
		// д���ֻ�������
		isFull = true;
		bufNotEmpty->wakeAll();
	}
	else
	{
		// �����ֻ���д��
		isFull = false;
		bufNotFull->wakeAll();
	}
	bufMutex->unlock();
}