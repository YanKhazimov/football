#include "teamsplitter.h"

TeamSplitter::TeamSplitter()
{

}

int getRating(QString name)
{
    int rating = 0;
    for(const QChar c: name)
        rating += c.toLatin1();
    return rating;
}

void TeamSplitter::split(QStringList home, QStringList away, QStringList unsorted)
{
    int homeStart = 0, awayStart = 0;

    for (QString homePlayer: home)
        homeStart += getRating(homePlayer);

    for (QString awayPlayer: away)
        awayStart += getRating(awayPlayer);

    m_homeRoster = home + unsorted;
    m_awayRoster = away;
}
