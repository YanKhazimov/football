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

QVariantList GamesModel::getPulse()
{
    return m_pulse;
}

int GamesModel::getPulseRosterConsistency()
{
    int rosterConsistency = 0;

    for(const QVariant& game: m_pulse)
        rosterConsistency += game.value<QPoint>().y();

    return static_cast<int>(1.f * rosterConsistency / m_pulse.size());
}

void GamesModel::resetPulse()
{
    m_pulse.clear();

    for (int i = 0; i < rowCount(); ++i)
    {
        qint64 daysBack = m_games[i]->getDate().daysTo(QDate::currentDate());
        if (daysBack > 31)
            continue;

        int x = static_cast<int>(100.f * static_cast<float>(31 - daysBack) / 31);
        float y = 0;

        QVector<QString> players = m_games[i]->getHometeam() + m_games[i]->getAwayteam();
        for (const QString &playerRef: players)
        {
            int gamesCounted = 10;
            int maxRelevancePoints = (1 + gamesCounted) * gamesCounted / 2; // 1 + 2 + ... + gamesCounted
            int relevancePoints = 0;
            int firstCountedGameIndex = qMax(0, i - gamesCounted);
            for (int j = firstCountedGameIndex; j < i; ++j)
            {
                QVector<PlayerRef> jPlayers = m_games[j]->getHometeam() + m_games[j]->getAwayteam();
                if (jPlayers.contains(playerRef))
                    relevancePoints += j + 1 - firstCountedGameIndex;
            }

            float percentage = 100.f * relevancePoints / maxRelevancePoints;
            y += percentage;
        }
        m_pulse.append(QVariant::fromValue(QPoint(x, static_cast<int>(y / players.size()))));
    }

    emit pulseChanged();
}

GamesModel::~GamesModel()
{
    for (auto g: m_games)
        delete g;
}

bool GamesModel::init()
{
    m_gamesFilename = "data/games";

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

        m_games << new Game(QDate::fromString(QString::fromStdString(dateLine), "M/d/yyyy"),
                            qHomeTokens, qAwayTokens, score);
    }
    input.close();

    resetPulse();

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
    resetPulse();
    endInsertRows();
}
