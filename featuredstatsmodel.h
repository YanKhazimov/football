#ifndef FEATUREDSTATSMODEL_H
#define FEATUREDSTATSMODEL_H

#include <QAbstractProxyModel>
#include "playerbase.h"

class FeaturedStat : public QObject
{
    Q_OBJECT

    QObjectList m_queryResultItems;

    using Criteria = std::function<bool(Player*,Player*)>;

    Criteria m_criteria;

protected:
    QString m_name;
    QString m_description;
    QAbstractItemModel* m_dataModel;

public:
    FeaturedStat(QString name, QString description, QAbstractItemModel* dataModel);
    FeaturedStat(const FeaturedStat& fs);
    FeaturedStat& operator= (const FeaturedStat& fs);
    QString getName() const;
    QString getDescription() const;
    virtual QObjectList calculate() = 0;
    void resetDataModel(QAbstractItemModel* dataModel);
};

class ClosestPlayersStat: public FeaturedStat
{
public:
    ClosestPlayersStat(QAbstractItemModel *dataModel);
    QObjectList calculate() override;
};

class FeaturedStatsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    FeaturedStatsModel();
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

private slots:
    void reset();

private:
    QList<FeaturedStat*> m_featuredStats;
    QAbstractItemModel* m_source;
};

#endif // FEATUREDSTATSMODEL_H
