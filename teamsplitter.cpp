#include "teamsplitter.h"

TeamSplitter::TeamSplitter()
{

}

void TeamSplitter::split(QStringList home, QStringList away, QStringList unsorted)
{
    for (auto h: home)
        m_homeRoster.append(h);
    for (auto a: away)
        m_awayRoster.append(a);
    for (auto u: unsorted)
        m_homeRoster.append(u);
}
