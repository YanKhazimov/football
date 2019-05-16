#include "player.h"

QString Player::shorten() const
{
    QStringList words = m_name.split(' ', QString::SkipEmptyParts);

    QString shortened;
    for (const QString& word: words)
        shortened.append(word[0]);

    return shortened;
}

Player::Player(const QString &name, int initialRating, QObject* parent, const QUrl photoUrl)
    : QObject(parent), m_name(name), m_photo(photoUrl), m_initialRating(initialRating)
{
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

bool Player::operator ==(const Player &p)
{
    return m_name == p.m_name &&
            m_photo == p.m_photo &&
            m_initialRating == p.m_initialRating;
}

int Player::getInitialRating() const
{
    return m_initialRating;
}

const QUrl Player::defaultPhotoUrl("qrc:/img/defaultphoto.png");
