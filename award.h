#ifndef AWARD_H
#define AWARD_H

#include <QString>
#include <QObject>
#include <QVariant>

#include "player.h"

class Award : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString category MEMBER m_category CONSTANT)
    Q_PROPERTY(QString rank MEMBER m_rank CONSTANT)
    Q_PROPERTY(int season MEMBER m_season CONSTANT)
    Q_PROPERTY(Player* player MEMBER m_player CONSTANT)
    Q_PROPERTY(QString score MEMBER m_score CONSTANT)

    int m_season;
    QString m_category;
    QString m_rank;
    Player* m_player;
    QString m_score;

public:
    Award() = default;
    Award(const Award& award);
    Award(int season, const QString& category, const QString& rank, Player* player, const QString& score, QObject* parent = nullptr);
    bool operator ==(const Award& award);
    bool operator !=(const Award& award);

    Q_INVOKABLE QString getImageSource();

    int getSeason();
    QString getRank();
    QString getCategory();
    QString getScore();
    Player* getPlayer();
};

class Awards : public QObject
{
    Q_OBJECT

    QList<Award*> m_list;

public:
    Awards() = default;
    Awards &operator =(const Awards& awards);
    ~Awards();

    void add(const Award& award);
    void clear();

    QVariant filter(const QStringList& rankFilter, const QList<int>& seasonFilter,
                    const QStringList& categoryFilter, const QStringList& playerRefFilter);
};

#endif // AWARD_H
