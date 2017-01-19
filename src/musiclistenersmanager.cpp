/*
 * Copyright 2016 Matthieu Gallien <matthieu_gallien@yahoo.fr>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "config-upnp-qt.h"

#include "musiclistenersmanager.h"

#include "databaseinterface.h"

#if defined UPNPQT_FOUND && UPNPQT_FOUND
#include "upnp/upnplistener.h"
#endif

#if defined KF5Baloo_FOUND && KF5Baloo_FOUND
#include "baloo/baloolistener.h"
#endif

#include <QtCore/QThread>
#include <QtCore/QMutex>

class MusicListenersManagerPrivate
{
public:

    QThread mListenersThread;

#if defined UPNPQT_FOUND && UPNPQT_FOUND
    UpnpListener mUpnpListener;
#endif

#if defined KF5Baloo_FOUND && KF5Baloo_FOUND
    BalooListener mBalooListener;
#endif

    DatabaseInterface mDatabaseInterface;

    DatabaseInterface* mViewDatabase = nullptr;

    QMutex mMutex;

};

MusicListenersManager::MusicListenersManager(QObject *parent)
    : QObject(parent), d(new MusicListenersManagerPrivate)
{
    d->mListenersThread.start();

    d->mDatabaseInterface.moveToThread(&d->mListenersThread);

    connect(&d->mDatabaseInterface, &DatabaseInterface::requestsInitDone,
            this, &MusicListenersManager::databaseReady);

    QMetaObject::invokeMethod(&d->mDatabaseInterface, "init", Qt::QueuedConnection,
                              Q_ARG(QString, QStringLiteral("listeners")));
}

MusicListenersManager::~MusicListenersManager()
{
    delete d;
}

DatabaseInterface *MusicListenersManager::viewDatabase() const
{
    return d->mViewDatabase;
}

void MusicListenersManager::setViewDatabase(DatabaseInterface *viewDatabase)
{
    if (d->mViewDatabase == viewDatabase) {
        return;
    }

    if (d->mViewDatabase) {
        disconnect(&d->mDatabaseInterface, &DatabaseInterface::artistAdded,
                   d->mViewDatabase, &DatabaseInterface::databaseArtistAdded);
        disconnect(&d->mDatabaseInterface, &DatabaseInterface::albumAdded,
                   d->mViewDatabase, &DatabaseInterface::databaseAlbumAdded);
        disconnect(&d->mDatabaseInterface, &DatabaseInterface::trackAdded,
                   d->mViewDatabase, &DatabaseInterface::databaseTrackAdded);
    }

    d->mViewDatabase = viewDatabase;

    if (d->mViewDatabase) {
        connect(d->mViewDatabase, &DatabaseInterface::requestsInitDone,
                this, &MusicListenersManager::databaseReady);

        QMetaObject::invokeMethod(d->mViewDatabase, "init", Qt::QueuedConnection,
                                  Q_ARG(QString, QStringLiteral("views")));

        connect(&d->mDatabaseInterface, &DatabaseInterface::artistAdded,
                   d->mViewDatabase, &DatabaseInterface::databaseArtistAdded);
        connect(&d->mDatabaseInterface, &DatabaseInterface::albumAdded,
                   d->mViewDatabase, &DatabaseInterface::databaseAlbumAdded);
        connect(&d->mDatabaseInterface, &DatabaseInterface::trackAdded,
                   d->mViewDatabase, &DatabaseInterface::databaseTrackAdded);
    }

    emit viewDatabaseChanged();
}

void MusicListenersManager::databaseReady()
{
    if (sender() == d->mViewDatabase) {
        d->mViewDatabase->setMutex(&d->mMutex);
        return;
    }

    if (sender() == &d->mDatabaseInterface) {
        d->mDatabaseInterface.setMutex(&d->mMutex);
    }

#if defined KF5Baloo_FOUND && KF5Baloo_FOUND
    d->mBalooListener.setDatabaseInterface(&d->mDatabaseInterface);
    d->mBalooListener.moveToThread(&d->mListenersThread);
#endif
#if defined UPNPQT_FOUND && UPNPQT_FOUND
    d->mUpnpListener.setDatabaseInterface(&d->mDatabaseInterface);
    d->mUpnpListener.moveToThread(&d->mListenersThread);
#endif
}


#include "moc_musiclistenersmanager.cpp"
