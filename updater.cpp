#include "updater.h"
#include <QCoreApplication>
#include <QProcess>
#include <QVersionNumber>

Updater::Updater(QObject *parent) : QObject(parent)
{
}

void Updater::checkForUpdates()
{
    // download version manifest and connect execution to versionManifestDownloaded
    showMessage("Checking for updates...");

    m_manifestDownloader = new Downloader(QUrl("https://dl.dropboxusercontent.com/s/y7zwgtpsucsyiqz/version_manifest.txt?dl=0"));
    QThread* manifestLoadingThread = new QThread();
    m_manifestDownloader->moveToThread(manifestLoadingThread);
    QObject::connect(manifestLoadingThread, &QThread::started, m_manifestDownloader, &Downloader::sendDownloadRequest);
    QObject::connect(m_manifestDownloader, &Downloader::resultReady, this, &Updater::versionManifestDownloaded);
    QObject::connect(m_manifestDownloader, &Downloader::resultReady, manifestLoadingThread, &QThread::quit);
    QObject::connect(m_manifestDownloader, &Downloader::resultReady, m_manifestDownloader, &Downloader::deleteLater);
    QObject::connect(manifestLoadingThread, &QThread::finished, manifestLoadingThread, &QThread::deleteLater);
    manifestLoadingThread->start();
}

void Updater::versionManifestDownloaded(QNetworkReply* reply)
{
    if (reply->error() || reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200)
    {
        showMessage(QString("Warning: Could not check for updates: error codes %2, %3. Version %1 will start.")
                    .arg(RAZDEVALKA_VERSION)
                    .arg(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt())
                    .arg(static_cast<int>(reply->error())));
        emit error();
    }
    else
    {
        QStringList latestVersionInfo = QString(reply->readLine()).split('\t');
        m_targetVersion = latestVersionInfo.at(0);
        if (QVersionNumber::fromString(m_targetVersion) > QVersionNumber::fromString(QString(RAZDEVALKA_VERSION))) // so that 2.10 > 2.9
        {
            showMessage(QString("Updating to version %1...").arg(m_targetVersion));

            QString link = latestVersionInfo.at(1);
            m_newVersionDownloader = new Downloader(QUrl(link));
            QThread* newVersion = new QThread();
            m_newVersionDownloader->moveToThread(newVersion);
            QObject::connect(newVersion, &QThread::started, m_newVersionDownloader, &Downloader::sendDownloadRequest);
            QObject::connect(m_newVersionDownloader, &Downloader::resultReady, this, &Updater::updateDownloaded);
            QObject::connect(m_newVersionDownloader, &Downloader::resultReady, newVersion, &QThread::quit);
            QObject::connect(m_newVersionDownloader, &Downloader::resultReady, m_newVersionDownloader, &Downloader::deleteLater);
            QObject::connect(newVersion, &QThread::finished, newVersion, &QThread::deleteLater);
            newVersion->start();
        }
        else
        {
            showMessage(QString("Latest version %1 is already installed. Launching...").arg(RAZDEVALKA_VERSION));
            emit success();
        }
    }
}

void Updater::updateDownloaded(QNetworkReply *reply)
{
    if (reply->error() || reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200)
    {
        showMessage(QString("Warning: Could not download the update: error codes %2, %3. Version %1 will start.")
                    .arg(RAZDEVALKA_VERSION)
                    .arg(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt())
                    .arg(static_cast<int>(reply->error())));
        emit error();
    }
    else
    {
        showMessage(QString("Downloaded version %1. Installing...").arg(m_targetVersion));

        QString zipname = QDir::tempPath() + QString("/Razdevalka.v%1.zip").arg(m_targetVersion);
        QFileInfoList entries;

        if (!saveZip(reply->readAll(), zipname))
        {
            showMessage(QString("Writing error! Version %1 will start.").arg(RAZDEVALKA_VERSION));
            emit error();
        }
        else if (!backUp("backup", entries))
        {
            showMessage(QString("Backup error! Version %1 will start.").arg(RAZDEVALKA_VERSION));
            emit error();
        }
        else if (!unpack(zipname, QCoreApplication::applicationDirPath()))
        {
            // restore backup
            for (QFileInfo entry: entries)
            {
                if (entry.isFile())
                    QFile::copy(QString("backup/" + entry.fileName()), entry.fileName());
                else if (entry.isDir())
                    QDir().rename(QString("backup/" + entry.fileName()), entry.fileName());
            }

            showMessage(QString("Unpack error! Version %1 will start.").arg(RAZDEVALKA_VERSION));
            emit error();
        }
        else
        {
            showMessage(QString("Version %1 is installed. Launching...").arg(m_targetVersion));
            emit success();
        }
    }

}

void Updater::runWithNoCheck()
{
    QString program = "Razdevalka.exe -nocheck";
    QProcess::startDetached(program);
    exit(0);
}

bool Updater::backUp(QString dirname, QFileInfoList& entries)
{
    QString exePath = QCoreApplication::applicationDirPath();

    // remove old backup if any
    QDir backupDir(exePath + "/" + dirname);
    if (backupDir.exists() && !backupDir.removeRecursively())
        return false;

    entries = QDir(exePath).entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList movedEntries;

    // creating backup dir
    if (!QDir(exePath).mkdir(dirname))
        return false;

    // backing up
    auto rollback = [dirname](const QFileInfoList& movedEntries) {
        for (QFileInfo entry: movedEntries)
        {
            if (QFileInfo(dirname + "/" + entry.fileName()).isFile())
                QFile::rename(QString(dirname + "/" + entry.fileName()), entry.fileName());
            else if (QFileInfo(dirname + "/" + entry.fileName()).isDir())
                QDir().rename(dirname + "/" + entry.fileName(), entry.fileName());
        }
    };

    for (QFileInfo entry: entries)
    {
        if ((entry.isFile() && QFile::rename(entry.fileName(), QString(dirname + "/" + entry.fileName()))) ||
                (entry.isDir() && QDir().rename(entry.fileName(), dirname + "/" + entry.fileName())))
        {
            movedEntries.append(entry.fileName());
        }
        else
        {
            rollback(movedEntries);
            return false;
        }
    }

    return true;
}

bool Updater::saveZip(const QByteArray &data, const QString& filename)
{
    QFile zip(filename);
    if (zip.exists())
        zip.remove();

    if (zip.open(QFile::OpenModeFlag::WriteOnly))
    {
        zip.write(data);
        zip.close();
        return true;
    }

    return false;
}

bool Updater::unpack(const QString& archiveFile, const QString& whereTo)
{
    ZipReader archive(archiveFile);
    bool result = !archive.fileInfoList().empty() && archive.extractAll(whereTo);
    archive.close();

    return result;
}

void Updater::showMessage(const QString &message)
{
    m_messages.append(message);
    emit newMessage();
}
