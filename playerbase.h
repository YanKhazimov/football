#ifndef QUERY_H
#define QUERY_H

#include <QAbstractListModel>
#include <QUrl>
#include <QMap>
#include "playersmodel.h"

class Playerbase: public QObject
{
    Q_OBJECT
    QMap<PlayerRef, Player*> m_base;

public:
    Playerbase();
    ~Playerbase();
    Player *getPlayer(PlayerRef id) const;
    QList<PlayerRef> listAllPlayers() const;
};

class PlayerStat : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* player MEMBER m_player CONSTANT)
    Q_PROPERTY(QString statValue MEMBER m_statValue CONSTANT)

    Player* m_player;
    QString m_statValue;

public:
    PlayerStat(Player* player, QString statValue);
    PlayerStat(const PlayerStat& playerStat);
};

class QueryResultItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> group READ getGroup CONSTANT)
    Q_PROPERTY(QString groupStatValue MEMBER m_groupStatValue CONSTANT)

public:
    QueryResultItem ();
    QueryResultItem (QString groupStatValue, QObjectList playersGroup);
    QList<QObject*> getGroup();
    QueryResultItem& operator= (const QueryResultItem& other);

private:
    QList<QObject*> m_playersGroup;
    QString m_groupStatValue;
};

class PlayerbaseQuery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name MEMBER m_name CONSTANT)
    Q_PROPERTY(QString description MEMBER m_description CONSTANT)
    Q_PROPERTY(QList<QObject*> queryResult MEMBER m_queryResultItems CONSTANT)

    QString m_name;
    QString m_description;
    QObjectList m_queryResultItems;

    using Query = int;

public:
    PlayerbaseQuery(const PlayersModel& model, Playerbase& base,
                    Query rule, QString title = "1", QString description = "11");
    ~PlayerbaseQuery();

    //Q_INVOKABLE QObjectList getQueryResult();
};

#endif // QUERY_H
