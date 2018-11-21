#include "globalstatsmodel.h"
#include <QDebug>

GlobalStatsModel::GlobalStatsModel()
    : m_sourceModel(nullptr)
{
//    QStandardItem* item;

//    item = new QStandardItem;
//    item->setData(QString("500"), Rating);
//    item->setData(QString("5-3 (62.5%)"), WinsLosses);
//    item->setData(QString("-20"), Progress);
//    item->setData(QString("50%"), Reliability);
//    this->appendRow(item);

//    item = new QStandardItem;
//    item->setData(QString("4500"), Rating);
//    item->setData(QString("1-3 (25%)"), WinsLosses);
//    item->setData(QString("-220"), Progress);
//    item->setData(QString("30%"), Reliability);
//    this->appendRow(item);
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
    float random = 99.9f;
//    for (auto p: model.getPlayers())
//    {
//        QStandardItem* item;

//        item = new QStandardItem;
//        item->setData(QString(p), PlayerName);
//        item->setData(QString::number(random), Rating);
//        item->setData(QString("5-3 (62.5%)"), WinsLosses);
//        item->setData(QString("-20"), Progress);
//        item->setData(QString("50%"), Reliability);
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

int GlobalStatsModel::rowCount(const QModelIndex &parent) const
{
    return m_players.size();
}

int GlobalStatsModel::columnCount(const QModelIndex &parent) const
{
    return 5;
}

QVariant GlobalStatsModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

QHash<int, QByteArray> GlobalStatsModel::roleNames() const
{
    QHash<int, QByteArray> result;

    result[Roles::PlayerName] = "PlayerName";
    result[Roles::Rating] = "Rating";
    result[Roles::WinsLosses] = "WinsLosses";
    result[Roles::Progress] = "Progress";
    result[Roles::Reliability] = "Reliability";

    return result;
}

Player *GlobalStatsModel::getPlayer(QString name)
{
    return m_base->getPlayer(name);
}

Player *GlobalStatsModel::getPlayer(int idx)
{
    QString name = data(index(idx, 0), Roles::PlayerName).toString();
    return m_base->getPlayer(name);
}

void GlobalStatsModel::resetModel()
{
    if (!m_sourceModel)
        return;

    beginResetModel();
    resetData();
    endResetModel();
}

int getRatingChange(uint scoreDiff, int totalRating1, int totalRating2)
{
    return scoreDiff / 3 * (totalRating1 - totalRating2);
}

void GlobalStatsModel::resetData()
{
    m_players.clear();
    QList<PlayerRef> allPlayers = m_base->listAllPlayers();
    for (PlayerRef p: allPlayers)
    {
        m_players.insert(p, {PlayerGameStats(m_base->getPlayer(p)->getInitialRating(), false)});
    }

    for (int i = 0; i < m_sourceModel->rowCount(); ++i)
    {
        QModelIndex sourceGameIndex = m_sourceModel->index(i, 0);
        int scoreDiff = sourceGameIndex.data(DataRoles::DataRole::ScoreDiff).toInt();
        QVector<PlayerRef> hometeam = sourceGameIndex.data(DataRoles::DataRole::Hometeam).value<QVector<PlayerRef>>();
        int hometeamTotal = 0;
        for (PlayerRef playerRef: hometeam)
        {
            hometeamTotal += m_players[playerRef].back().changedRating;
        }
        QVector<PlayerRef> awayteam = sourceGameIndex.data(DataRoles::DataRole::Awayteam).value<QVector<PlayerRef>>();
        int awayteamTotal = 0;
        for (PlayerRef playerRef: awayteam)
        {
            awayteamTotal += m_players[playerRef].back().changedRating;
        }

        int ratingChange = getRatingChange(scoreDiff, hometeamTotal, awayteamTotal);

        for (PlayerRef playerRef: hometeam)
        {
            m_players[playerRef].push_back(PlayerGameStats(ratingChange, true));
        }
        for (PlayerRef playerRef: awayteam)
        {
            m_players[playerRef].push_back(PlayerGameStats(-ratingChange, true));
        }
    }
}

GlobalStatsModel::PlayerGameStats::PlayerGameStats(int rating, bool part)
    : changedRating(rating), participation(part)
{
}
