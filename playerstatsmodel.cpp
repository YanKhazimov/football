#include "playerstatsmodel.h"

PlayerStatsModel::PlayerStatsModel()
{
    QStandardItem* item;

    item = new QStandardItem;
    item->setData(QString("Rating"), StatCategory);
    item->setData(QString("500"), StatValue);
    this->appendRow(item);

    item = new QStandardItem;
    item->setData(QString("Wins_Losses"), StatCategory);
    item->setData(QString("5-3 (62.5%)"), StatValue);
    this->appendRow(item);

    item = new QStandardItem;
    item->setData(QString("Reliability"), StatCategory);
    item->setData(QString("50%"), StatValue);
    this->appendRow(item);
}

PlayerStatsModel::PlayerStatsModel(const PlayerStatsModel &model)
{
    for (int r = 0; r < model.rowCount(); ++r)
    {
        this->appendRow(model.item(r));
    }
}

QString PlayerStatsModel::getStatCategory(int idx)
{
    return (idx >= rowCount()) ? "" : index(idx, 0).data(StatCategory).toString();
}

QHash<int, QByteArray> PlayerStatsModel::roleNames() const
{
    QHash<int, QByteArray> result;
    result[Roles::StatCategory] = "StatCategory";
    result[Roles::StatValue] = "StatValue";
    return result;
}
