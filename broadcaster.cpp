#include "broadcaster.h"

Broadcaster::Broadcaster(QObject *parent) : QObject(parent)
{
}

void Broadcaster::inform(QStringList receivers)
{
    emit broadcast(receivers);
}
