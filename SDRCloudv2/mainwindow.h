#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma execution_character_set("utf-8")


#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include <QQueue>


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
QT_END_NAMESPACE
QT_FORWARD_DECLARE_CLASS(QCustomPlot)
QT_FORWARD_DECLARE_CLASS(SDRDevice)
QT_FORWARD_DECLARE_CLASS(QTimer)
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
private:	// ˽�г�Ա����
	void iniUI();
	void connectSignalSlot();
	void console(const QString &info);
private slots:	//˽�вۺ���
	void openRTL(bool b);
	void closeRTL(bool b);
	void updateWave();
	void showMenuRTL();

private:	// ���������Ա����
	Ui::MainWindowClass ui;
	// ��Ҫһֱ����Ľ������
	QWidget *div;	// ��������
	QDockWidget *dockConsole;	// ���ɿ���̨�ĸ�������
	QDockWidget *dockWaterfall;	// �����ٲ�ͼ�ĸ�������
	QDockWidget *dockLeft;	// ��ม������
	QTextEdit *txtConsole;	// ����ؼ�
	QCustomPlot *figure;

private:	// �����Գ�Ա����
	SDRDevice *m_dongle;	// SDRӲ���������
	QQueue<qreal> *m_QueueBufX;	// ���ݻ������
	QQueue<qreal> *m_QueueBufY;
	QVector<qreal> *m_bufX;	// ���ݻ�����У�vector��ʽ������ʱ������Ϊ��ͼ�ε���ʾ
	QVector<qreal> *m_bufY;
	QTimer *updWaveTimer;
};
#endif // MAINWINDOW_H