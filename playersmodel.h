#ifndef PLAYERSMODEL_H
#define PLAYERSMODEL_H

#include <QAbstractListModel>
#include <QUrl>

class Player : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name MEMBER m_name CONSTANT)
    Q_PROPERTY(QUrl photo MEMBER m_photo CONSTANT)

    QString m_name;
    QUrl m_photo;

public:
    Player();
    Player(const QString& name, const QUrl photoUrl = QUrl("qrc:/img/playerImages/default.png"));
    QString getName() const;
};

class Playerbase : public QAbstractListModel
{
    Q_OBJECT

public:
    Q_INVOKABLE QObject *getPlayer(int index);

    Playerbase ();
    Playerbase (const Playerbase& pb);
    ~Playerbase ();
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    QList<QObject *> getPlayers() const;
    Q_INVOKABLE int getPlayersCount() const;

private:
    QList<QObject*> m_players;
};
#endif // PLAYERSMODEL_H
