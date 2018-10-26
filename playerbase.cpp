#include "playerbase.h"
#include <memory>

PlayerbaseQuery::PlayerbaseQuery(const PlayersModel& model, Playerbase& base, Query rule, QString title, QString description)
    : m_name(title), m_description(description)
{
    QList<PlayerRef> players = model.getPlayers();
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

QueryResultItem::QueryResultItem(QString groupStatValue, QObjectList playersGroup)
    : m_playersGroup(playersGroup), m_groupStatValue(groupStatValue)
{
}

QList<QObject *> QueryResultItem::getGroup()
{
    return m_playersGroup;
}

QueryResultItem &QueryResultItem::operator=(const QueryResultItem &other)
{
    m_groupStatValue = other.m_groupStatValue;

    for (auto ps: m_playersGroup)
        delete ps;

    for (auto ps: other.m_playersGroup)
        m_playersGroup << new PlayerStat(*qobject_cast<PlayerStat*>(ps));

    return *this;
}

Playerbase::Playerbase()
{
    m_base = {
        {"p1", new Player("p", QUrl("qrc:/img/playerImages/13128.png"))},
        {"p2", new Player("p2")},
        {"p3", new Player("p3", QUrl("qrc:/img/playerImages/238430.png"))},
        {"p4", new Player("p4", QUrl("qrc:/img/playerImages/242510.png"))},
        {"p5", new Player("p5", QUrl("qrc:/img/playerImages/5984.png"))},
        {"p6", new Player("p6", QUrl("qrc:/img/playerImages/242510.png"))},
        {"p7", new Player("p7")},
        {"p8", new Player("p8")},
        {"p9", new Player("p9")},
        {"p10", new Player("p10")},
        {"p11", new Player("p11")},
        {"p12", new Player("p12")}
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

PlayerStat::PlayerStat(Player *player, QString statValue)
    : m_player(player), m_statValue(statValue)
{
}

PlayerStat::PlayerStat(const PlayerStat &playerStat)
    : m_player(playerStat.m_player), m_statValue(playerStat.m_statValue)
{
}
