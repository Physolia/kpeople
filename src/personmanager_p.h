/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright 2013  David Edmundson <kde@davidedmundson.co.uk>
 * Copyright 2013  Martin Klapetek <mklapetek@kde.org>
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

#ifndef PERSONMANAGER_H
#define PERSONMANAGER_H

#include <QObject>
#include <QStringList>
#include <QMultiHash>

#include <QSqlDatabase>
#include <QSqlQuery>

#include "kpeople_export.h"

/**
 * This is a private internal class that manages all the internal mapping of contacts <---> persons
 * It stores the connection to the database as well as signals communicating with other clients
 *
 * It is a singleton.
 *
 */

class KPEOPLE_EXPORT PersonManager : public QObject
{
    Q_OBJECT

public:
    /**
     * Create or return a singleton instance of the PersonManager
     *
     * @databasePath the path to the database to be used.
     * If null the correct database is used.
     * This is for testing purposes only.
     */
    static PersonManager* instance(const QString &databasePath = QString());

//DATA RETRIEVAL------------

    /** Retuns a list of all known personIDs in the database*/
    QMultiHash< QString /*PersonID*/, QString /*ContactId*/> allPersons() const;

    /**
     * Returns the ID of a person associated with a given contact
     * If no person for that contact exists, an empty string is returned
     */
    QString personIdForContact(const QString &contactId) const;

    /**
     * Returns a list of contactIds associated with a given person
     */
    QStringList contactsForPersonId(const QString &personId) const;


public Q_SLOTS:
    //merge all ids (person IDs and contactIds into a single person)
    //returns the ID that will be created
    QString mergeContacts(const QStringList &ids);

    //unmerge a contact. Either remove a contact from a given person or remove a person
    //
    bool unmergeContact(const QString &id);

Q_SIGNALS:
    void contactRemovedFromPerson(const QString &contactId);
    void contactAddedToPerson(const QString &contactId, const QString &newPersonId);

protected:
    explicit PersonManager(const QString &databasePath, QObject* parent = 0);
    virtual ~PersonManager();

private:
    QSqlDatabase m_db;
};

#endif // PERSONMANAGER_H
