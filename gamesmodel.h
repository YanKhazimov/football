#ifndef GAMESMODEL_H
#define GAMESMODEL_H

#include <QAbstractListModel>
#include <QDate>
#include "player.h"

class Game : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QDate date CONSTANT)

    QDate m_date;
    QVector<PlayerRef> m_hometeam;
    QVector<PlayerRef> m_awayteam;

public:
    Game() = delete;
    Game(QDate date, QVector<PlayerRef> hometeam, QVector<PlayerRef> awayteam);
    QVector<PlayerRef> getAllPlayers();
};

class GamesModel : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QList<QObject*> games READ getGames CONSTANT)

public:
    GamesModel();
    ~GamesModel();
    bool init();

    QList<Game*> getGames() const;

private:
    QList<Game*> m_games;
};

#endif // GAMESMODEL_H
