/*
    Copyright (C) 2013  David Edmundson <davidedmundson@kde.org>

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

#ifndef ABSTRACT_PERSON_ACTION_H
#define ABSTRACT_PERSON_ACTION_H

class PersonData;

#include <QObject>
#include <QAction>

#include <kpeople/kpeople_export.h>
#include <KPeople/PersonData>

namespace KPeople
{

class KPEOPLE_EXPORT AbstractPersonAction : public QObject
{
    Q_OBJECT
public:
    AbstractPersonAction(QObject *parent);
    /** A list of actions that can be executed for a contact
     * Disabled actions should not be added
     *
     * @arg person the person AbstractContact::Ptr object
     * @arg contacts person's list of subcontacts
     * @arg parent parent object for newly created actions
     */
    virtual QList<QAction*> actionsForPerson(const PersonData &data, QObject *parent) const = 0;
};

}

#endif