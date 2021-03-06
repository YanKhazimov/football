#include "featuredstatsmodel.h"
#include "dataroles.h"

FeaturedStatsModel::FeaturedStatsModel(const Language& lang)
    : m_source(nullptr)
{
    m_featuredStats << new ClosestPlayersStat(m_source)
                    << new StreakStat(m_source)
                    << new SynergyStat(m_source);

    connect(&lang, &Language::languageChanged, this, &FeaturedStatsModel::onLanguageChanged);
}

FeaturedStatsModel::~FeaturedStatsModel()
{
    m_source = nullptr;
    reset();
}

int FeaturedStatsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return static_cast<int>(m_featuredStats.size());
}

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
    beginResetModel();
    for (FeaturedStat* fs: m_featuredStats)
    {
        fs->resetDataModel(m_source);
        fs->updateValue();
    }
    endResetModel();
}

void FeaturedStatsModel::onLanguageChanged(QString lang)
{
    for (FeaturedStat* stat: m_featuredStats)
        stat->setLanguage(lang);

    emit dataChanged(index(0, 0), index(rowCount() - 1, 0),
                     QVector<int>{
                         DataRoles::DataRole::FeaturedStatName,
                         DataRoles::DataRole::FeaturedStatDescription
                     });
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
        return QVariant::fromValue(m_featuredStats.at(index.row())->getValue());
    }

    return QVariant();
}

void FeaturedStatsModel::setSourceModel(QAbstractItemModel *source)
{
    if (m_source)
    {
        disconnect(m_source, SIGNAL(modelReset()),
                   this, SLOT(reset()));
    }

    m_source = source;

    if (m_source)
    {
        connect(m_source, SIGNAL(modelReset()),
                   this, SLOT(reset()));
    }

    reset();
}
