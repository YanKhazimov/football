#include "teamsplitter.h"
#include <QVariant>

TeamSplitter::TeamSplitter(const GlobalStatsModel& gsmodel)
    : m_model(gsmodel)
{
}

QString TeamSplitter::hash(QStringList team)
{
    team.sort();
    return team.join('|');
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
        if (!m_hashes.contains(hash(home)))
        {
            m_splitVariants.append(home + away + unsorted);
            m_hashes[hash(home)];
            m_hashes[hash(away + unsorted)];
        }
    }
}

QVariantList TeamSplitter::getSplitVariants() const
{
    QVariantList result;

    for (QStringList variant: m_splitVariants)
        result.append(QVariant::fromValue(variant));

    return result;
}

void TeamSplitter::setSplitVariants(QVariantList list)
{
    m_splitVariants.clear();

    for (const QVariant& variant: list)
        m_splitVariants.append(variant.value<QStringList>());

    emit variantsChanged();
}

QHash<QString, int> TeamSplitter::getBaskets(const QHash<QString, int>& playerRating, QMap<int, QPair<QStringList, int>>& ratingMap)
{
    QMultiMap<int, int> basketBottomsByGap;
    QMap<int, QPair<QStringList, int>>::iterator ratingIter = ratingMap.begin();
    ratingIter.value().second = 0;
    int prevRating = ratingIter.key();
    for (++ratingIter; ratingIter != ratingMap.end(); ++ratingIter)
    {
        ratingIter.value().second = ratingIter.key() - prevRating;
        prevRating = ratingIter.key();

        basketBottomsByGap.insert(-ratingIter.value().second, ratingIter.key()); // - because no reverse iterator
    }

    int desiredBaskets = 4;
    int basketSeparator = desiredBaskets - 1;

    QMultiMap<int, int>::iterator byGapIter = basketBottomsByGap.begin();

    // cut the rest
    byGapIter += qMin(basketSeparator - 1, ratingMap.size());
    if (byGapIter != basketBottomsByGap.end())
    {
        byGapIter = basketBottomsByGap.lowerBound(byGapIter.key() + 1);

        while (byGapIter != basketBottomsByGap.end())
           byGapIter = basketBottomsByGap.erase(byGapIter);
    }

    QHash<QString, int> playerBaskets;
    std::vector<int> basketBottoms = basketBottomsByGap.values().toVector().toStdVector();
    std::sort(basketBottoms.begin(), basketBottoms.end());

    for (QHash<QString, int>::const_iterator playerIter = playerRating.begin(); playerIter != playerRating.end(); ++playerIter)
    {
        std::vector<int>::iterator nextBasketIter = std::upper_bound(basketBottoms.begin(), basketBottoms.end(), playerIter.value());
        int basket = static_cast<int>(std::distance(basketBottoms.begin(), nextBasketIter));
        playerBaskets[playerIter.key()] = basket;
    }

    return playerBaskets;
}

void TeamSplitter::split(QStringList home, QStringList away, QStringList unsorted, int maxResults)
{
    m_splitVariants.clear();
    m_hashes.clear();

    if (home.size() + away.size() + unsorted.size() > 0)
    {
        int picksToGo = (home.size() + away.size() + unsorted.size()) / 2 - home.size();
        pick(picksToGo, home, away, unsorted);

        QHash<QString, int> playerRating;
        QMap<int, QPair<QStringList, int>> ratingMap;
        for (const QString& player: home + away + unsorted)
        {
            playerRating[player] = m_model.getRating(player);
            ratingMap[playerRating[player]].first.append(player);
        }

        QHash<QString, int> playerBaskets = getBaskets(playerRating, ratingMap);

        auto rosterComparator = [&playerRating, &playerBaskets] (const QStringList& list1, const QStringList& list2) -> bool
        {
            Q_ASSERT(list1.size() == list2.size());

            int ratingDiff1 = 0, ratingDiff2 = 0;
            QVector<int> basketDistribution1(list1.size(), 0), basketDistribution2(list1.size(), 0);

            for (int i = 0; i < list1.size() / 2; ++i)
            {
                ratingDiff1 += playerRating.value(list1.at(i));
                ratingDiff2 += playerRating.value(list2.at(i));
                ++basketDistribution1[playerBaskets.value(list1.at(i))];
                ++basketDistribution2[playerBaskets.value(list2.at(i))];
            }

            for (int i = list1.size() / 2; i < list1.size(); ++i)
            {
                ratingDiff1 -= playerRating.value(list1.at(i));
                ratingDiff2 -= playerRating.value(list2.at(i));
                --basketDistribution1[playerBaskets.value(list1.at(i))];
                --basketDistribution2[playerBaskets.value(list2.at(i))];
            }

            int basketDiff1 = 0, basketDiff2 = 0;
            for (int i = 0; i < list1.size(); ++i)
            {
                basketDiff1 += qAbs(basketDistribution1[i]);
                basketDiff2 += qAbs(basketDistribution2[i]);
            }

            return basketDiff1 < basketDiff2 || (basketDiff1 == basketDiff2 && qAbs(ratingDiff1) < qAbs(ratingDiff2));
        };

        qSort(m_splitVariants.begin(), m_splitVariants.end(), rosterComparator);
        m_splitVariants.erase(qMin(m_splitVariants.begin() + maxResults, m_splitVariants.end()), m_splitVariants.end());
    }
    emit variantsChanged();
}
