/*
 * This file is part of the KDE Akonadi Search Project
 * Copyright (C) 2013  Vishesh Handa <me@vhanda.in>
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

#include "query.h"
#include "contactquery.h"
#include <qjsondocument.h>

#include <QVariant>
#include <QDebug>

#include <QJsonDocument>

using namespace Akonadi::Search::PIM;

Query::Query()
{

}

Query::~Query()
{

}

Query *Query::fromJSON(const QByteArray &json)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);
    if (doc.isNull()) {
        qWarning() << "Could not parse json query" << error.errorString();
        return 0;
    }

    QVariantMap result = doc.toVariant().toMap();
    const QString type = result[QStringLiteral("type")].toString().toLower();
    if (type != QLatin1String("contact")) {
        qWarning() << "Can only handle contact queries";
        return 0;
    }

    ContactQuery *cq = new ContactQuery();
    cq->matchName(result[QStringLiteral("name")].toString());
    cq->matchNickname(result[QStringLiteral("nick")].toString());
    cq->matchEmail(result[QStringLiteral("email")].toString());
    cq->matchUID(result[QStringLiteral("uid")].toString());
    cq->match(result[QStringLiteral("$")].toString());

    const QString criteria = result[QStringLiteral("matchCriteria")].toString().toLower();
    if (criteria == QLatin1String("exact")) {
        cq->setMatchCriteria(ContactQuery::ExactMatch);
    } else if (criteria == QLatin1String("startswith")) {
        cq->setMatchCriteria(ContactQuery::StartsWithMatch);
    }

    cq->setLimit(result[QStringLiteral("limit")].toInt());

    return cq;
}
