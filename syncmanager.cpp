#include "syncmanager.h"

#include <QFile>

SyncManager::SyncManager(GamesModel &gm, PlayerSortFilterProxyModel &ssm, const Language& lang, QObject *parent)
    : QObject(parent), gamesModel(gm), sortingStatModel(ssm), language(lang)
{
}

void SyncManager::update()
{
    Downloader *worker = new Downloader;
    QThread* thread = new QThread();
    worker->moveToThread(thread);
    connect(thread, &QThread::started, worker, &Downloader::sendDownloadRequest);
    connect(worker, &Downloader::resultReady, this, &SyncManager::handleResults);
    connect(worker, &Downloader::resultReady, thread, &QThread::quit);
    connect(worker, &Downloader::resultReady, worker, &Downloader::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    thread->start();
}

void SyncManager::handleResults(QNetworkReply* pReply)
{
    if (pReply->error())
    {
        emit updateFinished(Language::dict.value("updateFail").value(language.get()));
        return;
    }
    else if (pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200)
    {
        emit updateFinished(Language::dict.value("updateFail").value(language.get()));
        return;
    }
    else
    {
        QByteArray downloadedData = pReply->readAll();
        QByteArray localData;

        QFile file("games");
        if (file.open(QIODevice::ReadOnly))
        {
            localData = file.readAll();
            file.close();
        }

        if (downloadedData == localData)
        {
            emit updateFinished(Language::dict.value("upToDate").value(language.get()));
            return;
        }

        if (file.open(QIODevice::WriteOnly))
        {
            file.write(downloadedData);
            file.close();
        }

        int selectedRow = sortingStatModel.getSelectedRow();
        Player* selectedPlayer = sortingStatModel.getPlayer(selectedRow);
        gamesModel.init();

        for (int i = 0; i < sortingStatModel.rowCount(); ++i)
        {
            QVariant playerPointer = sortingStatModel.data(sortingStatModel.index(i, 0), DataRoles::DataRole::Player);
            if (*(playerPointer.value<Player*>()) == *selectedPlayer)
            {
                sortingStatModel.selectRow(i);
                break;
            }
        }

        emit updateFinished(Language::dict.value("updated").value(language.get()));
    }

    pReply->deleteLater();
}

Downloader::Downloader(QObject *parent)
    : QObject(parent)
{
    connect(&m_networkAccessManager, &QNetworkAccessManager::finished,
            this, &Downloader::resultReady);
}

void Downloader::sendDownloadRequest()
{
    QUrl gamesFileUrl("https://dl.dropboxusercontent.com/s/fapzda1e3o5pby3/config?dl=0");
    m_networkAccessManager.get(QNetworkRequest(gamesFileUrl));
}
