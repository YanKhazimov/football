#include "playerbase.h"
#include <memory>
#include <QFile>
#include <QTextStream>
#include <QDir>

PlayerbaseQuery::PlayerbaseQuery(Playerbase& base, Query rule, QString title, QString description)
    : m_name(title), m_description(description)
{
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

QUrl Playerbase::getPhotoUrl(const QStringList& playerData)
{
    if (playerData.size() < 3)
        return Player::defaultPhotoUrl;

    QString localFilepath = QDir::currentPath() + "/data/" + playerData[2];

    if (!QFileInfo::exists(localFilepath))
        return Player::defaultPhotoUrl;

    return QUrl("file:///" + localFilepath);
}

void Playerbase::init()
{
    QFile inputFile("data/players");
    if (inputFile.open(QIODevice::ReadOnly))
    {
        for (auto p: m_base.keys())
            delete m_base.value(p);

       QTextStream in(&inputFile);
       QString name; int initialRating; QUrl photo; QStringList playerData;
       while (!in.atEnd())
       {
           playerData = in.readLine().split('\t', QString::SplitBehavior::SkipEmptyParts);

           name = playerData[0];
           initialRating = playerData[1].toInt();
           photo = getPhotoUrl(playerData);

           m_base[name] = new Player(name, initialRating, this, photo);
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
