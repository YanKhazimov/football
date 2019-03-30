#include "playersortfilterproxymodel.h"
#include "dataroles.h"

PlayerSortFilterProxyModel::PlayerSortFilterProxyModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
    setSortRole(DataRoles::DataRole::Rating);
}

void PlayerSortFilterProxyModel::setSourceModel(QAbstractItemModel *source)
{
    beginResetModel();

    if (sourceModel())
    {
        disconnect(sourceModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
                   this, SLOT(sourceDataChanged(QModelIndex,QModelIndex,QVector<int>)));
    }

    QSortFilterProxyModel::setSourceModel(source);

    if (sourceModel())
    {
        connect(sourceModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
                this, SLOT(sourceDataChanged(QModelIndex,QModelIndex,QVector<int>)));
    }

    sort();

    endResetModel();
}

bool PlayerSortFilterProxyModel::selectRow(int row)
{
    if (row == m_selectedIndex.row())
        return false;

    QModelIndex idx = index(row, 0);
    return setData(index(row, 0), true, DataRoles::DataRole::PlayerSelection);
}

bool PlayerSortFilterProxyModel::sortBy(int statRole)
{
    setSortRole(statRole);

    emit selectedRowChanged(m_selectedIndex.row());

    return true;
}

void PlayerSortFilterProxyModel::setFilter(bool enabled)
{
    beginResetModel();
    m_relevanceThreshold = enabled ? 35 : -1;
    invalidateFilter();
    endResetModel();

    for (int i = 0; i < sourceModel()->rowCount(); ++i)
    {
        if (sourceModel()->data(sourceModel()->index(i, 0), DataRoles::DataRole::PlayerSelection).toBool())
        {
            m_selectedIndex = mapFromSource(sourceModel()->index(i, 0));
            break;
        }
    }
    emit selectedRowChanged(m_selectedIndex.row());
}

Player* PlayerSortFilterProxyModel::getPlayer(int idx)
{
    Player* player = data(index(idx, 0), DataRoles::DataRole::Player).value<Player*>();
    return player;
}

int PlayerSortFilterProxyModel::getPlayerRating(QString name)
{
    for (int i = 0; i < sourceModel()->rowCount(); ++i)
        if (sourceModel()->index(i, 0).data(DataRoles::DataRole::PlayerName).toString() == name)
            return sourceModel()->index(i, 0).data(DataRoles::DataRole::Rating).toInt();

    return 0;
}

void PlayerSortFilterProxyModel::sourceDataChanged(QModelIndex topLeft, QModelIndex bottomRight, QVector<int> roles)
{
    if (roles.contains(DataRoles::DataRole::PlayerSelection))
    {
        if (topLeft.data(DataRoles::DataRole::PlayerSelection).toBool() == true)
        {
            m_selectedIndex = mapFromSource(topLeft);
        }
        else
        {
            m_selectedIndex = QModelIndex();
        }
        emit dataChanged(m_selectedIndex, m_selectedIndex, {DataRoles::DataRole::PlayerSelection});
        emit selectedRowChanged(m_selectedIndex.row());
    }
}

bool PlayerSortFilterProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    QVariant leftData = source_left.data(sortRole()),
            rightData = source_right.data(sortRole());
    if (leftData == rightData)
    {
        // descending order in the table view so reversing for alphabetical order
        return source_left.data(DataRoles::DataRole::PlayerName) >
                source_right.data(DataRoles::DataRole::PlayerName);
    }

    switch(sortRole())
    {
    case DataRoles::DataRole::WinsLosses:
        int leftWins = leftData.value<QVector<int>>()[0];
        int leftDraws = leftData.value<QVector<int>>()[1];
        int leftLosses = leftData.value<QVector<int>>()[2];
        int rightWins = rightData.value<QVector<int>>()[0];
        int rightDraws = rightData.value<QVector<int>>()[1];
        int rightLosses = rightData.value<QVector<int>>()[2];

        int gamesBehindx2 = (rightWins - leftWins) - (rightLosses - leftLosses);
        return gamesBehindx2 > 0 || (gamesBehindx2 == 0 && leftDraws > rightDraws);
    }

    return QSortFilterProxyModel::lessThan(source_left, source_right);
}

bool PlayerSortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);
    int relevance = sourceModel()->index(source_row, 0).data(DataRoles::DataRole::Relevance).toInt();
    return relevance >= m_relevanceThreshold;
}

void PlayerSortFilterProxyModel::sort()
{
    QSortFilterProxyModel::sort(0, Qt::DescendingOrder);
}
