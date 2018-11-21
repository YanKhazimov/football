#include "gamesmodel.h"

Game::Game(QDate date, QVector<PlayerRef> hometeam, QVector<PlayerRef> awayteam, QPair<int, int> score)
    : m_date(date), m_hometeam(hometeam), m_awayteam(awayteam), m_score(score)
{
}

QVector<PlayerRef> Game::getAllParticipants()
{
    return m_hometeam + m_awayteam;
}

QVector<PlayerRef> Game::getHometeam() const
{
    return m_hometeam;
}

QVector<PlayerRef> Game::getAwayteam() const
{
    return m_awayteam;
}

QPair<int, int> Game::getScore() const
{
    return m_score;
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
    beginResetModel();
    m_games << new Game(QDate::currentDate(), {"p1", "p2"}, {"p3", "p4"}, {20, 10})
            << new Game(QDate(2018, 12, 31), {"p1"}, {"p5"}, {15, 15})
            << new Game(QDate(2019, 1, 1), {"p1", "p2", "p7", "p8", "p9", "p10", "p11", "p12"}, {"p6"}, {30, 1});
    endResetModel();

    return true;
}

int GamesModel::rowCount(const QModelIndex &parent) const
{
    Q_ASSERT(parent == QModelIndex());
    return static_cast<int>(m_games.size());
}

QVariant GamesModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(index.parent() == QModelIndex() && index != QModelIndex());
    if (role == Qt::DisplayRole)
    {
        return QVariant::fromValue(m_games[index.row()]);
    }
    else if (role == DataRoles::DataRole::ScoreDiff)
    {
        Game* game = m_games[index.row()];
        return QVariant::fromValue(game->getScore());
    }
    else if (role == DataRoles::DataRole::Hometeam)
    {
        Game* game = m_games[index.row()];
        return QVariant::fromValue(game->getHometeam());
    }
    else if (role == DataRoles::DataRole::Awayteam)
    {
        Game* game = m_games[index.row()];
        return QVariant::fromValue(game->getAwayteam());
    }

    return QVariant();
}

QList<Game*> GamesModel::getGames() const
{
    return m_games;
}
