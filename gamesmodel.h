#ifndef GAMESMODEL_H
#define GAMESMODEL_H

#include <QAbstractListModel>
#include <QDate>
#include <QPoint>
#include "player.h"
#include "dataroles.h"

class Game : public QObject
{
    Q_OBJECT

    QDate m_date;
    QVector<PlayerRef> m_hometeam;
    QVector<PlayerRef> m_awayteam;
    QPair<int, int> m_score; // home, away

public:
    Game() = delete;
    Game(QDate date, QVector<PlayerRef> hometeam, QVector<PlayerRef> awayteam, QPair<int, int> score);
    QVector<PlayerRef> getHometeam() const;
    QVector<PlayerRef> getAwayteam() const;
    QPair<int, int> getScore() const;
    QDate getDate() const;
};

class GamesModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int pulseRosterConsistency READ getPulseRosterConsistency NOTIFY pulseChanged)

public:
    GamesModel() = default;
    ~GamesModel();
    Q_INVOKABLE bool init();

    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE bool isDateBusy(const QDate &date) const;
    Q_INVOKABLE void addGame(QDate date, QStringList hometeam, int homeScore, QStringList awayteam, int awayScore);
    Q_INVOKABLE QStringList getSeasons() const;
    void saveGames();

    Q_INVOKABLE QVariantList getPulse();
    int getPulseRosterConsistency();

private:
    void resetPulse();

    QList<Game*> m_games;
    QString m_gamesFilename;

    QVariantList m_pulse;

signals:
    void pulseChanged();
};

#endif // GAMESMODEL_H
