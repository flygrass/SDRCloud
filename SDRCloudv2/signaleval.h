#pragma once
#ifndef SIGNALEVAL_H
#define SIGNALEVAL_H

#include <QObject>

// �ź���������������������
class SignalEval : public QObject
{
	Q_OBJECT

public:
	SignalEval(QObject *parent);
	~SignalEval();
};

#endif // !SIGNALEVAL_H