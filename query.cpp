#include "query.h"
#include <memory>

PlayerbaseQuery::PlayerbaseQuery(const Playerbase& base, Query rule, QString title, QString description)
    : m_name(title), m_description(description)
{
    auto players = base.getPlayers().mid(0, rule);
    for (int i = 0; i < players.size(); ++i)
        m_queryResultItems.push_back(new QueryResultItem(QString::number(i), players[i]));
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
{
}

QueryResultItem::QueryResultItem(QString statValue, QObject *player)
    : m_player(player), m_statValue(statValue)
{
}
