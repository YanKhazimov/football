#include "globalstatsmodel.h"
#include "language.h"
#include <QDate>
#include <QDebug>
#include <math.h>

GlobalStatsModel::GlobalStatsModel(const Playerbase* base)
    : m_base(base), m_sourceModel(nullptr)
{
}

GlobalStatsModel::GlobalStatsModel(const GlobalStatsModel &model)
{
    m_base = model.m_base;
    m_sourceModel = model.m_sourceModel;
    m_playersData = model.m_playersData;
    m_seasonStartingRating = model.m_seasonStartingRating;
    m_selectedPlayer = model.m_selectedPlayer;
    m_minDate = model.m_minDate;
    m_maxDate = model.m_maxDate;
    m_awards = model.m_awards;
}

void GlobalStatsModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    beginResetModel();

    if (m_sourceModel)
    {
        disconnect(m_sourceModel, SIGNAL(rowsInserted(QModelIndex,int,int)),
                   this, SLOT(sourceRowsInserted(QModelIndex,int,int)));

        disconnect(m_sourceModel, SIGNAL(modelReset()), this, SLOT(resetModel()));
    }

    //QAbstractProxyModel::setSourceModel(sourceModel);

    if (sourceModel)
    {
        m_sourceModel = sourceModel;

        connect(m_sourceModel, SIGNAL(rowsInserted(QModelIndex,int,int)),
                      this, SLOT(sourceRowsInserted(QModelIndex,int,int)));

        connect(m_sourceModel, SIGNAL(modelReset()), this, SLOT(resetModel()));
    }

    int firstSeason = m_sourceModel->index(0, 0).data(DataRoles::DataRole::GameDate).toDate().year();
    m_minDate.setDate(firstSeason, 1, 1);
    m_maxDate = QDate::currentDate();
    resetData();
    resetAwards();

    endResetModel();
}

int GlobalStatsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return static_cast<int>(m_playersData.size());
}

int GlobalStatsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_sourceModel ? m_sourceModel->rowCount() + 1 : 0;
}

bool GlobalStatsModel::setSeasonFilter(QString filter)
{
    PlayerRef selectedPlayerCopy = m_selectedPlayer;
    QModelIndex selectedIndex = getIndexByRef(selectedPlayerCopy);

    if (Language::dict.value("all").values().contains(filter))
    {
        beginResetModel();
        int firstSeason = m_sourceModel->index(0, 0).data(DataRoles::DataRole::GameDate).toDate().year();
        m_minDate.setDate(firstSeason, 1, 1);
        m_maxDate = QDate::currentDate();
        resetData();
        endResetModel();

        m_selectedPlayer = selectedPlayerCopy;
        if (m_playersData.count(selectedPlayerCopy))
        {
            selectedIndex = getIndexByRef(selectedPlayerCopy);
            emit dataChanged(selectedIndex, selectedIndex, {DataRoles::DataRole::PlayerSelection});
        }

        return true;
    }

    int year = filter.toInt();
    if (year == 0)
        return false;

    beginResetModel();
    m_minDate.setDate(year, 1, 1);
    m_maxDate.setDate(year, 12, 31);
    resetData();
    endResetModel();

    m_selectedPlayer = selectedPlayerCopy;
    if (m_playersData.count(selectedPlayerCopy))
    {
        selectedIndex = getIndexByRef(selectedPlayerCopy);
        emit dataChanged(selectedIndex, selectedIndex, {DataRoles::DataRole::PlayerSelection});
    }

    return true;
}

QVariant GlobalStatsModel::getAwards(const QStringList &categoryFilter, const QStringList &rankFilter,
                                      const QList<int> &seasonFilter, const QStringList &playerFilter)
{
    return m_awards.filter(rankFilter, seasonFilter, categoryFilter, playerFilter);
}

