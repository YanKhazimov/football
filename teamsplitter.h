#ifndef TEAMSPLITTER_H
#define TEAMSPLITTER_H

#include <QObject>

class TeamSplitter: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList homeRoster MEMBER m_homeRoster NOTIFY changed)
    Q_PROPERTY(QStringList awayRoster MEMBER m_awayRoster NOTIFY changed)

public:
    TeamSplitter();

    Q_INVOKABLE void split(QStringList home, QStringList away, QStringList unsorted);

signals:
    void changed();

private:
    QStringList m_homeRoster, m_awayRoster;
};

#endif // TEAMSPLITTER_H
