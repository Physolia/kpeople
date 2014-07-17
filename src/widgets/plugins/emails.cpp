/*
 * Copyright 2014  Nilesh Suthar <nileshsuthar@live.in>
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

#include "emails.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListView>
#include <QDebug>
#include <baloo/query.h>
#include <baloo/resultiterator.h>
#include <KPluginFactory>
#include <KLocalizedString>


#include <Akonadi/Item>
#include <Akonadi/ItemFetchJob>
#include <Akonadi/ItemFetchScope>
#include <Akonadi/Collection>
#include <Akonadi/CollectionFetchJob>
#include <Akonadi/CollectionFetchScope>
#include <Akonadi/ServerManager>

#include <KMime/Message>
#include <KMimeType>
#include <QSizePolicy>
#include <QHeaderView>
#include <KABC/Addressee>
#include <KPluginFactory>
#include <KPluginLoader>

#include <QDebug>

using namespace Akonadi;


Emails::Emails(QObject* parent): AbstractFieldWidgetFactory(parent)
{
    me = new EmailListModel(emailList);
}

QWidget* Emails::createDetailsWidget(const KABC::Addressee& person, const KABC::AddresseeList& contacts, QWidget* parent) const
{
    Q_UNUSED(contacts);
    QWidget* widget = new QWidget(parent);

    QVBoxLayout* layout = new QVBoxLayout(widget);
    QListView* listview = new QListView(parent);
    layout->setContentsMargins(0, 0, 0, 0);

    Baloo::Query query;
    query.setSearchString(person.preferredEmail());
    query.setType("Email");
    Baloo::ResultIterator rt = query.exec();
    bool hasMsg = false;

    while (rt.next()) {
        hasMsg = true;

        Akonadi::Item it = Item::fromUrl(rt.url());
        ItemFetchJob* itemFetchJob = new ItemFetchJob(it);
        itemFetchJob->fetchScope().fetchFullPayload();
        connect(itemFetchJob, SIGNAL(finished(KJob*)), SLOT(jobFinished(KJob*)));
    }

    if (hasMsg) {
        listview->setModel(me);

        listview->setSelectionMode(QAbstractItemView::SingleSelection);
        listview->setSelectionBehavior(QAbstractItemView::SelectRows);
        listview->setEditTriggers(QAbstractItemView::NoEditTriggers);

        listview->show();
        layout->addWidget(listview);
        connect(listview, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onEmailDoubleClicked(QModelIndex)));
    } else {
        listview->hide();
        layout->addWidget(new QLabel("No Emails"));
    }
    widget->setLayout(layout);
    return widget;
}

void Emails::onEmailDoubleClicked(const QModelIndex& clicked)
{

    QUrl url = me->getItemUrl(clicked.row());
    QString cmd = "kmail --view ";
    cmd.append(url.toString());

    system(cmd.toStdString().c_str());

}

void Emails::jobFinished(KJob* job)
{
    if (job->error()) {
        qDebug() << "Error:" << job->errorString();
        return;
    }

    Akonadi::ItemFetchJob* fetchJob = qobject_cast<Akonadi::ItemFetchJob*>(job);
    const Akonadi::Item::List items = fetchJob->items();
    KMime::Message msg;

    foreach (const Akonadi::Item & item, items) {


        msg.setContent(item.payloadData());
        msg.setFrozen(true);
        msg.parse();
        KMime::Headers::Subject* subject = msg.subject();
        KMime::Headers::Date* date = msg.date();
        KMime::Content* textContent = msg.textContent();

        struct email mail;
        mail.subject = subject->asUnicodeString();
        mail.date = date->dateTime();
        mail.desc = textContent->body();
        mail.url = item.url();

        me->addEmail(mail);

    }
}

QString Emails::label() const
{
    return i18n("Mails");
}

int Emails::sortWeight() const
{
    return 0;
}


#include "emails.moc"
