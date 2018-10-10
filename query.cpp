#include "query.h"
#include <memory>

PlayerbaseQuery::PlayerbaseQuery(const PlayersModel& model, Playerbase& base, Query rule, QString title, QString description)
    : m_name(title), m_description(description)
{
    QSet<PlayerRef> players = model.getPlayers();
    int i = 0;
    for (auto p = players.constBegin(); p != players.constEnd() && i < rule; ++i, ++p)
        m_queryResultItems.push_back(new QueryResultItem(QString::number(i), base.getPlayer(*p)));
}

PlayerbaseQuery::~PlayerbaseQuery()
{
    for (auto q: m_queryResultItems)
        delete q;
}

QList<QObject*> PlayerbaseQuery::getQueryResult()
{
    return m_queryResultItems;
}

QueryResultItem::QueryResultItem()
    : m_player(nullptr)
{
}

QueryResultItem::QueryResultItem(QString statValue, QObject *player)
    : m_player(player), m_statValue(statValue)
{
}

Playerbase::Playerbase()
{
    m_base = {
        {"p1", new Player("p", QUrl("qrc:/img/playerImages/13128.png"))},
        {"p2", new Player("p2")},
        {"p3", new Player("p3")},//, QUrl("qrc:/img/playerImages/238430.png"))
        {"p4", new Player("p4")},//, QUrl("qrc:/img/playerImages/242510.png"))
        {"p5", new Player("p5", QUrl("qrc:/img/playerImages/5984.png"))},
        {"p6", new Player("p6")},//, QUrl("qrc:/img/playerImages/242510.png"))
    };
}

Playerbase::~Playerbase()
{
    for (auto p: m_base.keys())
        delete m_base.value(p);
}

QObject *Playerbase::getPlayer(PlayerRef id) const
{
    return m_base.value(id);
}
