#include "slack.h"

#include <math.h>

#include <QFont>
#include <QTimer>
#include <QX11Info>
#include <QDBusInterface>
#include <QTextDocument>
#include <QDesktopWidget>
#include <QtGui/QGraphicsLinearLayout>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeContext>
#include <QProcess>
#include <QCursor>

#include <KCModuleInfo>
#include <KCModuleProxy>
#include <KColorScheme>
#include <KConfigDialog>
#include <KGlobalSettings>
#include <KIconLoader>
#include <KWindowSystem>
#include <NETRootInfo>

#include <Plasma/FrameSvg>
#include <Plasma/PaintUtils>
#include <Plasma/Theme>
#include <Plasma/ToolTipManager>
#include <Plasma/DeclarativeWidget>
#include <Plasma/Package>

#include "syslog.h"

Slack::Slack(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args),
      m_lecture(false),
      isOgrenci(false)

{

    QString name = qgetenv("USER");
    if (name.isEmpty())
        name = qgetenv("USERNAME");
    
    if (name.compare("ogrenci") == 0) {
        qDebug() << "Ogrenci is not allowed to use slack";
        isOgrenci = true;
    }
    
    f = new FileIO(this);
    c = new Clock(this);
    n = new NetworkManager(this);

    setAcceptsHoverEvents(true);
    setAcceptDrops(false);
    setHasConfigurationInterface(false);
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setBackgroundHints(NoBackground);
    setMinimumSize(260,400);

}

Slack::~Slack()
{
    setLecture(false);
}

void Slack::init()
{
    initDeclarativeUI();
}

void Slack::initDeclarativeUI()
{
    if (isOgrenci) {
        setVisible(false);
    } else {

        QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(this);
        m_declarativeWidget = new Plasma::DeclarativeWidget(this);
        m_declarativeWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

        m_declarativeWidget->engine()->rootContext()->setContextProperty("slack", this);

        Plasma::PackageStructure::Ptr structure = Plasma::PackageStructure::load("Plasma/Generic");
        Plasma::Package package(QString(), "tr.org.etap.slack", structure);
        m_declarativeWidget->setQmlPath(package.filePath("mainscript"));
        layout->addItem(m_declarativeWidget);
        layout->setContentsMargins(0, 0, 0, 0);

        setCursor(QCursor(Qt::BlankCursor));
        setLayout(layout);
    }
}


bool Slack::lecture() const
{
    return m_lecture;
}

void Slack::setLecture(const bool &b)
{
    if (!isOgrenci){
        if(m_lecture == true && b == false) {
            if(c->isOkeyToSave()) {

                f->writeData(startTime + "\t" + c->getCurrentTime() + "\t"
                             + c->getCurrentDate());
                this->getList();

                setlogmask (LOG_UPTO (LOG_NOTICE));
                openlog ("eta-slack", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);

                message = startTime + " " + c->getCurrentTime() + " "
                        + c->getCurrentDate() + " Mac Address: " + n->getMac()
                        + " IP Address: " + n->getIP();

                QByteArray ba = message.toLatin1();
                const char *cmessage = ba.data();

                syslog (LOG_NOTICE,"User: %s %s",getenv("USER"),cmessage  );
                syslog (LOG_INFO, "bell rings");

                closelog ();
            }

        } else {
            startTime = c->getCurrentTime();
            c->startCounting();
        }
        m_lecture = b;
    }
}

QStringList Slack::getList()
{
    if (!isOgrenci) {
        return f->readData();
    }
    return QStringList("");
}

void Slack::playSound(QString sound)
{
    if (sound.compare("CONFIRMATION") == 0) {
        QProcess::startDetached("paplay /usr/share/kde4/apps/plasma/packages/tr.org.etap.slack/contents/sounds/confirmation.ogg");
    }
    
    if (sound.compare("STARTED") == 0) {
        QProcess::startDetached("paplay /usr/share/kde4/apps/plasma/packages/tr.org.etap.slack/contents/sounds/start.ogg");
    }
    
    if (sound.compare("STOPPED") == 0) {
        QProcess::startDetached("paplay /usr/share/kde4/apps/plasma/packages/tr.org.etap.slack/contents/sounds/stop.ogg");
    }
    
    if (sound.compare("LISTOPENED") == 0) {
        QProcess::startDetached("paplay /usr/share/kde4/apps/plasma/packages/tr.org.etap.slack/contents/sounds/openlist.ogg");
    }
    
    if (sound.compare("LISTCLOSED") == 0) {
        QProcess::startDetached("paplay /usr/share/kde4/apps/plasma/packages/tr.org.etap.slack/contents/sounds/closelist.ogg");
    }
}


#include "slack.moc"
