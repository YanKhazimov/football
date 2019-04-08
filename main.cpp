#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTextCodec>

#include "gamesmodel.h"
#include "gamefiltermodel.h"
#include "playerbase.h"
#include "personalstatsmodel.h"
#include "globalstatsmodel.h"
#include "teamsplitter.h"
#include "playersortfilterproxymodel.h"
#include "statpresenterproxymodel.h"
#include "featuredstatsmodel.h"
#include "language.h"

Q_DECLARE_METATYPE(GlobalStatsModel)
Q_DECLARE_METATYPE(PlayerbaseQuery*)

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QGuiApplication::setWindowIcon(QIcon(":/img/ball3d.ico"));

    qmlRegisterUncreatableType<Player>("com.abc.abclib", 1, 0, "Player", "");
    qmlRegisterUncreatableType<GlobalStatsModel>("com.abc.abclib", 1, 0, "GlobalStatsModel", "");
    qmlRegisterUncreatableType<PersonalStatsModel>("com.abc.abclib", 1, 0, "PersonalStatsModel", "");
    qmlRegisterUncreatableType<StatPresenterProxyModel>("com.abc.abclib", 1, 0, "GlobalStatPresenter", "");

    Language language;

    QTextCodec *codec = QTextCodec::codecForName("UTF8");
    QTextCodec::setCodecForLocale(codec);

    GamesModel gm;
    gm.init("games");

    // GamesModel - GlobalStatsModel - PlayerSortFilterProxyModel - StatPresenterProxyModel [ - StatTable ]

    Playerbase playerbase;
    GlobalStatsModel globalStatsModel(&playerbase);

    globalStatsModel.setSourceModel(&gm);

    PlayerSortFilterProxyModel sortingStatModel;
    sortingStatModel.setSourceModel(&globalStatsModel);

    StatPresenterProxyModel statModel;
    statModel.setSourceModel(&sortingStatModel);

    // PlayerSortFilterProxyModel - featuredStatsModel [ - SlideShow ]

    FeaturedStatsModel featuredStatsModel(language);
    featuredStatsModel.setSourceModel(&sortingStatModel);

    // PlayerSortFilterProxyModel - PersonalStatsModel [ - PlayerStatsTable ]

    PersonalStatsModel personalStatsModel(language);
    personalStatsModel.setSourceModel(&sortingStatModel);

    language.set("ru");

    QQmlApplicationEngine engine;
    QQmlContext* ctxt = engine.rootContext();

    ctxt->setContextProperty("gamesModel", QVariant::fromValue(&gm));
    ctxt->setContextProperty("featuredStatsModel", QVariant::fromValue(&featuredStatsModel));
    ctxt->setContextProperty("personalStatsModel", &personalStatsModel);
    ctxt->setContextProperty("globalStatsModel", &globalStatsModel);
    ctxt->setContextProperty("sortingStatModel", &sortingStatModel);
    ctxt->setContextProperty("statModel", &statModel);

    TeamSplitter teamSplitter(globalStatsModel);
    ctxt->setContextProperty("teamSplitter", &teamSplitter);

    ctxt->setContextProperty("lang", &language);

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
