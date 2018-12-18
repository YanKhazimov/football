#ifndef FEATUREDSTATS_H
#define FEATUREDSTATS_H

#include <QAbstractItemModel>
#include "playerbase.h"

class FeaturedStat
{
protected:
    QString m_name;
    QString m_description;
    QObjectList m_queryResultItems;
    QAbstractItemModel* m_dataModel;

private:
    virtual void calculate() = 0;

public:
    FeaturedStat(QString name, QString description, QAbstractItemModel* dataModel);
    FeaturedStat(const FeaturedStat& fs);
    virtual ~FeaturedStat();
    FeaturedStat& operator= (const FeaturedStat& fs);
    QString getName() const;
    QString getDescription() const;
    QObjectList getValue() const;
    void updateValue();
    void resetDataModel(QAbstractItemModel* dataModel);
};

class ClosestPlayersStat: public FeaturedStat
{
public:
    ClosestPlayersStat(QAbstractItemModel *dataModel);
    void calculate() override;
};

class SynergyStat: public FeaturedStat
{
public:
    SynergyStat(QAbstractItemModel *dataModel);
    void calculate() override;
};

#endif // FEATUREDSTATS_H
