#ifndef DATAROLES_H
#define DATAROLES_H

#include <QObject>

class DataRoles : public QObject
{
    Q_OBJECT
public:

    enum DataRole
    {
        Id=Qt::UserRole + 1,    //257
        GameDate,
        Hometeam,
        Awayteam,
        ScoreDiff,
        RatingHistory,
        PlayerName,             //263
        Player,
        Rating,
        WinsLosses,
        Progress,
        Dedication,
        Relevance,
        StatCategory,           //270
        StatValue,
        SourceRole,
        PlayerSelection,        //273
        FeaturedStatName,       //274
        FeaturedStatDescription,
        FeaturedStatQueryResult,
        Synergy,
        DataRoleEnd = Qt::UserRole + 2000
    };
    Q_ENUM(DataRole)
};

#endif // DATAROLES_H
