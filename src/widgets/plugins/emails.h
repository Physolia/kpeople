/*
    Copyright 2014  Nilesh Suthar <nileshsuthar@live.in>

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

#ifndef EMAILS_H
#define EMAILS_H

#include "abstractfieldwidgetfactory.h"
#include "plugins/emaillistmodel.h"

class KJob;
class QModelIndex;

namespace KContacts {
class Addressee;
class Addressee::List;
}
using namespace KPeople;

class Emails : public AbstractFieldWidgetFactory
{
    Q_OBJECT
public:
    explicit Emails(QObject *parent = 0);
    virtual QString label() const;
    virtual int sortWeight() const;
    virtual QWidget *createDetailsWidget(const KContacts::Addressee &person, const KContacts::Addressee::List &contacts, QWidget *parent) const;

private Q_SLOTS:
    void onEmailDoubleClicked(const QModelIndex &clicked);
    void jobFinished(KJob *job);

private:
    QList<EmailItem> m_emailList;
    EmailListModel *m_emailsModel;

};

#endif // EMAILS_H
