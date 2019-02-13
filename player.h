#ifndef PLAYER_H
#define PLAYER_H

#include <QAbstractListModel>
#include <QUrl>

class Player : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name MEMBER m_name CONSTANT)
    Q_PROPERTY(QUrl photo MEMBER m_photo CONSTANT)
    Q_PROPERTY(QString shortened READ shorten CONSTANT)

    QString m_name;
    QUrl m_photo;
    int m_initialRating;

    static const QUrl m_defaultPhoto;

    QString shorten() const;

public:
    Player(const QString& name, QObject* parent = nullptr, const QUrl photoUrl = m_defaultPhoto);
    Player(const Player& p);
    Player& operator =(const Player& p);

    int getInitialRating() const;
};

Q_DECLARE_METATYPE(Player*)

using PlayerRef = QString;

#endif // PLAYER_H
