/*
    Copyright (C) 2013  David Edmundson <D.Edmundson@lboro.ac.uk>

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


#include "emailplugin.h"

#include "persondata.h"

#include <KLocalizedString>
#include <KIcon>
#include <KToolInvocation>
#include <KDebug>
#include <KPluginFactory>

EmailPlugin::EmailPlugin(QObject *parent, const QVariantList &args):
    AbstractPersonPlugin(parent)
{
    Q_UNUSED(args);
}

QList< QAction* > EmailPlugin::actionsForPerson(const KPeople::PersonDataPtr &personData, QObject *parent)
{
    QList<QAction*> actions;
    Q_FOREACH (const QString &email, personData->emails()) {
        QAction *action = new QAction(KIcon("main-unread"), i18nc("%1 is a specific email address","Email %1").arg(email), parent);
        connect(action, SIGNAL(triggered(bool)), SLOT(onEmailTriggered()));
        action->setProperty("emailAddress", email);
        actions << action;
    }
    return actions;
}

void EmailPlugin::onEmailTriggered()
{
    QAction *action = qobject_cast<QAction*>(sender());
    QString email = action->property("emailAddress").toString();
    KToolInvocation::invokeMailer(email, QString());
}

K_PLUGIN_FACTORY( EmailPluginFactory, registerPlugin<EmailPlugin>(); )
K_EXPORT_PLUGIN( EmailPluginFactory("email_kpeople_plugin") )
