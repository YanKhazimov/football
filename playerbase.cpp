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
        {"Ян Хазимов", new Player("Ян Хазимов", 2189, this, QUrl("qrc:/img/playerImages/1.png"))},
        {"Александр Гоносков", new Player("Александр Гоносков", 1972, this)},
        {"Сема Сологубов", new Player("Сема Сологубов", 1920, this)},
        {"Тима", new Player("Тима", 1891, this)},
        {"Андрей Нижегородцев", new Player("Андрей Нижегородцев", 1908, this)},
        {"Евгений Кистенев", new Player("Евгений Кистенев", 1870, this)},
        {"Дмитрий Сачков", new Player("Дмитрий Сачков", 1813, this)},
        {"Егор Федосеев", new Player("Егор Федосеев", 1789, this)},
        {"Егор Мазур", new Player("Егор Мазур", 1731, this, QUrl("qrc:/img/playerImages/242510.png"))},
        {"Дмитрий Шагин", new Player("Дмитрий Шагин", 1732, this)},
        {"Кирилл Волков", new Player("Кирилл Волков", 1640, this)},
        {"Сергей Новиков", new Player("Сергей Новиков", 1638, this)},
        {"Антон Володин", new Player("Антон Володин", 1656, this)},
        {"Коля Зайцев", new Player("Коля Зайцев", 1626, this)},
        {"Филипп Смирнов", new Player("Филипп Смирнов", 1595, this)},
        {"Евгений Красичков", new Player("Евгений Красичков", 1525, this)},
        {"Александр Шадрин", new Player("Александр Шадрин", 1517, this)},
        {"Александр Леушкин", new Player("Александр Леушкин", 1526, this, QUrl("qrc:/img/playerImages/5984.png"))},
        {"Коля Чалков", new Player("Коля Чалков", 1523, this)},
        {"Леха 5метров", new Player("Леха 5метров", 1800, this)},
        {"Рома Павлов", new Player("Рома Павлов", 1732, this)},
        {"Серега Мартынов", new Player("Серега Мартынов", 1800, this)},
        {"Саня Таран", new Player("Саня Таран", 1800, this)},
        {"Сергей Саушкин", new Player("Сергей Саушкин", 1800, this)},
        {"Игорь Серов", new Player("Игорь Серов", 1650, this, QUrl("qrc:/img/playerImages/13128.png"))},
        {"Денис Захаров", new Player("Денис Захаров", 1900, this)},
        {"Дима", new Player("Дима", 1950, this)}
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
