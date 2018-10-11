#include "player.h"

Player::Player(const QString &name, const QUrl photoUrl)
    : m_name(name), m_photo(photoUrl)
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

const QUrl Player::m_defaultPhoto("qrc:/img/playerImages/default.png");
