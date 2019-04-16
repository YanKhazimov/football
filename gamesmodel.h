#ifndef GAMESMODEL_H
#define GAMESMODEL_H

#include <QAbstractListModel>
#include <QDate>
#include "player.h"
#include "dataroles.h"

class Game : public QObject
{
    Q_OBJECT

    QDate m_date;
    QVector<PlayerRef> m_hometeam;
    QVector<PlayerRef> m_awayteam;
    QPair<int, int> m_score; // home, away

public:
    Game() = delete;
    Game(QDate date, QVector<PlayerRef> hometeam, QVector<PlayerRef> awayteam, QPair<int, int> score);
    QVector<PlayerRef> getHometeam() const;
    QVector<PlayerRef> getAwayteam() const;
    QPair<int, int> getScore() const;
    QDate getDate() const;
};

class GamesModel : public QAbstractListModel //?
{
    Q_OBJECT

public:
    GamesModel() = default;
    ~GamesModel();
    bool init(QString gamesFilename);

    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE bool isDateBusy(const QDate &date) const;
    Q_INVOKABLE void addGame(QDate date, QStringList hometeam, int homeScore, QStringList awayteam, int awayScore);
    Q_INVOKABLE QStringList getSeasons() const;
    void saveGames();

private:
    QList<Game*> m_games;
    QString m_gamesFilename;
};

#endif // GAMESMODEL_H