QVariant GlobalStatsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.parent().isValid())
        return QVariant();

    Q_ASSERT(index.model() == this);

    if (index.row() >= static_cast<int>(m_playersData.size()) || index.column() >= m_sourceModel->rowCount() + 1)
        return QVariant();

    const std::pair<PlayerRef, QVector<GlobalStatsModel::PlayerGameStats>> playerData = getPlayerData(index);

    if (role == DataRoles::DataRole::PlayerName)
    {
        return QVariant::fromValue(playerData.first);
    }
    else if (role == DataRoles::DataRole::Player)
    {
        return QVariant::fromValue(m_base->getPlayer(playerData.first));
    }
    else if (role == DataRoles::DataRole::Rating)
    {
        return QVariant::fromValue(playerData.second.back().changedRating);
    }
    else if (role == DataRoles::DataRole::WinsLosses)
    {
        int wins = 0, draws = 0, losses = 0;
        for (int i = 0; i < playerData.second.size(); ++i)
        {
            const int& win = playerData.second[i].resultSign;

            if (win < 0)
                ++losses;
            else if (win > 0)
                ++wins;
            else
                ++draws;
        }

        return QVariant::fromValue(QVector<int>{ wins, draws, losses });
    }
    else if (role == DataRoles::DataRole::CurrentStreak)
    {
        if (playerData.second.empty())
            return QVariant::fromValue(0);

        int i = playerData.second.size() - 1;
        const int& win = playerData.second[i].resultSign;
        int result = 1;
        for (--i; i >= 0 && playerData.second[i].resultSign == win; --i)
        {
            ++result;
        }
        return QVariant::fromValue(result * win);
    }
    else if (role == DataRoles::DataRole::Progress)
    {
        int initialRating = m_seasonStartingRating[playerData.first];
        int currentRating = playerData.second.empty() ? initialRating : playerData.second.back().changedRating;
        return QVariant::fromValue(currentRating - initialRating);
    }
    else if (role == DataRoles::DataRole::Dedication)
    {
        int firstParticipationGlobalIndex = m_sourceModel->rowCount();
        for (int i = 0; i < m_sourceModel->rowCount(); ++i)
        {
            QModelIndex idx = m_sourceModel->index(i, 0);
            if (idx.data(DataRoles::DataRole::Hometeam).value<QVector<PlayerRef>>().contains(playerData.first) ||
                    idx.data(DataRoles::DataRole::Awayteam).value<QVector<PlayerRef>>().contains(playerData.first))
            {
                firstParticipationGlobalIndex = i;
                break;
            }
        }
        if (firstParticipationGlobalIndex == m_sourceModel->rowCount())
            return QVariant::fromValue(0);

        int seasonGames = 0;
        for (int i = 0; i < m_sourceModel->rowCount(); ++i)
        {
            QDate gameDate = m_sourceModel->index(i, 0).data(DataRoles::DataRole::GameDate).value<QDate>();
            if (m_minDate <= gameDate && gameDate <= m_maxDate // a current season game
                    && firstParticipationGlobalIndex <= i) // player has played
                ++seasonGames;
        }
        float percentage = 100.f * playerData.second.size() / seasonGames;
        return QVariant::fromValue(percentage);
    }
    else if (role == DataRoles::DataRole::Relevance)
    {
        int gamesCounted = 10;//qMin(10, m_sourceModel->rowCount() - firstParticipationGlobalIndex);

        int maxRelevancePoints = (1 + gamesCounted) * gamesCounted / 2; // 1 + 2 + ... + gamesCounted
        int relevancePoints = 0;
        int firstCountedGameIndex = m_sourceModel->rowCount() - gamesCounted;
        for (int i = firstCountedGameIndex; i < m_sourceModel->rowCount(); ++i)
        {
            QModelIndex gameIndex = m_sourceModel->index(i, 0);
            QVector<PlayerRef> participants =
                    gameIndex.data(DataRoles::DataRole::Hometeam).value<QVector<PlayerRef>>() +
                    gameIndex.data(DataRoles::DataRole::Awayteam).value<QVector<PlayerRef>>();
            if (qFind(participants, playerData.first) != participants.end())
                relevancePoints += i + 1 - firstCountedGameIndex;
        }

        float percentage = 100.f * relevancePoints / maxRelevancePoints;
        return QVariant::fromValue(percentage);
    }
    else if (role == DataRoles::DataRole::PlayerSelection)
    {
        auto iter = m_playersData.begin();
        std::advance(iter, std::min(index.row(), static_cast<int>(m_playersData.size())));
        return QVariant::fromValue(iter != m_playersData.end() && iter->first == m_selectedPlayer);
    }
    else if (role == DataRoles::DataRole::RatingHistory)
    {
        QList<int> onlyRating {m_seasonStartingRating[playerData.first]};
        for (const PlayerGameStats& game: playerData.second)
        {
            onlyRating.append(game.changedRating);
        }
        return QVariant::fromValue(onlyRating);
    }
    else if (role == DataRoles::DataRole::Synergy)
    {
        typedef QVector<int> WDL;
        QMap<PlayerRef, WDL> synergy;

        auto collectSynergy = [&playerData, &synergy](const QModelIndex& gameIdx, DataRoles::DataRole teamRole, int outcomeIdx) -> bool
        {
            QVector<PlayerRef> team = gameIdx.data(teamRole).value<QVector<PlayerRef>>();
            if (team.indexOf(playerData.first) == -1)
                return false;

            for (PlayerRef p: team)
            {
                if (synergy.count(p) > 0)
                    ++synergy[p][outcomeIdx];
                else
                {
                    WDL wdl(3, 0);
                    ++wdl[outcomeIdx];
                    synergy[p] = wdl;
                }
            }
            return true;
        };

        for (const PlayerGameStats& game: playerData.second)
        {
            int scoreDiff = game.sourceIndex.data(DataRoles::DataRole::ScoreDiff).toInt();
            if (game.resultSign > 0)
            {
                // add a win
                collectSynergy(game.sourceIndex, scoreDiff > 0 ? DataRoles::DataRole::Hometeam : DataRoles::DataRole::Awayteam, 0);
            }
            else if (game.resultSign < 0)
            {
                // add a loss
                collectSynergy(game.sourceIndex, scoreDiff > 0 ? DataRoles::DataRole::Awayteam : DataRoles::DataRole::Hometeam, 2);
            }
            else
            {
                // add a draw
                if (!collectSynergy(game.sourceIndex, DataRoles::DataRole::Hometeam, 1))
                    collectSynergy(game.sourceIndex, DataRoles::DataRole::Awayteam, 1);
            }
        }
        return QVariant::fromValue(synergy);
    }

    return QVariant();
}

