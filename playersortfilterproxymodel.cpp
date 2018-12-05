#include <QDebug>
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

    return setData(index(row, 0), true, DataRoles::DataRole::PlayerSelection);
}

bool PlayerSortFilterProxyModel::sortBy(int statRole)
{
    setSortRole(statRole);

    emit selectedRowChanged(m_selectedIndex.row());

    return true;
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
    }
}

bool PlayerSortFilterProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    if (source_left.data(sortRole()) == source_right.data(sortRole()))
    {
        // descending order in the table view so reversing for alphabetical order
        return source_left.data(DataRoles::DataRole::PlayerName) >
                source_right.data(DataRoles::DataRole::PlayerName);
    }

    switch(sortRole())
    {
    case DataRoles::DataRole::WinsLosses:
//    case DataRoles::DataRole::Dedication:

//    case DataRoles::DataRole::Rating:
//    case DataRoles::DataRole::Progress:
        ;
    }

    return QSortFilterProxyModel::lessThan(source_left, source_right);
}

void PlayerSortFilterProxyModel::sort()
{
    QSortFilterProxyModel::sort(0, Qt::DescendingOrder);
}
