#pragma once
/*
	������������
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma execution_character_set("utf-8")


#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
//#include <QQueue>
#include <stdio.h>
#include <stdlib.h>
//#include <QThread>
//#include <QReadWriteLock>
//#include <QBuffer>
//#include <QAudioOutput>


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
class QProgressBar;
QT_END_NAMESPACE
//QT_FORWARD_DECLARE_CLASS(QCustomPlot)
//QT_FORWARD_DECLARE_CLASS(SDRDevice)
QT_FORWARD_DECLARE_CLASS(Figure)
QT_FORWARD_DECLARE_CLASS(RtlDevice)
//QT_FORWARD_DECLARE_CLASS(QTimer)
QT_FORWARD_DECLARE_CLASS(QToolBox)
QT_FORWARD_DECLARE_CLASS(Demodulator)
//QT_FORWARD_DECLARE_CLASS(QAudioOutput)
//QT_FORWARD_DECLARE_CLASS(PCMIODevice)
QT_FORWARD_DECLARE_CLASS(SoundPlayer)
QT_FORWARD_DECLARE_CLASS(ImageViewer)
class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	static const QString demodMethodNames[8];
public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();
	friend class UpdateFigureBufWorker;
	
private:	// ˽�г�Ա����
	void iniUI();
	void iniLeftToolBoxUI();
	void iniStatusBar();
	void connectSignalSlot();
	void console(const QString &info);
	void displayDefaultConfig();	// ��Ĭ�ϲ�����ʾ�����������  
	void stopRtl();
	void startRtl();

private slots:	//˽�вۺ���
	void openRTL(bool b);
	void closeRTL(bool b);
	//void updatePsdWave();	// ���¹�����ͼ
	void showMenuRTL();
	void btnStartPlaySlot();	// ��ʼ���Ű�ť�ۺ���
	void lostDevice(QString info);
	void openImgWindow(bool b);

signals:
	/*to do*/

private:	// ���������Ա����
	Ui::MainWindowClass ui;
	// ��Ҫһֱ����Ľ������
	QWidget *div;	// ��������
	QDockWidget *dockConsole;	// ���ɿ���̨�ĸ�������
	QDockWidget *dockWaterfall;	// �����ٲ�ͼ�ĸ�������
	QDockWidget *dockLeft;	// ��ม������
	QTextEdit *txtConsole;	// ����ؼ�
	Figure *figure;	/*QCustomPlot *figure;*/
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
	QComboBox *comboxRecord;
	QProgressBar *pgbRecord;	// ��ʾ¼��ʱ���Ľ�����
	QPushButton *btnClearRecord; // �����¼������
	QLabel *labDeviceName;
	ImageViewer *imgViewer;

	

private:	// �����Գ�Ա����
	RtlDevice *m_dongle;	// RTLӲ������
	Demodulator *m_demod;	// ���������
	SoundPlayer *m_player;	// ������
	//FILE *m_logFile;	// Debug��־�ļ�
	quint32 m_recordTime;	// ��Ƶ¼��ʱ��
	quint32 m_currentRecordTime;	// ��¼����Ƶʱ��
};







#endif // MAINWINDOW_H