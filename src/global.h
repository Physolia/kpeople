/*
 * Copyright 2013  David Edmundson <davidedmundson@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QStringList>

#include "kpeople_export.h"

namespace KPeople
{
    /**
     * Merge all ids (person IDs and contactIds into a single person)
     * returns the ID that will be created.
     * @arg ids a list of all identifiers to be merged
     *
     * @return the identifier of the new person
     */
    KPEOPLE_EXPORT QString mergeContacts(const QStringList &ids);

    /**
     * Unmerge a contact. Either remove a contact from a given person or remove a person
     *
     * @return Whether the unmerge was successful
     */
    KPEOPLE_EXPORT bool unmergeContact(const QString &id);
};

#endif // GLOBAL_H