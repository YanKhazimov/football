#include "featuredstats.h"
#include "dataroles.h"

void FeaturedStat::setLanguage(QString lang)
{
    if (lang == "en")
    {
        m_name += "f";
    }
    else {
        m_name += "ф";
    }
}

FeaturedStat::FeaturedStat(QString name, QString description, QAbstractItemModel *dataModel)
    : m_name(name), m_description(description), m_dataModel(dataModel)
{
}

FeaturedStat::FeaturedStat(const FeaturedStat &fs)
    : m_name(fs.m_name), m_description(fs.m_description)
{
}

FeaturedStat::~FeaturedStat()
{
    for (QObject* group: m_queryResultItems)
        delete group;
}

FeaturedStat &FeaturedStat::operator=(const FeaturedStat &fs)
{
    m_name = fs.m_name;
    m_description = fs.m_description;
    return *this;
}

QString FeaturedStat::getName() const
{
    return m_name;
}

QString FeaturedStat::getDescription() const
{
    return m_description;
}

QObjectList FeaturedStat::getValue() const
{
    return m_queryResultItems;
}

void FeaturedStat::updateValue()
{
    for (QObject* group: m_queryResultItems)
        delete group;

    m_queryResultItems.clear();

    if (m_dataModel == nullptr)
        return;

    calculate();
}

void FeaturedStat::resetDataModel(QAbstractItemModel *dataModel)
{
    m_dataModel = dataModel;
}

ClosestPlayersStat::ClosestPlayersStat(QAbstractItemModel *dataModel)
    : FeaturedStat("RIVALRIES TO WATCH", "Closest-rated players", dataModel)
{
}

void ClosestPlayersStat::calculate()
{
    int playersShown = 0;

    auto fits = [&playersShown](int additionalSize) {
        return (playersShown + additionalSize) <= 9;
    };

    std::map<int, QList<Player*>> playersByRating;
    for (int i = 0; i < m_dataModel->rowCount(); ++i)
    {
        QModelIndex playerIndex = m_dataModel->index(i, 0);
        Player* playerPtr = playerIndex.data(DataRoles::DataRole::Player).value<Player*>();
        playersByRating[playerIndex.data(DataRoles::DataRole::Rating).toInt()].append(playerPtr);
    }

    std::map<int, QList<Player*>>::reverse_iterator reverseIter; // higher-rated shown first
    for (reverseIter = playersByRating.rbegin(); reverseIter != playersByRating.rend(); ++reverseIter)
    {
        if (fits(reverseIter->second.size()) && reverseIter->second.size() > 1)
        {
            QObjectList group;
            for (Player* p: reverseIter->second)
                group << new PlayerStat(p, /*QString::number(reverseIter->first)*/"=");

            m_queryResultItems.append(new QueryResultItem("", group));
            playersShown += reverseIter->second.size();
        }
    }

    std::multimap<int, std::map<int, QList<Player*>>::reverse_iterator> groupsByDiff;

    reverseIter = playersByRating.rbegin();
    if (reverseIter != playersByRating.rend())
        ++reverseIter;

    for (; reverseIter != playersByRating.rend(); ++reverseIter)
    {
        std::map<int, QList<Player*>>::reverse_iterator prev = std::prev(reverseIter);
        groupsByDiff.insert(std::make_pair(prev->first - reverseIter->first, reverseIter));
    }

    for (std::pair<int, std::map<int, QList<Player*>>::reverse_iterator> diffIter: groupsByDiff)
    {
        const QList<Player*>& lowerGroup = diffIter.second->second;
        const QList<Player*>& higherGroup = std::prev(diffIter.second)->second;

        if (!fits(lowerGroup.size() + higherGroup.size()))
            return;

        QObjectList group;

        for (Player* higherRatedPlayer: higherGroup)
            group.append(new PlayerStat(higherRatedPlayer, "+" + QString::number(diffIter.first)));
        for (Player* lowerRatedPlayer: lowerGroup)
            group.append(new PlayerStat(lowerRatedPlayer, QString::number(-diffIter.first)));

        m_queryResultItems.append(new QueryResultItem("", group));
        playersShown += group.size();
    }
}

SynergyStat::SynergyStat(QAbstractItemModel *dataModel)
    : FeaturedStat("STRONGEST SYNERGY", "Highest W/L ratio together", dataModel)
{
}

void SynergyStat::calculate()
{
    int playerCount = m_dataModel->rowCount();
    typedef QVector<int> WDL;
    std::multimap<float, QPair<Player*, Player*>> groupsSynergy;

    for (int i = 0; i < playerCount - 1; ++i)
    {
        QModelIndex iIndex = m_dataModel->index(i, 0);
        QMap<PlayerRef, WDL> synergy = iIndex.data(DataRoles::DataRole::Synergy).value<QMap<PlayerRef, WDL>>();
        for (int j = i + 1; j < playerCount; ++j)
        {
            QModelIndex jIndex = m_dataModel->index(j, 0);
            PlayerRef jName = jIndex.data(DataRoles::DataRole::PlayerName).value<PlayerRef>();

            if (synergy.count(jName) > 0)
            {
                const WDL& ijWDL = synergy[jName];
                float synergy = (ijWDL[0] + static_cast<float>(ijWDL[1]) / 2) /
                        (ijWDL[0] + ijWDL[1] + ijWDL[2]);
                Player* iPlayer = iIndex.data(DataRoles::DataRole::Player).value<Player*>();
                Player* jPlayer = jIndex.data(DataRoles::DataRole::Player).value<Player*>();
                groupsSynergy.insert(std::make_pair(synergy, qMakePair(iPlayer, jPlayer)));
            }
        }
    }

    // TODO: regroup into larger groups

    int maxShown = 9;
    int shown = 0;
    std::multimap<float, QPair<Player*, Player*>>::reverse_iterator iter = groupsSynergy.rbegin();
    for (; iter != groupsSynergy.rend() && (shown += 2) <= maxShown; ++iter)
    {
        QObjectList group;

        group << new PlayerStat(iter->second.first, "")
              << new PlayerStat(iter->second.second, "");

        QString groupStat = QString::number(static_cast<int>(iter->first * 100)) + "%";
        m_queryResultItems.append(new QueryResultItem(groupStat, group));
    }
}
