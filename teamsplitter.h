#ifndef TEAMSPLITTER_H
#define TEAMSPLITTER_H

#include <QObject>
#include <QMap>
#include <QHash>
#include "globalstatsmodel.h"

class TeamSplitter: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList splitVariants READ getSplitVariants WRITE setSplitVariants NOTIFY variantsChanged)

public:
    TeamSplitter(const GlobalStatsModel& gsmodel);

    Q_INVOKABLE void split(QStringList home, QStringList away, QStringList unsorted, int maxResults);
    QVariantList getSplitVariants() const;
    void setSplitVariants(QVariantList list);

signals:
    void variantsChanged();

private:
    QList<QStringList> m_splitVariants;
    QMap<QString, bool> m_hashes;

    QString hash(QStringList team);
    void pick(int picksToGo, QStringList home, const QStringList& away, QStringList unsorted, int pickFrom = 0);
    QHash<QString, int> getBaskets(const QHash<QString, int> &playerRating, QMap<int, QPair<QStringList, int> > &ratingMap);
    const GlobalStatsModel& m_model;
};

#endif // TEAMSPLITTER_H
