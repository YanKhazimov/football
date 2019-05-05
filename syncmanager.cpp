#include "syncmanager.h"

#include <QFile>

SyncManager::SyncManager(GamesModel &gm, PlayerSortFilterProxyModel &ssm, GlobalStatsModel &gsm, const Language& lang, QObject *parent)
    : QObject(parent), m_gamesModel(gm), m_sortingStatModel(ssm), m_globalStatsModel(gsm), m_language(lang),
      m_playersDownloaded(false), m_gamesDownloaded(false),
      m_gamesUrl("https://dl.dropboxusercontent.com/s/fapzda1e3o5pby3/games?dl=0"),
      m_playersUrl("https://dl.dropboxusercontent.com/s/wfr30z1c3muqehb/players?dl=0")
{
}

void SyncManager::update()
{
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
        emit updateFinished(Language::dict.value("updateFail").value(m_language.get()));
        pReply->deleteLater();
        return;
    }

    if (pReply->url() == m_gamesUrl)
    {
        m_downloadedGamesData = pReply->readAll();
        pReply->deleteLater();
        m_gamesDownloaded = true;
        if (!m_playersDownloaded)
            return;
    }
    else if (pReply->url() == m_playersUrl)
    {
        m_downloadedPlayersData = pReply->readAll();
        pReply->deleteLater();
        m_playersDownloaded = true;
        if (!m_gamesDownloaded)
            return;
    }

    // downloaded all, start updating
    m_gamesDownloaded = m_playersDownloaded = false;

    readLocalData();

    if (m_downloadedGamesData == m_localGamesData && m_downloadedPlayersData == m_localPlayersData)
    {
        emit updateFinished(Language::dict.value("upToDate").value(m_language.get()));
        return;
    }

    if (m_downloadedPlayersData != m_localPlayersData)
    {
        QFile file("players");
        if (file.open(QIODevice::WriteOnly))
        {
            file.write(m_downloadedPlayersData);
            file.close();
        }
        m_globalStatsModel.setPlayerbase(Playerbase());
    }

    if (m_downloadedGamesData != m_localGamesData)
    {
        QFile file("games");
        if (file.open(QIODevice::WriteOnly))
        {
            file.write(m_downloadedGamesData);
            file.close();
        }
        applyGames();
    }

    emit updateFinished(Language::dict.value("updated").value(m_language.get()));
}

void SyncManager::readLocalData()
{
    QFile gamesFile("games");
    if (gamesFile.open(QIODevice::ReadOnly))
    {
        m_localGamesData = gamesFile.readAll();
        gamesFile.close();
    }

    QFile playersFile("players");
    if (playersFile.open(QIODevice::ReadOnly))
    {
        m_localPlayersData = playersFile.readAll();
        playersFile.close();
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
