#include "language.h"

void Language::set(QString str)
{
    m_lang = str;

    if (str == "en")
    {
        m_featured_stats = "FEATURED STATS";
    }
    else if (str == "ru")
    {
        m_featured_stats = "СТАТЫ";
    }
    emit languageChanged(str);
}
