#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <memory>

#include "gamesmodel.h"
#include "playersmodel.h"
#include "playerbase.h"
#include "playerstatsmodel.h"
#include "globalstatsmodel.h"
#include "teamsplitter.h"

Q_DECLARE_METATYPE(Player*)
Q_DECLARE_METATYPE(PlayersModel)
Q_DECLARE_METATYPE(PlayerStatsModel)
Q_DECLARE_METATYPE(GlobalStatsModel)
Q_DECLARE_METATYPE(PlayerbaseQuery*)

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QGuiApplication::setWindowIcon(QIcon(":/img/ball3d.ico"));

    qmlRegisterUncreatableType<Player>("com.abc.abclib", 1, 0, "Player", "");

    Playerbase playerbase;

    GamesModel gm;
    gm.init();

    PlayersModel pm(gm);

    QList<QObject*> featuredStatsModel;
    featuredStatsModel << new PlayerbaseQuery (pm, playerbase, 2, "ON FIRE", "Longest win streak");
    featuredStatsModel << new PlayerbaseQuery (pm, playerbase, 7, "STRONGEST SYNERGY", "Highest W/L ratio together");
    featuredStatsModel << new PlayerbaseQuery (pm, playerbase, 2, "RIVALRIES TO WATCH", "Closest-rated players");

    PlayerStatsModel playerStatsModel;
    GlobalStatsModel globalStatsModel(pm, &playerbase);

    QQmlApplicationEngine engine;
    QQmlContext* ctxt = engine.rootContext();

    ctxt->setContextProperty("featuredStatsModel", QVariant::fromValue(featuredStatsModel));
    ctxt->setContextProperty("playerStatsModel", &playerStatsModel);
    ctxt->setContextProperty("globalStatsModel", &globalStatsModel);

    TeamSplitter ts;
    ctxt->setContextProperty("teamSplitter", &ts);

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
