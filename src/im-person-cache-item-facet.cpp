/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2011  Martin Klapetek <email>

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


#include "im-person-cache-item-facet.h"

#include <KDebug>

#include <Soprano/Vocabulary/NAO>
#include <Nepomuk/Vocabulary/NCO>

#include "person-cache-item_p.h"
#include <ontologies/telepathy.h>

/****************************** IMPersonCacheItemFacet::Private ********************************/

class IMPersonCacheItemFacetPrivate : public PersonCacheItemPrivate {

// protected:
//     Q_DECLARE_PUBLIC(IMPersonCacheItemFacet);
//     IMPersonCacheItemFacet * const q_ptr;
//
// public:
//     explicit IMPersonCacheItemFacetPrivate(IMPersonCacheItemFacet *q)
//     : q_ptr(q)
//     { }
//
//     virtual ~IMPersonCacheItemFacetPrivate()
//     { }

};


/****************************** IMPersonCacheItemFacet *****************************************/

IMPersonCacheItemFacet::IMPersonCacheItemFacet(const QUrl &uri)
    : PersonCacheItem(uri)
// : d_ptr(new IMPersonCacheItemFacetPrivate(this))
{
    kDebug();
}

IMPersonCacheItemFacet::~IMPersonCacheItemFacet()
{
    kDebug();

//     delete d_ptr;
}

QString IMPersonCacheItemFacet::label() const
{
    Q_D(const IMPersonCacheItemFacet);
    return d->data.value(Soprano::Vocabulary::NAO::prefLabel());
}

QString IMPersonCacheItemFacet::avatarPath() const
{
    Q_D(const IMPersonCacheItemFacet);
    return d->data.value(Soprano::Vocabulary::NAO::prefSymbol());
}

QString IMPersonCacheItemFacet::imNickname() const
{
    Q_D(const IMPersonCacheItemFacet);
    return d->data.value(Nepomuk::Vocabulary::NCO::imNickname());
}

QString IMPersonCacheItemFacet::imAccountType() const
{
    Q_D(const IMPersonCacheItemFacet);
    return d->data.value(Nepomuk::Vocabulary::NCO::imAccountType());
}

int IMPersonCacheItemFacet::imStatusType() const
{
    Q_D(const IMPersonCacheItemFacet);
    return d->data.value(Nepomuk::Vocabulary::Telepathy::statusType()).toInt();
}

QString IMPersonCacheItemFacet::accountId() const
{
    Q_D(const IMPersonCacheItemFacet);
    return d->data.value(Nepomuk::Vocabulary::Telepathy::accountIdentifier());
}

QString IMPersonCacheItemFacet::contactId() const
{
    Q_D(const IMPersonCacheItemFacet);
    return d->data.value(Nepomuk::Vocabulary::NCO::imID());
}