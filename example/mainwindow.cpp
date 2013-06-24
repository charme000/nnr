#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitSlots();
    InitVar();
    InitWidgets();
}

MainWindow::~MainWindow()
{
    delete ui;

    if(m_base != 0)
    {
        delete m_base;
    }

    if(m_bus != NULL)
    {
        delete m_bus;
    }

    delete m_statue;
    delete m_dataSize;
    delete m_sl;
    delete m_slm;
}

void MainWindow::changeEvent(QEvent* e)
{
    QMainWindow::changeEvent(e);

    switch(e->type())
    {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void MainWindow::InitSlots()
{
    connect(ui->btn_listen, SIGNAL(clicked()), this, SLOT(BtnListen()));
    connect(ui->btn_connect, SIGNAL(clicked()), this, SLOT(BtnConnect()));
    connect(ui->btn_send, SIGNAL(clicked()), this, SLOT(SendMsg()));
    connect(ui->btn_clear,SIGNAL(clicked()),this,SLOT(BtnClear()));
    connect(ui->btn_disconnect,SIGNAL(clicked()),this,SLOT(BtnDisconnect()));
    connect(ui->btn_disconnect_all,SIGNAL(clicked()),this,SLOT(BtnDisconnectAll()));
	connect(ui->check_not_use_uuid_c,SIGNAL(toggled (bool)), this, SLOT(BtnNotUseUuidClient(bool)));
	connect(ui->check_not_use_uuid_s,SIGNAL(toggled (bool)), this, SLOT(BtnNotUseUuidServer(bool)));
}

void MainWindow::InitVar()
{
    m_base = 0;
    m_slm = new QStringListModel(this);
    m_sl = new QStringList();
    m_bus = new Bus();
}

void MainWindow::InitWidgets()
{
    m_statue = new QLabel;
    m_dataSize = new QLabel;
    ui->statusBar->addWidget(m_statue);
    ui->statusBar->addWidget(m_dataSize);
    m_statue->setText(tr("准备就绪"));
    m_dataSize->setText(tr("数据接受状况"));
}

void MainWindow::BtnClear()
{
    ui->edit_context->clear();
}

void MainWindow::BtnNotUseUuidClient(bool checked)
{
	ui->edit_name2->setDisabled(checked);
}

void MainWindow::BtnNotUseUuidServer(bool checked)
{
	ui->edit_name1->setDisabled(checked);
}

void MainWindow::BtnConnect()
{
    m_base = new Base(this);
    m_base->setAttribute(_CLIENT_ATTRIBUTE_);
    m_base->setFlag(PipeFlag);
    m_base->setValue(ui->edit_port1->text());
    m_base->setUuidName(ui->edit_name2->text());
	m_base->setIp(ui->edit_ip1->text());
    m_base->CreateManager(*m_bus);

	if (ui->check_not_use_uuid_c->isChecked() == true)
	{
		m_base->setUseUuid(false);
	}else
	{
		m_base->setUseUuid(true);
	}


    m_base->Set(SLOT(ConnectToServerSuccess(QString)), NULL, SLOT(ConnectError(QAbstractSocket::SocketError)), SLOT(ServerDisconnect(QString)), NULL,SLOT(AddToBusFailed(QString)), SLOT(ReciveData(QByteArray&, QByteArray&)),SLOT(ByteWriten(qint64)),NULL);
    m_base->Start();
    m_statue->setText(tr("查找服务端..."));
    ui->btn_listen->setDisabled(true);
    ui->edit_name1->setDisabled(true);
    ui->edit_port2->setDisabled(true);
	ui->edit_ip2->setDisabled(true);
}

void MainWindow::BtnListen()
{
    m_base = new Base(this);

    m_base->setAttribute(_SERVER_ATTRIBUTE_);
    m_base->setFlag(PipeFlag);
    m_base->setValue(ui->edit_port2->text());
    m_base->setUuidName(ui->edit_name1->text());
	m_base->setIp(ui->edit_ip2->text());
	m_base->CreateManager(*m_bus);

	if (ui->check_not_use_uuid_s->isChecked() == true)
	{
		m_base->setUseUuid(false);
	}else
	{
		m_base->setUseUuid(true);
	}

   
    m_base->Set(NULL, SLOT(AcceptNewClientSuccess(QString)), SLOT(ConnectError(QAbstractSocket::SocketError)), NULL, SLOT(ClientDisconnect(QString)),SLOT(AddToBusFailed(QString)), SLOT(ReciveData(QByteArray&, QByteArray&)),SLOT(ByteWriten(qint64)),NULL);
    m_base->Start();
    m_statue->setText(tr("等到客户端连接..."));

    ui->btn_connect->setDisabled(true);
    ui->edit_name2->setDisabled(true);
    ui->edit_port1->setDisabled(true);
	ui->edit_ip1->setDisabled(true);
}

void MainWindow::BtnDisconnect()
{
    QString uuid = ui->edit_disconnect_uuid->text();

    if(uuid.isEmpty() == false)
    {
        m_base->Stop(uuid,false);
        m_sl->removeOne(uuid);
        m_slm->setStringList(*m_sl);
        ui->lv_user->setModel(m_slm);
        m_statue->setText(tr("客户端%1主动退出").arg(uuid));
    }
}

void MainWindow::BtnDisconnectAll()
{
    m_base->StopAll(false);
    m_sl->clear();
    m_slm->setStringList(*m_sl);
    ui->lv_user->setModel(m_slm);
    m_statue->setText(tr("所有客户端都主动退出"));
}

void MainWindow::ReciveData(QByteArray& package, QByteArray& data)
{
    quint32 curCmd = 0;
    QUuid curSrc = QString("");
    QUuid curDst = QString("");
    quint8 curType = 0;

    if(package.isEmpty() == false)
    {
        if(package.at(1) == 1)
        {
            QString protocol = _PROTOCOL_1_0;
            m_bus->setProtocol(protocol);
            USING_PROTOCOL(m_bus, 1_0);
            curCmd = p->getCmd(package);
            curType = p->getType(package);
            curDst = p->getDst(package);
            curSrc = p->getSrc(package);
        }

        QString news = QString("[data]->%1").arg(QString(data));
        QString dst = curDst.toString();

        if(curDst.isNull() == true)
        {
            dst= QString("Me");
        }

        ui->edit_context->append("from" + curSrc.toString() + "to " + dst + ":");
        ui->edit_context->append("[cmd]->" + QString::number(curCmd));
        ui->edit_context->append("[type]->" + QString::number(curType));
        ui->edit_context->append(news);
        ui->edit_context->append("\n");
        package.clear();
        data.clear();
    }
}

void MainWindow::SendMsg()
{
    QUuid Src;
    QUuid Dst;

    if(m_base->attribute() == "server")
    {
        Src = ui->edit_name1->text();
    }
    else if(m_base->attribute() == "client")
    {
        Src = ui->edit_name2->text();
    }

    Dst = ui->edit_dst->text();
    QString protocol = _PROTOCOL_1_0;
    m_bus->setProtocol(protocol);
    USING_PROTOCOL(m_bus, 1_0);
    p->setCmd(1);
    p->setSrc(Src);
    p->setDst(Dst);
    p->setType(ui->edit_type->text().toInt());
    QByteArray data = ui->edit_msg->text().toAscii();
    m_bus->Send(data);
}

void MainWindow::ConnectToServerSuccess(QString name)
{
    m_statue->setText(tr("连接到服务端 [%1]").arg(name));
    m_sl->append(name);
    m_slm->setStringList(*m_sl);
    ui->lv_user->setModel(m_slm);
}

void MainWindow::AcceptNewClientSuccess(QString name)
{
    m_statue->setText(tr("发现客户端 [%1]").arg(name));
    m_sl->append(name);
    m_slm->setStringList(*m_sl);
    ui->lv_user->setModel(m_slm);
}

void MainWindow::ConnectError(QAbstractSocket::SocketError err)
{
    switch(err)
    {
        case QAbstractSocket::RemoteHostClosedError:
            m_statue->setText(tr("远程主机关闭"));
            break;
        case QAbstractSocket::HostNotFoundError:
            m_statue->setText(tr("找不到主机"));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            m_statue->setText(tr("服务端可能未启动"));
            break;
        default:
            break;
    }
}

void MainWindow::ClientDisconnect(QString guid)
{
    m_statue->setText(tr("客户端 [%1] 退出").arg(guid));
    m_sl->removeOne(guid);
    m_slm->setStringList(*m_sl);
    ui->lv_user->setModel(m_slm);
}

void MainWindow::ServerDisconnect(QString guid)
{
    m_statue->setText(tr("服务端 [%1] 退出").arg(guid));
    m_sl->removeOne(guid);
    m_slm->setStringList(*m_sl);
    ui->lv_user->setModel(m_slm);
}

void MainWindow::ByteWriten(qint64 len)
{
    m_dataSize->setText(tr("发送%1字节").arg(QString::number(len)));
}

void MainWindow::AddToBusFailed(QString uuid)
{
    m_statue->setText(tr("UUID:%1已经存在于Bus中").arg(uuid));
}
