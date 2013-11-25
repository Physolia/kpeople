/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright 2013  David Edmundson <d.edmundson@lboro.ac.uk>
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

#include "contactmonitor.h"

ContactMonitor::ContactMonitor(const QString& contactId)
: m_contactId(contactId)
{

}

ContactMonitor::~ContactMonitor()
{

}

void ContactMonitor::setContact(const KABC::Addressee& contact)
{
    m_contact = contact;
    Q_EMIT contactChanged();
}

KABC::Addressee ContactMonitor::contact() const
{
    return m_contact;
}

QString ContactMonitor::contactId() const
{
    return m_contactId;
}


#include "contactmonitor.moc"
