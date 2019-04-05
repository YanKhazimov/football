#include "language.h"

void Language::set(QString str)
{
    m_lang = str;

    if (str == "en")
    {
        m_featured_stats = "FEATURED STATS";
        m_ratingColumn = "Rating";
    }
    else if (str == "ru")
    {
        m_featured_stats = "СТАТЫ";
        m_ratingColumn = "Рейтинг";
    }
    emit languageChanged(str);
}
