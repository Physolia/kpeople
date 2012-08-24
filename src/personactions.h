/*
    Persons Model
    Copyright (C) 2012  Aleix Pol Gonzalez <aleixpol@blue-systems.com>

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

#ifndef PERSONACTIONS_H
#define PERSONACTIONS_H

#include "kpeople_export.h"
#include <qabstractitemmodel.h>

class QAction;
struct PersonActionsPrivate;

class KPEOPLE_EXPORT PersonActions : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QAbstractItemModel* peopleModel READ model WRITE setModel)
    Q_PROPERTY(int row READ row WRITE setRow)
    public:
        explicit PersonActions(QObject* parent = 0);
        virtual ~PersonActions();
        
        QList<QAction*> actions();
        void setModel(QAbstractItemModel* model);
        QAbstractItemModel* model() const;
        int row() const;
        void setRow(int row);
        void initialize(QAbstractItemModel* model, int row);
        Q_INVOKABLE void trigger(int actionsRow);
        
        virtual QVariant data(const QModelIndex& index, int role) const;
        virtual int rowCount(const QModelIndex& parent) const;

    public slots:
        void emailTriggered();
        void chatTriggered();

    signals:
        void actionsChanged();

    private:
        PersonActionsPrivate * const d_ptr;
        Q_DECLARE_PRIVATE(PersonActions);
};

#endif // PERSONACTIONS_H