bool GlobalStatsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_ASSERT(index.model() == this);
    if (role == DataRoles::DataRole::PlayerSelection)
    {
        auto iter = m_playersData.begin();
        std::advance(iter, std::min(index.row(), static_cast<int>(m_playersData.size())));
        m_selectedPlayer = value.toBool() ? iter->first : "";
        emit dataChanged(index, index, {role});
        return true;
    }

    return QAbstractTableModel::setData(index, value, role);
}

QHash<int, QByteArray> GlobalStatsModel::roleNames() const
{
    QHash<int, QByteArray> result;

    result[DataRoles::DataRole::PlayerName] = "PlayerName";
    result[DataRoles::DataRole::Rating] = "Rating";
    result[DataRoles::DataRole::WinsLosses] = "WinsLosses";
    result[DataRoles::DataRole::Progress] = "Progress";
    result[DataRoles::DataRole::Relevance] = "Relevance";
    result[DataRoles::DataRole::Dedication] = "Dedication";

    return result;
}

Player *GlobalStatsModel::getPlayer(QString name)
{
    return m_base->getPlayer(name);
}

int GlobalStatsModel::getRating(const PlayerRef &name) const
{
    if (m_playersData.count(name) == 0 ||
            m_playersData.at(name).empty())
    {
        throw std::exception();
        return -1;
    }

    return m_playersData.at(name).last().changedRating;
}

void GlobalStatsModel::resetModel()
{
    if (!m_sourceModel)
        return;

    beginResetModel();
    resetData();
    resetAwards();
    endResetModel();
}

void GlobalStatsModel::sourceRowsInserted(QModelIndex parent, int first, int last)
{
    resetModel(); // TODO accurate updates instead of reset
}

