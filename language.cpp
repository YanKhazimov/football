#include "language.h"

const QMap<QString, QMap<QString, QString>> Language::dict = QMap<QString, QMap<QString, QString>>
{
    { "featuredStats", { { "en", "FEATURED STATS" }, { "ru", "ВЫБОР РЕДАКЦИИ" } } },
    { "rivalriesToWatch", { { "en", "RIVALRIES TO WATCH" }, { "ru", "НОС К НОСУ" } } },
    { "closestRatedPlayers", { { "en", "Closest-rated players" }, { "ru", "Ближайшие по рейтингу" } } },
    { "strongestSynergy", { { "en", "STRONGEST SYNERGY" }, { "ru", "КРЕПКАЯ СВЯЗКА" } } },
    { "highestWLRatioTogether", { { "en", "Highest W/L ratio together" }, { "ru", "Лучший % побед вместе" } } },
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
    m_lang = str;

    m_featuredStats = dict["featuredStats"][m_lang];
    m_ratingColumn = dict["ratingColumn"][m_lang];
    m_playerColumn = dict["playerColumn"][m_lang];
    m_wdlColumn = dict["wdlColumn"][m_lang];
    m_progressColumn = dict["progressColumn"][m_lang];
    m_relevanceColumn = dict["relevanceColumn"][m_lang];
    m_dedicationColumn = dict["dedicationColumn"][m_lang];
    m_home = dict["home"][m_lang];
    m_away = dict["away"][m_lang];
    m_playersTab = dict["playersTab"][m_lang];
    m_nextGameTab = dict["nextGameTab"][m_lang];
    m_calendarTab = dict["calendarTab"][m_lang];

    emit languageChanged(m_lang);
}

QString Language::get() const
{
    return m_lang;
}
