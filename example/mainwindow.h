#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QLabel>
#include "Base.h"
#include "Bus.h"
#include "PackageProtocol_1_0.h"
#include "Common.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget* parent = 0);
        ~MainWindow();

    protected:
        void changeEvent(QEvent* e);

    private:
        Ui::MainWindow* ui;
        Base* m_base;
        Bus* m_bus;
        QStringList* m_sl;
        QStringListModel* m_slm;
        QLabel* m_statue;
        QLabel *m_dataSize;     
    private:
        void InitSlots();
        void InitVar();
        void InitWidgets();

    public Q_SLOTS:
        void BtnListen();
        void BtnConnect();
        void BtnClear();
        void BtnDisconnect();
        void BtnDisconnectAll();
		void BtnNotUseUuidClient(bool checked);
		void BtnNotUseUuidServer(bool checked);
        void ReciveData(QByteArray& package, QByteArray& data);
        void SendMsg();
        void AddToBusFailed(QString uuid);
        void ConnectToServerSuccess(QString name);
        void AcceptNewClientSuccess(QString name);
        void ClientDisconnect(QString guid);
        void ServerDisconnect(QString guid);
        void ConnectError(QAbstractSocket::SocketError err);     
        void ByteWriten(qint64 len);
};

#endif // MAINWINDOW_H
