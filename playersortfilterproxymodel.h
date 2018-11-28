#ifndef PLAYERSORTFILTERMODEL_H
#define PLAYERSORTFILTERMODEL_H

#include <QSortFilterProxyModel>
#include "player.h"

class PlayerSortFilterProxyModel: public QSortFilterProxyModel
{
    Q_OBJECT

public:
    PlayerSortFilterProxyModel(QObject *parent = nullptr);

    virtual void setSourceModel(QAbstractItemModel *sourceModel) Q_DECL_OVERRIDE;

    //virtual QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE bool selectRow(int row);
    Q_INVOKABLE void sortBy(int statRole);

protected:
    virtual bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const Q_DECL_OVERRIDE;

private:
    void sort();
};

#endif // PLAYERSORTFILTERMODEL_H
