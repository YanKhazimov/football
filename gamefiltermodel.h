#ifndef GAMEFILTERMODEL_H
#define GAMEFILTERMODEL_H

#include <QSortFilterProxyModel>
#include <QDate>

class GameFilterModel: public QSortFilterProxyModel
{
    Q_OBJECT

public:
    GameFilterModel(QObject *parent = nullptr);

    void setPeriod(int yearSince, int yearTo);
    //virtual void setSourceModel(QAbstractItemModel *source) Q_DECL_OVERRIDE;

public slots:
    void sourceDataChanged(QModelIndex topLeft, QModelIndex bottomRight, QVector<int> roles);

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;

private:
    QDate m_minDate, m_maxDate;
};

#endif // GAMEFILTERMODEL_H
