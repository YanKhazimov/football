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
#include "config.h"
#include "syncmanager.h"

Q_DECLARE_METATYPE(GlobalStatsModel)
Q_DECLARE_METATYPE(PlayerbaseQuery*)

#define APP_VERSION 2.0

int main(int argc, char *argv[])
{
    //rename("Razdevalka.exe","Razdevalka2.exe");

    QGuiApplication app(argc, argv);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setWindowIcon(QIcon(":/img/ball3d.ico"));

    qmlRegisterUncreatableType<Player>("com.abc.abclib", 1, 0, "Player", "");
    qmlRegisterUncreatableType<GlobalStatsModel>("com.abc.abclib", 1, 0, "GlobalStatsModel", "");
    qmlRegisterUncreatableType<PersonalStatsModel>("com.abc.abclib", 1, 0, "PersonalStatsModel", "");
    qmlRegisterUncreatableType<StatPresenterProxyModel>("com.abc.abclib", 1, 0, "GlobalStatPresenter", "");

    Language language;

    QTextCodec *codec = QTextCodec::codecForName("UTF8");
    QTextCodec::setCodecForLocale(codec);

    Config config;
    config.load("config");

    GamesModel gm;
    gm.init();

    // GamesModel - GlobalStatsModel - PlayerSortFilterProxyModel - StatPresenterProxyModel [ - StatTable ]

    Playerbase playerbase;
    GlobalStatsModel globalStatsModel(&playerbase);
    globalStatsModel.setSourceModel(&gm);

    PlayerSortFilterProxyModel sortingStatModel(DataRoles::DataRole::Rating);
    sortingStatModel.setSourceModel(&globalStatsModel);

    StatPresenterProxyModel statModel;
    statModel.setSourceModel(&sortingStatModel);

    // PlayerSortFilterProxyModel - featuredStatsModel [ - SlideShow ]

    FeaturedStatsModel featuredStatsModel(language);
    featuredStatsModel.setSourceModel(&sortingStatModel);

    // PlayerSortFilterProxyModel - PersonalStatsModel [ - PlayerStatsTable ]

    PersonalStatsModel personalStatsModel(language);
    personalStatsModel.setSourceModel(&sortingStatModel);

    language.set(config.value("lang"));

    SyncManager updater(gm, sortingStatModel, language, &playerbase);

    // season models

    GlobalStatsModel seasonModel(&playerbase);
    seasonModel.setSourceModel(&gm);
    seasonModel.setSeasonFilter(QString::number(QDate::currentDate().year()));

    QObject::connect(&gm, &GlobalStatsModel::modelReset, [&seasonModel]() {
        seasonModel.setSeasonFilter(QString::number(QDate::currentDate().year()));
    });

    PlayerSortFilterProxyModel progressAwardSortingModel(DataRoles::DataRole::Progress);
    progressAwardSortingModel.setSourceModel(&seasonModel);
    PlayerSortFilterProxyModel dedicationAwardSortingModel(DataRoles::DataRole::Dedication);
    dedicationAwardSortingModel.setSourceModel(&seasonModel);

    StatPresenterProxyModel progressAwardPresenterModel, dedicationAwardPresenterModel;
    progressAwardPresenterModel.setSourceModel(&progressAwardSortingModel);
    dedicationAwardPresenterModel.setSourceModel(&dedicationAwardSortingModel);

    QQmlApplicationEngine engine;
    QQmlContext* ctxt = engine.rootContext();

    ctxt->setContextProperty("gamesModel", QVariant::fromValue(&gm));
    ctxt->setContextProperty("featuredStatsModel", QVariant::fromValue(&featuredStatsModel));
    ctxt->setContextProperty("personalStatsModel", &personalStatsModel);
    ctxt->setContextProperty("globalStatsModel", &globalStatsModel);
    ctxt->setContextProperty("sortingStatModel", &sortingStatModel);
    ctxt->setContextProperty("statModel", &statModel);

    ctxt->setContextProperty("progressModel", &progressAwardPresenterModel);
    ctxt->setContextProperty("dedicationModel", &dedicationAwardPresenterModel);

    TeamSplitter teamSplitter(globalStatsModel);
    ctxt->setContextProperty("teamSplitter", &teamSplitter);

    ctxt->setContextProperty("lang", &language);
    ctxt->setContextProperty("config", &config);
    ctxt->setContextProperty("updater", &updater);

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
