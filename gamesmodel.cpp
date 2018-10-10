#include "gamesmodel.h"

Game::Game(QDate date, QVector<PlayerRef> hometeam, QVector<PlayerRef> awayteam)
    : m_date(date), m_hometeam(hometeam), m_awayteam(awayteam)
{
}

QVector<PlayerRef> Game::getAllPlayers()
{
    return m_hometeam + m_awayteam;
}

GamesModel::GamesModel()
{
}

GamesModel::~GamesModel()
{
    for (auto g: m_games)
        delete g;
}

bool GamesModel::init()
{
    m_games << new Game(QDate::currentDate(), {"p1", "p2"}, {"p3", "p4"})
            << new Game(QDate(2018, 12, 31), {"p1", "p2"}, {"p4", "p5"});
}

QList<Game*> GamesModel::getGames() const
{
    return m_games;
}
