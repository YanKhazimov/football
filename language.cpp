#include "language.h"

const QMap<QString, QMap<QString, QString>> Language::dict = QMap<QString, QMap<QString, QString>>
{
    { "featuredStats", { { "en", "FEATURED STATS" }, { "ru", "ТРЕКЕР" } } },
    { "rivalriesToWatch", { { "en", "RIVALRIES TO WATCH" }, { "ru", "НОС К НОСУ" } } },
    { "closestRatedPlayers", { { "en", "Closest-rated players" }, { "ru", "Ближайшие по рейтингу" } } },
    { "strongestSynergy", { { "en", "STRONGEST SYNERGY" }, { "ru", "КРЕПКАЯ СВЯЗКА" } } },
    { "highestWLRatioTogether", { { "en", "Highest W/L ratio together" }, { "ru", "Лучший % побед вместе" } } },
    { "onFire", { { "en", "ON FIRE" }, { "ru", "НА ХОДУ" } } },
    { "longestActiveWinStreaks", { { "en", "Longest active win streaks" }, { "ru", "Текущая серия побед" } } },
    { "ratingColumn", { { "en", "Rating" }, { "ru", "Рейтинг" } } },
    { "playerColumn", { { "en", "Player" }, { "ru", "Игрок" } } },
    { "wdlColumn", { { "en", "W-D-L" }, { "ru", "В-Н-П" } } },
    { "progressColumn", { { "en", "Progress" }, { "ru", "Прогресс" } } },
    { "relevanceColumn", { { "en", "Relevance" }, { "ru", "Актуальность" } } },
    { "dedicationColumn", { { "en", "Dedication" }, { "ru", "Надежность" } } },
    { "home", { { "en", "Home" }, { "ru", "Дома" } } },
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
    { "changeTheme", { { "en", "Change pallette" }, { "ru", "Сменить скин" } } },
    { "gold", { { "en", "gold" }, { "ru", "золото" } } },
    { "silver", { { "en", "silver" }, { "ru", "серебро" } } },
    { "bronze", { { "en", "bronze" }, { "ru", "бронза" } } },
    { "pulseStats", { { "en", "Last month: %1 games, %2% core roster" }, { "ru", "За месяц: %1 игры, %2% костяк" } } },
    { "statsInfo", {
            { "en",
                "<li>- <b>Rating</b>: at the end of currently selected season(s)</li>" \
                "<li>- <b>W-D-L</b>: amount of wins-draws-losses during currently selected season(s)</li>" \
                "<li>- <b>Progress</b>: rating difference between the end and the start of currently selected season(s)</li>" \
                "<li>- <b>Relevance</b>: how much all the other stats reflect player's current state judging by the last 10 games (more recent games matter more)</li>" \
                "<li>- <b>Dedication</b>: % of the games attended during the period since the beginning of currently selected season(s) (or player's first ever appearance) and until the end of currently selected season(s)</li>"
            },
            { "ru",
                "<li>- <b>Рейтинг</b>: на конец выбранного периода</li>" \
                "<li>- <b>В-Н-П</b>: количество побед-ничьих-поражений за выбранный период</li>" \
                "<li>- <b>Прогресс</b>: разница в рейтинге, показанная игроком за выбранный период</li>" \
                "<li>- <b>Актуальность</b>: насколько остальные показатели отражают текущую форму игрока (расчет по последним 10 играм, более поздние игры влияют больше)</li>" \
                "<li>- <b>Надежность</b>: % сыгранных игроком игр с начала выбранного периода (или самой первой его игры) до конца выбранного периода</li>"
            }
        } },
    { "pitchInfo", {
            { "en",
                "<li>- Drag players from the bench area to the central area to prepare them for team assignment</li>" \
                "<li>- Once everyone is in the cenral area, keep clicking the <b>Split</b> button on the left until you find the best split</li>" \
                "<li>- To ensure particular players are assigned the same team, drag them to either team zone before splitting the rest</li>"
            },
            {
                "ru",
                "<li>- Перетащите игроков из зоны запасных в центр, чтобы подготовить их к автораспределению по командам</li>" \
                "<li>- Для автораспределения нажимайте <b>Поделить</b>, пока не увидите подходящий вариант составов</li>" \
                "<li>- Чтобы определить кого-то в одну команду, перетащите их на половину поля команды перед автораспределением остальных</li>"
            }
        } }
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
    m_updateData = dict["updateData"][m_lang];
    m_switchRelevance = dict["switchRelevance"][m_lang];
    m_changeSeason = dict["changeSeason"][m_lang];
    m_changeLanguage = dict["changeLanguage"][m_lang];
    m_changeTheme = dict["changeTheme"][m_lang];
    m_gold = dict["gold"][m_lang];
    m_silver = dict["silver"][m_lang];
    m_bronze = dict["bronze"][m_lang];
    m_pulseStats = dict["pulseStats"][m_lang];
    m_statsInfo = dict["statsInfo"][m_lang];
    m_pitchInfo = dict["pitchInfo"][m_lang];

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
