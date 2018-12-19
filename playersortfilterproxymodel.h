#ifndef PLAYERSORTFILTERMODEL_H
#define PLAYERSORTFILTERMODEL_H

#include <QSortFilterProxyModel>
#include "player.h"

class PlayerSortFilterProxyModel: public QSortFilterProxyModel
{
    Q_OBJECT

public:
    PlayerSortFilterProxyModel(QObject *parent = nullptr);

    virtual void setSourceModel(QAbstractItemModel *source) Q_DECL_OVERRIDE;

    bool selectRow(int row);
    bool sortBy(int statRole);

signals:
    void selectedRowChanged(int selectedRow);

public slots:
    void sourceDataChanged(QModelIndex topLeft, QModelIndex bottomRight, QVector<int> roles);

protected:
    virtual bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const Q_DECL_OVERRIDE;

private:
    void sort();

    QPersistentModelIndex m_selectedIndex;
};

#endif // PLAYERSORTFILTERMODEL_H
