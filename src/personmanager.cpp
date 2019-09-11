/*
    Copyright (C) 2013  David Edmundson <davidedmundson@kde.org>
    Copyright 2013  Martin Klapetek <mklapetek@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "personmanager_p.h"

#include <QVariant>
#include "kpeople_debug.h"
#include <QStandardPaths>
#include <QDir>
#include <QSqlQuery>

#ifndef Q_OS_ANDROID
#include <QDBusConnection>
#include <QDBusMessage>
#endif

class Transaction
{
public:
    Transaction(const QSqlDatabase &db);
    void cancel();
    ~Transaction();
    Transaction(const Transaction &) = delete;
    Transaction& operator=(const Transaction &) = delete;
private:
    QSqlDatabase m_db;
    bool m_cancelled = false;
};

Transaction::Transaction(const QSqlDatabase &db) :
    m_db(db),
    m_cancelled(false)
{
    m_db.transaction();
}

void Transaction::cancel()
{
    m_db.rollback();
    m_cancelled = true;
}

Transaction::~Transaction()
{
    if (!m_cancelled) {
        m_db.commit();
    }
}

PersonManager::PersonManager(const QString &databasePath, QObject *parent):
    QObject(parent),
    m_db(QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QStringLiteral("kpeoplePersonsManager")))
{
    m_db.setDatabaseName(databasePath);
    if (!m_db.open()) {
        qCWarning(KPEOPLE_LOG) << "Couldn't open the database at" << databasePath;
    }
    m_db.exec(QStringLiteral("CREATE TABLE IF NOT EXISTS persons (contactID VARCHAR UNIQUE NOT NULL, personID INT NOT NULL)"));
    m_db.exec(QStringLiteral("CREATE INDEX IF NOT EXISTS contactIdIndex ON persons (contactId)"));
    m_db.exec(QStringLiteral("CREATE INDEX IF NOT EXISTS personIdIndex ON persons (personId)"));

#ifndef Q_OS_ANDROID
    QDBusConnection::sessionBus().connect(QString(), QStringLiteral("/KPeople"), QStringLiteral("org.kde.KPeople"),
                                          QStringLiteral("ContactAddedToPerson"), this, SIGNAL(contactAddedToPerson(QString,QString)));
    QDBusConnection::sessionBus().connect(QString(), QStringLiteral("/KPeople"), QStringLiteral("org.kde.KPeople"),
                                          QStringLiteral("ContactRemovedFromPerson"), this, SIGNAL(contactRemovedFromPerson(QString)));
#endif
}

PersonManager::~PersonManager()
{

}

QMultiHash< QString, QString > PersonManager::allPersons() const
{
    QMultiHash<QString /*PersonID*/, QString /*ContactID*/> contactMapping;

    QSqlQuery query = m_db.exec(QStringLiteral("SELECT personID, contactID FROM persons"));
    while (query.next()) {
        const QString personUri = QLatin1String("kpeople://") + query.value(0).toString(); // we store as ints internally, convert it to a string here
        const QString contactID = query.value(1).toString();
        contactMapping.insertMulti(personUri, contactID);
    }
    return contactMapping;
}

QStringList PersonManager::contactsForPersonUri(const QString &personUri) const
{
    if (!personUri.startsWith(QLatin1String("kpeople://"))) {
        return QStringList();
    }

    QStringList contactUris;
    //TODO port to the proper qsql method for args
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral("SELECT contactID FROM persons WHERE personId = ?"));
    query.bindValue(0, personUri.mid(strlen("kpeople://")));
    query.exec();

    while (query.next()) {
        contactUris << query.value(0).toString();
    }
    return contactUris;
}

QString PersonManager::personUriForContact(const QString &contactUri) const
{
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral("SELECT personId FROM persons WHERE contactId = ?"));
    query.bindValue(0, contactUri);
    query.exec();
    if (query.next()) {
        return QLatin1String("kpeople://") + query.value(0).toString();
    }
    return QString();
}

