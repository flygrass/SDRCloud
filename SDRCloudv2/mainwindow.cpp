#include "mainwindow.h"
#include <QDockWidget>
#include <QTextEdit>
#include <QMessageBox>
#include "qcustomplot.h"
#include "rtldevice2.h"
#include "dialogopendev.h"
#include <QTimer>
#include <QToolBox>
#include <QRadioButton>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QCheckBox>
#include <QComboBox>


//const QString MainWindow::demodMethodNames[8] = { tr("NFM"), tr("WFM"),tr("AM"),tr("DSB"),tr("USB"),tr("LSB") ,tr("RAW"),tr("CW") };
const QString MainWindow::demodMethodNames[8] = { "NFM", "WFM","AM","DSB","USB","LSB" ,"RAW","CW" };
MainWindow::MainWindow(FILE *logFile, QWidget *parent)
	: QMainWindow(parent),
	m_logFile(logFile),
	updateFigureBufWorker(nullptr)
{
	ui.setupUi(this);
	setWindowTitle(tr("SDRCloud"));
	iniUI();	// �����ʼ��UI
	

	// ��ʽ��ʼ����Ա����
	//m_QueueBufX = new QQueue<qreal>;
	//m_QueueBufY = new QQueue<qreal>;
	//m_bufX = new QVector<qreal>;
	//m_bufY = new QVector<qreal>;
	//m_dongle = new RTLDevice(m_QueueBufX, m_QueueBufY, m_bufX, m_bufY, 0);
	//updWaveTimer = new QTimer;
	m_dongle = new RtlDevice(nullptr);


	connectSignalSlot();	// ���������źźͲ�
	
}

void MainWindow::iniUI()
{
	// ��ʼ��������
	resize(1280, 840);
	setWindowTitle(tr("SDRCloud"));
	//// ʵ�����ͻ�����������
	//div = new QWidget;	// ��ʱ�����ã������Ҫ�ڿͻ�ȥ���и��Ӳ���������
	//setCentralWidget(div);
	
	// ���customplot������ͻ���
	initFigure();
	

	// ʵ�����·�����̨��������
	dockConsole = new QDockWidget(tr("��Ϣ"));
	dockConsole->setAllowedAreas(Qt::BottomDockWidgetArea);
	//dockConsole->setFeatures(QDockWidget::NoDockWidgetFeatures);
	// ʵ��������̨���
	txtConsole = new QTextEdit;
	txtConsole->setMinimumSize(300, 100);
	dockConsole->setWidget(txtConsole);

	// ʵ�����ٲ�ͼ��������
	dockWaterfall = new QDockWidget;
	dockWaterfall->setAllowedAreas(Qt::BottomDockWidgetArea);
	dockWaterfall->setFeatures(QDockWidget::NoDockWidgetFeatures);

	// ʵ������ม������
	dockLeft = new QDockWidget;
	dockLeft->setAllowedAreas(Qt::LeftDockWidgetArea);
	dockLeft->setFeatures(QDockWidget::NoDockWidgetFeatures);
	
	// ����DockWidget
	addDockWidget(Qt::LeftDockWidgetArea, dockLeft, Qt::Vertical);
	addDockWidget(Qt::BottomDockWidgetArea, dockWaterfall, Qt::Vertical);	// ������Dock��ֱ����
	addDockWidget(Qt::BottomDockWidgetArea, dockConsole, Qt::Vertical);

	iniLeftToolBoxUI();
}

void MainWindow::connectSignalSlot()
{
	// ����RTL->��RTL�豸action����ۺ���
	connect(ui.actOpenRTL, SIGNAL(triggered(bool)), this, SLOT(openRTL(bool)));
	// ���Ӽ�ʱ������figure����ͼ
	//connect(updWaveTimer, SIGNAL(timeout()), this, SLOT(updateWave()));
	// ���ӹر�RTL�豸��action�Ͳۺ���
	connect(ui.actCloseRTL, SIGNAL(triggered(bool)), this, SLOT(closeRTL(bool)));
	// ����menuRTL����ʾ�ۺ���
	connect(ui.menuRTL, SIGNAL(aboutToShow()), this, SLOT(showMenuRTL()));
	// ������ʼ��������Ӳ���϶�ȡ����
	connect(btnStartPlay, SIGNAL(clicked()), this, SLOT(btnStartPlaySlot()));
}

