#ifndef PLAYERSTATSMODEL_H
#define PLAYERSTATSMODEL_H

#include <QStandardItemModel>

class PlayerStatsModel: QStandardItemModel
{
public:
    PlayerStatsModel();

    using Stat = QPair<QString, QString>;

private:
    QVector<Stat> m_stats;
};

#endif // PLAYERSTATSMODEL_H
