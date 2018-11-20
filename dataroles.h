#ifndef DATAROLES_H
#define DATAROLES_H

#include <QObject>

class DataRoles : public QObject
{
    Q_OBJECT
public:

    enum DataRole
    {
        Id=Qt::UserRole + 1,
        Winners,
        RatingChange,
        DataRoleEnd = Qt::UserRole + 2000
    };
    Q_ENUM(DataRole)
};

#endif // DATAROLES_H
