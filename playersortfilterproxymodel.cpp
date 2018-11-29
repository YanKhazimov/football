#include <QDebug>
#include "playersortfilterproxymodel.h"
#include "dataroles.h"

PlayerSortFilterProxyModel::PlayerSortFilterProxyModel(QObject* parent)
    : QSortFilterProxyModel(parent), CustomExtendableModel(this)
{
    setSortRole(DataRoles::DataRole::Rating);
}

void PlayerSortFilterProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    beginResetModel();

//    if (m_sourceModel)
//    {
//        disconnect(m_sourceModel, SIGNAL(modelReset()), this, SLOT(resetModel()));
//    }

    QSortFilterProxyModel::setSourceModel(sourceModel);

    if (sourceModel)
    {
        //m_sourceModel = sourceModel;
    }

    sort();

    endResetModel();
}

QVariant PlayerSortFilterProxyModel::data(const QModelIndex &index, int role) const
{
    return sourceModel()->data(mapToSource(index), role);
}

bool PlayerSortFilterProxyModel::selectRow(int row)
{
    //setData(index(m_selectedPlayerIndex, 0), false, DataRoles::DataRole::PlayerSelection);

    return setData(index(row, 0), true, DataRoles::DataRole::PlayerSelection);
}

bool PlayerSortFilterProxyModel::sortBy(int statRole)
{
    setSortRole(statRole);
    sort();
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
