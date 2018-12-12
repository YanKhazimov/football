#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "gamesmodel.h"
#include "playersmodel.h"
#include "playerbase.h"
#include "playerstatsmodel.h"
#include "globalstatsmodel.h"
#include "teamsplitter.h"
#include "playersortfilterproxymodel.h"
#include "statpresenterproxymodel.h"

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
    qmlRegisterUncreatableType<GlobalStatsModel>("com.abc.abclib", 1, 0, "GlobalStatsModel", "");
    qmlRegisterUncreatableType<PlayerStatsModel>("com.abc.abclib", 1, 0, "PlayerStatsModel", "");
    qmlRegisterUncreatableType<StatPresenterProxyModel>("com.abc.abclib", 1, 0, "GlobalStatPresenter", "");

    GamesModel gm;
    gm.init();

    Playerbase playerbase;
    GlobalStatsModel globalStatsModel(&playerbase);

    globalStatsModel.setSourceModel(&gm);

    PlayerSortFilterProxyModel sortingStatModel;
    sortingStatModel.setSourceModel(&globalStatsModel);

    StatPresenterProxyModel statModel;
    statModel.setSourceModel(&sortingStatModel);


    PlayersModel pm(gm);

    QList<QObject*> featuredStatsModel;
    featuredStatsModel << new PlayerbaseQuery (pm, playerbase, 2, "RIVALRIES TO WATCH", "Closest-rated players");
    featuredStatsModel << new PlayerbaseQuery (pm, playerbase, 2, "ON FIRE", "Active win streak");
    featuredStatsModel << new PlayerbaseQuery (pm, playerbase, 7, "STRONGEST SYNERGY", "Highest W/L ratio together");


    StatPresenterProxyModel statModel2;
    statModel2.setSourceModel(&globalStatsModel);

    PlayerStatsModel playerStatsModel;
    playerStatsModel.setSourceModel(&statModel2);


    QQmlApplicationEngine engine;
    QQmlContext* ctxt = engine.rootContext();

    ctxt->setContextProperty("featuredStatsModel", QVariant::fromValue(featuredStatsModel));
    ctxt->setContextProperty("playerStatsModel", &playerStatsModel);
    ctxt->setContextProperty("globalStatsModel", &globalStatsModel);
    ctxt->setContextProperty("statModel", &statModel);

    TeamSplitter ts;
    ctxt->setContextProperty("teamSplitter", &ts);

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
