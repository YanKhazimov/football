#ifndef PLAYERSTATSMODEL_H
#define PLAYERSTATSMODEL_H

#include <QStandardItemModel>

class PlayerStatsModel: public QStandardItemModel
{
    Q_OBJECT
    Q_PROPERTY(int length READ rowCount CONSTANT)

public:
    PlayerStatsModel();
    PlayerStatsModel(const PlayerStatsModel& model);
    //using QStandardItemModel::QStandardItemModel;

    using Stat = QPair<QString, QString>;

    enum Roles {
        StatCategory = Qt::UserRole + 1,
        StatValue
    };
    Q_INVOKABLE QString getStatCategory(int idx);

    virtual QHash<int, QByteArray> roleNames() const override;
private:
    QVector<Stat> m_stats;
};

#endif // PLAYERSTATSMODEL_H
