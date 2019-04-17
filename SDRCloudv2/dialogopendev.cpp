#include "dialogopendev.h"
#include <QListWidget>
#include <QListWidgetItem>
#include "rtldevice.h"


DialogOpenDev::DialogOpenDev(SDRDevice *dongle, QWidget *parent)
	: QDialog(parent),
	m_dongle(dongle)
{
	ui.setupUi(this);
	connect(ui.listWidgetDevice, SIGNAL(itemSelectionChanged()), this, SLOT(itSelSlot()));
	connect(ui.btnRefresh, SIGNAL(clicked()), this, SLOT(refreshDevList()));
	// ��ѰRTL�豸
	openRTL();
}

DialogOpenDev::~DialogOpenDev()
{
}

void DialogOpenDev::setHint(QString hint)
{
	ui.labHint->setText(hint);
}

void DialogOpenDev::addListItem(QListWidgetItem *item)
{
	ui.listWidgetDevice->addItem(item);
}

void DialogOpenDev::clearAllItem()
{
	ui.listWidgetDevice->clear();
}

QListWidgetItem *DialogOpenDev::getSelectedItem()
{
	return ui.listWidgetDevice->currentItem();
}

void DialogOpenDev::setBtnOKState()
{
	if (getSelectedItem() == nullptr)
	{
		ui.btnOK->setEnabled(false);
	}
	else
	{
		ui.btnOK->setEnabled(true);
	}
}

//	��ѰRTL�豸
void DialogOpenDev::openRTL()
{
	quint32 count = 0;
	QListWidgetItem *item = nullptr;
	

	count = m_dongle->getDeviceCount();
	if (count == 0)	// δ�ҵ��豸
	{
		setHint(tr("δ�ҵ�RTL�豸�������RTL�豸���ٳ���ˢ�°�ť��"));
		setBtnOKState();
	}
	else // �ҵ���
	{
		QString devName;
		for (quint32 i = 0; i < count; i++)	// ���λ�ȡ�豸������ӵ��б�����ʾ
		{
			devName = m_dongle->getDeviceName(i);
			item = new QListWidgetItem(QString::asprintf("index:[%d]=>name:", i) + devName);
			item->setData(Qt::UserRole, QVariant(i));
			addListItem(item);
		}
		setHint(tr("�����б���ѡ��һ�����õ�RTL�豸Ȼ��򿪣�"));
		setBtnOKState();
	}
}

void DialogOpenDev::refreshDevList()
{
	clearAllItem();
	openRTL();
}

void DialogOpenDev::itSelSlot()
{
	setBtnOKState();
}