void MainWindow::openRTL(bool b)
{
	Q_UNUSED(b);
	if (m_dongle->getState() != RtlDevice::CLOSED)
	{
		/*qDebug() << "dongle state=" << m_dongle->getState();*/
		QMessageBox::warning(this, tr("����"), tr("��ǰ�Ѿ�����һ���򿪵�RTL�豸�����ȹر����ٳ��Դ�"));
	}	// !if (m_dongle
	else
	{
		quint32 index = 0;
		int ret = QDialog::Rejected;
		QListWidgetItem *item = nullptr;
		DialogOpenDev *dialogDev = new DialogOpenDev(m_dongle, this);
		
		ret = dialogDev->exec();
		if (ret == QDialog::Accepted)
		{
			item = dialogDev->getSelectedItem();
			if (item != nullptr)
			{	// ���������item�����ǿյ�
				index = (quint32)item->data(Qt::UserRole).toInt();
				QMessageBox::information(this, tr("�ɹ�"), QString::asprintf("��򿪵��豸�����ǣ�%d", index));
				// todo:������ʵ�ʴ�RTL�豸���п��ܳɹ�Ҳ����ʧ��
				// �豸�򿪳ɹ���Ӧ�������ｫ�豸״̬���ģ�������ظ���
				if (m_dongle->open(index))
				{
					displayDefaultConfig();
				}
				else
				{
					console(QString("�豸��ʧ��"));
				}
				/*if (m_dongle->open(index))
				{
					console(QString("�豸�ɹ���"));
					if (m_dongle->readAsyncData())
					{
						console(QString("�豸��ʼ�첽��ȡ����"));
						updWaveTimer->start(100);
					}
					else
					{
						console(QString("�豸�첽��ȡ����ʧ��"));
					}
					
				}
				else
				{
					console(QString("�豸��ʧ��"));
				}*/
			}

		}
		// ����Ի���
		delete dialogDev;
	}	// !else m_dongle
}

//void MainWindow::updateWave()
//{
//	//QVector<qreal> x(101);
//	//QVector<qreal> y(100);
//	//for (int i = 0; i < 100; i++)
//	//{
//	//	x.append(i);
//	//	y.append(50);
//	//}
//	//x.append(100);
//	/*figure->graph(0)->setData(m_QueueBufX->toVector(), m_QueueBufY->toVector(), true);*/
//	/*figure->graph(0)->setData(x, y, true);*/
//
//	//figure->graph(0)->setData(*m_bufX, *m_bufY, true);
//	//figure->replot();
//	
//}

void MainWindow::console(const QString & info)
{
	txtConsole->append(info + "\n");
}

void MainWindow::showMenuRTL()
{
	if (m_dongle->getState() == RtlDevice::CLOSED)
	{
		ui.actCloseRTL->setEnabled(false);
	}
	else
	{
		ui.actCloseRTL->setEnabled(true);
	}
}

void MainWindow::closeRTL(bool b)
{
	Q_UNUSED(b);
	if (m_dongle->close())
	{
		console(QString("�ɹ��ر�RTL�豸"));
	}
	else
	{
		console(QString("�ر�RTL�豸ʧ��"));
	}
}

