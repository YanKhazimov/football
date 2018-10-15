#ifndef GLOBALSTATSMODEL_H
#define GLOBALSTATSMODEL_H

#include <QStandardItemModel>
#include "playersmodel.h"
#include "query.h"

class GlobalStatsModel: public QStandardItemModel
{
    Q_OBJECT
    Q_PROPERTY(int length READ rowCount CONSTANT)

public:
    GlobalStatsModel();
    GlobalStatsModel(const GlobalStatsModel& model);
    GlobalStatsModel(const PlayersModel& model, Playerbase* base);

    using Stat = QPair<QString, QString>;

    enum Roles {
        PlayerName = Qt::UserRole + 1,
        Rating,
        WinsLosses,
        Progress,
        Reliability
    };
    virtual QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE Player* getPlayer(QString name);

private:
    QVector<Stat> m_stats;
    Playerbase* m_base;
};

#endif // GLOBALSTATSMODEL_H
