#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>


class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = 0);

private:
    QString Mac;
    QString IP;
    void setMac();
    void setIP();

public slots:
    QString getMac() const;
    QString getIP() const;
};

#endif // NETWORKMANAGER_H

