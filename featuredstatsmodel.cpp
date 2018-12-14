#include "featuredstatsmodel.h"
#include "dataroles.h"

FeaturedStatsModel::FeaturedStatsModel()
    : m_source(nullptr)
{
    m_featuredStats.append(new ClosestPlayersStat(m_source));
    m_stats.append(QObjectList());
    /*m_featuredStats.append(FeaturedStat("RIVALRIES TO WATCH", "Closest-rated players",
                                        [](Player* p1, Player* p2){return false;}));*/
}

int FeaturedStatsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return static_cast<int>(m_featuredStats.size());
}

//int FeaturedStatsModel::columnCount(const QModelIndex &parent) const
//{
//    Q_UNUSED(parent);
//    return 1;
//}

QHash<int, QByteArray> FeaturedStatsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DataRoles::DataRole::FeaturedStatName] = "name";
    roles[DataRoles::DataRole::FeaturedStatDescription] = "description";
    roles[DataRoles::DataRole::FeaturedStatQueryResult] = "queryResult";
    return roles;
}

void FeaturedStatsModel::reset()
{
    for (FeaturedStat* fs: m_featuredStats)
    {
        fs->resetDataModel(m_source);
        setStat(m_featuredStats.indexOf(fs), fs->calculate());
    }
}

void FeaturedStatsModel::setStat(int i, const QObjectList &newValue)
{
    // TODO: delete old

    m_stats[i] = newValue;
}

QVariant FeaturedStatsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_featuredStats.size())
        return QVariant();

    const FeaturedStat& stat = *m_featuredStats.at(index.row());

    if (role == DataRoles::DataRole::FeaturedStatName)
    {
        return QVariant::fromValue(stat.getName());
    }
    else if (role == DataRoles::DataRole::FeaturedStatDescription)
    {
        return QVariant::fromValue(stat.getDescription());
    }
    else if (role == DataRoles::DataRole::FeaturedStatQueryResult)
    {
        return QVariant::fromValue(m_stats.at(index.row()));
    }

    return QVariant();
}

void FeaturedStatsModel::setSourceModel(QAbstractItemModel *source)
{
    if (m_source)
    {
        disconnect(m_source, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&, QVector<int>)),
                   this, SLOT(onSourceDataChanged(const QModelIndex&, const QModelIndex&, QVector<int>)));
        disconnect(m_source, SIGNAL(rowsInserted(const QModelIndex&, int, int)),
                   this, SLOT(onSourceRowsInserted(const QModelIndex&, int, int)));
        disconnect(m_source, SIGNAL(modelReset()),
                   this, SLOT(reset()));
    }

    m_source = source;

    if (m_source)
    {
        connect(m_source, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&, QVector<int>)),
                   this, SLOT(onSourceDataChanged(const QModelIndex&, const QModelIndex&, QVector<int>)));
        connect(m_source, SIGNAL(rowsInserted(const QModelIndex&, int, int)),
                   this, SLOT(onSourceRowsInserted(const QModelIndex&, int, int)));
        connect(m_source, SIGNAL(modelReset()),
                   this, SLOT(reset()));
    }

    reset();
}

FeaturedStat::FeaturedStat(QString name, QString description, QAbstractItemModel *dataModel)
    : m_name(name), m_description(description), m_dataModel(dataModel)
{
}

FeaturedStat::FeaturedStat(const FeaturedStat &fs)
    : m_name(fs.m_name), m_description(fs.m_description), m_criteria(fs.m_criteria)
{
}

FeaturedStat &FeaturedStat::operator=(const FeaturedStat &fs)
{
    m_name = fs.m_name;
    m_description = fs.m_description;
    m_criteria = fs.m_criteria;
    return *this;
}

QString FeaturedStat::getName() const
{
    return m_name;
}

QString FeaturedStat::getDescription() const
{
    return m_description;
}

void FeaturedStat::resetDataModel(QAbstractItemModel *dataModel)
{
    m_dataModel = dataModel;
}

ClosestPlayersStat::ClosestPlayersStat(QAbstractItemModel *dataModel)
    : FeaturedStat("r2w", "crp", dataModel)
{
}

QObjectList ClosestPlayersStat::calculate()
{
    QObjectList result;

    std::map<int, QList<Player*>> playersByRating;
    for (int i = 0; i < m_dataModel->rowCount(); ++i)
    {
        QModelIndex playerIndex = m_dataModel->index(i, 0);
        Player* playerPtr = playerIndex.data(DataRoles::DataRole::Player).value<Player*>();
        playersByRating[playerIndex.data(DataRoles::DataRole::Progress).toInt()].append(playerPtr);
    }

    std::map<int, QList<Player*>>::reverse_iterator reverseIter; // higher-rated shown first
    for (reverseIter = playersByRating.rbegin(); reverseIter != playersByRating.rend(); ++reverseIter)
    {
        if (reverseIter->second.size() > 1)
        {
            QObjectList group;
            for (Player* p: reverseIter->second)
                group << new PlayerStat(p, QString::number(reverseIter->first)/*"="*/); // where is it deleted?

            result.append(new QueryResultItem("", group)); // where is it deleted?
        }
    }

    std::multimap<int, std::map<int, QList<Player*>>::reverse_iterator> groupsByDiff;

    reverseIter = playersByRating.rbegin();
    for (++reverseIter; reverseIter != playersByRating.rend(); ++reverseIter)
    {
        std::map<int, QList<Player*>>::reverse_iterator prev = std::prev(reverseIter);
        groupsByDiff.insert(std::make_pair(prev->first - reverseIter->first, reverseIter));
    }

    for (std::pair<int, std::map<int, QList<Player*>>::reverse_iterator> diffIter: groupsByDiff)
    {
        QObjectList group;

        int diff = diffIter.first;
        QString extraSign = diffIter.first > 0 ? "+" : "";
        const QList<Player*>& lowerGroup = diffIter.second->second;
        const QList<Player*>& higherGroup = std::prev(diffIter.second)->second;

        for (Player* higherRatedPlayer: higherGroup)
            group.append(new PlayerStat(higherRatedPlayer, extraSign + QString::number(diff)));
        for (Player* lowerRatedPlayer: lowerGroup)
            group.append(new PlayerStat(lowerRatedPlayer, extraSign + QString::number(-diff)));

        result.append(new QueryResultItem("", group));
    }

    return result;
}
