#include "syncmanager.h"

#include <QFile>
#include <QDir>

SyncManager::SyncManager(GamesModel &gm, PlayerSortFilterProxyModel &ssm, const Language& lang, Playerbase *pb, QObject *parent)
    : QObject(parent), m_gamesModel(gm), m_sortingStatModel(ssm), m_language(lang), m_playerbase(pb),
      m_gamesUrl("https://dl.dropboxusercontent.com/s/fapzda1e3o5pby3/games"),
      m_playersUrl("https://dl.dropboxusercontent.com/s/wfr30z1c3muqehb/players")
{
}

void SyncManager::update()
{
    m_upToDate = true;
    m_updateError = false;

    m_downloadsToDo.clear();
    m_downloadsToDo.append(m_gamesUrl);
    m_downloadsToDo.append(m_playersUrl);

    Downloader *gamesDownloader = new Downloader(m_gamesUrl);
    QThread* gamesLoadingThread = new QThread();
    gamesDownloader->moveToThread(gamesLoadingThread);
    connect(gamesLoadingThread, &QThread::started, gamesDownloader, &Downloader::sendDownloadRequest);
    connect(gamesDownloader, &Downloader::resultReady, this, &SyncManager::handleResults);
    connect(gamesDownloader, &Downloader::resultReady, gamesLoadingThread, &QThread::quit);
    connect(gamesDownloader, &Downloader::resultReady, gamesDownloader, &Downloader::deleteLater);
    connect(gamesLoadingThread, &QThread::finished, gamesLoadingThread, &QThread::deleteLater);
    gamesLoadingThread->start();

    Downloader *playersDownloader = new Downloader(m_playersUrl);
    QThread* playersLoadingThread = new QThread();
    playersDownloader->moveToThread(playersLoadingThread);
    connect(playersLoadingThread, &QThread::started, playersDownloader, &Downloader::sendDownloadRequest);
    connect(playersDownloader, &Downloader::resultReady, this, &SyncManager::handleResults);
    connect(playersDownloader, &Downloader::resultReady, playersLoadingThread, &QThread::quit);
    connect(playersDownloader, &Downloader::resultReady, playersDownloader, &Downloader::deleteLater);
    connect(playersLoadingThread, &QThread::finished, playersLoadingThread, &QThread::deleteLater);
    playersLoadingThread->start();
}

void SyncManager::handleResults(QNetworkReply* pReply)
{
    if (pReply->error() || pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200)
    {
        m_updateError = true;
        emit updateFinished(Language::dict.value("updateFail").value(m_language.get())
                            + " : " + pReply->url().fileName());
        pReply->deleteLater();
        return;
    }

    QByteArray downloadedData = pReply->readAll();
    QString localFilename;
    if (pReply->url() == m_gamesUrl)
    {
        localFilename = QDir::currentPath() + "/data/games";
    }
    else if (pReply->url() == m_playersUrl)
    {
        localFilename = QDir::currentPath() + "/data/players";

        if (downloadedData != readLocalData(localFilename))
        {
            QTextStream in(downloadedData);
            QStringList playerData;
            while (!in.atEnd())
            {
                playerData = in.readLine().split('\t', QString::SplitBehavior::SkipEmptyParts);

                if (playerData.size() < 3)
                    continue;

                QUrl url("https://dl.dropboxusercontent.com/s/" + playerData[2] + "/" + playerData[2]);
                m_downloadsToDo.append(url);
                Downloader *photoDownloader = new Downloader(url);
                QThread* photoLoadingThread = new QThread();
                photoDownloader->moveToThread(photoLoadingThread);
                QObject::connect(photoLoadingThread, &QThread::started, photoDownloader, &Downloader::sendDownloadRequest);
                QObject::connect(photoDownloader, &Downloader::resultReady, this, &SyncManager::handleResults);
                QObject::connect(photoDownloader, &Downloader::resultReady, photoLoadingThread, &QThread::quit);
                QObject::connect(photoDownloader, &Downloader::resultReady, photoDownloader, &Downloader::deleteLater);
                QObject::connect(photoLoadingThread, &QThread::finished, photoLoadingThread, &QThread::deleteLater);
                photoLoadingThread->start();
            }
        }
    }
    else
    {
        localFilename = QDir::currentPath() + "/data/" + pReply->url().fileName();
    }

    bool sameData = downloadedData == readLocalData(localFilename);
    if (!sameData)
        writeLocalData(localFilename, downloadedData);

    m_upToDate = m_upToDate && sameData;

    pReply->deleteLater();
    m_downloadsToDo.removeOne(pReply->url());
    if (!m_downloadsToDo.empty())
        return;
    if (m_upToDate && !m_updateError)
    {
        emit updateFinished(Language::dict.value("upToDate").value(m_language.get()));
        return;
    }

    m_playerbase->init();
    applyGames();

    emit updateFinished(Language::dict.value("updated").value(m_language.get()));
}

QByteArray SyncManager::readLocalData(QString file)
{
    QByteArray data;
    QFile gamesFile(file);
    if (gamesFile.open(QIODevice::ReadOnly))
    {
        data = gamesFile.readAll();
        gamesFile.close();
    }

    return data;
}

void SyncManager::writeLocalData(QString filename, const QByteArray &data)
{
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(data);
        file.close();
    }
}

void SyncManager::applyGames()
{
    int selectedRow = m_sortingStatModel.getSelectedRow();
    Player* selectedPlayer = m_sortingStatModel.getPlayer(selectedRow);
    m_gamesModel.init();

    for (int i = 0; i < m_sortingStatModel.rowCount(); ++i)
    {
        QVariant playerPointer = m_sortingStatModel.data(m_sortingStatModel.index(i, 0), DataRoles::DataRole::Player);
        if (*(playerPointer.value<Player*>()) == *selectedPlayer)
        {
            m_sortingStatModel.selectRow(i);
            break;
        }
    }
}

Downloader::Downloader(QUrl url, QObject *parent)
    : QObject(parent), m_url(url)
{
    connect(&m_networkAccessManager, &QNetworkAccessManager::finished,
            this, &Downloader::resultReady);
}

void Downloader::sendDownloadRequest()
{
    m_networkAccessManager.get(QNetworkRequest(m_url));
}
