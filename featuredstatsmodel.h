#ifndef FEATUREDSTATSMODEL_H
#define FEATUREDSTATSMODEL_H

#include <QAbstractListModel>
#include "featuredstats.h"

class FeaturedStatsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    FeaturedStatsModel();
    ~FeaturedStatsModel();
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    void setSourceModel(QAbstractItemModel* source);

    QHash<int, QByteArray> roleNames() const override;

private slots:
    void reset();

private:
    QList<FeaturedStat*> m_featuredStats;
    QAbstractItemModel* m_source;
};

#endif // FEATUREDSTATSMODEL_H
