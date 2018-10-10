#ifndef PLAYER_H
#define PLAYER_H

#include <QAbstractListModel>
#include <QUrl>

class Player : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name MEMBER m_name CONSTANT)
    Q_PROPERTY(QUrl photo MEMBER m_photo CONSTANT)

    QString m_name;
    QUrl m_photo;

    static const QUrl m_defaultPhoto;

public:
    Player();
    Player(const QString& name, const QUrl photoUrl = m_defaultPhoto);
    Player(const Player& p);
    Player& operator =(const Player& p);
};

using PlayerRef = QString;

#endif // PLAYER_H
