#include "config.h"
#include <fstream>
#include <QFile>
#include <QTextStream>

bool Config::load(const QString &filename)
{
    std::ifstream input(filename.toUtf8().constData());
    if (!input.is_open())
        return false;

    m_filename = filename;
    std::string line;
    while (getline(input, line))
    {
        size_t pos = line.find_first_of('=');
        if (pos != std::string::npos)
        {
            m_config[QString::fromStdString(line.substr(0, pos))] = QString::fromStdString(line.substr(pos + 1));
        }
    }
    input.close();

    return true;
}

QString Config::value(const QString &key)
{
    return m_config.value(key);
}

void Config::set(const QString &key, const QString &value)
{
    m_config[key] = value;

    save();
}

bool Config::save()
{
    QFile file(m_filename);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream stream(&file);

    for (const QString& key: m_config.keys())
        stream << key << '=' << m_config[key] << '\n';

    file.close();

    return true;
}
