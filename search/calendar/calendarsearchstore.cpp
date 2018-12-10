/*
 * This file is part of the KDE Akonadi Search Project
 * Copyright (C) 2014-2018 Laurent Montel <montel@kde.org>
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
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "calendarsearchstore.h"

using namespace Akonadi::Search;

CalendarSearchStore::CalendarSearchStore(QObject *parent)
    : PIMSearchStore(parent)
{
    m_prefix.insert(QStringLiteral("collection"), QStringLiteral("C"));
    m_prefix.insert(QStringLiteral("organizer"), QStringLiteral("O"));
    m_prefix.insert(QStringLiteral("partstatus"), QStringLiteral("PS"));
    m_prefix.insert(QStringLiteral("summary"), QStringLiteral("S"));
    m_prefix.insert(QStringLiteral("location"), QStringLiteral("L"));

    m_boolWithValue <<  QStringLiteral("partstatus");

    setDbPath(findDatabase(QStringLiteral("calendars")));
}

QStringList CalendarSearchStore::types()
{
    return QStringList() << QStringLiteral("Akonadi") << QStringLiteral("Calendar");
}

