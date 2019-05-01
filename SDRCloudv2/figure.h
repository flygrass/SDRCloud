#pragma once

#include <QObject>
#include <QQueue>
#include <QThread>
#define QCUSTOMPLOT_USE_LIBRARY
#include "qcustomplot.h"

QT_FORWARD_DECLARE_CLASS(QTimer)
QT_FORWARD_DECLARE_CLASS(Demodulator)
class Figure : public QCustomPlot
{
	Q_OBJECT

public:
	Figure(Demodulator *demod, QWidget *parent=nullptr);
	~Figure();
	friend class UpdateFigureBufWorker;

	void startRedrawing();
	void stopRedrawing();
	

private:
	void initFigure();	// �˺����ڳ������������ֻ࣬Ӧ��ִ��1��
	void m_updateFigureBuf();	// �˺���������
	void startUpdateFigureBuf();	///< �������ݵ����źŶ�figure��buf���и���,��û������ʱ���Լ�ͣ��
private slots:
	void updatePsdWave();	// ���¹�����ͼ
signals:
	void updateFigureBufSignal(Figure *fig);
private:
	Demodulator *m_demod;
	QTimer *redrawingFigureTimer;
	QQueue<qreal> m_figureBufX;
	QQueue<qreal> m_figureBufY;
	QVector<qreal> m_figureX;
	QVector<qreal> m_figureY;
	QReadWriteLock m_figLock;
	quint32 m_figureBufLen;
	QThread updateFigureBufThread;	// ����figure��buffer���߳�
	UpdateFigureBufWorker *updateFigureBufWorker;
};


class UpdateFigureBufWorker :public QObject
{
	Q_OBJECT
public slots:
	void doWork(Figure *fig);
signals:
	void done();
};