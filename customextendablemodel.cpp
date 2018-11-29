#include "customextendablemodel.h"

CustomExtendableModel::CustomExtendableModel(QAbstractProxyModel *proxy)
    : m_proxy(proxy)
{
}

CustomExtendableModel* CustomExtendableModel::getSourceModel()
{
    if (m_proxy == nullptr)
    {
        return nullptr;
    }
    return dynamic_cast<CustomExtendableModel*>(m_proxy->sourceModel());
}

QModelIndex CustomExtendableModel::mapIndexToSource(const QModelIndex &index) const
{
    if (m_proxy != nullptr)
        return m_proxy->mapToSource(index);

    return index;
}

QModelIndex CustomExtendableModel::mapIndexFromSource(const QModelIndex &index) const
{
    if (m_proxy != nullptr)
        return m_proxy->mapFromSource(index);

    return index;
}

bool CustomExtendableModel::sortBy(int role)
{
    if (getSourceModel() == nullptr)
        return false;

    return getSourceModel()->sortBy(role);
}

bool CustomExtendableModel::selectRow(int row)
{
    if (getSourceModel() == nullptr)
        return false;

    return getSourceModel()->selectRow(row);
}

bool CustomExtendableModel::serializeSubnode(const QModelIndex &node, const QVector<QPair<int, int> > &p)
{
    if (getSourceModel() == nullptr)
        return false;

    return getSourceModel()->serializeSubnode(mapIndexToSource(node), p);
}
