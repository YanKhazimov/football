#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QObject>
#include <QMap>

class Language: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString lang READ get NOTIFY languageChanged)

    Q_PROPERTY(QString featuredStats MEMBER m_featuredStats NOTIFY languageChanged)
    Q_PROPERTY(QString ratingColumn MEMBER m_ratingColumn NOTIFY languageChanged)
    Q_PROPERTY(QString playerColumn MEMBER m_playerColumn NOTIFY languageChanged)
    Q_PROPERTY(QString wdlColumn MEMBER m_wdlColumn NOTIFY languageChanged)
    Q_PROPERTY(QString progressColumn MEMBER m_progressColumn NOTIFY languageChanged)
    Q_PROPERTY(QString relevanceColumn MEMBER m_relevanceColumn NOTIFY languageChanged)
    Q_PROPERTY(QString dedicationColumn MEMBER m_dedicationColumn NOTIFY languageChanged)
    Q_PROPERTY(QString home MEMBER m_home NOTIFY languageChanged)
    Q_PROPERTY(QString away MEMBER m_away NOTIFY languageChanged)
    Q_PROPERTY(QString playersTab MEMBER m_playersTab NOTIFY languageChanged)
    Q_PROPERTY(QString nextGameTab MEMBER m_nextGameTab NOTIFY languageChanged)
    Q_PROPERTY(QString calendarTab MEMBER m_calendarTab NOTIFY languageChanged)
    Q_PROPERTY(QString dateBusy MEMBER m_dateBusy NOTIFY languageChanged)
    Q_PROPERTY(QString all MEMBER m_all NOTIFY languageChanged)
    Q_PROPERTY(QString languageChangeWarning MEMBER m_languageChangeWarning NOTIFY languageChanged)
    Q_PROPERTY(QString splitNoun MEMBER m_splitNoun NOTIFY languageChanged)
    Q_PROPERTY(QString splitVerb MEMBER m_splitVerb NOTIFY languageChanged)
    Q_PROPERTY(QString benchAll MEMBER m_benchAll NOTIFY languageChanged)
    Q_PROPERTY(QString saveGame MEMBER m_saveGame NOTIFY languageChanged)

    QString m_featuredStats;
    QString m_ratingColumn;
    QString m_playerColumn;
    QString m_wdlColumn;
    QString m_progressColumn;
    QString m_relevanceColumn;
    QString m_dedicationColumn;
    QString m_home;
    QString m_away;
    QString m_playersTab;
    QString m_nextGameTab;
    QString m_calendarTab;
    QString m_dateBusy;
    QString m_all;
    QString m_languageChangeWarning;
    QString m_splitNoun;
    QString m_splitVerb;
    QString m_benchAll;
    QString m_saveGame;

    QString m_lang = "ru";

public:
    Q_INVOKABLE void set(QString str);
    Q_INVOKABLE void setNext();
    Q_INVOKABLE QString getNext();
    QString get() const;

    static const QMap<QString, QMap<QString, QString>> dict;

signals:
    void languageChanged(QString str);
};

#endif // LANGUAGE_H
