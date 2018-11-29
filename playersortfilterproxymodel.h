#ifndef PLAYERSORTFILTERMODEL_H
#define PLAYERSORTFILTERMODEL_H

#include <QSortFilterProxyModel>
#include "customextendablemodel.h"
#include "player.h"

class PlayerSortFilterProxyModel: public QSortFilterProxyModel, public CustomExtendableModel
{
    Q_OBJECT

public:
    PlayerSortFilterProxyModel(QObject *parent = nullptr);

    virtual void setSourceModel(QAbstractItemModel *sourceModel) Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    //virtual QHash<int, QByteArray> roleNames() const override;

    bool selectRow(int row) Q_DECL_OVERRIDE;
    bool sortBy(int statRole) Q_DECL_OVERRIDE;

protected:
    virtual bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const Q_DECL_OVERRIDE;

private:
    void sort();
};

#endif // PLAYERSORTFILTERMODEL_H
