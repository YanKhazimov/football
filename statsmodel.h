#ifndef STATSMODEL_H
#define STATSMODEL_H

#include <QAbstractTableModel>

class StatsModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    StatsModel();
    StatsModel(const StatsModel& model);

    enum Roles {
        PlayerName = Qt::UserRole + 1,
        Rating,
        WinsLosses,
        Progress,
        Reliability
    };

private:
};

#endif // STATSMODEL_H
