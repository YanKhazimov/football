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
    return 3211;
}

const QUrl Player::m_defaultPhoto("qrc:/img/playerImages/default.png");
