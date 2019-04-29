#ifndef TEAMSPLITTER_H
#define TEAMSPLITTER_H

#include <QObject>
#include <QMap>
#include "globalstatsmodel.h"

class TeamSplitter: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QVariant> splitVariants MEMBER m_splitVariants NOTIFY variantsChanged)

public:
    TeamSplitter(const GlobalStatsModel& gsmodel);

    Q_INVOKABLE void split(QStringList home, QStringList away, QStringList unsorted, int maxResults);

signals:
    void variantsChanged();

private:
    QList<QVariant> m_splitVariants;
    QMap<QString, bool> m_hashes;

    QString hash(QStringList team);
    void pick(int picksToGo, QStringList home, const QStringList& away, QStringList unsorted, int pickFrom = 0);
    const GlobalStatsModel& m_model;
};

#endif // TEAMSPLITTER_H
