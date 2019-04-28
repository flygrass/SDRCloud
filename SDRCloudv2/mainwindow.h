#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma execution_character_set("utf-8")


#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include <QQueue>
#include <stdio.h>
#include <stdlib.h>
#include <QThread>
#include <QReadWriteLock>
#include <QBuffer>
#include <QAudioOutput>


QT_BEGIN_NAMESPACE
class QAction;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QMenuBar;
class QPushButton;
class QTextEdit;
class QRadioButton;
class QDoubleSpinBox;
class QSlider;
class QCheckBox;
class QComboBox;
QT_END_NAMESPACE
QT_FORWARD_DECLARE_CLASS(QCustomPlot)
//QT_FORWARD_DECLARE_CLASS(SDRDevice)
QT_FORWARD_DECLARE_CLASS(RtlDevice)
QT_FORWARD_DECLARE_CLASS(QTimer)
QT_FORWARD_DECLARE_CLASS(QToolBox)
QT_FORWARD_DECLARE_CLASS(Demodulator)
QT_FORWARD_DECLARE_CLASS(QAudioOutput)
QT_FORWARD_DECLARE_CLASS(PCMIODevice)
class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	static const QString demodMethodNames[8];
public:
	MainWindow(FILE *logFile, QWidget *parent = Q_NULLPTR);
	~MainWindow();
	friend class UpdateFigureBufWorker;
	friend class UpdateAudioBufWorker;

private:	// ˽�г�Ա����
	void iniUI();
	void iniLeftToolBoxUI();
	void iniStatusBar();
	void connectSignalSlot();
	void console(const QString &info);
	void displayDefaultConfig();	// ��Ĭ�ϲ�����ʾ�����������  
	void initFigure();	// �˺����ڳ������������ֻ࣬Ӧ��ִ��1��
	void initAudioPlayer();
	void stopReceiveData();
	void startReceiveData();
	void startAudioPlay();
	void stopAudioPlay();
	
private:	// ��Щ������Ҫֱ�ӵ��ã��������ӿں���
	void updateFigureBuf();	// �˺���������
	void updateAudioBuf();	// �˺�����������Ӧ��ֱ�ӵ���
private slots:	//˽�вۺ���
	void openRTL(bool b);
	void closeRTL(bool b);
	void updatePsdWave();	// ���¹�����ͼ
	void showMenuRTL();
	void btnStartPlaySlot();	// ��ʼ���Ű�ť�ۺ���
	void handleStateChanged(QAudio::State newState);
signals:
	void updateFigureBufSignal(MainWindow *w);
	void updateAudioBufSignal(MainWindow *w);
private:	// ���������Ա����
	Ui::MainWindowClass ui;
	// ��Ҫһֱ����Ľ������
	QWidget *div;	// ��������
	QDockWidget *dockConsole;	// ���ɿ���̨�ĸ�������
	QDockWidget *dockWaterfall;	// �����ٲ�ͼ�ĸ�������
	QDockWidget *dockLeft;	// ��ม������
	QTextEdit *txtConsole;	// ����ؼ�
	QCustomPlot *figure;
	QToolBox *leftToolBox;
	QPushButton *btnStartPlay;
	QRadioButton *rbtnDemodMode[8];
	QDoubleSpinBox *sboxTunerFreq;	// ����tuner��Ƶ��
	QSlider *slideRFGain;
	QSlider *slideIFGain;
	QCheckBox *chkboxOffsetTuning;
	QCheckBox *chkboxTunerAGC;	// ������Ƶı�������tuner�����浽�����ֹ����ƻ����Զ�����(manual or automatic)
	QCheckBox *chkboxRtlAGC;
	QComboBox *comboxSampleRate;
	QComboBox *comboxSampleMode;
	QLabel *labDeviceName;
	

private:	// �����Գ�Ա����
	//SDRDevice *m_dongle;	// SDRӲ���������
	//QQueue<qreal> *m_QueueBufX;	// ���ݻ������
	//QQueue<qreal> *m_QueueBufY;
	//QVector<qreal> *m_bufX;	// ���ݻ�����У�vector��ʽ������ʱ������Ϊ��ͼ�ε���ʾ
	//QVector<qreal> *m_bufY;
	QTimer *updateFigureTimer;
	RtlDevice *m_dongle;	// RTLӲ������
	Demodulator *m_demod;	// ���������
	FILE *m_logFile;
	QQueue<qreal> m_figureBufX;
	QQueue<qreal> m_figureBufY;
	QVector<qreal> m_figureX;
	QVector<qreal> m_figureY;
	QReadWriteLock m_figLock;
	quint32 m_figureBufLen;
	QThread updateFigureBufThread;	// ����figure��buffer���߳�
	UpdateFigureBufWorker *updateFigureBufWorker;
	UpdateAudioBufWorker *updateAudioBufWorker;
	QThread updateAudioBufThread;	// ����audio��buffer���߳�
	// ��������
	QAudioOutput *m_audio;
	/*QBuffer m_audioBuffer;*/
	PCMIODevice *m_pcmIODevice;
};



class UpdateFigureBufWorker :public QObject
{
	Q_OBJECT
public slots:
	void doWork(MainWindow *w);
signals:
	void done();
};


class UpdateAudioBufWorker :public QObject
{
	Q_OBJECT
public slots:
	void doWork(MainWindow *w);
signals:
	void done();
};
#endif // MAINWINDOW_H