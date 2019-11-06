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
#include "updater.h"
#include "broadcaster.h"

Q_DECLARE_METATYPE(GlobalStatsModel)
Q_DECLARE_METATYPE(PlayerbaseQuery*)

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QGuiApplication::setWindowIcon(QIcon(":/img/ball3d.ico"));
    QGuiApplication::setApplicationName(QString("Razdevalka v.") + RAZDEVALKA_VERSION);

    Config config;
    config.load("config");

    if (argc == 1) // initial launch without parameters
    {
        Updater updater;
        QObject::connect(&updater, &Updater::success, &updater, &Updater::runWithNoCheck);

        QQmlApplicationEngine engine;
        QQmlContext* ctxt = engine.rootContext();
        ctxt->setContextProperty("updater", &updater);
        engine.load(QUrl(QStringLiteral("qrc:/qml/UpdaterWindow.qml")));
        QObject::connect(engine.rootObjects().first(), SIGNAL(closed()), &updater, SLOT(runWithNoCheck()));

        updater.checkForUpdates();
        return !engine.rootObjects().isEmpty() ? app.exec() : -1;
    }

    qmlRegisterUncreatableType<Player>("com.abc.abclib", 1, 0, "Player", "");
    qmlRegisterUncreatableType<GlobalStatsModel>("com.abc.abclib", 1, 0, "GlobalStatsModel", "");
    qmlRegisterUncreatableType<PersonalStatsModel>("com.abc.abclib", 1, 0, "PersonalStatsModel", "");
    qmlRegisterUncreatableType<StatPresenterProxyModel>("com.abc.abclib", 1, 0, "GlobalStatPresenter", "");
    qmlRegisterUncreatableType<Broadcaster>("com.abc.abclib", 1, 0, "Signaller", "");

    Language language;

    QTextCodec *codec = QTextCodec::codecForName("UTF8");
    QTextCodec::setCodecForLocale(codec);

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

    SyncManager syncManager(gm, sortingStatModel, language, &playerbase);

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
    ctxt->setContextProperty("syncManager", &syncManager);

    Broadcaster broadcaster;
    ctxt->setContextProperty("broadcaster", &broadcaster);

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
