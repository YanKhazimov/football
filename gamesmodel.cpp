#include "gamesmodel.h"

Game::Game(QDate date, QVector<PlayerRef> hometeam, QVector<PlayerRef> awayteam, QPair<int, int> score)
    : m_date(date), m_hometeam(hometeam), m_awayteam(awayteam), m_score(score)
{
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

QDate Game::getDate() const
{
    return m_date;
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
    m_games << new Game(QDate(2018, 12, 21), {"player A"}, {"player D"}, {15, 15})
            << new Game(QDate(2018, 12, 22), {"player A"}, {"player E"}, {15, 15})
            << new Game(QDate(2018, 12, 23), {"player A"}, {"player E"}, {15, 15})
            << new Game(QDate(2018, 12, 24), {"player A"}, {"player E"}, {15, 15})
            << new Game(QDate(2018, 12, 25), {"player A"}, {"player E"}, {15, 15})
            << new Game(QDate(2018, 12, 26), {"player A"}, {"player E"}, {15, 15})
            << new Game(QDate(2018, 12, 27), {"player A"}, {"player E"}, {15, 15})
            << new Game(QDate(2018, 12, 28), {"player A"}, {"player E"}, {15, 15})
            << new Game(QDate(2018, 12, 29), {"player A"}, {"player E"}, {15, 15})
            << new Game(QDate(2018, 12, 30), {"player A"}, {"player E"}, {15, 15})
            << new Game(QDate(2018, 12, 31), {"player A"}, {"player E"}, {15, 15})
            << new Game(QDate(2019, 1, 1),
    {"player A", "player BBB", "player G", "player H", "player I", "player J", "player K"}, {"player F"}, {30, 1})
            << new Game(QDate::currentDate(), {"player A", "player BBB"}, {"player C"/*, "player D"*/}, {10, 10});
    endResetModel();

    return true;
}

int GamesModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : static_cast<int>(m_games.size());
}

QVariant GamesModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(index.parent() == QModelIndex() && index != QModelIndex());
    if (role == DataRoles::DataRole::GameDate)
    {
        return QVariant::fromValue(m_games[index.row()]->getDate());
    }
    else if (role == DataRoles::DataRole::ScoreDiff)
    {
        Game* game = m_games[index.row()];
        auto score = game->getScore();
        return QVariant::fromValue(score.first - score.second);
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
