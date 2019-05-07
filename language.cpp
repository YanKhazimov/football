#include "language.h"

const QMap<QString, QMap<QString, QString>> Language::dict = QMap<QString, QMap<QString, QString>>
{
    { "featuredStats", { { "en", "FEATURED STATS" }, { "ru", "ТРЕКЕР" } } },
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
    { "calendarTab", { { "en", "Calendar" }, { "ru", "Календарь" } } },
    { "rewardsTab", { { "en", "Awards" }, { "ru", "Достижения" } } },
    { "dateBusy", { { "en", "Date<br/>busy" }, { "ru", "Есть<br/>игра" } } },
    { "all", { { "en", "All" }, { "ru", "Все" } } },
    { "languageChangeWarning", { { "en", "Changes will apply after the app is restarted" }, { "ru", "Изменения вступят в силу после рестарта приложения" } } },
    { "splitVerb", { { "en", "Split" }, { "ru", "Поделить" } } },
    { "splitNoun", { { "en", "Split" }, { "ru", "Состав" } } },
    { "benchAll", { { "en", "Bench All" }, { "ru", "Сбросить" } } },
    { "saveGame", { { "en", "Save Game" }, { "ru", "Сохранить" } } },
    { "updated", { { "en", "Data updated" }, { "ru", "Данные обновлены" } } },
    { "upToDate", { { "en", "Data up-to-date" }, { "ru", "Нет обновлений" } } },
    { "updateFail", { { "en", "Cannot update data" }, { "ru", "Не удалось обновить данные" } } },
    { "updateData", { { "en", "Update data" }, { "ru", "Обновить данные" } } },
    { "switchRelevance", { { "en", "Relevant/All" }, { "ru", "Полный/актуальный список" } } },
    { "changeSeason", { { "en", "Change season" }, { "ru", "Сменить сезон" } } },
    { "changeLanguage", { { "en", "Change language" }, { "ru", "Сменить язык" } } },
    { "changeTheme", { { "en", "Change pallette" }, { "ru", "Сменить скин" } } }
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
    m_rewardsTab = dict["rewardsTab"][m_lang];
    m_dateBusy = dict["dateBusy"][m_lang];
    m_all = dict["all"][m_lang];
    m_languageChangeWarning = dict["languageChangeWarning"][m_lang];
    m_splitVerb = dict["splitVerb"][m_lang];
    m_splitNoun = dict["splitNoun"][m_lang];
    m_benchAll = dict["benchAll"][m_lang];
    m_saveGame = dict["saveGame"][m_lang];
    m_updated = dict["updated"][m_lang];
    m_upToDate = dict["upToDate"][m_lang];
    m_updateFail = dict["updateFail"][m_lang];
    m_updateFail = dict["updateData"][m_lang];
    m_updateFail = dict["switchRelevance"][m_lang];
    m_updateFail = dict["changeSeason"][m_lang];
    m_updateFail = dict["changeLanguage"][m_lang];
    m_updateFail = dict["changeTheme"][m_lang];

    emit languageChanged(m_lang);
}

void Language::setNext()
{
    set(m_lang == "en" ? "ru" : "en");
}

QString Language::getNext()
{
    return m_lang == "en" ? "ru" : "en";
}

QString Language::get() const
{
    return m_lang;
}

QString Language::getText(QString propertyName)
{
    return dict.value(propertyName).value(m_lang);
}
