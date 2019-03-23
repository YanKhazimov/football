#include <QDebug>
#include "playerstatsmodel.h"
#include "dataroles.h"

PlayerStatsModel::PlayerStatsModel()
    : m_sourceModel(nullptr), m_selectedPlayer(nullptr)
{
    QStandardItem* item;

    item = new QStandardItem;
    item->setData(QString("Rating"), DataRoles::DataRole::StatCategory);
    item->setData(DataRoles::DataRole::Rating, DataRoles::DataRole::SourceRole);
    this->appendRow(item);

    item = new QStandardItem;
    item->setData(QString("WinsLosses"), DataRoles::DataRole::StatCategory);
    item->setData(DataRoles::DataRole::WinsLosses, DataRoles::DataRole::SourceRole);
    this->appendRow(item);

    item = new QStandardItem;
    item->setData(QString("Progress"), DataRoles::DataRole::StatCategory);
    item->setData(DataRoles::DataRole::Progress, DataRoles::DataRole::SourceRole);
    this->appendRow(item);

    item = new QStandardItem;
    item->setData(QString("Relevance"), DataRoles::DataRole::StatCategory);
    item->setData(DataRoles::DataRole::Relevance, DataRoles::DataRole::SourceRole);
    this->appendRow(item);

    item = new QStandardItem;
    item->setData(QString("Dedication"), DataRoles::DataRole::StatCategory);
    item->setData(DataRoles::DataRole::Dedication, DataRoles::DataRole::SourceRole);
    this->appendRow(item);
}

PlayerStatsModel::PlayerStatsModel(const PlayerStatsModel &model)
{
    for (int r = 0; r < model.rowCount(); ++r)
    {
        this->appendRow(model.item(r));
    }
}

void PlayerStatsModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    beginResetModel();

    if (m_sourceModel)
    {
        disconnect(m_sourceModel, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
                   this, SLOT(onDataChanged(QModelIndex,QModelIndex,QVector<int>)));

//        disconnect(m_sourceModel, SIGNAL(rowsInserted(QModelIndex,int,int)),
//                   this, SLOT(sourceRowsInserted(QModelIndex,int,int)));

        disconnect(m_sourceModel, SIGNAL(modelReset()), this, SLOT(resetModel()));
    }

    //QAbstractProxyModel::setSourceModel(sourceModel);

    if (sourceModel)
    {
        m_sourceModel = sourceModel;

        connect(m_sourceModel, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
                      this, SLOT(onDataChanged(QModelIndex,QModelIndex,QVector<int>)));

//        connect(m_sourceModel, SIGNAL(rowsInserted(QModelIndex,int,int)),
//                      this, SLOT(onRowsInserted(QModelIndex,int,int)));

        connect(m_sourceModel, SIGNAL(modelReset()), this, SLOT(resetModel()));
    }

    resetModel();

    endResetModel();
}

QString PlayerStatsModel::getStatCategory(int idx)
{
    return (idx >= rowCount()) ? "" : index(idx, 0).data(DataRoles::DataRole::StatCategory).toString();
}

int PlayerStatsModel::getStatRole(int row)
{
    return index(row, 0).data(DataRoles::DataRole::SourceRole).toInt();
}

QHash<int, QByteArray> PlayerStatsModel::roleNames() const
{
    QHash<int, QByteArray> result;
    result[DataRoles::DataRole::StatCategory] = "StatCategory";
    result[DataRoles::DataRole::StatValue] = "StatValue";
    return result;
}

void PlayerStatsModel::onDataChanged(QModelIndex topLeft, QModelIndex bottomRight, QVector<int> roles)
{
    if (roles.contains(DataRoles::DataRole::PlayerSelection))
    {
        Q_ASSERT(topLeft == bottomRight);

        bool selected = topLeft.data(DataRoles::DataRole::PlayerSelection).toBool();

        for (int row = 0; row < rowCount(); ++row)
        {
            QStandardItem* statItem = item(row, 0);
            int sourceRole = statItem->data(DataRoles::DataRole::SourceRole).toInt();
            QVariant statValue = selected ? topLeft.data(sourceRole) : QVariant();
            statItem->setData(statValue, DataRoles::DataRole::StatValue);
        }

        if (selected)
        {
            m_selectedPlayer = topLeft.data(DataRoles::DataRole::Player).value<Player*>();
            m_ratingHistory = topLeft.data(DataRoles::DataRole::RatingHistory).value<QList<int>>();
        }
        else
        {
            m_selectedPlayer = nullptr;
            m_ratingHistory = {};
        }
        emit playerChanged();
    }
}

void PlayerStatsModel::resetModel()
{
    // update selected player
    int playerCount = m_sourceModel->rowCount();
    for (int p = 0; p < playerCount; ++p)
    {
        QModelIndex sourceIndex = m_sourceModel->index(p, 0);

        if (sourceIndex.data(DataRoles::DataRole::PlayerSelection).toBool())
        {
            m_selectedPlayer = sourceIndex.data(DataRoles::DataRole::Player).value<Player*>();
            m_ratingHistory = sourceIndex.data(DataRoles::DataRole::RatingHistory).value<QList<int>>();

            for (int row = 0; row < rowCount(); ++row)
            {
                QStandardItem* statItem = item(row, 0);
                int sourceRole = statItem->data(DataRoles::DataRole::SourceRole).toInt();
                QVariant statValue = sourceIndex.data(sourceRole);
                statItem->setData(statValue, DataRoles::DataRole::StatValue);
            }

            emit playerChanged();

            break;
        }
    }
}
