#ifndef BROADCASTER_H
#define BROADCASTER_H

#include <QObject>

class Broadcaster : public QObject
{
    Q_OBJECT
public:
    explicit Broadcaster(QObject *parent = nullptr);

    Q_INVOKABLE void inform(QStringList receivers);

signals:
    void broadcast(QStringList receivers);
};

#endif // BROADCASTER_H
