#include <QDebug>
#include "personalstatsmodel.h"
#include "dataroles.h"

PersonalStatsModel::PersonalStatsModel(const Language &lang)
    : m_selectedPlayer(nullptr), m_sourceModel(nullptr)
{
    QStandardItem* item;

    item = new QStandardItem;
    item->setData(DataRoles::DataRole::Rating, DataRoles::DataRole::SourceRole);
    this->appendRow(item);

    item = new QStandardItem;
    item->setData(DataRoles::DataRole::WinsLosses, DataRoles::DataRole::SourceRole);
    this->appendRow(item);

    item = new QStandardItem;
    item->setData(DataRoles::DataRole::Progress, DataRoles::DataRole::SourceRole);
    this->appendRow(item);

    item = new QStandardItem;
    item->setData(DataRoles::DataRole::Relevance, DataRoles::DataRole::SourceRole);
    this->appendRow(item);

    item = new QStandardItem;
    item->setData(DataRoles::DataRole::Dedication, DataRoles::DataRole::SourceRole);
    this->appendRow(item);

    onLanguageChanged(lang.get());
    connect(&lang, &Language::languageChanged, this, &PersonalStatsModel::onLanguageChanged);
}

PersonalStatsModel::PersonalStatsModel(const PersonalStatsModel &model)
{
    for (int r = 0; r < model.rowCount(); ++r)
    {
        this->appendRow(model.item(r));
    }
}

void PersonalStatsModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    beginResetModel();

    if (m_sourceModel)
    {
        disconnect(m_sourceModel, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
                   this, SLOT(onDataChanged(QModelIndex,QModelIndex,QVector<int>)));

        disconnect(m_sourceModel, SIGNAL(modelReset()), this, SLOT(resetModel()));
    }

    if (sourceModel)
    {
        m_sourceModel = sourceModel;

        connect(m_sourceModel, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
                      this, SLOT(onDataChanged(QModelIndex,QModelIndex,QVector<int>)));

        connect(m_sourceModel, SIGNAL(modelReset()), this, SLOT(resetModel()));
    }

    resetModel();

    endResetModel();
}

QString PersonalStatsModel::getStatCategory(int idx)
{
    return (idx >= rowCount()) ? "" : index(idx, 0).data(DataRoles::DataRole::StatCategory).toString();
}

int PersonalStatsModel::getStatRole(int row)
{
    return index(row, 0).data(DataRoles::DataRole::SourceRole).toInt();
}

QHash<int, QByteArray> PersonalStatsModel::roleNames() const
{
    QHash<int, QByteArray> result;
    result[DataRoles::DataRole::StatCategory] = "StatCategory";
    result[DataRoles::DataRole::StatValue] = "StatValue";
    return result;
}

QVariant convertStat(QVariant stat, int role)
{
    if (!stat.isValid())
        return QVariant();

    if (role == DataRoles::DataRole::WinsLosses)
    {
        QVector<int> wdl = stat.value<QVector<int>>();
        Q_ASSERT(wdl.size() == 3);
        return QVariant::fromValue(QString("%1-%2-%3")
                            .arg(QString::number(wdl[0]))
                            .arg(QString::number(wdl[1]))
                            .arg(QString::number(wdl[2])));
    }
    else if (role == DataRoles::DataRole::Progress) {
        int progress = stat.toInt();
        QString extraSign = progress > 0 ? "+" : "";
        return QVariant::fromValue(extraSign + QString::number(progress));
    }
    else if (role == DataRoles::DataRole::Dedication || role == DataRoles::DataRole::Relevance)
    {
        int rounded = static_cast<int>(stat.toFloat());
        return QVariant::fromValue(QString::number(rounded) + "%");
    }

    return stat;
}

void PersonalStatsModel::onDataChanged(QModelIndex topLeft, QModelIndex bottomRight, QVector<int> roles)
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
            statItem->setData(convertStat(statValue, sourceRole), DataRoles::DataRole::StatValue);
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

void PersonalStatsModel::onLanguageChanged(QString lang)
{
    //Language* dict = qobject_cast<Language*>(sender());

    item(0)->setData(QVariant::fromValue(Language::dict.value("ratingColumn").value(lang)),
                     DataRoles::DataRole::StatCategory);
    item(1)->setData(QVariant::fromValue(Language::dict.value("wdlColumn").value(lang)),
                     DataRoles::DataRole::StatCategory);
    item(2)->setData(QVariant::fromValue(Language::dict.value("progressColumn").value(lang)),
                     DataRoles::DataRole::StatCategory);
    item(3)->setData(QVariant::fromValue(Language::dict.value("relevanceColumn").value(lang)),
                     DataRoles::DataRole::StatCategory);
    item(4)->setData(QVariant::fromValue(Language::dict.value("dedicationColumn").value(lang)),
                     DataRoles::DataRole::StatCategory);
}

void PersonalStatsModel::resetModel()
{
    // update selected player
    m_selectedPlayer = nullptr;
    m_ratingHistory.clear();

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
                statItem->setData(convertStat(statValue, sourceRole), DataRoles::DataRole::StatValue);
            }

            break;
        }
    }

    emit playerChanged();
}
