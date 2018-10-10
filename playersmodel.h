#ifndef PLAYERSMODEL_H
#define PLAYERSMODEL_H

#include <QAbstractListModel>
#include <QSet>
#include "player.h"
#include "gamesmodel.h"

class PlayersModel : public QAbstractListModel
{
    Q_OBJECT

public:
    PlayersModel ();
    PlayersModel (const PlayersModel& pb);
    PlayersModel (const GamesModel& gamesModel);
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    QSet<PlayerRef> getPlayers() const;
    Q_INVOKABLE int getPlayersCount() const;

private:
    QSet<PlayerRef> m_players;
};

#endif // PLAYERSMODEL_H
