#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QUrl>

class Player : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name MEMBER m_name CONSTANT)
    Q_PROPERTY(QUrl photo MEMBER m_photo NOTIFY photoChanged)
    Q_PROPERTY(QString shortened READ shorten CONSTANT)

    QString m_name;
    QUrl m_photo;
    int m_initialRating;

    QString shorten() const;

public:
    Player(const QString& name, int initialRating, QObject* parent = nullptr, const QUrl photoUrl = defaultPhotoUrl);
    Player(const Player& p);
    Player& operator =(const Player& p);
    bool operator ==(const Player& p);

    int getInitialRating() const;
    static const QUrl defaultPhotoUrl;

    Q_INVOKABLE QString getName() const;

signals:
    void photoChanged();
};

Q_DECLARE_METATYPE(Player*)

using PlayerRef = QString;

#endif // PLAYER_H
