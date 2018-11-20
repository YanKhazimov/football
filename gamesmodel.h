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
    QVector<PlayerRef> m_emptyteam;
    QPair<uint, uint> m_score;

public:
    Game() = delete;
    Game(QDate date, QVector<PlayerRef> hometeam, QVector<PlayerRef> awayteam, QPair<uint, uint> score);
    QVector<PlayerRef> getAllPlayers();
    const QVector<PlayerRef>& getWinners() const;
    const QVector<PlayerRef>& getLosers() const;
};

class GamesModel : public QAbstractListModel
{
    Q_OBJECT
    //Q_PROPERTY(QList<QObject*> games READ getGames CONSTANT)

public:
    GamesModel();
    ~GamesModel();
    bool init();

    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QList<Game*> getGames() const;

private:
    QList<Game*> m_games;
};

#endif // GAMESMODEL_H
