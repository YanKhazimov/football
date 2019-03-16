#include "gamefiltermodel.h"
#include "dataroles.h"

GameFilterModel::GameFilterModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
}

void GameFilterModel::setPeriod(int yearSince, int yearTo)
{
    m_minDate.setDate(yearSince, 1, 1);
    m_maxDate.setDate(yearTo, 12, 31);
    invalidateFilter();
}

void GameFilterModel::setSourceModel(QAbstractItemModel *source)
{
    beginResetModel();

    if (sourceModel())
    {
        disconnect(sourceModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
                   this, SLOT(sourceDataChanged(QModelIndex,QModelIndex,QVector<int>)));
    }

    QAbstractProxyModel::setSourceModel(source);

    if (sourceModel())
    {
        connect(sourceModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
                      this, SLOT(sourceDataChanged(QModelIndex,QModelIndex,QVector<int>)));
    }

    int n = sourceModel()->rowCount();
    if (n > 0)
    {
        QDate firstGameDate = sourceModel()->index(0, 0).data(DataRoles::DataRole::GameDate).value<QDate>();
        QDate lastGameDate = sourceModel()->index(n - 1, 0).data(DataRoles::DataRole::GameDate).value<QDate>();
        m_minDate = firstGameDate;
        m_maxDate.setDate(lastGameDate.year(), 12, 31);
        invalidateFilter();
    }

    endResetModel();
}

bool GameFilterModel::setSeasonFilter(QString filter)
{
    if (filter == "All")
    {
        beginResetModel();
        m_minDate.setDate(2000, 0, 0);// = sourceModel()->index(0, 0).data(DataRoles::DataRole::GameDate).value<QDate>();
        m_maxDate = QDate::currentDate();// sourceModel()->index(sourceModel()->rowCount() - 1, 0).data(DataRoles::DataRole::GameDate).value<QDate>();
        invalidateFilter();
        endResetModel();
        return true;
    }

    int year = filter.toInt();
    if (year == 0)
        return false;

    beginResetModel();
    m_minDate.setDate(year, 1, 1);
    m_maxDate.setDate(year, 12, 31);
    invalidateFilter();
    endResetModel();
    return true;
}

void GameFilterModel::sourceDataChanged(QModelIndex topLeft, QModelIndex bottomRight, QVector<int> roles)
{
    int iii = 0;
}

bool GameFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex sourceGameIndex = sourceModel()->index(source_row, 0, source_parent);
    QDate gameDate = sourceGameIndex.data(DataRoles::DataRole::GameDate).value<QDate>();
    return m_minDate <= gameDate && gameDate <= m_maxDate;
}
