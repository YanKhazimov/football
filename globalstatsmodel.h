#ifndef GLOBALSTATSMODEL_H
#define GLOBALSTATSMODEL_H

#include <QAbstractProxyModel>
#include <set>
#include "playersmodel.h"
#include "playerbase.h"
#include "dataroles.h"

class GlobalStatsModel: public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(int length READ rowCount CONSTANT)

public:
    GlobalStatsModel();
    GlobalStatsModel(const GlobalStatsModel& model);
    GlobalStatsModel(const PlayersModel& model, Playerbase* base);

    void setSourceModel(QAbstractItemModel *sourceModel);

    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    using Stat = QPair<QString, QString>;

    enum Roles {
        PlayerName = Qt::UserRole + 1,
        Rating,
        WinsLosses,
        Progress,
        Reliability
    };/*
    enum Stats {
        PlayerName = 0,
        Rating,
        WinsLosses,
        Progress,
        Reliability
    };*/
    virtual QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE Player* getPlayer(QString name);
    Q_INVOKABLE Player* getPlayer(int idx);

private slots:
    void resetModel();

private:
    void resetData();

    struct PlayerGameStats {
        int changedRating;
        bool participation;

        PlayerGameStats() = default;
        PlayerGameStats(int rating, bool part);
    };

    Playerbase* m_base;
    QMap<PlayerRef, QVector<PlayerGameStats>> m_players;
    QAbstractItemModel *m_sourceModel;
};

#endif // GLOBALSTATSMODEL_H
