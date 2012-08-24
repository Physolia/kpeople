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

#include "personactions.h"
#include "persons-model.h"
#include <klocalizedstring.h>
#include <qabstractitemmodel.h>
#include <QDebug>
#include <qaction.h>
#include <KToolInvocation>

Q_DECLARE_METATYPE(QModelIndex);

struct PersonActionsPrivate {
    PersonActionsPrivate() : row(-1), model(0) {}
    
    int row;
    QAbstractItemModel* model;
    QList<QAction*> actions;
};

PersonActions::PersonActions(QObject* parent)
    : QAbstractListModel(parent)
    , d_ptr(new PersonActionsPrivate)
{}

PersonActions::~PersonActions()
{
    delete d_ptr;
}

QList< QAction* > PersonActions::actions()
{
    Q_D(const PersonActions);
    return d->actions;
}

QAbstractItemModel* PersonActions::model() const
{
    Q_D(const PersonActions);
    return d->model;
}

void PersonActions::setModel(QAbstractItemModel* model)
{
    Q_D(PersonActions);
    d->model = model;
    initialize(d->model, d->row);
}

int PersonActions::row() const
{
    Q_D(const PersonActions);
    return d->row;
}

void PersonActions::setRow(int row)
{
    Q_D(PersonActions);
    d->row = row;
    initialize(d->model, d->row);
}

void PersonActions::initialize(QAbstractItemModel* model, int row)
{
    if(!model || row<0)
        return;
    
    Q_D(PersonActions);
    d->model = model;
    d->row = row;
    QModelIndex idx = model->index(row, 0);
    int rows = model->rowCount(idx);
    
    beginResetModel();
    qDeleteAll(d->actions);
    d->actions.clear();
    for(int i=0; i<rows; i++) {
        QModelIndex idxContact = idx.child(i, 0);
        QAction* action = new QAction(idxContact.data(Qt::DecorationRole).value<QIcon>(),
                                      i18n("Start '%1'", idxContact.data().toString()), this);
        action->setProperty("idx", qVariantFromValue(idxContact));
        bool b = false;
        switch(idxContact.data(PersonsModel::ContactTypeRole).toInt()) {
            case PersonsModel::Email:
                b = connect(action, SIGNAL(triggered(bool)), SLOT(emailTriggered()));
                break;
            case PersonsModel::IM:
                b = connect(action, SIGNAL(triggered(bool)), SLOT(chatTriggered()));
                break;
            case PersonsModel::Phone:
            case PersonsModel::MobilePhone:
            case PersonsModel::Postal:
                break;
        }
        if(b) {
            d->actions += action;
        } else
            delete action;
    }
    endResetModel();
    emit actionsChanged();
}

void PersonActions::emailTriggered()
{
    QAction* action = qobject_cast<QAction*>(sender());
    QModelIndex idxContact = action->property("idx").value<QModelIndex>();
    KToolInvocation::invokeMailer(idxContact.data(PersonsModel::EmailRole).toString(), QString());
}

void PersonActions::chatTriggered()
{
    QAction* action = qobject_cast<QAction*>(sender());
    QModelIndex idxContact = action->property("idx").value<QModelIndex>();
    //TODO: call startChat
}

QVariant PersonActions::data(const QModelIndex& index, int role) const
{
    Q_D(const PersonActions);
    int row = index.row();
    if(index.isValid() && row<d->actions.size()) {
        switch(role) {
            case Qt::DisplayRole: return d->actions[row]->text();
            case Qt::DecorationRole: return d->actions[row]->icon();
            case Qt::ToolTip: return d->actions[row]->toolTip();
        }
    }
    return QVariant();
}

int PersonActions::rowCount(const QModelIndex& parent) const
{
    Q_D(const PersonActions);
    return parent.isValid() ? 0 : d->actions.size();
}

void PersonActions::trigger(int actionsRow)
{
    Q_D(PersonActions);
    d->actions[actionsRow]->trigger();
}