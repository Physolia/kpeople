/*
    Duplicates Example
    Copyright (C) 2014 Aleix Pol Gonzalez <aleixpol@blue-systems.com>

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

#include <QApplication>
#include <QTimer>
#include "mergedialog.h"
#include <personsmodel.h>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    KPeople::MergeDialog* dialog = new KPeople::MergeDialog;
    KPeople::PersonsModel* model = new KPeople::PersonsModel(&app);

    QTimer* t = new QTimer(&app);
    t->setInterval(500);
    t->setSingleShot(true);
    QObject::connect(model, SIGNAL(modelInitialized(bool)), t, SLOT(start()));
    QObject::connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)), t, SLOT(start()));
    QObject::connect(t, &QTimer::timeout, [dialog, model](){
        dialog->setPersonsModel(model);
        dialog->show();
    });

    return app.exec();
}