QString PersonManager::mergeContacts(const QStringList &ids)
{
    // no merging if we have only 0 || 1 ids
    if (ids.size() < 2) {
        return QString();
    }

    QStringList metacontacts;
    QStringList contacts;

    bool rc = true;

#ifndef Q_OS_ANDROID
    QList<QDBusMessage> pendingMessages;
#endif

    // separate the passed ids to metacontacts and simple contacts
    for (const QString &id : ids) {
        if (id.startsWith(QLatin1String("kpeople://"))) {
            metacontacts << id;
        } else {
            contacts << id;
        }
    }

    // create new personUriString
    //   - if we're merging two simple contacts, create completely new id
    //   - if we're merging an existing metacontact, take the first id and use it
    QString personUriString;
    if (metacontacts.isEmpty()) {
        // query for the highest existing ID in the database and +1 it
        int personUri = 0;
        QSqlQuery query = m_db.exec(QStringLiteral("SELECT MAX(personID) FROM persons"));
        if (query.next()) {
            personUri = query.value(0).toInt();
            personUri++;
        }

        personUriString = QLatin1String("kpeople://") + QString::number(personUri);
    } else {
        personUriString = metacontacts.first();
    }

    // start a db transaction (ends automatically on destruction)
    Transaction t(m_db);

    // processed passed metacontacts
    if (metacontacts.count() > 1) {
        // collect all the contacts from other persons
        QStringList personContacts;
        for (const QString &id : qAsConst(metacontacts)) {
            if (id != personUriString) {
                personContacts << contactsForPersonUri(id);
            }
        }

        // iterate over all of the contacts and change their personID to the new personUriString
        for (const QString &id : qAsConst(personContacts)) {
            QSqlQuery updateQuery(m_db);
            updateQuery.prepare(QStringLiteral("UPDATE persons SET personID = ? WHERE contactID = ?"));
            updateQuery.bindValue(0, personUriString.mid(strlen("kpeople://")));
            updateQuery.bindValue(1, id);
            if (!updateQuery.exec()) {
                rc = false;
            }

#ifndef Q_OS_ANDROID
            QDBusMessage message = QDBusMessage::createSignal(QStringLiteral("/KPeople"),
                                   QStringLiteral("org.kde.KPeople"),
                                   QStringLiteral("ContactRemovedFromPerson"));

            message.setArguments(QVariantList() << id);
            pendingMessages << message;

            message = QDBusMessage::createSignal(QStringLiteral("/KPeople"),
                                                 QStringLiteral("org.kde.KPeople"),
                                                 QStringLiteral("ContactAddedToPerson"));

            message.setArguments(QVariantList() << id << personUriString);
            pendingMessages << message;
#endif
        }
    }

    // process passed contacts
    if (!contacts.isEmpty()) {

        for (const QString &id : qAsConst(contacts)) {
            QSqlQuery insertQuery(m_db);
            insertQuery.prepare(QStringLiteral("INSERT INTO persons VALUES (?, ?)"));
            insertQuery.bindValue(0, id);
            insertQuery.bindValue(1, personUriString.mid(strlen("kpeople://"))); //strip kpeople://
            if (!insertQuery.exec()) {
                rc = false;
            }

#ifndef Q_OS_ANDROID
            //FUTURE OPTIMIZATION - this would be best as one signal, but arguments become complex
            QDBusMessage message = QDBusMessage::createSignal(QStringLiteral("/KPeople"),
                                   QStringLiteral("org.kde.KPeople"),
                                   QStringLiteral("ContactAddedToPerson"));

            message.setArguments(QVariantList() << id << personUriString);
            pendingMessages << message;
#endif
        }
    }

    //if success send all messages to other clients
    //otherwise roll back our database changes and return an empty string
    if (rc) {
#ifndef Q_OS_ANDROID
        for (const QDBusMessage &message : qAsConst(pendingMessages)) {
            QDBusConnection::sessionBus().send(message);
        }
#endif
    } else {
        t.cancel();
        personUriString.clear();
    }

    return personUriString;
}

bool PersonManager::unmergeContact(const QString &id)
{
    //remove rows from DB
    if (id.startsWith(QLatin1String("kpeople://"))) {
        QSqlQuery query(m_db);

        const QStringList contactUris = contactsForPersonUri(id);
        query.prepare(QStringLiteral("DELETE FROM persons WHERE personId = ?"));
        query.bindValue(0, id.mid(strlen("kpeople://")));
        query.exec();

#ifndef Q_OS_ANDROID
        for (const QString &contactUri : contactUris) {
            //FUTURE OPTIMIZATION - this would be best as one signal, but arguments become complex
            QDBusMessage message = QDBusMessage::createSignal(QStringLiteral("/KPeople"),
                                   QStringLiteral("org.kde.KPeople"),
                                   QStringLiteral("ContactRemovedFromPerson"));

            message.setArguments(QVariantList() << contactUri);
            QDBusConnection::sessionBus().send(message);
        }
#endif
    } else {
        QSqlQuery query(m_db);
        query.prepare(QStringLiteral("DELETE FROM persons WHERE contactId = ?"));
        query.bindValue(0, id);
        query.exec();
        //emit signal(dbus)
        Q_EMIT contactRemovedFromPerson(id);
    }

    //TODO return if removing rows worked
    return true;
}

PersonManager *PersonManager::instance(const QString &databasePath)
{
    static PersonManager *s_instance = nullptr;
    if (!s_instance) {
        QString path = databasePath;
        if (path.isEmpty()) {
            path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QStringLiteral("/kpeople/");

            QDir().mkpath(path);
            path += QLatin1String("persondb");
        }
        s_instance = new PersonManager(path);
    }
    return s_instance;
}
