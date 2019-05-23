#include "mainwindow.h"
#include <QDockWidget>
#include <QTextEdit>
#include <QMessageBox>
//#include "qcustomplot.h"
#include "figure.h"
#include "rtldevice.h"
#include "dialogopendev.h"
#include <QTimer>
#include <QToolBox>
#include <QRadioButton>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QCheckBox>
#include <QComboBox>
#include <QProgressBar>
#include "demodulator.h"
#include "soundplayer.h"
//#include "imageviewer.h"

const QString MainWindow::demodMethodNames[8] = { "NFM", "WFM","AM","DSB","USB","LSB" ,"RAW","CW" };
MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	m_recordTime(0),
	m_currentRecordTime(0)
	//imgViewer(nullptr)
{
	ui.setupUi(this);
	setWindowTitle(tr("SDRCloud"));
	
	m_dongle = new RtlDevice(nullptr);
	m_demod = new Demodulator(m_dongle);
	m_player = new SoundPlayer(m_demod);	// ��ʼ����Ƶ����

	iniUI();	// �����ʼ��UI

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
	/*initFigure();*/
	figure = new Figure(m_demod);
	setCentralWidget(figure);

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
	iniStatusBar();

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
	// ������ͼ�񴰿�
	connect(ui.actOpenImgWindow, SIGNAL(triggered(bool)), this, SLOT(openImgWindow(bool)));

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
					labDeviceName->setText(QString("�豸:%0").arg(m_dongle->getDeviceName()));
					displayDefaultConfig();
				}
				else
				{
					console(QString("�豸��ʧ��"));
				}
			}

		}
		// ����Ի���
		delete dialogDev;
	}	// !else m_dongle
}

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
	if (m_dongle->getState() == RtlDevice::RUNNING)
	{
		stopRtl();
		btnStartPlay->setText("��ʼ����");
	}
	if (m_dongle->close())
	{
		labDeviceName->setText(tr("�豸:δ��"));
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

	// 1.��������¼�����UI
	QGroupBox *groupRecordUIs = new QGroupBox;
	comboxRecord = new QComboBox;
	btnClearRecord = new QPushButton;
	QHBoxLayout *recordHLayout = new QHBoxLayout;
	recordHLayout->addWidget(comboxRecord);
	recordHLayout->addWidget(btnClearRecord);
	QVBoxLayout *recordLayout = new QVBoxLayout;
	pgbRecord = new QProgressBar;
	recordLayout->addLayout(recordHLayout);
	recordLayout->addWidget(pgbRecord);
	recordLayout->addStretch();
	groupRecordUIs->setLayout(recordLayout);
	leftToolBox->addItem(groupRecordUIs, tr("¼������"));


	// 2.���������ʽѡ��ĵ�ѡ��ť��
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

	// 3.����tuner���õ���ذ�ť
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

	// 4.����rtl2832u���õ���ذ�ť
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

void MainWindow::iniStatusBar()
{
	labDeviceName = new QLabel(tr("�豸:δ��"));
	statusBar()->addWidget(labDeviceName);
}
MainWindow::~MainWindow()
{
	//if (m_logFile != nullptr)
	//{
	//	fclose(m_logFile);
	//	m_logFile = nullptr;
	//}
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
	bool isOffsetTuningOn = m_dongle->isOffsetTuningOn();
	chkboxOffsetTuning->setChecked(isOffsetTuningOn);

	// ����tuner��Rtl��AGC��ʼĬ��״̬
	bool isTunerAgcOn = m_dongle->isTunerAgcOn();
	chkboxTunerAGC->setChecked(isTunerAgcOn);
	bool isRtlAgcOn = m_dongle->isRtlAgcOn();
	chkboxRtlAGC->setChecked(isRtlAgcOn);

	// ����tuner���淶Χ��
	quint32 minGain = 0;
	quint32 maxGain = 0;
	m_dongle->getTunerGainRange(minGain, maxGain);
	slideRFGain->setRange(minGain, maxGain);
	quint32 tunerGain = m_dongle->getTunerGain();
	slideRFGain->setValue(tunerGain);

}

void MainWindow::startRtl()
{
	qDebug() << "start receive data";
	m_dongle->startReadData();	// ��ʼ��Ӳ���ж�ȡ���ݣ���������Զ���ʼ����
	m_demod->startDemodFM();	// ע�⣺����ĵ���˳���ܹ����ģ�redraw��play����������running��demod�������start redraw��start play֮ǰ
								// ����demod����running�ģ��ǿ���redraw��play�̵߳��źžͻᱻ�˷ѵ�
	figure->startRedrawing();
	m_player->startPlay();
}

void MainWindow::stopRtl()
{
	m_player->stopPlay();
	figure->stopRedrawing();
	m_dongle->stopReadData();
	m_demod->stopDemodFM();
}

void MainWindow::btnStartPlaySlot()
{
	if (m_dongle->getState() == RtlDevice::RUNNING)
	{	
		stopRtl();
		btnStartPlay->setText("��ʼ����");
	}
	else
	{
		startRtl();
		btnStartPlay->setText("ֹͣ����");
	}
}

void MainWindow::lostDevice(QString info)
{
	console("�豸��ʧ����رպ����²��һ�µ��Ӱ�����"+info);
}

void MainWindow::openImgWindow(bool b)
{
	//if (imgViewer == nullptr) {
	//	imgViewer = new ImageViewer();
	//}
	//imgViewer->setWindowTitle(tr("NOAA������ͼ����"));
	//imgViewer->show();
	;

}
