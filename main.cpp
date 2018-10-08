#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <memory>

#include "playersmodel.h"
#include "query.h"

Q_DECLARE_METATYPE(Player*)
Q_DECLARE_METATYPE(Playerbase)
Q_DECLARE_METATYPE(PlayerbaseQuery*)

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterUncreatableType<Player>("com.abc.abclib", 1, 0, "Player", "");
    qmlRegisterUncreatableType<PlayerbaseQuery>("com.abc.abclib", 1, 0, "Fact", "");

    Playerbase pb;
    QList<QObject*> fm;
    fm << new PlayerbaseQuery (pb, 2, "ON FIRE", "Longest win streak");
    fm << new PlayerbaseQuery (pb, 7, "STRONGEST SYNERGY", "Highest W/L ratio together");
    fm << new PlayerbaseQuery (pb, 1, "TOP GRINDER", "Most progress");
    fm << new PlayerbaseQuery (pb, 2, "RIVALRIES TO WATCH", "Closest-rated players");

    QStringList stats;
    stats << "Current Rating: 500" << "Wins/Losses: 5-3 (62.5%)" << "Reliability: 50%";

    QQmlApplicationEngine engine;
    QQmlContext* ctxt = engine.rootContext();
    ctxt->setContextProperty("fm", QVariant::fromValue(fm));
    ctxt->setContextProperty("statsExample", QVariant::fromValue(stats));
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return app.exec();
}