void MainWindow::iniLeftToolBoxUI()
{
	// ����߹��������г�ʼ��������================================================================
	// ������ʼ���ŵİ�ť
	btnStartPlay = new QPushButton(tr("��ʼ����"));
	btnStartPlay->setStyleSheet(
		//����״̬��ʽ
		"QPushButton{"
		"background-color:rgba(100,225,100,30);"//����ɫ��Ҳ��������ͼƬ��
		"border-style:outset;"                  //�߿���ʽ��inset/outset��
		"border-width:4px;"                     //�߿�������
		"border-radius:10px;"                   //�߿�Բ�ǰ뾶����
		"border-color:rgba(150,150,150,200);"    //�߿���ɫ
		"font:bold 10px;"                       //���壬�����С
		"color:rgba(0,0,0,100);"                //������ɫ
		"padding:6px;"                          //���
		"}"
		//��갴����ʽ
		"QPushButton:pressed{"
		"background-color:rgba(100,255,100,200);"
		"border-color:rgba(255,255,255,30);"
		"border-style:inset;"
		"color:rgba(0,0,0,100);"
		"}"
		//�����ͣ��ʽ
		"QPushButton:hover{"
		"background-color:rgba(100,255,100,100);"
		"border-color:rgba(255,255,255,200);"
		"color:rgba(0,0,0,200);"
		"}");
	dockLeft->setTitleBarWidget(btnStartPlay);
	leftToolBox = new QToolBox;
	// 1.���������ʽѡ��ĵ�ѡ��ť��
	QHBoxLayout *demodLayout = new QHBoxLayout;
	// �ȴ����ĸ��в���
	QVBoxLayout *demodLayoutc[4];
	for (int i = 0; i < 4; i++)
	{
		demodLayoutc[i] = new QVBoxLayout;
	}
	QGroupBox *groupDemodBtns = new QGroupBox;

	for (int i = 0; i < 8; i++)
	{
		rbtnDemodMode[i] = new QRadioButton(MainWindow::demodMethodNames[i]);


		demodLayoutc[qFloor(i / 2.0)]->addWidget(rbtnDemodMode[i]);
	}

	for (int i = 0; i < 4; i++)
	{
		demodLayoutc[i]->addStretch();
		demodLayout->addLayout(demodLayoutc[i]);
	}
	demodLayout->addStretch();
	groupDemodBtns->setLayout(demodLayout);

	leftToolBox->setMinimumWidth(250);
	leftToolBox->addItem(groupDemodBtns, tr("�������"));

	// 2.����tuner���õ���ذ�ť
	QGroupBox *groupTunerSetting = new QGroupBox;
	QVBoxLayout *tunerSettingLayout = new QVBoxLayout;
	QHBoxLayout *tsLayout1 = new QHBoxLayout;
	QLabel *labTunerFreq = new QLabel(tr("RFƵ��:"));
	sboxTunerFreq = new QDoubleSpinBox;
	tsLayout1->addWidget(labTunerFreq);
	tsLayout1->addWidget(sboxTunerFreq);

	QLabel *labRFGain = new QLabel(tr("RF����"));
	slideRFGain = new QSlider(Qt::Horizontal);

	QLabel *labIFGain = new QLabel(tr("IF����"));
	slideIFGain = new QSlider(Qt::Horizontal);

	chkboxOffsetTuning = new QCheckBox(tr("Offset Tuning"));
	chkboxOffsetTuning->setLayoutDirection(Qt::RightToLeft);
	// ��Ӷ�AGC�Ŀ���
	QGroupBox *groupAGCSetting = new QGroupBox(tr("AGC����"));
	QHBoxLayout *agcSettingLayout = new QHBoxLayout;
	chkboxTunerAGC = new QCheckBox(tr("Tuner AGC"));
	chkboxTunerAGC->setLayoutDirection(Qt::RightToLeft);
	chkboxRtlAGC = new QCheckBox(tr("RTL AGC"));
	chkboxRtlAGC->setLayoutDirection(Qt::RightToLeft);
	agcSettingLayout->addWidget(chkboxTunerAGC);
	agcSettingLayout->addWidget(chkboxRtlAGC);
	groupAGCSetting->setLayout(agcSettingLayout);

	tunerSettingLayout->addLayout(tsLayout1);
	tunerSettingLayout->addWidget(labRFGain);
	tunerSettingLayout->addWidget(slideRFGain);
	tunerSettingLayout->addWidget(labIFGain);
	tunerSettingLayout->addWidget(slideIFGain);
	tunerSettingLayout->addWidget(groupAGCSetting);
	tunerSettingLayout->addWidget(chkboxOffsetTuning, 0, Qt::AlignHCenter);
	tunerSettingLayout->addStretch();

	groupTunerSetting->setLayout(tunerSettingLayout);
	leftToolBox->addItem(groupTunerSetting, tr("tuner����"));

	// 2.����rtl2832u���õ���ذ�ť
	QGroupBox *groupRtlSetting = new QGroupBox;
	QVBoxLayout *rtlSettingLayout = new QVBoxLayout;

	QGroupBox *groupSampleRateContainer = new QGroupBox(tr("����������"));
	QVBoxLayout *sampleRateLayout = new QVBoxLayout;
	comboxSampleRate = new QComboBox;
	sampleRateLayout->addWidget(comboxSampleRate);
	groupSampleRateContainer->setLayout(sampleRateLayout);

	QGroupBox *groupSampleModeContainer = new QGroupBox(tr("����ģʽ����"));
	QVBoxLayout *sampleModeLayout = new QVBoxLayout;
	comboxSampleMode = new QComboBox;
	sampleModeLayout->addWidget(comboxSampleMode);
	groupSampleModeContainer->setLayout(sampleModeLayout);

	rtlSettingLayout->addWidget(groupSampleRateContainer);
	rtlSettingLayout->addWidget(groupSampleModeContainer);
	rtlSettingLayout->addStretch();

	groupRtlSetting->setLayout(rtlSettingLayout);
	leftToolBox->addItem(groupRtlSetting, tr("��������"));
	// todo:����Ҫ��Ӷ�tuner�Լ�rtl2832u��AGCģʽ�Ŀ��ذ�ť

	dockLeft->setWidget(leftToolBox);
	// ����߹��������ֽ���================================================================

}

MainWindow::~MainWindow()
{
	if (m_logFile != nullptr)
	{
		fclose(m_logFile);
		m_logFile = nullptr;
	}
	if (m_dongle->getState() != RtlDevice::CLOSED)
	{
		closeRTL(true);
	}
}

