#include "globalstatsmodel.h"


GlobalStatsModel::GlobalStatsModel()
{
    QStandardItem* item;

    item = new QStandardItem;
    item->setData(QString("500"), Rating);
    item->setData(QString("5-3 (62.5%)"), WinsLosses);
    item->setData(QString("-20"), Progress);
    item->setData(QString("50%"), Reliability);
    this->appendRow(item);

    item = new QStandardItem;
    item->setData(QString("4500"), Rating);
    item->setData(QString("1-3 (25%)"), WinsLosses);
    item->setData(QString("-220"), Progress);
    item->setData(QString("30%"), Reliability);
    this->appendRow(item);
}

GlobalStatsModel::GlobalStatsModel(const GlobalStatsModel &model)
{
    for (int r = 0; r < model.rowCount(); ++r)
    {
        this->appendRow(model.item(r));
    }
}

GlobalStatsModel::GlobalStatsModel(const PlayersModel &model, Playerbase* base)
    : m_base(base)
{
    float random = 99.9f;
    for (auto p: model.getPlayers())
    {
        QStandardItem* item;

        item = new QStandardItem;
        item->setData(QString(p), PlayerName);
        item->setData(QString::number(random), Rating);
        item->setData(QString("5-3 (62.5%)"), WinsLosses);
        item->setData(QString("-20"), Progress);
        item->setData(QString("50%"), Reliability);
        this->appendRow(item);
        random /= 1.1f;
    }
}

QHash<int, QByteArray> GlobalStatsModel::roleNames() const
{
    QHash<int, QByteArray> result;

    result[Roles::PlayerName] = "PlayerName";
    result[Roles::Rating] = "Rating";
    result[Roles::WinsLosses] = "WinsLosses";
    result[Roles::Progress] = "Progress";
    result[Roles::Reliability] = "Reliability";

    return result;
}

Player *GlobalStatsModel::getPlayer(QString name)
{
    return m_base->getPlayer(name);
}
