#include "query.h"

Player::Player(const QString &name, const QUrl photoUrl)
    : m_name(name), m_photo(photoUrl)
{
}

QString Player::getName() const
{
    return m_name;
}

Playerbase::Playerbase()
{
    m_players << new Player("r", QUrl("qrc:/img/playerImages/13128.png"))
              << new Player("yellow")//, QUrl("qrc:/img/playerImages/238395.png"))
              << new Player("g")
              << new Player("g")//, QUrl("qrc:/img/playerImages/238430.png"))
              << new Player("g")//, QUrl("qrc:/img/playerImages/242510.png"))
              << new Player("g")//, QUrl("qrc:/img/playerImages/5984.png"))
              << new Player("g");//, QUrl("qrc:/img/playerImages/6235.png"));
}

Playerbase::Playerbase(const Playerbase &pb)
{
    m_players = pb.m_players;
}

Playerbase::~Playerbase()
{
    for (auto p: m_players)
        delete p;
}

int Playerbase::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_players.size();
}

QVariant Playerbase::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

QList<QObject *> Playerbase::getPlayers() const
{
    return m_players;
}

int Playerbase::getPlayersCount() const
{
    return m_players.size();
}

QObject* Playerbase::getPlayer(int index)
{
    return m_players[index];
}
