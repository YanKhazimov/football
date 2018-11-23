#include "playerstatsmodel.h"
#include "dataroles.h"

PlayerStatsModel::PlayerStatsModel()
{
    QStandardItem* item;

    item = new QStandardItem;
    item->setData(QString("Rating"), DataRoles::DataRole::StatCategory);
    item->setData(QString("500"), DataRoles::DataRole::StatValue);
    this->appendRow(item);

    item = new QStandardItem;
    item->setData(QString("WinsLosses"), DataRoles::DataRole::StatCategory);
    item->setData(QString("5-3 (62.5%)"), DataRoles::DataRole::StatValue);
    this->appendRow(item);

    item = new QStandardItem;
    item->setData(QString("Progress"), DataRoles::DataRole::StatCategory);
    item->setData(QString("-20"), DataRoles::DataRole::StatValue);
    this->appendRow(item);

    item = new QStandardItem;
    item->setData(QString("Dedication"), DataRoles::DataRole::StatCategory);
    item->setData(QString("50%"), DataRoles::DataRole::StatValue);
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
    return (idx >= rowCount()) ? "" : index(idx, 0).data(DataRoles::DataRole::StatCategory).toString();
}

QHash<int, QByteArray> PlayerStatsModel::roleNames() const
{
    QHash<int, QByteArray> result;
    result[DataRoles::DataRole::StatCategory] = "StatCategory";
    result[DataRoles::DataRole::StatValue] = "StatValue";
    return result;
}
