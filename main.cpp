#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <memory>

#include "gamesmodel.h"
#include "playersmodel.h"
#include "query.h"
#include "playerstatsmodel.h"

Q_DECLARE_METATYPE(Player*)
Q_DECLARE_METATYPE(PlayersModel)
Q_DECLARE_METATYPE(PlayerStatsModel)
Q_DECLARE_METATYPE(PlayerbaseQuery*)

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterUncreatableType<Player>("com.abc.abclib", 1, 0, "Player", "");
    qmlRegisterUncreatableType<PlayerbaseQuery>("com.abc.abclib", 1, 0, "Fact", "");

    Playerbase playerbase;

    GamesModel gm;
    gm.init();

    PlayersModel pm(gm);
    QList<QObject*> fm;
    fm << new PlayerbaseQuery (pm, playerbase, 2, "ON FIRE", "Longest win streak");
    fm << new PlayerbaseQuery (pm, playerbase, 7, "STRONGEST SYNERGY", "Highest W/L ratio together");
    fm << new PlayerbaseQuery (pm, playerbase, 1, "TOP GRINDER", "Most progress");
    fm << new PlayerbaseQuery (pm, playerbase, 2, "RIVALRIES TO WATCH", "Closest-rated players");

    PlayerStatsModel statsModel;

    QQmlApplicationEngine engine;
    QQmlContext* ctxt = engine.rootContext();
    ctxt->setContextProperty("fm", QVariant::fromValue(fm));
    ctxt->setContextProperty("statsExample", &statsModel);
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
