#include <QDebug>
#include "playersortfilterproxymodel.h"
#include "dataroles.h"

PlayerSortFilterProxyModel::PlayerSortFilterProxyModel(QObject* parent)
    : QSortFilterProxyModel(parent)
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

bool PlayerSortFilterProxyModel::selectRow(int row)
{
    //setData(index(m_selectedPlayerIndex, 0), false, DataRoles::DataRole::PlayerSelection);

    QModelIndex idx = index(row, 0);
    Q_ASSERT(idx.model() == this);
    return setData(idx, true, DataRoles::DataRole::PlayerSelection);
}

void PlayerSortFilterProxyModel::sortBy(int statRole)
{
    setSortRole(statRole);
    sort();
}

bool PlayerSortFilterProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    if (source_left.data(sortRole()) == source_right.data(sortRole()))
    {
        return source_left.data(DataRoles::DataRole::PlayerName) <
                source_right.data(DataRoles::DataRole::PlayerName);
    }

    switch(sortRole())
    {
    case DataRoles::DataRole::Rating:
    case DataRoles::DataRole::WinsLosses:
    case DataRoles::DataRole::Progress:
    case DataRoles::DataRole::Dedication:
        ;
    }

    return QSortFilterProxyModel::lessThan(source_left, source_right);
}

void PlayerSortFilterProxyModel::sort()
{
    QSortFilterProxyModel::sort(0, Qt::DescendingOrder);
}
