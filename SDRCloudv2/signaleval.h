#pragma once
/*
	�ź���������������������
*/
#ifndef SIGNALEVAL_H
#define SIGNALEVAL_H

#include <QObject>

class SignalEval : public QObject
{
	Q_OBJECT

public:
	SignalEval(QObject *parent);
	~SignalEval();
};

#endif // !SIGNALEVAL_H