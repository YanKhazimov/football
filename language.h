#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QObject>

class Language: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString featured_stats MEMBER m_featured_stats NOTIFY languageChanged)

    QString m_featured_stats;

    QString m_lang;

public:
    Q_INVOKABLE void set(QString str);

signals:
    void languageChanged(QString str);
};

#endif // LANGUAGE_H
