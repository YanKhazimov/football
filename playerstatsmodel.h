#ifndef PLAYERSTATSMODEL_H
#define PLAYERSTATSMODEL_H

#include <QStandardItemModel>
#include "player.h"

class PlayerStatsModel: public QStandardItemModel
{
    Q_OBJECT
    Q_PROPERTY(int length READ rowCount CONSTANT)
    Q_PROPERTY(Player* selectedPlayer MEMBER m_selectedPlayer NOTIFY playerChanged)
    Q_PROPERTY(QList<int> ratingHistory MEMBER m_ratingHistory NOTIFY playerChanged)

public:
    PlayerStatsModel();
    PlayerStatsModel(const PlayerStatsModel& model);

    virtual void setSourceModel(QAbstractItemModel *sourceModel);

    Q_INVOKABLE QString getStatCategory(int idx);
    Q_INVOKABLE int getStatRole(int row);

    virtual QHash<int, QByteArray> roleNames() const override;

private slots:
    void resetModel();
    void onDataChanged(QModelIndex topLeft, QModelIndex bottomRight, QVector<int> roles);

signals:
    void playerChanged();

private:
    Player* m_selectedPlayer;
    QList<int> m_ratingHistory;
    QAbstractItemModel *m_sourceModel;
};

#endif // PLAYERSTATSMODEL_H
