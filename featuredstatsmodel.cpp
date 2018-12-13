#include "featuredstatsmodel.h"
#include "dataroles.h"

FeaturedStatsModel::FeaturedStatsModel()
    : m_source(nullptr)
{
    m_featuredStats.append(new ClosestPlayersStat(m_source));
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
        fs->calculate();
    }
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
        return QVariant();
    }

    return QVariant();//sourceModel()->data(mapToSource(index), role);
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

#include <QDebug>
QObjectList ClosestPlayersStat::calculate()
{
    int playerCount = m_dataModel->rowCount();

    QVector<QVector<int>> diffs(playerCount, QVector<int>(playerCount, 0));
    std::map<int, QVector<int>> playersByDiff;

    QObjectList result;
    for (int i = 0; i < playerCount; ++i)
    {
        QModelIndex firstPlayerIndex = m_dataModel->index(i, 0);
        qDebug() << m_dataModel->data(firstPlayerIndex, DataRoles::DataRole::PlayerName).toString();

        int baseRating = m_dataModel->data(m_dataModel->index(0, 0), DataRoles::DataRole::Rating).toInt();
        int iRating = m_dataModel->data(m_dataModel->index(i, 0), DataRoles::DataRole::Rating).toInt();
        playersByDiff[iRating - baseRating].push_back(i);

        std::map<int, QVector<int>>::iterator base = playersByDiff.find(0);
        std::map<int, QVector<int>>::iterator other = base;

        for (++other; other != playersByDiff.end(); ++other)
        {

        }

        std::map<int, QVector<int>>::reverse_iterator reverseOther = std::map<int, QVector<int>>::reverse_iterator(base);
        for (++reverseOther; reverseOther != playersByDiff.rend(); ++reverseOther)
        {

        }

        for (int j = i + 1; j < playerCount; ++j)
        {
            int iRating = m_dataModel->data(m_dataModel->index(i, 0), DataRoles::DataRole::Rating).toInt();
            int jRating = m_dataModel->data(m_dataModel->index(j, 0), DataRoles::DataRole::Rating).toInt();

            diffs[i][j] = iRating - jRating;
            diffs[j][i] = jRating - iRating;

            playersByDiff[qAbs(iRating - jRating)].push_back(j);
        }
    }

    return result;
}
