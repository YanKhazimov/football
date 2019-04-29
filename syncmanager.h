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
#include "language.h"

class Downloader: public QObject
{
    Q_OBJECT

public slots:
    void sendDownloadRequest();

public:
    Downloader(QObject *parent = 0);

signals:
    void resultReady(QNetworkReply *reply);

private:
    QNetworkAccessManager m_networkAccessManager;
};

class SyncManager : public QObject
{
    Q_OBJECT
public:
    explicit SyncManager(GamesModel& gm, PlayerSortFilterProxyModel& ssm, const Language &lang, QObject *parent = 0);
    Q_INVOKABLE void update();

private slots:
    void handleResults(QNetworkReply* pReply);

private:
    GamesModel &gamesModel;
    PlayerSortFilterProxyModel &sortingStatModel;
    const Language& language;

signals:
    void updateFinished(QString message);
};

#endif // SYNCMANAGER_H
