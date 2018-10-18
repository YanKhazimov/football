#include "playersmodel.h"

PlayersModel::PlayersModel()
{
}

PlayersModel::PlayersModel(const PlayersModel &pb)
{
    m_players = pb.m_players;
}

PlayersModel::PlayersModel(const GamesModel &gamesModel)
{
    for (auto game: gamesModel.getGames())
        for (auto player: game->getAllPlayers())
            if (!m_players.contains(player))
                m_players.append(player);
}

int PlayersModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_players.size();
}

QVariant PlayersModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

QList<PlayerRef> PlayersModel::getPlayers() const
{
    return m_players;
}

int PlayersModel::getPlayersCount() const
{
    return m_players.size();
}
