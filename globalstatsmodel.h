#ifndef GLOBALSTATSMODEL_H
#define GLOBALSTATSMODEL_H

#include <QAbstractProxyModel>
#include <map>
#include "playersmodel.h"
#include "playerbase.h"
#include "dataroles.h"

class GlobalStatsModel: public QAbstractTableModel//, public QAbstractProxyModel
{
    Q_OBJECT
    Q_PROPERTY(int length READ rowCount CONSTANT)

public:
    GlobalStatsModel() = default;
    GlobalStatsModel(const Playerbase* base);
    GlobalStatsModel(const GlobalStatsModel& model);
    GlobalStatsModel(const PlayersModel& model, Playerbase* base);

    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;


    virtual void setSourceModel(QAbstractItemModel *sourceModel);// Q_DECL_OVERRIDE;
//    Q_INVOKABLE virtual QModelIndex mapToSource(const QModelIndex &proxyIndex) const Q_DECL_OVERRIDE;
//    Q_INVOKABLE virtual QModelIndex mapFromSource(const QModelIndex &sourceIndex) const Q_DECL_OVERRIDE;

    using Stat = QPair<QString, QString>;

//    enum Roles {
//        PlayerName = Qt::UserRole + 1, //257
//        Rating,
//        WinsLosses,
//        Progress,
//        Dedication
//    };
    virtual QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE Player* getPlayer(QString name);
    Q_INVOKABLE Player* getPlayer(int idx);
    Q_INVOKABLE bool selectRow(int row);

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

    const Playerbase* m_base;
    QAbstractItemModel *m_sourceModel;

    QVector<QDate> m_dates;
    std::map<PlayerRef, QVector<PlayerGameStats>> m_players;
    int m_selectedPlayerIndex = -1;
};

#endif // GLOBALSTATSMODEL_H
