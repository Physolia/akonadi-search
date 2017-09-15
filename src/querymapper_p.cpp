/*
 * Copyright (C) 2013  Vishesh Handa <me@vhanda.in>
 * Copyright (C) 2017  Daniel Vrátil <dvratil@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <xapian.h>

#include "querymapper_p.h"
#include "querypropertymapper_p.h"

#include <QVariant>
#include <QByteArray>

namespace Akonadi {
namespace Search {

Xapian::Query::op mapRelation(SearchTerm::Relation rel)
{
    switch (rel) {
    case SearchTerm::RelAnd: return Xapian::Query::OP_AND;
    case SearchTerm::RelOr: return Xapian::Query::OP_OR;
    }
    Q_UNREACHABLE();
}

Xapian::Query negateQuery(const Xapian::Query &query, bool negate)
{
    if (negate) {
        return Xapian::Query(Xapian::Query::OP_AND_NOT, Xapian::Query::MatchAll, query);
    } else {
        return query;
    }
}

Xapian::Query constructQuery(const QueryPropertyMapper &mapper,
                             const QString &property, const QVariant &value,
                             SearchTerm::Condition cond)
{
    if (value.isNull()) {
        return {};
    }

    const auto prop = property.toLower();
    if (mapper.hasBoolProperty(prop)) {
        const auto p = mapper.prefix(prop);
        if (p.empty()) {
            return {};
        }

        std::string term("B");
        bool isTrue = false;

        if (value.isNull()) {
            isTrue = true;
        }

        if (value.type() == QVariant::Bool) {
            isTrue = value.toBool();
        }

        if (isTrue) {
            term += p;
        } else {
            term += 'N' + p;
        }

        return Xapian::Query(term);
    } else if (mapper.hasBoolValueProperty(prop)) {
        const auto term = mapper.prefix(prop);
        std::string val(value.toString().toUtf8().constData());
        return Xapian::Query(term + val);
    } else if (mapper.hasValueProperty(prop)
            && (cond == SearchTerm::CondEqual
                || cond == SearchTerm::CondGreaterThan || cond == SearchTerm::CondGreaterOrEqual 
                || cond == SearchTerm::CondLessThan || cond == SearchTerm::CondLessOrEqual)) {
        auto numVal = value.toLongLong();
        if (cond == SearchTerm::CondGreaterThan) {
            ++numVal;
        }
        if (cond == SearchTerm::CondLessThan) {
            --numVal;
        }
        const int valueNumber = mapper.valueProperty(prop);
        if (cond == SearchTerm::CondGreaterOrEqual || cond == SearchTerm::CondGreaterThan) {
            return Xapian::Query(Xapian::Query::OP_VALUE_GE, valueNumber, std::to_string(numVal));
        } else if (cond == SearchTerm::CondLessOrEqual || cond == SearchTerm::CondLessThan) {
            return Xapian::Query(Xapian::Query::OP_VALUE_LE, valueNumber, std::to_string(numVal));
        } else if (cond == SearchTerm::CondEqual) {
            const Xapian::Query gtQuery(Xapian::Query::OP_VALUE_GE, valueNumber, std::to_string(numVal));
            const Xapian::Query ltQuery(Xapian::Query::OP_VALUE_LE, valueNumber, std::to_string(numVal));
            return Xapian::Query(Xapian::Query::OP_AND, gtQuery, ltQuery);
        }
    } else if ((cond == SearchTerm::CondContains || cond == SearchTerm::CondEqual)
                && mapper.hasPrefix(prop)) {
        Xapian::QueryParser parser;
        std::string p = mapper.prefix(prop);
        std::string str(value.toString().toUtf8().constData());
        int flags = Xapian::QueryParser::FLAG_DEFAULT;
        if (cond == SearchTerm::CondContains) {
            flags |= Xapian::QueryParser::FLAG_PARTIAL;
        }
        return parser.parse_query(str, flags, p);
    }

    return {};
}

Xapian::Query constructQuery(const QueryPropertyMapper &mapper,
                             const QString &property,
                             const SearchTerm &term)
{
    return negateQuery(
                constructQuery(mapper, property, term.value(), term.condition()), 
                term.isNegated());
}

Xapian::Query constructQuery(const QueryPropertyMapper &mapper,
                             const QString &property,
                             bool val)
{
    return constructQuery(mapper, property, val, SearchTerm::CondEqual);
}

}
}
