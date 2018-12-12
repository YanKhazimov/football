#ifndef FEATUREDSTATSMODEL_H
#define FEATUREDSTATSMODEL_H

#include <QAbstractProxyModel>
#include "playerbase.h"

class FeaturedStat : public QObject
{
    Q_OBJECT

    QString m_name;
    QString m_description;
    QObjectList m_queryResultItems;

    using Criteria = int;

    Criteria m_criteria;

public:
    FeaturedStat(QString title, QString description, Criteria rule);
    FeaturedStat(const FeaturedStat& fs);
    FeaturedStat& operator= (const FeaturedStat& fs);
    QString getName() const;
    QString getDescription() const;
};

class FeaturedStatsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    FeaturedStatsModel ();
    int rowCount(const QModelIndex &parent) const override;
    //int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    void setSourceModel(QAbstractItemModel* source);

//    Q_INVOKABLE virtual QModelIndex parent(const QModelIndex &child) const override;
//    Q_INVOKABLE virtual QModelIndex index(int row, int column,
//                              const QModelIndex &parent = QModelIndex()) const override;
//    Q_INVOKABLE virtual QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
//    Q_INVOKABLE virtual QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;

    QHash<int, QByteArray> roleNames() const override;

private:
    QList<FeaturedStat> m_featuredStats;
    QAbstractItemModel* m_source;
};

#endif // FEATUREDSTATSMODEL_H
