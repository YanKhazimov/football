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
#include "playerbase.h"
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
    explicit SyncManager(GamesModel& gm, PlayerSortFilterProxyModel& ssm, const Language &lang, Playerbase* pb, QObject *parent = 0);
    Q_INVOKABLE void update();

private slots:
    void handleResults(QNetworkReply* pReply);

private:
    GamesModel &m_gamesModel;
    PlayerSortFilterProxyModel &m_sortingStatModel;
    const Language& m_language;
    Playerbase* m_playerbase;
    QByteArray m_downloadedGamesData, m_downloadedPlayersData;
    QByteArray m_localGamesData, m_localPlayersData;
    const QUrl m_gamesUrl, m_playersUrl;
    QList<QUrl> m_downloadsToDo;

    QByteArray readLocalData(QString file);
    void writeLocalData(QString filename, const QByteArray& data);
    void applyGames();

    bool m_upToDate, m_updateError;

signals:
    void updateFinished(QString message);
};

#endif // SYNCMANAGER_H
