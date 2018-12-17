#ifndef FEATUREDSTATSMODEL_H
#define FEATUREDSTATSMODEL_H

#include <QAbstractProxyModel>
#include "playerbase.h"

class FeaturedStat : public QObject // ?
{
    Q_OBJECT

    using Criteria = std::function<bool(Player*,Player*)>;
    Criteria m_criteria;

protected:
    QString m_name;
    QString m_description;
    QAbstractItemModel* m_dataModel;

    QObjectList m_queryResultItems;

public:
    FeaturedStat(QString name, QString description, QAbstractItemModel* dataModel);
    FeaturedStat(const FeaturedStat& fs);
    ~FeaturedStat();
    FeaturedStat& operator= (const FeaturedStat& fs);
    QString getName() const;
    QString getDescription() const;
    QObjectList getValue() const;
    virtual void recalculate() = 0;
    void resetDataModel(QAbstractItemModel* dataModel);
};

class ClosestPlayersStat: public FeaturedStat
{
public:
    ClosestPlayersStat(QAbstractItemModel *dataModel);
    void recalculate() override;
};

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
