#include "globalstatsmodel.h"
#include <QDebug>

GlobalStatsModel::GlobalStatsModel(const Playerbase* base)
    : m_sourceModel(nullptr), m_base(base)
{
}

GlobalStatsModel::GlobalStatsModel(const GlobalStatsModel &model)
{
//    for (int r = 0; r < model.rowCount(); ++r)
//    {
//        this->appendRow(model.item(r));
//    }
}

GlobalStatsModel::GlobalStatsModel(const PlayersModel &model, Playerbase* base)
    : m_base(base)
{
//    float random = 99.9f;
//    for (auto p: model.getPlayers())
//    {
//        QStandardItem* item;

//        item = new QStandardItem;
//        item->setData(QString(p), PlayerName);
//        item->setData(QString::number(random), Rating);
//        item->setData(QString("5-3 (62.5%)"), WinsLosses);
//        item->setData(QString("-20"), Progress);
//        item->setData(QString("50%"), Dedication);
//        this->appendRow(item);
//        random /= 1.1f;
//    }
}

void GlobalStatsModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    beginResetModel();

    if (m_sourceModel)
    {
//        disconnect(sourceModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
//                   this, SLOT(sourceDataChanged(QModelIndex,QModelIndex,QVector<int>)));

//        disconnect(sourceModel(), SIGNAL(rowsInserted(QModelIndex,int,int)),
//                   this, SLOT(sourceRowsInserted(QModelIndex,int,int)));

        disconnect(m_sourceModel, SIGNAL(modelReset()), this, SLOT(resetModel()));
    }

    //QAbstractProxyModel::setSourceModel(sourceModel);

    if (sourceModel)
    {
        m_sourceModel = sourceModel;

//        connect(sourceModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
//                      this, SLOT(sourceDataChanged(QModelIndex,QModelIndex,QVector<int>)));

//        connect(sourceModel(), SIGNAL(rowsInserted(QModelIndex,int,int)),
//                      this, SLOT(sourceRowsInserted(QModelIndex,int,int)));

        connect(m_sourceModel, SIGNAL(modelReset()), this, SLOT(resetModel()));
    }

    resetData();

    endResetModel();
}

//QModelIndex GlobalStatsModel::mapToSource(const QModelIndex &proxyIndex) const
//{
//    return QModelIndex();
//}

//QModelIndex GlobalStatsModel::mapFromSource(const QModelIndex &sourceIndex) const
//{
//    return QModelIndex();
//}

int GlobalStatsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_players.size();
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

    if (index.row() >= m_players.size() || index.column() >= m_sourceModel->rowCount() + 1)
        return QVariant();

    if (role == DataRoles::DataRole::PlayerName)
    {
        auto playerIter = m_players.begin();
        std::advance(playerIter, index.row());
        return QVariant::fromValue(playerIter->first);
    }
    else if (role == DataRoles::DataRole::Player)
    {
        auto playerIter = m_players.begin();
        std::advance(playerIter, index.row());
        return QVariant::fromValue(m_base->getPlayer(playerIter->first));
    }
    else if (role == DataRoles::DataRole::Rating)
    {
        auto playerIter = m_players.begin();
        std::advance(playerIter, index.row());
        return QVariant::fromValue(playerIter->second.back().changedRating);
    }
    else if (role == DataRoles::DataRole::WinsLosses)
    {
        auto playerIter = m_players.begin();
        std::advance(playerIter, index.row());
        int wins = 0, draws = 0, losses = 0;
        for (int i = 1; i < playerIter->second.size(); ++i)
        {
            QModelIndex sourceGameIndex = m_sourceModel->index(i - 1, 0);
            int scoreDiff = sourceGameIndex.data(DataRoles::DataRole::ScoreDiff).toInt();
            QVector<PlayerRef> awayteam = sourceGameIndex.data(DataRoles::DataRole::Awayteam).value<QVector<PlayerRef>>();
            if (awayteam.contains(playerIter->first))
            {
                scoreDiff = -scoreDiff;
            }
            else
            {
                QVector<PlayerRef> hometeam = sourceGameIndex.data(DataRoles::DataRole::Hometeam).value<QVector<PlayerRef>>();
                if (!hometeam.contains(playerIter->first))
                    continue;
            }

            if (scoreDiff < 0)
                ++losses;
            else if (scoreDiff > 0)
                ++wins;
            else
                ++draws;
        }

        return QVariant::fromValue(QString("%1-%2-%3").arg(QString::number(wins)).arg(QString::number(draws)).arg(QString::number(losses)));
    }
    else if (role == DataRoles::DataRole::Progress)
    {
        auto playerIter = m_players.begin();
        std::advance(playerIter, index.row());
        int initialRating = playerIter->second[0].changedRating;
        int currentRating = playerIter->second.back().changedRating;
        return QVariant::fromValue(currentRating - initialRating);
    }
    else if (role == DataRoles::DataRole::Dedication)
    {
        auto playerIter = m_players.begin();
        std::advance(playerIter, index.row());
        int attended = 0, missed = 0;
        for (int i = 1; i < playerIter->second.size(); ++i)
        {
            if (playerIter->second[i].participation)
                ++attended;
            else
                ++missed;
        }
        float percentage = static_cast<float>(attended)/(attended + missed) * 100;
        return QVariant::fromValue(QString::number(percentage) + "%");
    }
    else if (role == DataRoles::DataRole::PlayerSelection)
    {
        return QVariant::fromValue(m_selectedPlayerIndex == index.row());
    }
    else if (role == DataRoles::DataRole::RatingHistory)
    {
        auto playerIter = m_players.begin();
        std::advance(playerIter, index.row());

        const QVector<PlayerGameStats>& ratings = playerIter->second;
        QVector<QPair<QDate, int>> history(ratings.size(),
                                           qMakePair(QDate(2000, 1, 1), ratings[0].changedRating));
        for (int i = 1; i < ratings.size(); ++i)
        {
            if (ratings[i].participation)
            {
                QModelIndex sourceIndex = m_sourceModel->index(i - 1, 0);
                QDate date = m_sourceModel->data(sourceIndex, DataRoles::DataRole::GameDate).value<QDate>();
                history[i] = qMakePair(date, ratings[i].changedRating);
            }
        }
        return QVariant::fromValue(history);
    }

    return QVariant();
}

