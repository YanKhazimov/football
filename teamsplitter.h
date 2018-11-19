#ifndef TEAMSPLITTER_H
#define TEAMSPLITTER_H

#include <QObject>

class TeamSplitter: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QVariant> splitVariants MEMBER m_splitVariants)

public:
    TeamSplitter();

    Q_INVOKABLE void split(QStringList home, QStringList away, QStringList unsorted, int maxResults);

private:
    QList<QVariant> m_splitVariants;
    void pick(int picksToGo, QStringList home, const QStringList& away, QStringList unsorted, int pickFrom = 0);
};

#endif // TEAMSPLITTER_H
