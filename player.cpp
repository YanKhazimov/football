#include "player.h"

QString Player::shorten() const
{
    if (m_name.isEmpty() || !m_name.chopped(1).contains(' '))
        return m_name;

    return QString(m_name.at(0)) + QString(m_name.at(m_name.lastIndexOf(' ') + 1));
}

Player::Player(const QString &name, QObject* parent, const QUrl photoUrl)
    : QObject(parent), m_name(name), m_photo(photoUrl)
{
    m_initialRating = getRating();
}

Player::Player(const Player &p)
    : m_name(p.m_name), m_photo(p.m_photo), m_initialRating(p.m_initialRating)
{
}

Player &Player::operator =(const Player &p)
{
    m_name = p.m_name;
    m_photo = p.m_photo;
    m_initialRating = p.m_initialRating;

    return *this;
}

int Player::getRating() const
{
    int rating = 0;
    for(const QChar c: m_name)
        rating += c.toLatin1();
    return rating;
}

int Player::getInitialRating() const
{
    return m_initialRating;
}

const QUrl Player::m_defaultPhoto("qrc:/img/playerImages/default.png");
