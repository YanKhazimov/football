#ifndef GLOBALSTATSMODEL_H
#define GLOBALSTATSMODEL_H

#include <map>
#include <QDate>
#include "playerbase.h"
#include "dataroles.h"
#include "award.h"

class GlobalStatsModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    GlobalStatsModel() = default;
    GlobalStatsModel(const Playerbase* base);
    GlobalStatsModel(const GlobalStatsModel& model);

    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    Q_INVOKABLE virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

    virtual void setSourceModel(QAbstractItemModel *sourceModel);

    virtual QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE Player* getPlayer(QString name);

    int getRating(const PlayerRef& name) const;

    Q_INVOKABLE bool setSeasonFilter(QString filter);

    Q_INVOKABLE QVariant getAwards(const QStringList& categoryFilter, const QStringList& rankFilter,
                                   const QList<int>& seasonFilter, const QStringList& playerFilter);

private slots:
    void resetModel();
    void sourceRowsInserted(QModelIndex parent, int first, int last);

private:    
    struct PlayerGameStats {
        int changedRating;
        int resultSign;
        QModelIndex sourceIndex;

        PlayerGameStats() = default;
        PlayerGameStats(int rating, int sign, QModelIndex srcIdx);
    };

    void resetData();
    void resetAwards();
    void resetSeasonAwards(int season);
    std::pair<PlayerRef, QVector<PlayerGameStats>> getPlayerData(const QModelIndex& index) const;

    QModelIndex getIndexByRef(const PlayerRef& ref);

    const Playerbase* m_base;
    QAbstractItemModel *m_sourceModel;

    std::map<PlayerRef, QVector<PlayerGameStats>> m_playersData;
    QMap<PlayerRef, int> m_seasonStartingRating;
    Awards m_awards;
    PlayerRef m_selectedPlayer;
    QDate m_minDate, m_maxDate;
};

#endif // GLOBALSTATSMODEL_H
