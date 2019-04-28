#pragma once
#ifndef RWTURNLOCK_H
#define RWTURNLOCK_H

#include <QObject>
QT_FORWARD_DECLARE_CLASS(QWaitCondition);
QT_FORWARD_DECLARE_CLASS(QMutex);
// RWTurnLock������-д���ֻ���
// Description����������ڶ���д����Ҫ�ϸ��ֻ�����һ�鹲���ڴ�ĳ���
// Public Method:
// lockForWrite()
// lockForRead
// unlock
// releaseLock
class RWTurnLock
{
public:
	RWTurnLock();
	~RWTurnLock();
	void lockForWrite();
	void lockForRead();
	void unlock();
	//void releaseLock();	// �ͷŵ�������������´����ã���ȷ���ٴ��������ʱ���ﲻ���ٳ��־�̬����
private:
	bool isFull;
	QMutex *bufMutex;
	QWaitCondition *bufNotFull;
	QWaitCondition *bufNotEmpty;
};

#endif // !RWTURNLOCK_H
