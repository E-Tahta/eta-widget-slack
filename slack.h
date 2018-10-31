#ifndef SLACK_H
#define SLACK_H

#include <QGraphicsSceneHoverEvent>
#include <QList>

#include <Plasma/Applet>
#include <QStringList>

#include "fileio.h"
#include "clock.h"
#include "networkmanager.h"

namespace Plasma
{
class DeclarativeWidget;

}

class Slack : public Plasma::Applet
{
    Q_OBJECT
    Q_PROPERTY(bool lecture READ lecture WRITE setLecture)

public:
    Slack(QObject *parent, const QVariantList &args);
    ~Slack();
    void init();
    bool lecture() const;
    void setLecture(const bool &b);
    Q_INVOKABLE QStringList getList();
    Q_INVOKABLE void playSound(QString sound);

private:
    void initDeclarativeUI();
    Plasma::DeclarativeWidget *m_declarativeWidget;
    bool m_lecture;
    bool isOgrenci;
    FileIO *f;
    Clock *c;
    NetworkManager *n;
    QString startTime;
    QString message;
};

K_EXPORT_PLASMA_APPLET(slack, Slack)

#endif
