#include "featuredstatsmodel.h"
#include "dataroles.h"

FeaturedStatsModel::FeaturedStatsModel()
{
    m_featuredStats.append(FeaturedStat("RIVALRIES TO WATCH", "Closest-rated players", 13));
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

QVariant FeaturedStatsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_featuredStats.size())
        return QVariant();

    const FeaturedStat& stat = m_featuredStats.at(index.row());

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
        return QVariant();
    }

    return QVariant();//sourceModel()->data(mapToSource(index), role);
}

void FeaturedStatsModel::setSourceModel(QAbstractItemModel *source)
{

}

//QModelIndex FeaturedStatsModel::parent(const QModelIndex &child) const
//{
//    Q_UNUSED(child);
//    return QModelIndex();
//}

//QModelIndex FeaturedStatsModel::index(int row, int column, const QModelIndex &parent) const
//{
//    return QModelIndex();
//}

//QModelIndex FeaturedStatsModel::mapToSource(const QModelIndex &proxyIndex) const
//{
//    Q_UNUSED(proxyIndex);
//    return QModelIndex();
//}

//QModelIndex FeaturedStatsModel::mapFromSource(const QModelIndex &sourceIndex) const
//{
//    Q_UNUSED(sourceIndex);
//    return QModelIndex();
//}

FeaturedStat::FeaturedStat(QString title, QString description, FeaturedStat::Criteria rule)
    : m_name(title), m_description(description), m_criteria(rule)
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
