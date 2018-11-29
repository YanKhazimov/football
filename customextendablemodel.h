#ifndef CUSTOMEXTENDABLEMODEL_H
#define CUSTOMEXTENDABLEMODEL_H

#include <QAbstractProxyModel>

class CustomExtendableModel
{
public:
    CustomExtendableModel(QAbstractProxyModel *proxy);
    CustomExtendableModel* getSourceModel();

    Q_INVOKABLE virtual bool sortBy(int role);
    Q_INVOKABLE virtual bool selectRow(int row);

    virtual bool serializeSubnode(const QModelIndex &node, const QVector<QPair<int, int>>& p);

private:
    QAbstractProxyModel *m_proxy;
    QModelIndex mapIndexToSource(const QModelIndex& index) const;
    QModelIndex mapIndexFromSource(const QModelIndex& index) const;
};

#endif // CUSTOMEXTENDABLEMODEL_H
