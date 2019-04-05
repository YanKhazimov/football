#ifndef PERSONALSTATSMODEL_H
#define PERSONALSTATSMODEL_H

#include <QStandardItemModel>
#include "player.h"
#include "language.h"

class PersonalStatsModel: public QStandardItemModel
{
    Q_OBJECT
    Q_PROPERTY(int length READ rowCount CONSTANT)
    Q_PROPERTY(Player* selectedPlayer MEMBER m_selectedPlayer NOTIFY playerChanged)
    Q_PROPERTY(QList<int> ratingHistory MEMBER m_ratingHistory NOTIFY playerChanged)

public:
    PersonalStatsModel(const Language& lang);
    PersonalStatsModel(const PersonalStatsModel& model);

    virtual void setSourceModel(QAbstractItemModel *sourceModel);

    Q_INVOKABLE QString getStatCategory(int idx);
    Q_INVOKABLE int getStatRole(int row);

    virtual QHash<int, QByteArray> roleNames() const override;

private slots:
    void resetModel();
    void onDataChanged(QModelIndex topLeft, QModelIndex bottomRight, QVector<int> roles);

public slots:
    void onLanguageChanged(QString lang);

signals:
    void playerChanged();

private:
    Player* m_selectedPlayer;
    QList<int> m_ratingHistory;
    QAbstractItemModel *m_sourceModel;
};

#endif // PERSONALSTATSMODEL_H
