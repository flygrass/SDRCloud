#include "mainwindow.h"
#include <QDockWidget>
#include <QTextEdit>
#include <QMessageBox>
#include "qcustomplot.h"
#include "rtldevice.h"
#include "dialogopendev.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowTitle(tr("SDRCloud"));
	iniUI();	// �����ʼ��UI
	

	// ��ʽ��ʼ����Ա����
	m_QueueBufX = new QQueue<qreal>;
	m_QueueBufY = new QQueue<qreal>;
	m_bufX = new QVector<qreal>;
	m_bufY = new QVector<qreal>;
	m_dongle = new RTLDevice(m_QueueBufX, m_QueueBufY, m_bufX, m_bufY, 0);
	updWaveTimer = new QTimer;

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
	figure = new QCustomPlot;
	figure->addGraph();
	figure->xAxis->setLabel(tr("time"));
	figure->yAxis->setLabel(tr("signal"));
	figure->xAxis->setRange(0, 2000);
	figure->yAxis->setRange(-200, 200);
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
	
}

void MainWindow::connectSignalSlot()
{
	// ����RTL->��RTL�豸action����ۺ���
	connect(ui.actOpenRTL, SIGNAL(triggered(bool)), this, SLOT(openRTL(bool)));
	// ���Ӽ�ʱ������figure����ͼ
	connect(updWaveTimer, SIGNAL(timeout()), this, SLOT(updateWave()));
	// ���ӹر�RTL�豸��action�Ͳۺ���
	connect(ui.actCloseRTL, SIGNAL(triggered(bool)), this, SLOT(closeRTL(bool)));
	// ����menuRTL����ʾ�ۺ���
	connect(ui.menuRTL, SIGNAL(aboutToShow()), this, SLOT(showMenuRTL()));
}

void MainWindow::openRTL(bool b)
{
	Q_UNUSED(b);
	if (m_dongle->getState() != SDRDevice::CLOSED)
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
				}
			}

		}
		// ����Ի���
		delete dialogDev;
	}	// !else m_dongle
}

void MainWindow::updateWave()
{
	//QVector<qreal> x(101);
	//QVector<qreal> y(100);
	//for (int i = 0; i < 100; i++)
	//{
	//	x.append(i);
	//	y.append(50);
	//}
	//x.append(100);
	/*figure->graph(0)->setData(m_QueueBufX->toVector(), m_QueueBufY->toVector(), true);*/
	/*figure->graph(0)->setData(x, y, true);*/
	figure->graph(0)->setData(*m_bufX, *m_bufY, true);
	figure->replot();
}

void MainWindow::console(const QString & info)
{
	txtConsole->append(info + "\n");
}

void MainWindow::showMenuRTL()
{
	if (m_dongle->getState() == SDRDevice::CLOSED)
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