int getHomeRatingChange(int scoreDiff, int totalRating1, int totalRating2)
{
    if (scoreDiff == 0)
        return 0;

    double coeff = 8.0;
    double winnersRatingSum = static_cast<double>(scoreDiff > 0 ? totalRating1 : totalRating2);
    double losersRatingSum = static_cast<double>(scoreDiff > 0 ? totalRating2 : totalRating1);
    double chances = pow(10.0, losersRatingSum/1000.0) /
            (pow(10.0, winnersRatingSum/1000.0) + pow(10.0, losersRatingSum/1000.0));
    int absChange = static_cast<int>(ceil(coeff * chances * std::min(5.0, 1.0 + fabs(double(scoreDiff))/3.0)));
    return scoreDiff > 0 ? absChange : -absChange;
    //return scoreDiff;

    float handicap = totalRating1 > totalRating2 ?
                totalRating2 / totalRating1 - 1 :
                totalRating1 / totalRating2 - 1;
    float expectedScoreDiff = handicap * 1000 / 3;

    if (expectedScoreDiff * scoreDiff > 0)
    {
        //res =
    }
    //return (1 + scoreDiff / 3) * static_cast<int>(difficulty * 10);
}

void GlobalStatsModel::resetData()
{
    m_playersData.clear();
    m_seasonStartingRating.clear();
    m_selectedPlayer.clear();

    QModelIndex selectedIndex = getIndexByRef(m_selectedPlayer);
    emit dataChanged(selectedIndex, selectedIndex, {DataRoles::DataRole::PlayerSelection});

    QMap<PlayerRef, int> currentRatings;
    for (PlayerRef playerRef: m_base->listAllPlayers())
    {
        currentRatings[playerRef] = m_base->getPlayer(playerRef)->getInitialRating();
    }

    auto sign = [](int val) -> int {
        return (0 < val) - (val < 0);
    };

    auto deduceRating = [](const PlayerRef& player) -> int {
        return player.mid(player.size() - 4, 4).toInt();
    };

    for (int i = 0; i < m_sourceModel->rowCount(); ++i)
    {
        QModelIndex sourceGameIndex = m_sourceModel->index(i, 0);
        int scoreDiff = sourceGameIndex.data(DataRoles::DataRole::ScoreDiff).toInt();
        QVector<PlayerRef> hometeam = sourceGameIndex.data(DataRoles::DataRole::Hometeam).value<QVector<PlayerRef>>();
        int hometeamTotal = 0;
        for (PlayerRef playerRef: hometeam)
        {
            int aprioriRating = m_base->getPlayer(playerRef) ? currentRatings[playerRef] : deduceRating(playerRef);
            hometeamTotal += aprioriRating;
        }
        QVector<PlayerRef> awayteam = sourceGameIndex.data(DataRoles::DataRole::Awayteam).value<QVector<PlayerRef>>();
        int awayteamTotal = 0;
        for (PlayerRef playerRef: awayteam)
        {
            int aprioriRating = m_base->getPlayer(playerRef) ? currentRatings[playerRef] : deduceRating(playerRef);
            awayteamTotal += aprioriRating;
        }
        int ratingChange = getHomeRatingChange(scoreDiff, hometeamTotal, awayteamTotal);

        QDate gameDate = sourceGameIndex.data(DataRoles::DataRole::GameDate).value<QDate>();
        if (m_minDate <= gameDate && m_seasonStartingRating.empty())
        {
            m_seasonStartingRating = currentRatings;
        }
        if (m_maxDate < gameDate)
            break;

        for (const QString &playerRef: hometeam)
        {
            if (!m_base->getPlayer(playerRef))
                continue;

            currentRatings[playerRef] += ratingChange;
            if (m_minDate <= gameDate)
                m_playersData[playerRef].push_back(PlayerGameStats(currentRatings[playerRef], sign(scoreDiff), sourceGameIndex));
        }
        for (PlayerRef playerRef: awayteam)
        {
            if (!m_base->getPlayer(playerRef))
                continue;

            currentRatings[playerRef] -= ratingChange;

            if (m_minDate <= gameDate)
                m_playersData[playerRef].push_back(PlayerGameStats(currentRatings[playerRef], sign(-scoreDiff), sourceGameIndex));
        }
    }
}

