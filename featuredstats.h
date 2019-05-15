#ifndef FEATUREDSTATS_H
#define FEATUREDSTATS_H

#include <QAbstractItemModel>
#include "playerbase.h"
#include "language.h"

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
    ~FeaturedStat();
    FeaturedStat& operator= (const FeaturedStat& fs);
    QString getName() const;
    QString getDescription() const;
    QObjectList getValue() const;
    void updateValue();
    void resetDataModel(QAbstractItemModel* dataModel);
    virtual void setLanguage(const QString& lang) = 0;
};

class ClosestPlayersStat: public FeaturedStat
{
public:
    ClosestPlayersStat(QAbstractItemModel *dataModel);
    void calculate() override;
    virtual void setLanguage(const QString& lang) Q_DECL_OVERRIDE;
};

class SynergyStat: public FeaturedStat
{
public:
    SynergyStat(QAbstractItemModel *dataModel);
    void calculate() override;
    virtual void setLanguage(const QString& lang) Q_DECL_OVERRIDE;
};

class StreakStat: public FeaturedStat
{
public:
    StreakStat(QAbstractItemModel *dataModel);
    void calculate() override;
    virtual void setLanguage(const QString& lang) Q_DECL_OVERRIDE;
};

#endif // FEATUREDSTATS_H
