#ifndef PLAYERSTATSMODEL_H
#define PLAYERSTATSMODEL_H

#include <QStandardItemModel>
#include <QDate>
#include "player.h"

class QValueAtDate: public QObject
{
    Q_OBJECT

public:
    QDate date;
    int value;
    QValueAtDate(QDate d, int v);
};

class PlayerStatsModel: public QStandardItemModel
{
    Q_OBJECT
    Q_PROPERTY(int length READ rowCount CONSTANT)
    Q_PROPERTY(Player* selectedPlayer MEMBER m_selectedPlayer NOTIFY playerChanged)
    Q_PROPERTY(QList<QObject*> ratingHistory MEMBER m_ratingHistory NOTIFY playerChanged)

public:
    PlayerStatsModel();
    PlayerStatsModel(const PlayerStatsModel& model);
    //using QStandardItemModel::QStandardItemModel;

    virtual void setSourceModel(QAbstractItemModel *sourceModel);

    using Stat = QPair<QString, QString>;
    Q_INVOKABLE QString getStatCategory(int idx);

    virtual QHash<int, QByteArray> roleNames() const override;

private slots:
    void resetData();
    void onDataChanged(QModelIndex topLeft, QModelIndex bottomRight, QVector<int> roles);

signals:
    void playerChanged();

private:
    Player* m_selectedPlayer;
    QObjectList m_ratingHistory;
    QAbstractItemModel *m_sourceModel;
    QVector<Stat> m_stats;
};

#endif // PLAYERSTATSMODEL_H
