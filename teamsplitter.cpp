#include "teamsplitter.h"
#include <QVariant>

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

void TeamSplitter::pick(int picksToGo, QStringList home, const QStringList& away, QStringList unsorted, int pickFrom)
{
    if (picksToGo > 0)
    {
        for (int i = pickFrom; i < unsorted.size(); ++i)
        {
            pick(picksToGo - 1, home + QStringList { unsorted.at(i) }, away,
                 unsorted.mid(0, i) + unsorted.mid(i + 1, unsorted.size() - (i + 1)),
                 i);
        }
    }
    else
    {
        m_splitVariants.append(QVariant::fromValue(QStringList(home + away + unsorted))); // TODO: exclude mirror variants
    }
}

void TeamSplitter::split(QStringList home, QStringList away, QStringList unsorted, int maxResults)
{
    m_splitVariants.clear();

    int picksToGo = (home.size() + away.size() + unsorted.size()) / 2 - home.size();

    pick(picksToGo, home, away, unsorted);

    auto rosterComparator = [&] (QVariant variant1, QVariant variant2) -> bool
    {
        QStringList list1 = variant1.value<QStringList>();
        QStringList list2 = variant2.value<QStringList>();
        Q_ASSERT(list1.size() == list1.size());

        int diff1 = 0, diff2 = 0;
        for (int i = 0; i < list1.size() / 2; ++i)
        {
            diff1 += getRating(list1.at(i));
            diff2 += getRating(list2.at(i));
        }

        for (int i = list1.size() / 2; i < list1.size(); ++i)
        {
            diff1 -= getRating(list1.at(i));
            diff2 -= getRating(list2.at(i));
        }

        return qAbs(diff1) < qAbs(diff2);
    };

    qSort(m_splitVariants.begin(), m_splitVariants.end(), rosterComparator);
    m_splitVariants.erase(qMin(m_splitVariants.begin() + maxResults, m_splitVariants.end()), m_splitVariants.end());
}
