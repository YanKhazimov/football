#include "gamesmodel.h"

Game::Game(QDate date, QVector<PlayerRef> hometeam, QVector<PlayerRef> awayteam, QPair<uint, uint> score)
    : m_date(date), m_hometeam(hometeam), m_awayteam(awayteam), m_score(score)
{
}

QVector<PlayerRef> Game::getAllPlayers()
{
    return m_hometeam + m_awayteam;
}

const QVector<PlayerRef> &Game::getWinners() const
{
    if (m_score.first > m_score.second)
        return m_hometeam;
    else if (m_score.second > m_score.first)
        return m_awayteam;
    else
        return m_emptyteam;
}

const QVector<PlayerRef> &Game::getLosers() const
{
    if (m_score.first > m_score.second)
        return m_awayteam;
    else if (m_score.second > m_score.first)
        return m_hometeam;
    else
        return m_emptyteam;
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
    else if (role == DataRoles::DataRole::Winners)
    {
        Game* game = m_games[index.row()];
        game->getWinners();
    }

    return QVariant();
}

QList<Game*> GamesModel::getGames() const
{
    return m_games;
}
