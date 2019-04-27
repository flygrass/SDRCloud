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
class RWTurnLock
{
public:
	RWTurnLock();
	~RWTurnLock();
	void lockForWrite();
	void lockForRead();
	void unlock();
private:
	bool isFull;
	QMutex *bufMutex;
	QWaitCondition *bufNotFull;
	QWaitCondition *bufNotEmpty;
};

#endif // !RWTURNLOCK_H
