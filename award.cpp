#include "award.h"

Award::Award(const Award &award)
    : m_season(award.m_season), m_category(award.m_category), m_rank(award.m_rank), m_player(award.m_player), m_score(award.m_score)
{
}

Award::Award(int season, const QString& category, const QString& rank, Player *player, const QString &score, QObject *parent)
    : QObject(parent), m_season(season), m_category(category), m_rank(rank), m_player(player), m_score(score)
{
}

bool Award::operator ==(const Award &award)
{
    return m_season == award.m_season &&
            m_category == award.m_category &&
            m_rank == award.m_rank &&
            *m_player == *award.m_player &&
            m_score == award.m_score;
}

bool Award::operator !=(const Award &award)
{
    return !(*this == award);
}

QString Award::getDescription()
{
    return QString("%1 %2 - %3").arg(m_season).arg(m_category).arg(m_rank.toUpper());
}

QString Award::getImageSource()
{
    return QString("qrc:/img/medal_%1_%2.png").arg(m_category.toLower()).arg(m_rank.toLower());
}

int Award::getSeason()
{
    return m_season;
}

QString Award::getRank()
{
    return m_rank.toLower();
}

QString Award::getCategory()
{
    return m_category;
}

Player *Award::getPlayer()
{
    return m_player;
}

Awards &Awards::operator =(const Awards &awards)
{
    m_list = awards.m_list;

    return *this;
}

Awards::~Awards()
{
    for (Award* award: m_list)
        delete award;
}

void Awards::add(const Award& award)
{
    m_list.append(new Award(award));
}

void Awards::clear()
{
    for (Award* award: m_list)
        delete award;

    m_list.clear();
}

QVariant Awards::filter(const QStringList& rankFilter, const QList<int>& seasonFilter,
                        const QStringList& categoryFilter, const QStringList& playerRefFilter)
{
    QObjectList filteredAwards;

    for (Award* award: m_list)
    {
        bool categoryFits = categoryFilter.empty() || categoryFilter.contains(award->getCategory());
        bool rankFits = rankFilter.empty() || rankFilter.contains(award->getRank());
        bool seasonFits = seasonFilter.empty() || seasonFilter.contains(award->getSeason());
        bool playerFits = playerRefFilter.empty() || playerRefFilter.contains(award->getPlayer()->getName());

        if (categoryFits && rankFits && seasonFits && playerFits)
            filteredAwards.append(award);
    }

    return QVariant::fromValue(filteredAwards);
}
