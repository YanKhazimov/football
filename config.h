#ifndef CONFIG_H
#define CONFIG_H

#include <QMap>
#include <QObject>

class Config : public QObject
{
    Q_OBJECT

public:
    bool load(const QString& filename);
    Q_INVOKABLE QString value(const QString& key);
    Q_INVOKABLE void set(const QString& key, const QString &value);

private:
    QMap<QString, QString> m_config;
    QString m_filename;

    bool save();
};

#endif // CONFIG_H
