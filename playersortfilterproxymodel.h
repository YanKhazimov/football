#ifndef PLAYERSORTFILTERMODEL_H
#define PLAYERSORTFILTERMODEL_H

#include <QSortFilterProxyModel>
#include "dataroles.h"
#include "player.h"

class PlayerSortFilterProxyModel: public QSortFilterProxyModel
{
    Q_OBJECT

public:
    PlayerSortFilterProxyModel(const DataRoles::DataRole sortingRole, QObject *parent = nullptr);

    virtual void setSourceModel(QAbstractItemModel *source) Q_DECL_OVERRIDE;

    bool selectRow(int row);
    bool sortBy(int statRole);
    Q_INVOKABLE void setFilter(bool enabled);
    Q_INVOKABLE Player* getPlayer(int idx);
    Q_INVOKABLE int getPlayerRating(QString name);

    int getSelectedRow() const;

signals:
    void selectedRowChanged(int selectedRow);

public slots:
    void sourceDataChanged(QModelIndex topLeft, QModelIndex bottomRight, QVector<int> roles);

protected:
    virtual bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const Q_DECL_OVERRIDE;
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;

private:
    void sort();

    QPersistentModelIndex m_selectedIndex;
    int m_relevanceThreshold = -1;
};

#endif // PLAYERSORTFILTERMODEL_H
