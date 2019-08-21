#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include <QFile>
#include <QDir>
#include "qt_zip/zipreader.h"
#include "qt_zip/zipwriter.h"
#include "syncmanager.h"

#define RAZDEVALKA_VERSION "2.0"

class Updater : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList messages MEMBER m_messages NOTIFY newMessage)

    Downloader* m_manifestDownloader, *m_newVersionDownloader;
    QStringList m_messages;
    QString m_targetVersion;

    bool backUp(QString dirname, QFileInfoList &entries);
    bool saveZip(const QByteArray& data, const QString &filename);
    bool unpack(const QString &archiveFile, const QString &whereTo);
    void showMessage(const QString& message);

public:
    explicit Updater(QObject *parent = nullptr);
    void checkForUpdates();

signals:
    void success();
    void error();
    void newMessage();

public slots:
    void versionManifestDownloaded(QNetworkReply* reply);
    void updateDownloaded(QNetworkReply* reply);
    void runWithNoCheck();
};

#endif // UPDATER_H
