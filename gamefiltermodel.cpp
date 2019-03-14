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

bool GameFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex sourceGameIndex = source_parent.child(source_row, 0);
    QDate gameDate = sourceGameIndex.data(DataRoles::DataRole::GameDate).value<QDate>();
    return m_minDate <= gameDate && gameDate <= m_maxDate;
}
