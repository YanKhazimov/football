#ifndef PLAYERSMODEL_H
#define PLAYERSMODEL_H

#include <QAbstractListModel>
#include <QList>
#include "player.h"
#include "gamesmodel.h"

class PlayersModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int length READ getPlayersCount CONSTANT)

public:
    PlayersModel ();
    PlayersModel (const PlayersModel& pb);
    PlayersModel (const GamesModel& gamesModel);
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    QList<PlayerRef> getPlayers() const;
    Q_INVOKABLE int getPlayersCount() const;

private:
    QList<PlayerRef> m_players;
};

#endif // PLAYERSMODEL_H
