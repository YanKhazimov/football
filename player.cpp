#include "player.h"

Player::Player(const QString &name, QObject* parent, const QUrl photoUrl)
    : m_name(name), m_photo(photoUrl), QObject(parent)
{
}

Player::Player(const Player &p)
    : m_name(p.m_name), m_photo(p.m_photo)
{
}

Player &Player::operator =(const Player &p)
{
    m_name = p.m_name;
    m_photo = p.m_photo;

    return *this;
}

int Player::getRating() const
{
    int rating = 0;
    for(const QChar c: m_name)
        rating += c.toLatin1();
    return rating;
}

const QUrl Player::m_defaultPhoto("qrc:/img/playerImages/default.png");
