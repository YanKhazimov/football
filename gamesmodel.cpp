#include "gamesmodel.h"
#include <fstream>
#include <QFile>
#include <QTextStream>

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

QStringList GamesModel::getSeasons() const
{
    QStringList seasons;

    for (Game* game: m_games)
    {
        QString season = QString::number(game->getDate().year());
        if (seasons.isEmpty() || season != seasons.last())
            seasons.append(season);
    }

    return seasons;
}

void GamesModel::saveGames()
{
    QFile file(m_gamesFilename);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");

        for (auto game: m_games)
        {
            stream << game->getDate().toString(Qt::SystemLocaleShortDate);

            QPair<int, int> score = game->getScore();
            stream << '\n' << score.first;
            for (auto player: game->getHometeam())
                stream << '\t' << player.toUtf8();

            stream << '\n' << score.second;
            for (auto player: game->getAwayteam())
                stream << '\t' << player.toUtf8();

            stream << '\n';
        }

        file.close();
    }
}

GamesModel::~GamesModel()
{
    for (auto g: m_games)
        delete g;
}

bool GamesModel::init()
{
    m_gamesFilename = "games";

    beginResetModel();

    for (auto g: m_games)
        delete g;
    m_games.clear();

    std::ifstream input(m_gamesFilename.toStdString().c_str());
    std::string dateLine, homeLine, awayLine;
    while (getline(input, dateLine) && getline(input, homeLine) && getline(input, awayLine))
    {
        QVector<PlayerRef> qHomeTokens = QString::fromStdString(homeLine).split("\t", QString::SplitBehavior::SkipEmptyParts).toVector();
        QVector<PlayerRef> qAwayTokens = QString::fromStdString(awayLine).split("\t", QString::SplitBehavior::SkipEmptyParts).toVector();

        QPair<int, int> score (qHomeTokens.takeFirst().toInt(), qAwayTokens.takeFirst().toInt());

        m_games << new Game(QDate::fromString(QString::fromStdString(dateLine), Qt::SystemLocaleShortDate),
                            qHomeTokens, qAwayTokens, score);
    }
    input.close();

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

bool GamesModel::isDateBusy(const QDate& date) const
{
    for (Game* g: m_games)
    {
        if (g->getDate() == date)
            return true;
    }

    return false;
}

void GamesModel::addGame(QDate date,
                         QStringList hometeam,
                         int homeScore,
                         QStringList awayteam,
                         int awayScore)
{
    int row = 0;
    for (; row < rowCount(); ++row)
    {
        if (m_games[row]->getDate() > date)
            break;
    }

    beginInsertRows(QModelIndex(), row, row);
    m_games.insert(row, new Game(date, hometeam.toVector(), awayteam.toVector(), {homeScore, awayScore}));
    saveGames();
    endInsertRows();
}
