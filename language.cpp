#include "language.h"

const QMap<QString, QMap<QString, QString>> Language::dict = QMap<QString, QMap<QString, QString>>
{
    { "featuredStats", { { "en", "FEATURED STATS" }, { "ru", "ВЫБОР РЕДАКЦИИ" } } },
    { "ratingColumn", { { "en", "Rating" }, { "ru", "Рейтинг" } } },
    { "playerColumn", { { "en", "Player" }, { "ru", "Игрок" } } },
    { "wdlColumn", { { "en", "W-D-L" }, { "ru", "В-Н-П" } } },
    { "progressColumn", { { "en", "Progress" }, { "ru", "Прогресс" } } },
    { "relevanceColumn", { { "en", "Relevance" }, { "ru", "Актуальность" } } },
    { "dedicationColumn", { { "en", "Dedication" }, { "ru", "Надежность" } } },
    { "home", { { "en", "Home" }, { "ru", "Хозяева" } } },
    { "away", { { "en", "Away" }, { "ru", "Гости" } } },
    { "playersTab", { { "en", "Players" }, { "ru", "Игроки" } } },
    { "nextGameTab", { { "en", "Next Game" }, { "ru", "След. игра" } } },
    { "calendarTab", { { "en", "Calendar" }, { "ru", "Календарь" } } }
};

void Language::set(QString str)
{
    //m_lang = str;

    m_featuredStats = dict["featuredStats"][str];
    m_ratingColumn = dict["ratingColumn"][str];
    m_playerColumn = dict["playerColumn"][str];
    m_wdlColumn = dict["wdlColumn"][str];
    m_progressColumn = dict["progressColumn"][str];
    m_relevanceColumn = dict["relevanceColumn"][str];
    m_dedicationColumn = dict["dedicationColumn"][str];
    m_home = dict["home"][str];
    m_away = dict["away"][str];
    m_playersTab = dict["playersTab"][str];
    m_nextGameTab = dict["nextGameTab"][str];
    m_calendarTab = dict["calendarTab"][str];

    emit languageChanged(str);
}
