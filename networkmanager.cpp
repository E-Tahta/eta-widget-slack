#include <QList>
#include <QNetworkInterface>
#include <QTcpSocket>
#include "networkmanager.h"


NetworkManager::NetworkManager(QObject *parent) :
    QObject(parent)
{
    Mac = "unknown";
    IP = "unknown";
    setMac();
    setIP();

}

void NetworkManager::setMac()
{
    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();

    for (int i = 0; i < ifaces.count(); i++) {
        QNetworkInterface iface = ifaces.at(i);
        if ( !iface.flags().testFlag(QNetworkInterface::IsLoopBack)
             && iface.name().contains("eth")) {
            Mac = QString(iface.hardwareAddress().toUtf8());
        }

    }
}

void NetworkManager::setIP()
{
    QTcpSocket socket;
    socket.connectToHost("8.8.8.8", 53); // google DNS, or something else reliable
    if (socket.waitForConnected()) {
       IP = socket.localAddress().toString();
    }
}

QString NetworkManager::getMac() const
{
    return Mac;
}

QString NetworkManager::getIP() const
{
    return IP;
}

#include "networkmanager.moc"
