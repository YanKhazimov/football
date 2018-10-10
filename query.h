#ifndef QUERY_H
#define QUERY_H

#include <QAbstractListModel>
#include <QUrl>
#include <QMap>
#include "playersmodel.h"

class Playerbase
{
    QMap<PlayerRef, Player*> m_base;

public:
    Playerbase();
    ~Playerbase();
    QObject *getPlayer(PlayerRef id) const;
};

class PlayerbaseQuery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name MEMBER m_name CONSTANT)
    Q_PROPERTY(QString description MEMBER m_description CONSTANT)

    QString m_name;
    QString m_description;
    QList<QObject*> m_queryResultItems;

    using Query = int;

public:
    PlayerbaseQuery(const PlayersModel& model, Playerbase& base,
                    Query rule, QString title = "1", QString description = "11");
    ~PlayerbaseQuery();

    Q_INVOKABLE QList<QObject*> getQueryResult();
};

class QueryResultItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString statValue MEMBER m_statValue CONSTANT)
    Q_PROPERTY(QObject* player MEMBER m_player CONSTANT)
public:
    QueryResultItem ();
    QueryResultItem (QString statValue, QObject* player);

private:
    QObject* m_player;
    QString m_statValue;
};

#endif // QUERY_H
