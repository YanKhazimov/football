#include "globalstatsmodel.h"
#include <QDate>
#include <QDebug>

GlobalStatsModel::GlobalStatsModel(const Playerbase* base)
    : m_base(base), m_sourceModel(nullptr)
{
}

GlobalStatsModel::GlobalStatsModel(const GlobalStatsModel &model)
{
//    for (int r = 0; r < model.rowCount(); ++r)
//    {
//        this->appendRow(model.item(r));
//    }
}

void GlobalStatsModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    beginResetModel();

    if (m_sourceModel)
    {
//        disconnect(sourceModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
//                   this, SLOT(sourceDataChanged(QModelIndex,QModelIndex,QVector<int>)));

        disconnect(m_sourceModel, SIGNAL(rowsInserted(QModelIndex,int,int)),
                   this, SLOT(sourceRowsInserted(QModelIndex,int,int)));

        disconnect(m_sourceModel, SIGNAL(modelReset()), this, SLOT(resetModel()));
    }

    //QAbstractProxyModel::setSourceModel(sourceModel);

    if (sourceModel)
    {
        m_sourceModel = sourceModel;

//        connect(sourceModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
//                      this, SLOT(sourceDataChanged(QModelIndex,QModelIndex,QVector<int>)));

        connect(m_sourceModel, SIGNAL(rowsInserted(QModelIndex,int,int)),
                      this, SLOT(sourceRowsInserted(QModelIndex,int,int)));

        connect(m_sourceModel, SIGNAL(modelReset()), this, SLOT(resetModel()));
    }

    resetData();

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
    else if (role == DataRoles::DataRole::Progress)
    {
        int initialRating = m_base->getPlayer(playerData.first)->getInitialRating();
        int currentRating = playerData.second.empty() ? initialRating : playerData.second.back().changedRating;
        return QVariant::fromValue(currentRating - initialRating);
    }
    else if (role == DataRoles::DataRole::Dedication)
    {
        int firstParticipationGlobalIndex = playerData.second.first().sourceIndex.row();
        float percentage = 100.f * playerData.second.size() / (m_sourceModel->rowCount() - firstParticipationGlobalIndex);
        return QVariant::fromValue(percentage);
    }
    else if (role == DataRoles::DataRole::Relevance)
    {
        int firstParticipationGlobalIndex = playerData.second.first().sourceIndex.row();
        int gamesCounted = qMin(10, m_sourceModel->rowCount() - firstParticipationGlobalIndex);

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
        return QVariant::fromValue(m_selectedPlayerIndex == index.row());
    }
    else if (role == DataRoles::DataRole::RatingHistory)
    {
        QList<int> onlyRating {m_base->getPlayer(playerData.first)->getInitialRating()};
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
            if (game.resultSign * scoreDiff > 0)
            {
                // add a win
                collectSynergy(game.sourceIndex, DataRoles::DataRole::Hometeam, 0);
            }
            else if (game.resultSign * scoreDiff < 0)
            {
                // add a loss
                collectSynergy(game.sourceIndex, DataRoles::DataRole::Awayteam, 2);
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
        m_selectedPlayerIndex = value.toBool() ? index.row() : -1;
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

Player *GlobalStatsModel::getPlayer(int idx)
{
    QString name = data(index(idx, 0), DataRoles::DataRole::PlayerName).toString();
    return m_base->getPlayer(name);
}

int GlobalStatsModel::getPlayerRating(QString name)
{
    return m_playersData[name].last().changedRating;
}

void GlobalStatsModel::resetModel()
{
    if (!m_sourceModel)
        return;

    beginResetModel();
    resetData();
    endResetModel();
}

void GlobalStatsModel::sourceRowsInserted(QModelIndex parent, int first, int last)
{
    int ii = 0;
}

int getHomeRatingChange(int scoreDiff, float totalRating1, float totalRating2)
{
    return scoreDiff;
    float handicap = totalRating1 > totalRating2 ?
                totalRating2 / totalRating1 - 1 :
                totalRating1 / totalRating2 - 1;
    float expectedScoreDiff = handicap * 1000 / 3;

    int res;
    if (expectedScoreDiff * scoreDiff > 0)
    {
        //res =
    }
    //return (1 + scoreDiff / 3) * static_cast<int>(difficulty * 10);
}

void GlobalStatsModel::resetData()
{
    m_dates.resize(m_sourceModel->rowCount());
    m_playersData.clear();

    QMap<PlayerRef, int> currentRatings;
    for (PlayerRef playerRef: m_base->listAllPlayers())
    {
        currentRatings[playerRef] = m_base->getPlayer(playerRef)->getInitialRating();
    }

    auto sign = [](int val) -> int {
        return (0 < val) - (val < 0);
    };

    for (int i = 0; i < m_sourceModel->rowCount(); ++i)
    {
        QModelIndex sourceGameIndex = m_sourceModel->index(i, 0);
        m_dates[i] = sourceGameIndex.data(DataRoles::DataRole::GameDate).value<QDate>();
        int scoreDiff = sourceGameIndex.data(DataRoles::DataRole::ScoreDiff).toInt();
        QVector<PlayerRef> hometeam = sourceGameIndex.data(DataRoles::DataRole::Hometeam).value<QVector<PlayerRef>>();
        int hometeamTotal = 0;
        for (PlayerRef playerRef: hometeam)
        {
            hometeamTotal += currentRatings[playerRef];
        }
        QVector<PlayerRef> awayteam = sourceGameIndex.data(DataRoles::DataRole::Awayteam).value<QVector<PlayerRef>>();
        int awayteamTotal = 0;
        for (PlayerRef playerRef: awayteam)
        {
            awayteamTotal += currentRatings[playerRef];
        }
        int ratingChange = getHomeRatingChange(scoreDiff, hometeamTotal, awayteamTotal);

        for (const QString &playerRef: hometeam)
        {
            currentRatings[playerRef] += ratingChange;
            m_playersData[playerRef].push_back(PlayerGameStats(currentRatings[playerRef], sign(scoreDiff), sourceGameIndex));
        }
        for (PlayerRef playerRef: awayteam)
        {
            currentRatings[playerRef] -= ratingChange;
            m_playersData[playerRef].push_back(PlayerGameStats(currentRatings[playerRef], sign(-scoreDiff), sourceGameIndex));
        }
    }
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

GlobalStatsModel::PlayerGameStats::PlayerGameStats(int rating, int sign, QModelIndex srcIdx)
    : changedRating(rating), resultSign(sign), sourceIndex(srcIdx)
{
}
