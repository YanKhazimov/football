#include "playerbase.h"
#include <memory>

PlayerbaseQuery::PlayerbaseQuery(Playerbase& base, Query rule, QString title, QString description)
    : m_name(title), m_description(description)
{
    QList<PlayerRef> players = base.listAllPlayers();
    int i = 0;
    for (auto p = players.constBegin(); p != players.constEnd() && i < rule; ++i, ++p)
    {
        m_queryResultItems.push_back(new QueryResultItem("g" + QString::number(i),
        {new PlayerStat(base.getPlayer(*p), QString::number(10 + i)),
         new PlayerStat(base.getPlayer(*p), QString::number(20 + i))}));
    }
}

PlayerbaseQuery::~PlayerbaseQuery()
{
    for (auto q: m_queryResultItems)
    {
        for (auto ps: qobject_cast<QueryResultItem*>(q)->getGroup())
        {
            delete ps;
        }
        delete q;
    }
}

//QObjectList PlayerbaseQuery::getQueryResult()
//{
//    return m_queryResultItems;
//}

QueryResultItem::QueryResultItem()
{
}

QueryResultItem::~QueryResultItem()
{
    for (QObject* statGroup: m_playerStatsGroup)
        delete statGroup;
}

QueryResultItem::QueryResultItem(QString groupStatValue, QObjectList playersGroup)
    : m_playerStatsGroup(playersGroup), m_groupStatValue(groupStatValue)
{
}

QList<QObject *> QueryResultItem::getGroup()
{
    return m_playerStatsGroup;
}

QueryResultItem &QueryResultItem::operator=(const QueryResultItem &other)
{
    m_groupStatValue = other.m_groupStatValue;

    for (auto ps: m_playerStatsGroup)
        delete ps;

    for (auto ps: other.m_playerStatsGroup)
        m_playerStatsGroup << new PlayerStat(*qobject_cast<PlayerStat*>(ps));

    return *this;
}

Playerbase::Playerbase()
{
    m_base = {
        {"player A", new Player("player A", this, QUrl("qrc:/img/playerImages/13128.png"))},
        {"player BBB", new Player("player BBB", this)},
        {"player C", new Player("player C", this, QUrl("qrc:/img/playerImages/242510.png"))},
        {"player D", new Player("player D", this, QUrl("qrc:/img/playerImages/5984.png"))},
        {"player E", new Player("player E", this)},
        {"player F", new Player("player F", this)},
        {"player G", new Player("player G", this)},
        {"player H", new Player("player H", this)},
        {"player I", new Player("player I", this)},
        {"player J", new Player("player J", this)},
        {"player K", new Player("player K", this)}
    };
}

Playerbase::~Playerbase()
{
    for (auto p: m_base.keys())
        delete m_base.value(p);
}

Player *Playerbase::getPlayer(PlayerRef id) const
{
    return m_base.value(id);
}

QList<PlayerRef> Playerbase::listAllPlayers() const
{
    return m_base.keys();
}

PlayerStat::PlayerStat(Player *player, QString statValue)
    : m_player(player), m_statValue(statValue)
{
}

PlayerStat::PlayerStat(const PlayerStat &playerStat)
    : m_player(playerStat.m_player), m_statValue(playerStat.m_statValue)
{
}
