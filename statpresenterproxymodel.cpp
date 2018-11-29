#include <QDebug>
#include "statpresenterproxymodel.h"
#include "playersortfilterproxymodel.h"
#include "dataroles.h"

StatPresenterProxyModel::StatPresenterProxyModel(QObject* parent)
    : QIdentityProxyModel(parent), CustomExtendableModel(this)
{
}

QVariant StatPresenterProxyModel::data(const QModelIndex &index, int role) const
{
    QVariant sourceData = sourceModel()->data(mapToSource(index), role);

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
    else if (role == DataRoles::DataRole::Dedication)
    {
        int rounded = static_cast<int>(sourceData.toFloat());
        return QVariant::fromValue(QString::number(rounded) + "%");
    }

    return sourceData;
}

bool StatPresenterProxyModel::sortBy(int statRole)
{
    return CustomExtendableModel::sortBy(statRole);
}

bool StatPresenterProxyModel::selectRow(int row)
{
    return CustomExtendableModel::selectRow(row);
}
