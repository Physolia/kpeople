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


#include "resource-watcher-service.h"

#include <nepomuk/resourcewatcher.h>
#include <Nepomuk/Vocabulary/PIMO>

class ResourceWatcherServicePrivate {

public:
    ResourceWatcherServicePrivate()
        : watcher(0)
    { }

    Nepomuk::ResourceWatcher *watcher;

};

ResourceWatcherService::ResourceWatcherService(QObject *parent)
    : QObject(parent),
      d_ptr(new ResourceWatcherServicePrivate())
{
    d_ptr->watcher = new Nepomuk::ResourceWatcher(this);

    d_ptr->watcher->addType(Nepomuk::Vocabulary::PIMO::Person());

    connect(d_ptr->watcher, SIGNAL(resourceCreated(Nepomuk::Resource,QList<QUrl>)),
            this, SIGNAL(personCreated(Nepomuk::Resource,QList<QUrl>)));

    connect(d_ptr->watcher, SIGNAL(resourceRemoved(QUrl,QList<QUrl>)),
            this, SIGNAL(personRemoved(QUrl,QList<QUrl>)));

    connect(d_ptr->watcher, SIGNAL(propertyAdded(Nepomuk::Resource,Nepomuk::Types::Property,QVariant)),
            this, SLOT(onPropertyChanged(Nepomuk::Resource,Nepomuk::Types::Property,QVariant)));

    connect(d_ptr->watcher, SIGNAL(propertyRemoved(Nepomuk::Resource,Nepomuk::Types::Property,QVariant)),
            this, SLOT(onPropertyChanged(Nepomuk::Resource,Nepomuk::Types::Property,QVariant)));

    d_ptr->watcher->start();
}

ResourceWatcherService::~ResourceWatcherService()
{

}

void ResourceWatcherService::onPropertyChanged(Nepomuk::Resource res, Nepomuk::Types::Property property, QVariant value)
{

}