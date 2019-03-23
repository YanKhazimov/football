#include <QDebug>
#include "statpresenterproxymodel.h"
#include "playersortfilterproxymodel.h"
#include "dataroles.h"

StatPresenterProxyModel::StatPresenterProxyModel(QObject* parent)
    : QIdentityProxyModel(parent)
{
}

QVariant StatPresenterProxyModel::data(const QModelIndex &index, int role) const
{
    QVariant sourceData = sourceModel()->data(mapToSource(index), role);
    if (!sourceData.isValid())
        return QVariant();

    if (role == DataRoles::DataRole::WinsLosses)
    {
        QVector<int> wdl = sourceData.value<QVector<int>>();
        Q_ASSERT(wdl.size() == 3);
        return QVariant::fromValue(QString("%1-%2-%3")
                            .arg(QString::number(wdl[0]))
                            .arg(QString::number(wdl[1]))
                            .arg(QString::number(wdl[2])));
    }
    else if (role == DataRoles::DataRole::Progress) {
        int progress = sourceData.toInt();
        QString extraSign = progress > 0 ? "+" : "";
        return QVariant::fromValue(extraSign + QString::number(progress));
    }
    else if (role == DataRoles::DataRole::Dedication || role == DataRoles::DataRole::Relevance)
    {
        int rounded = static_cast<int>(sourceData.toFloat());
        return QVariant::fromValue(QString::number(rounded) + "%");
    }

    return sourceData;
}

void StatPresenterProxyModel::setSourceModel(QAbstractItemModel *source)
{
    beginResetModel();

    if (sourceModel())
    {
        disconnect(sourceModel(), SIGNAL(selectedRowChanged(int)),
                   this, SIGNAL(selectedRowChanged(int)));

        disconnect(sourceModel(), SIGNAL(modelReset()),
                   this, SIGNAL(modelReset()));

        disconnect(sourceModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
                   this, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)));
    }

    QIdentityProxyModel::setSourceModel(source);

    if (sourceModel())
    {
        connect(sourceModel(), SIGNAL(selectedRowChanged(int)),
                this, SIGNAL(selectedRowChanged(int)));

        connect(sourceModel(), SIGNAL(modelReset()),
                this, SIGNAL(modelReset()));

        connect(sourceModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
                   this, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)));
    }

    endResetModel();
}

bool StatPresenterProxyModel::sortBy(int statRole)
{
    PlayerSortFilterProxyModel* source = dynamic_cast<PlayerSortFilterProxyModel*>(sourceModel());
    return source && source->sortBy(statRole);
}

bool StatPresenterProxyModel::selectRow(int row)
{
    PlayerSortFilterProxyModel* source = dynamic_cast<PlayerSortFilterProxyModel*>(sourceModel());
    return source && source->selectRow(row);
}

//void StatPresenterProxyModel::setFilter(bool enabled)
//{
//    PlayerSortFilterProxyModel* source = dynamic_cast<PlayerSortFilterProxyModel*>(sourceModel());

//    if (source)
//        source->setFilter(enabled);
//}