void GlobalStatsModel::resetSeasonAwards(int season)
{
    if (season >= QDate::currentDate().year())
        return; // awards only for completed seasons

    QMap<DataRoles::DataRole, QString> roleStrings {
        {DataRoles::DataRole::Progress, "Progress"},
        {DataRoles::DataRole::Dedication, "Dedication"}
    };

    for (DataRoles::DataRole role: roleStrings.keys())
    {
        std::map<QVariant, QStringList> sortedByCategory;
        for (int i = 0; i < rowCount(); ++i)
        {
            QString playerName = index(i, 0).data(DataRoles::DataRole::PlayerName).toString();

            if (m_playersData.count(playerName) > 0 && m_playersData.at(playerName).size() < 5)
                continue; // not awarding players with <5 games in the season

            QVariant score = index(i, 0).data(role);
            if (score.type() == QMetaType::Float)
            {
                score = QVariant::fromValue(static_cast<int>(score.toFloat()));
            }
            sortedByCategory[score].append(playerName);
        }

        std::map<QVariant, QStringList>::reverse_iterator iter = sortedByCategory.rbegin();
        int medalsGiven = 0;

        QStringList ranks {"GOLD", "SILVER", "BRONZE"};
        for (QStringList::iterator rank = ranks.begin(); rank != ranks.end(); )
        {
            if (iter == sortedByCategory.rend() || medalsGiven >= 3)
                break;

            QString roleRepresentation;
            if (role == DataRoles::DataRole::Progress) {
                int progress = iter->first.toInt();
                QString extraSign = progress > 0 ? "+" : "";
                roleRepresentation = extraSign + QString::number(progress);
            }
            else if (role == DataRoles::DataRole::Dedication)
            {
                roleRepresentation = QString::number(iter->first.toInt()) + "%";
            }

            for (QString playerName: iter->second)
            {
                m_awards.add(Award(season, roleStrings.value(role), *rank, m_base->getPlayer(playerName), roleRepresentation));
            }

            medalsGiven += iter->second.size();
            rank += iter->second.size();
            ++iter;
        }
    }
}

void GlobalStatsModel::resetAwards()
{
    m_awards.clear();

    std::map<PlayerRef, QVector<PlayerGameStats>> backupData = m_playersData;
    QMap<PlayerRef, int> startingRatingBackup = m_seasonStartingRating;
    QDate minBackup = m_minDate, maxBackup = m_maxDate;

    for (int season = minBackup.year(); season <= maxBackup.year(); ++season)
    {
        m_minDate.setDate(season, 1, 1);
        m_maxDate.setDate(season, 12, 31);

        m_playersData.clear();
        for (auto playerData: backupData)
        {
            for (auto game: playerData.second)
            {
                if (game.sourceIndex.data(DataRoles::DataRole::GameDate).toDate().year() == season)
                    m_playersData[playerData.first].append(game);
            }
        }

        resetSeasonAwards(season);

        for (auto playerData: m_playersData)
            m_seasonStartingRating[playerData.first] = playerData.second.last().changedRating;
    }

    m_playersData = backupData;
    m_seasonStartingRating = startingRatingBackup;
    m_minDate = minBackup;
    m_maxDate = maxBackup;
}

std::pair<PlayerRef, QVector<GlobalStatsModel::PlayerGameStats> > GlobalStatsModel::getPlayerData(const QModelIndex &index) const
{
    std::map<PlayerRef, QVector<PlayerGameStats>>::const_iterator playerIter = m_playersData.begin();
    if (playerIter != m_playersData.end())
    {
        std::advance(playerIter, index.row());
        if (playerIter != m_playersData.end())
        {
            return *playerIter;
        }
    }

    return std::make_pair<PlayerRef, QVector<PlayerGameStats>>(PlayerRef(), QVector<PlayerGameStats>());
}

QModelIndex GlobalStatsModel::getIndexByRef(const PlayerRef &ref)
{
    std::map<PlayerRef, QVector<PlayerGameStats>>::const_iterator playerIter = m_playersData.find(ref);
    std::map<PlayerRef, QVector<PlayerGameStats>>::const_iterator begin = m_playersData.begin();
    return playerIter == m_playersData.end() ? QModelIndex() : index(static_cast<int>(std::distance(begin, playerIter)), 0);
}

GlobalStatsModel::PlayerGameStats::PlayerGameStats(int rating, int sign, QModelIndex srcIdx)
    : changedRating(rating), resultSign(sign), sourceIndex(srcIdx)
{
}