void MainWindow::displayDefaultConfig()
{
	QVector<quint32> sampleRateList;
	sampleRateList = m_dongle->getDefaultSampleRateList();

	// ��combobox����ʾ����Ƶ���б�
	comboxSampleRate->clear();
	for (int i = 0; i < sampleRateList.count(); i++)
	{
		comboxSampleRate->addItem(QString("%0 MSPS").arg(sampleRateList[i] / 1000000.0), QVariant(sampleRateList[i]));
	}
	quint32 sampleRate = m_dongle->getSampleRateIndex();
	comboxSampleRate->setCurrentIndex(sampleRate);

	// ��combobox��������ʾ����ģʽ�б�
	comboxSampleMode->clear();
	for (int i = 0; i < 3; i++)
	{
		comboxSampleMode->addItem(RtlDevice::sampleModeStrings[i], QVariant(i));
	}
	quint32 sampleMode = m_dongle->getSampleMode();
	comboxSampleMode->setCurrentIndex(sampleMode);

	// ����offset tuning��ѡ��Ĭ��״̬
	bool isOffsetTuningOn = m_dongle->getOffsetTuningState();
	chkboxOffsetTuning->setChecked(isOffsetTuningOn);

	// ����tuner��Rtl��AGC��ʼĬ��״̬
	bool isTunerAgcOn = m_dongle->getTunerAgcState();
	chkboxTunerAGC->setChecked(isTunerAgcOn);
	bool isRtlAgcOn = m_dongle->getRtlAgcState();
	chkboxRtlAGC->setChecked(isRtlAgcOn);

	// ����tuner���淶Χ��
	quint32 minGain = 0;
	quint32 maxGain = 0;
	m_dongle->getTunerGainRange(minGain, maxGain);
	slideRFGain->setRange(minGain, maxGain);
	quint32 tunerGain = m_dongle->getTunerGain();
	slideRFGain->setValue(tunerGain);

}

void MainWindow::initFigure()
{
	m_figureBufLen = 2000;
	figure = new QCustomPlot;
	figure->addGraph();
	figure->xAxis->setLabel(tr("time"));
	figure->yAxis->setLabel(tr("signal"));
	figure->xAxis->setRange(0, m_figureBufLen);
	figure->yAxis->setRange(-1.0, 1.0);
	setCentralWidget(figure);
	
	// ����figure buffer���߳�
	if (updateFigureBufWorker == nullptr)
	{
		updateFigureBufWorker = new UpdateFigureBufWorker();
	}
	updateFigureBufWorker->moveToThread(&updateFigureBufThread);
	connect(this, &MainWindow::updateFigureBufSignal, updateFigureBufWorker, &UpdateFigureBufWorker::doWork);
	updateFigureBufThread.start();

	// ��ʼ�����»�ͼ��timer
	updateFigureTimer = new QTimer();
	connect(updateFigureTimer, SIGNAL(timeout()), this, SLOT(updatePsdWave()));
}

void MainWindow::updateFigureBuf()
{
	QVector<qreal> data;
	quint32 step = 20;
	while (true)
	{
		if (m_dongle->getState() != RtlDevice::RUNNING)	// ���Ӳ����û�������У���Ҳ��û�б�Ҫ��ȡ����������
		{
			break;
		}
		m_dongle->getData(data);	// �˺����ᱻ����

		for (int i = 0; i < data.count(); i = i+step)
		{
			m_figureBufY.enqueue(data[i]);
		}

		for (int i = m_figureBufY.count(); i > m_figureBufLen; i--)
		{
			m_figureBufY.removeFirst();
		}

		if (m_figureBufX.isEmpty() || m_figureBufX.count() != m_figureBufY.count())
		{
			for (int i = m_figureBufX.count(); i < m_figureBufY.count(); i++)
			{
				m_figureBufX.enqueue(i);
			}
		}

		m_figureX = m_figureBufX.toVector();
		m_figureY = m_figureBufY.toVector();

	}
}

void MainWindow::updatePsdWave()
{
	emit updateFigureBufSignal(this);	// ������������ز�������
	figure->graph(0)->setData(m_figureX, m_figureY, true);
	figure->replot();
}

void MainWindow::startReceiveData()
{
	qDebug() << "start receive data";
	m_dongle->startReadData();	// ��ʼ��Ӳ���ж�ȡ����
	qDebug() << "start the timer for update the figure";
	if (updateFigureTimer->isActive() == false)
	{
		updateFigureTimer->start(0);	// ���쿪ʼ����figure
	}
}



void UpdateFigureBufWorker::doWork(MainWindow *w)
{
	qDebug() << "start doing work(update figure buffer)";
	w->updateFigureBuf();
}

void MainWindow::btnStartPlaySlot()
{
	if (m_dongle->getState() == RtlDevice::RUNNING)
	{	
		stopReceiveData();
	}
	else
	{
		startReceiveData();
	}
}

void MainWindow::stopReceiveData()
{
	updateFigureTimer->stop();
	m_dongle->stopReadData();
}
