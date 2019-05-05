#ifndef SYNCMANAGER_H
#define SYNCMANAGER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QThread>

#include "gamesmodel.h"
#include "playersortfilterproxymodel.h"
#include "globalstatsmodel.h"
#include "language.h"

class Downloader: public QObject
{
    Q_OBJECT

public slots:
    void sendDownloadRequest();

public:
    Downloader(QUrl url, QObject *parent = 0);

signals:
    void resultReady(QNetworkReply *reply);

private:
    QNetworkAccessManager m_networkAccessManager;
    QUrl m_url;
};

class SyncManager : public QObject
{
    Q_OBJECT
public:
    explicit SyncManager(GamesModel& gm, PlayerSortFilterProxyModel& ssm, GlobalStatsModel& gsm, const Language &lang, QObject *parent = 0);
    Q_INVOKABLE void update();

private slots:
    void handleResults(QNetworkReply* pReply);

private:
    GamesModel &m_gamesModel;
    PlayerSortFilterProxyModel &m_sortingStatModel;
    GlobalStatsModel &m_globalStatsModel;
    const Language& m_language;
    bool m_playersDownloaded, m_gamesDownloaded;
    QByteArray m_downloadedGamesData, m_downloadedPlayersData;
    QByteArray m_localGamesData, m_localPlayersData;
    const QUrl m_gamesUrl, m_playersUrl;

    void readLocalData();
    void applyGames();

signals:
    void updateFinished(QString message);
};

#endif // SYNCMANAGER_H
