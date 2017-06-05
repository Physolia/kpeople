/*
 * Convience dialog for displaying PersonDetailsView widgets
 * Copyright 2013  David Edmundson <davidedmundson@kde.org>
 *
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

#ifndef PERSONDETAILSDIALOG_H
#define PERSONDETAILSDIALOG_H

#include <QDialog>

#include <kpeople/widgets/kpeoplewidgets_export.h>
#include "../persondata.h"

namespace KPeople
{
class PersonDetailsDialogPrivate;

class KPEOPLEWIDGETS_EXPORT PersonDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PersonDetailsDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    virtual ~PersonDetailsDialog();

    void setPerson(PersonData *person);

private:
    Q_DECLARE_PRIVATE(PersonDetailsDialog)
    PersonDetailsDialogPrivate *d_ptr;
};

}
#endif // PERSONDETAILSDIALOG_H
