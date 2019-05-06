#include "playerbase.h"
#include <memory>
#include <QFile>
#include <QTextStream>

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
    init();
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

void Playerbase::init()
{
    QFile inputFile("players");
    if (inputFile.open(QIODevice::ReadOnly))
    {
        for (auto p: m_base.keys())
            delete m_base.value(p);

       QTextStream in(&inputFile);
       QString line, name;
       while (!in.atEnd())
       {
          line = in.readLine();
          name = line.mid(0, line.indexOf('\t'));

          //QUrl personalUrl("file:///" + QGuiApplication::applicationDirPath() + "/" + photoFilenameByName(name) + ".png");
          //QUrl personalUrl("./" + photoFilenameByName(name) + ".png");

          m_base[name] = new Player(name, line.mid(line.indexOf('\t')).toInt(), this,
                                    /*personalUrl.isValid() ? personalUrl :*/ Player::defaultPhotoUrl);
       }
       inputFile.close();
    }
}

PlayerStat::PlayerStat(Player *player, QString statValue)
    : m_player(player), m_statValue(statValue)
{
}

PlayerStat::PlayerStat(const PlayerStat &playerStat)
    : m_player(playerStat.m_player), m_statValue(playerStat.m_statValue)
{
}