bool GlobalStatsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
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

bool GlobalStatsModel::selectRow(int row)
{
    qDebug() << "set selection" << row;
    setData(index(m_selectedPlayerIndex, 0), false, DataRoles::DataRole::PlayerSelection);
    return setData(index(row, 0), true, DataRoles::DataRole::PlayerSelection);
}

void GlobalStatsModel::resetModel()
{
    if (!m_sourceModel)
        return;

    beginResetModel();
    resetData();
    endResetModel();
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
    m_players.clear();

    for (PlayerRef p: m_base->listAllPlayers())
    {
        m_players[p] = QVector<PlayerGameStats>(m_sourceModel->rowCount() + 1,
                                                PlayerGameStats(m_base->getPlayer(p)->getInitialRating(), false));
    }

    QMap<PlayerRef, int> ratingChangeByPlayer;
    for (int i = 0; i < m_sourceModel->rowCount(); ++i)
    {
        QModelIndex sourceGameIndex = m_sourceModel->index(i, 0);
        m_dates[i] = sourceGameIndex.data(DataRoles::DataRole::GameDate).value<QDate>();
        int scoreDiff = sourceGameIndex.data(DataRoles::DataRole::ScoreDiff).toInt();
        QVector<PlayerRef> hometeam = sourceGameIndex.data(DataRoles::DataRole::Hometeam).value<QVector<PlayerRef>>();
        int hometeamTotal = 0;
        for (PlayerRef playerRef: hometeam)
        {
            hometeamTotal += m_players[playerRef][i].changedRating;
        }
        QVector<PlayerRef> awayteam = sourceGameIndex.data(DataRoles::DataRole::Awayteam).value<QVector<PlayerRef>>();
        int awayteamTotal = 0;
        for (PlayerRef playerRef: awayteam)
        {
            awayteamTotal += m_players[playerRef][i].changedRating;
        }
        int ratingChange = getHomeRatingChange(scoreDiff, hometeamTotal, awayteamTotal);

        for (const QString &playerRef: hometeam)
        {
            m_players[playerRef][i + 1].participation = true;
            ratingChangeByPlayer[playerRef] += ratingChange;
            m_players[playerRef][i + 1].changedRating += ratingChangeByPlayer[playerRef];
        }
        for (PlayerRef playerRef: awayteam)
        {
            m_players[playerRef][i + 1].participation = true;
            ratingChangeByPlayer[playerRef] -= ratingChange;
            m_players[playerRef][i + 1].changedRating += ratingChangeByPlayer[playerRef];
        }
    }
}

GlobalStatsModel::PlayerGameStats::PlayerGameStats(int rating, bool part)
    : changedRating(rating), participation(part)
{
}
