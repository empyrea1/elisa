/*
 * Copyright 2016-2017 Matthieu Gallien <matthieu_gallien@yahoo.fr>
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "databaseinterface.h"

#include <KI18n/KLocalizedString>

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include <QMutex>
#include <QVariant>
#include <QAtomicInt>
#include <QDebug>

#include <algorithm>

class DatabaseInterfacePrivate
{
public:

    DatabaseInterfacePrivate(const QSqlDatabase &tracksDatabase)
        : mTracksDatabase(tracksDatabase), mSelectAlbumQuery(mTracksDatabase),
          mSelectTrackQuery(mTracksDatabase), mSelectAlbumIdFromTitleQuery(mTracksDatabase),
          mInsertAlbumQuery(mTracksDatabase), mSelectTrackIdFromTitleAlbumIdArtistQuery(mTracksDatabase),
          mInsertTrackQuery(mTracksDatabase), mSelectTracksFromArtist(mTracksDatabase),
          mSelectTrackFromIdQuery(mTracksDatabase), mSelectCountAlbumsForArtistQuery(mTracksDatabase),
          mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery(mTracksDatabase), mSelectAllAlbumsQuery(mTracksDatabase),
          mSelectAllAlbumsFromArtistQuery(mTracksDatabase), mSelectAllArtistsQuery(mTracksDatabase),
          mInsertArtistsQuery(mTracksDatabase), mSelectArtistByNameQuery(mTracksDatabase),
          mSelectArtistQuery(mTracksDatabase), mSelectTrackFromFilePathQuery(mTracksDatabase),
          mRemoveTrackQuery(mTracksDatabase), mRemoveAlbumQuery(mTracksDatabase),
          mRemoveArtistQuery(mTracksDatabase), mSelectAllTracksQuery(mTracksDatabase),
          mInsertTrackMapping(mTracksDatabase), mSelectAllTracksFromSourceQuery(mTracksDatabase),
          mInsertMusicSource(mTracksDatabase), mSelectMusicSource(mTracksDatabase),
          mUpdateTrackMapping(mTracksDatabase),
          mSelectTracksMapping(mTracksDatabase), mSelectTracksMappingPriority(mTracksDatabase),
          mUpdateAlbumArtUriFromAlbumIdQuery(mTracksDatabase), mSelectTracksMappingPriorityByTrackId(mTracksDatabase),
          mSelectAllTrackFilesFromSourceQuery(mTracksDatabase),  mSelectAlbumIdsFromArtist(mTracksDatabase),
          mRemoveTracksMappingFromSource(mTracksDatabase), mRemoveTracksMapping(mTracksDatabase),
          mSelectTracksWithoutMappingQuery(mTracksDatabase), mSelectAlbumIdFromTitleAndArtistQuery(mTracksDatabase),
          mSelectAlbumIdFromTitleWithoutArtistQuery(mTracksDatabase),
          mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery(mTracksDatabase), mSelectAlbumArtUriFromAlbumIdQuery(mTracksDatabase),
          mInsertComposerQuery(mTracksDatabase), mSelectComposerByNameQuery(mTracksDatabase),
          mSelectComposerQuery(mTracksDatabase), mInsertLyricistQuery(mTracksDatabase),
          mSelectLyricistByNameQuery(mTracksDatabase), mSelectLyricistQuery(mTracksDatabase),
          mInsertGenreQuery(mTracksDatabase), mSelectGenreByNameQuery(mTracksDatabase),
          mSelectGenreQuery(mTracksDatabase), mSelectAllTracksShortQuery(mTracksDatabase),
          mSelectAllAlbumsShortQuery(mTracksDatabase), mSelectAllComposersQuery(mTracksDatabase),
          mSelectAllLyricistsQuery(mTracksDatabase), mSelectCountAlbumsForComposerQuery(mTracksDatabase),
          mSelectCountAlbumsForLyricistQuery(mTracksDatabase), mSelectAllGenresQuery(mTracksDatabase),
          mSelectGenreForArtistQuery(mTracksDatabase), mSelectGenreForAlbumQuery(mTracksDatabase),
          mUpdateTrackQuery(mTracksDatabase), mUpdateAlbumArtistQuery(mTracksDatabase),
          mUpdateAlbumArtistInTracksQuery(mTracksDatabase)
    {
    }

    QSqlDatabase mTracksDatabase;

    QSqlQuery mSelectAlbumQuery;

    QSqlQuery mSelectTrackQuery;

    QSqlQuery mSelectAlbumIdFromTitleQuery;

    QSqlQuery mInsertAlbumQuery;

    QSqlQuery mSelectTrackIdFromTitleAlbumIdArtistQuery;

    QSqlQuery mInsertTrackQuery;

    QSqlQuery mSelectTracksFromArtist;

    QSqlQuery mSelectTrackFromIdQuery;

    QSqlQuery mSelectCountAlbumsForArtistQuery;

    QSqlQuery mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery;

    QSqlQuery mSelectAllAlbumsQuery;

    QSqlQuery mSelectAllAlbumsFromArtistQuery;

    QSqlQuery mSelectAllArtistsQuery;

    QSqlQuery mInsertArtistsQuery;

    QSqlQuery mSelectArtistByNameQuery;

    QSqlQuery mSelectArtistQuery;

    QSqlQuery mSelectTrackFromFilePathQuery;

    QSqlQuery mRemoveTrackQuery;

    QSqlQuery mRemoveAlbumQuery;

    QSqlQuery mRemoveArtistQuery;

    QSqlQuery mSelectAllTracksQuery;

    QSqlQuery mInsertTrackMapping;

    QSqlQuery mSelectAllTracksFromSourceQuery;

    QSqlQuery mInsertMusicSource;

    QSqlQuery mSelectMusicSource;

    QSqlQuery mUpdateTrackMapping;

    QSqlQuery mSelectTracksMapping;

    QSqlQuery mSelectTracksMappingPriority;

    QSqlQuery mUpdateAlbumArtUriFromAlbumIdQuery;

    QSqlQuery mSelectTracksMappingPriorityByTrackId;

    QSqlQuery mSelectAllTrackFilesFromSourceQuery;

    QSqlQuery mSelectAlbumIdsFromArtist;

    QSqlQuery mRemoveTracksMappingFromSource;

    QSqlQuery mRemoveTracksMapping;

    QSqlQuery mSelectTracksWithoutMappingQuery;

    QSqlQuery mSelectAlbumIdFromTitleAndArtistQuery;

    QSqlQuery mSelectAlbumIdFromTitleWithoutArtistQuery;

    QSqlQuery mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery;

    QSqlQuery mSelectAlbumArtUriFromAlbumIdQuery;

    QSqlQuery mInsertComposerQuery;

    QSqlQuery mSelectComposerByNameQuery;

    QSqlQuery mSelectComposerQuery;

    QSqlQuery mInsertLyricistQuery;

    QSqlQuery mSelectLyricistByNameQuery;

    QSqlQuery mSelectLyricistQuery;

    QSqlQuery mInsertGenreQuery;

    QSqlQuery mSelectGenreByNameQuery;

    QSqlQuery mSelectGenreQuery;

    QSqlQuery mSelectAllTracksShortQuery;

    QSqlQuery mSelectAllAlbumsShortQuery;

    QSqlQuery mSelectAllComposersQuery;

    QSqlQuery mSelectAllLyricistsQuery;

    QSqlQuery mSelectCountAlbumsForComposerQuery;

    QSqlQuery mSelectCountAlbumsForLyricistQuery;

    QSqlQuery mSelectAllGenresQuery;

    QSqlQuery mSelectGenreForArtistQuery;

    QSqlQuery mSelectGenreForAlbumQuery;

    QSqlQuery mUpdateTrackQuery;

    QSqlQuery mUpdateAlbumArtistQuery;

    QSqlQuery mUpdateAlbumArtistInTracksQuery;

    QHash<qulonglong, MusicAudioTrack> mTracksCache;

    QHash<qulonglong, MusicAlbum> mAlbumsCache;

    QHash<qulonglong, MusicArtist> mArtistsCache;

    QSet<qulonglong> mModifiedTrackIds;

    QSet<qulonglong> mModifiedAlbumIds;

    QSet<qulonglong> mModifiedArtistIds;

    QSet<qulonglong> mInsertedTracks;

    QSet<qulonglong> mInsertedAlbums;

    QSet<qulonglong> mInsertedArtists;

    qulonglong mAlbumId = 1;

    qulonglong mArtistId = 1;

    qulonglong mComposerId = 1;

    qulonglong mLyricistId = 1;

    qulonglong mGenreId = 1;

    qulonglong mTrackId = 1;

    qulonglong mDiscoverId = 1;

    QAtomicInt mStopRequest = 0;

    bool mInitFinished = false;

};

DatabaseInterface::DatabaseInterface(QObject *parent) : QObject(parent), d(nullptr)
{
}

DatabaseInterface::~DatabaseInterface()
{
    if (d) {
        d->mTracksDatabase.close();
    }
}

void DatabaseInterface::init(const QString &dbName, const QString &databaseFileName)
{
    QSqlDatabase tracksDatabase = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), dbName);

    if (!databaseFileName.isEmpty()) {
        tracksDatabase.setDatabaseName(QStringLiteral("file:") + databaseFileName);
    } else {
        tracksDatabase.setDatabaseName(QStringLiteral("file:memdb1?mode=memory"));
    }
    tracksDatabase.setConnectOptions(QStringLiteral("foreign_keys = ON;locking_mode = EXCLUSIVE;QSQLITE_OPEN_URI;QSQLITE_BUSY_TIMEOUT=500000"));

    auto result = tracksDatabase.open();
    if (result) {
        qDebug() << "database open";
    } else {
        qDebug() << "database not open";
    }
    qDebug() << "DatabaseInterface::init" << (tracksDatabase.driver()->hasFeature(QSqlDriver::Transactions) ? "yes" : "no");

    tracksDatabase.exec(QStringLiteral("PRAGMA foreign_keys = ON;"));

    d = std::make_unique<DatabaseInterfacePrivate>(tracksDatabase);

    initDatabase();
    initRequest();

    if (!databaseFileName.isEmpty()) {
        reloadExistingDatabase();
    }
}

MusicAlbum DatabaseInterface::albumFromTitleAndArtist(const QString &title, const QString &artist)
{
    auto result = MusicAlbum();

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return result;
    }

    result = internalAlbumFromTitleAndArtist(title, artist);

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return result;
    }

    return result;
}

QList<QMap<DatabaseInterface::PropertyType, QVariant>> DatabaseInterface::allData(DataUtils::DataType aType)
{
    auto result = QList<QMap<PropertyType, QVariant>>{};

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return result;
    }

    switch (aType)
    {
    case DataUtils::DataType::AllArtists:
        result = internalAllArtistsPartialData();
        break;
    case DataUtils::DataType::AllAlbums:
        result = internalAllAlbumsPartialData();
        break;
    case DataUtils::DataType::AllTracks:
        result = internalAllTracksPartialData();
        break;
    case DataUtils::DataType::AllGenres:
        result = internalAllGenresPartialData();
        break;
    case DataUtils::DataType::AllComposers:
        result = internalAllComposersPartialData();
        break;
    case DataUtils::DataType::AllLyricists:
        result = internalAllLyricistsPartialData();
        break;
    case DataUtils::DataType::UnknownType:
        break;
    };

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return result;
    }

    return result;
}


QList<MusicAudioTrack> DatabaseInterface::allTracks()
{
    auto result = QList<MusicAudioTrack>();

    if (!d) {
        return result;
    }

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return result;
    }

    auto queryResult = d->mSelectAllTracksQuery.exec();

    if (!queryResult || !d->mSelectAllTracksQuery.isSelect() || !d->mSelectAllTracksQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::allAlbums" << d->mSelectAllTracksQuery.lastQuery();
        qDebug() << "DatabaseInterface::allAlbums" << d->mSelectAllTracksQuery.boundValues();
        qDebug() << "DatabaseInterface::allAlbums" << d->mSelectAllTracksQuery.lastError();

        return result;
    }

    while(d->mSelectAllTracksQuery.next()) {
        const auto &currentRecord = d->mSelectAllTracksQuery.record();

        result.push_back(buildTrackFromDatabaseRecord(currentRecord));
    }

    d->mSelectAllTracksQuery.finish();

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return result;
    }

    return result;
}

QList<MusicAudioTrack> DatabaseInterface::allTracksFromSource(const QString &musicSource)
{
    auto result = QList<MusicAudioTrack>();

    if (!d) {
        return result;
    }

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return result;
    }

    d->mSelectAllTracksFromSourceQuery.bindValue(QStringLiteral(":source"), musicSource);

    auto queryResult = d->mSelectAllTracksFromSourceQuery.exec();

    if (!queryResult || !d->mSelectAllTracksFromSourceQuery.isSelect() || !d->mSelectAllTracksFromSourceQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::allAlbums" << d->mSelectAllTracksFromSourceQuery.lastQuery();
        qDebug() << "DatabaseInterface::allAlbums" << d->mSelectAllTracksFromSourceQuery.boundValues();
        qDebug() << "DatabaseInterface::allAlbums" << d->mSelectAllTracksFromSourceQuery.lastError();

        d->mSelectAllTracksFromSourceQuery.finish();

        transactionResult = finishTransaction();
        if (!transactionResult) {
            return result;
        }

        return result;
    }

    while(d->mSelectAllTracksFromSourceQuery.next()) {
        const auto &currentRecord = d->mSelectAllTracksFromSourceQuery.record();

        result.push_back(buildTrackFromDatabaseRecord(currentRecord));
    }

    d->mSelectAllTracksFromSourceQuery.finish();

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return result;
    }

    return result;
}

QList<MusicAlbum> DatabaseInterface::allAlbums()
{
    auto result = QList<MusicAlbum>();

    if (!d) {
        return result;
    }

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return result;
    }

    auto queryResult = d->mSelectAllAlbumsQuery.exec();

    if (!queryResult || !d->mSelectAllAlbumsQuery.isSelect() || !d->mSelectAllAlbumsQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::allAlbums" << d->mSelectAllAlbumsQuery.lastQuery();
        qDebug() << "DatabaseInterface::allAlbums" << d->mSelectAllAlbumsQuery.boundValues();
        qDebug() << "DatabaseInterface::allAlbums" << d->mSelectAllAlbumsQuery.lastError();

        return result;
    }

    while(d->mSelectAllAlbumsQuery.next()) {
        const auto &currentRecord = d->mSelectAllAlbumsQuery.record();

        auto albumId = currentRecord.value(0).toULongLong();

        auto &newAlbum = d->mAlbumsCache[albumId];

        if (newAlbum.isValid()) {
            result.push_back(newAlbum);
            continue;
        }

        newAlbum.setDatabaseId(albumId);
        newAlbum.setTitle(currentRecord.value(1).toString());
        newAlbum.setId(currentRecord.value(2).toString());
        newAlbum.setArtist(currentRecord.value(3).toString());
        newAlbum.setAlbumPath(currentRecord.value(4).toUrl());
        newAlbum.setAlbumArtURI(currentRecord.value(5).toUrl());
        newAlbum.setTracksCount(currentRecord.value(6).toInt());
        newAlbum.setIsSingleDiscAlbum(currentRecord.value(7).toBool());
        newAlbum.setTracks(fetchTracks(newAlbum.databaseId()));
        newAlbum.setValid(true);

        d->mSelectGenreForAlbumQuery.bindValue(QStringLiteral(":albumId"), newAlbum.databaseId());

        queryResult = d->mSelectGenreForAlbumQuery.exec();

        if (!queryResult || !d->mSelectGenreForAlbumQuery.isSelect() || !d->mSelectGenreForAlbumQuery.isActive()) {
            Q_EMIT databaseError();

            qDebug() << "DatabaseInterface::internalAlbumFromId" << d->mSelectGenreForAlbumQuery.lastQuery();
            qDebug() << "DatabaseInterface::internalAlbumFromId" << d->mSelectGenreForAlbumQuery.boundValues();
            qDebug() << "DatabaseInterface::internalAlbumFromId" << d->mSelectGenreForAlbumQuery.lastError();

            d->mSelectGenreForAlbumQuery.finish();

            return result;
        }

        QStringList allGenres;
        while(d->mSelectGenreForAlbumQuery.next()) {
            allGenres.push_back(d->mSelectGenreForAlbumQuery.record().value(0).toString());
        }

        newAlbum.setGenres(allGenres);

        d->mSelectGenreForAlbumQuery.finish();

        result.push_back(newAlbum);
    }

    d->mSelectAllAlbumsQuery.finish();

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return result;
    }

    return result;
}

QList<MusicArtist> DatabaseInterface::allArtists()
{
    auto result = QList<MusicArtist>{};

    if (!d) {
        return result;
    }

    result = internalAllPeople(d->mSelectAllArtistsQuery, d->mSelectCountAlbumsForArtistQuery);

    return result;
}

QList<MusicAudioGenre> DatabaseInterface::allGenres()
{
    auto result = QList<MusicAudioGenre>();

    if (!d) {
        return result;
    }

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return result;
    }

    auto queryResult = d->mSelectAllGenresQuery.exec();

    if (!queryResult || !d->mSelectAllGenresQuery.isSelect() || !d->mSelectAllGenresQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::allAlbums" << d->mSelectAllGenresQuery.lastQuery();
        qDebug() << "DatabaseInterface::allAlbums" << d->mSelectAllGenresQuery.boundValues();
        qDebug() << "DatabaseInterface::allAlbums" << d->mSelectAllGenresQuery.lastError();

        return result;
    }

    while(d->mSelectAllGenresQuery.next()) {
        auto newGenre = MusicAudioGenre();

        const auto &currentRecord = d->mSelectAllGenresQuery.record();

        newGenre.setDatabaseId(currentRecord.value(0).toULongLong());
        newGenre.setName(currentRecord.value(1).toString());

        result.push_back(newGenre);
    }

    d->mSelectAllGenresQuery.finish();

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return result;
    }

    return result;
}

QList<MusicArtist> DatabaseInterface::allComposers()
{
    return internalAllPeople(d->mSelectAllComposersQuery, d->mSelectCountAlbumsForComposerQuery);
}

QList<MusicArtist> DatabaseInterface::allLyricists()
{
    return internalAllPeople(d->mSelectAllLyricistsQuery, d->mSelectCountAlbumsForLyricistQuery);
}

QList<MusicAudioTrack> DatabaseInterface::tracksFromAuthor(const QString &ArtistName)
{
    auto allTracks = QList<MusicAudioTrack>();

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return allTracks;
    }

    allTracks = internalTracksFromAuthor(ArtistName);

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return allTracks;
    }

    return allTracks;
}

MusicArtist DatabaseInterface::internalArtistFromId(qulonglong artistId)
{
    auto &result = d->mArtistsCache[artistId];

    if (result.isValid()) {
        return result;
    }

    if (!d || !d->mTracksDatabase.isValid() || !d->mInitFinished) {
        return result;
    }

    d->mSelectArtistQuery.bindValue(QStringLiteral(":artistId"), artistId);

    auto queryResult = d->mSelectArtistQuery.exec();

    if (!queryResult || !d->mSelectArtistQuery.isSelect() || !d->mSelectArtistQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::internalArtistFromId" << d->mSelectArtistQuery.lastQuery();
        qDebug() << "DatabaseInterface::internalArtistFromId" << d->mSelectArtistQuery.boundValues();
        qDebug() << "DatabaseInterface::internalArtistFromId" << d->mSelectArtistQuery.lastError();

        d->mSelectArtistQuery.finish();

        return result;
    }

    if (!d->mSelectArtistQuery.next()) {
        d->mSelectArtistQuery.finish();

        return result;
    }

    const auto &currentRecord = d->mSelectArtistQuery.record();

    result.setDatabaseId(currentRecord.value(0).toULongLong());
    result.setName(currentRecord.value(1).toString());
    result.setValid(true);

    d->mSelectArtistQuery.finish();

    d->mSelectCountAlbumsForArtistQuery.bindValue(QStringLiteral(":artistName"), result.name());

    queryResult = d->mSelectCountAlbumsForArtistQuery.exec();

    if (!queryResult || !d->mSelectCountAlbumsForArtistQuery.isSelect() || !d->mSelectCountAlbumsForArtistQuery.isActive() || !d->mSelectCountAlbumsForArtistQuery.next()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::internalArtistFromId" << d->mSelectCountAlbumsForArtistQuery.lastQuery();
        qDebug() << "DatabaseInterface::internalArtistFromId" << d->mSelectCountAlbumsForArtistQuery.boundValues();
        qDebug() << "DatabaseInterface::internalArtistFromId" << d->mSelectCountAlbumsForArtistQuery.lastError();

        d->mSelectCountAlbumsForArtistQuery.finish();

        return result;
    }

    result.setAlbumsCount(d->mSelectCountAlbumsForArtistQuery.record().value(0).toInt());

    d->mSelectCountAlbumsForArtistQuery.finish();

    return result;
}

MusicAudioTrack DatabaseInterface::trackFromDatabaseId(qulonglong id)
{
    auto result = MusicAudioTrack();

    if (!d) {
        return result;
    }

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return result;
    }

    result = internalTrackFromDatabaseId(id);

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return result;
    }

    return result;
}

qulonglong DatabaseInterface::trackIdFromTitleAlbumTrackDiscNumber(const QString &title, const QString &artist, const QString &album,
                                                                   int trackNumber, int discNumber)
{
    auto result = qulonglong(0);

    if (!d) {
        return result;
    }

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return result;
    }

    result = internalTrackIdFromTitleAlbumTracDiscNumber(title, artist, album, trackNumber, discNumber);

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return result;
    }

    return result;
}

qulonglong DatabaseInterface::trackIdFromFileName(const QUrl &fileName)
{
    auto result = qulonglong(0);

    if (!d) {
        return result;
    }

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return result;
    }

    result = internalTrackIdFromFileName(fileName);

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return result;
    }

    return result;
}

void DatabaseInterface::applicationAboutToQuit()
{
    d->mStopRequest = 1;
}

void DatabaseInterface::removeAllTracksFromSource(const QString &sourceName)
{
    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return;
    }

    initChangesTrackers();

    auto sourceId = internalSourceIdFromName(sourceName);

    auto allFileNames = internalAllFileNameFromSource(sourceId);

    internalRemoveTracksList(allFileNames, sourceId);

    if (!d->mInsertedArtists.isEmpty()) {
        QList<MusicArtist> newArtists;
        for (auto artistId : qAsConst(d->mInsertedArtists)) {
            newArtists.push_back(internalArtistFromId(artistId));
        }
        Q_EMIT artistsAdded(newArtists);
    }

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return;
    }
}

void DatabaseInterface::getAlbumFromAlbumId(qulonglong id)
{
    auto result = MusicAlbum();

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        Q_EMIT sentAlbumData(result);
    }

    result = internalAlbumFromId(id);

    transactionResult = finishTransaction();
    if (!transactionResult) {
        Q_EMIT sentAlbumData(result);
    }

    Q_EMIT sentAlbumData(result);
}

void DatabaseInterface::askRestoredTracks(const QString &musicSource)
{
    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return;
    }

    auto result = internalAllFileNameFromSource(internalSourceIdFromName(musicSource));

    Q_EMIT restoredTracks(musicSource, result);

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return;
    }
}

void DatabaseInterface::initChangesTrackers()
{
    d->mModifiedTrackIds.clear();
    d->mModifiedAlbumIds.clear();
    d->mModifiedArtistIds.clear();
    d->mInsertedTracks.clear();
    d->mInsertedAlbums.clear();
    d->mInsertedArtists.clear();
}

void DatabaseInterface::recordModifiedTrack(qulonglong trackId)
{
    d->mModifiedTrackIds.insert(trackId);
    d->mTracksCache.remove(trackId);
}

void DatabaseInterface::recordModifiedAlbum(qulonglong albumId)
{
    d->mModifiedAlbumIds.insert(albumId);
    d->mAlbumsCache.remove(albumId);
}

void DatabaseInterface::insertTracksList(const QList<MusicAudioTrack> &tracks, const QHash<QString, QUrl> &covers, const QString &musicSource)
{
    if (d->mStopRequest == 1) {
        return;
    }

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return;
    }

    initChangesTrackers();

    for(const auto &oneTrack : tracks) {
        d->mSelectTracksMapping.bindValue(QStringLiteral(":fileName"), oneTrack.resourceURI());

        auto result = d->mSelectTracksMapping.exec();

        if (!result || !d->mSelectTracksMapping.isSelect() || !d->mSelectTracksMapping.isActive()) {
            Q_EMIT databaseError();

            qDebug() << "DatabaseInterface::insertTracksList" << d->mSelectTracksMapping.lastQuery();
            qDebug() << "DatabaseInterface::insertTracksList" << d->mSelectTracksMapping.boundValues();
            qDebug() << "DatabaseInterface::insertTracksList" << d->mSelectTracksMapping.lastError();

            d->mSelectTracksMapping.finish();

            rollBackTransaction();
            return;
        }

        bool isNewTrack = !d->mSelectTracksMapping.next();

        if (isNewTrack) {
            insertTrackOrigin(oneTrack.resourceURI(), oneTrack.fileModificationTime(), insertMusicSource(musicSource));
        } else if (!d->mSelectTracksMapping.record().value(0).isNull() && d->mSelectTracksMapping.record().value(0).toULongLong() != 0) {
            updateTrackOrigin(d->mSelectTracksMapping.record().value(0).toULongLong(), oneTrack.resourceURI(), oneTrack.fileModificationTime());
        } else {
            continue;
        }

        d->mSelectTracksMapping.finish();

        const auto insertedTrackId = internalInsertTrack(oneTrack, covers, 0,
                                                         (isNewTrack ? TrackFileInsertType::NewTrackFileInsert : TrackFileInsertType::ModifiedTrackFileInsert));

        if (isNewTrack && insertedTrackId != 0) {
            d->mInsertedTracks.insert(insertedTrackId);
        }

        if (d->mStopRequest == 1) {
            transactionResult = finishTransaction();
            if (!transactionResult) {
                return;
            }
            return;
        }
    }

    if (!d->mInsertedArtists.isEmpty()) {
        QList<MusicArtist> newArtists;
        for (auto artistId : qAsConst(d->mInsertedArtists)) {
            newArtists.push_back(internalArtistFromId(artistId));
        }
        Q_EMIT artistsAdded(newArtists);
    }

    QList<MusicAlbum> newAlbums;
    for (auto albumId : qAsConst(d->mInsertedAlbums)) {
        d->mModifiedAlbumIds.remove(albumId);
        newAlbums.push_back(internalAlbumFromId(albumId));
    }

    if (!newAlbums.isEmpty()) {
        Q_EMIT albumsAdded(newAlbums);
    }

    for (auto albumId : qAsConst(d->mModifiedAlbumIds)) {
        Q_EMIT albumModified(internalAlbumFromId(albumId), albumId);
    }

    for (auto trackId : qAsConst(d->mInsertedTracks)) {
        d->mModifiedTrackIds.remove(trackId);
    }

    for (auto trackId : qAsConst(d->mModifiedTrackIds)) {
        Q_EMIT trackModified(internalTrackFromDatabaseId(trackId));
    }

    QList<MusicAudioTrack> newTracks;
    for (auto trackId : qAsConst(d->mInsertedTracks)) {
        newTracks.push_back(internalTrackFromDatabaseId(trackId));
    }

    if (!newTracks.isEmpty()) {
        Q_EMIT tracksAdded(newTracks);
    }

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return;
    }
}

void DatabaseInterface::removeTracksList(const QList<QUrl> &removedTracks)
{
    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return;
    }

    initChangesTrackers();

    internalRemoveTracksList(removedTracks);

    if (!d->mInsertedArtists.isEmpty()) {
        QList<MusicArtist> newArtists;
        for (auto artistId : qAsConst(d->mInsertedArtists)) {
            newArtists.push_back(internalArtistFromId(artistId));
        }
        Q_EMIT artistsAdded(newArtists);
    }

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return;
    }
}

void DatabaseInterface::modifyTracksList(const QList<MusicAudioTrack> &modifiedTracks, const QHash<QString, QUrl> &covers,
                                         const QString &musicSource)
{
    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return;
    }

    initChangesTrackers();

    for (const auto &oneModifiedTrack : modifiedTracks) {
        if (oneModifiedTrack.albumArtist().isEmpty()) {
            continue;
        }

        bool modifyExistingTrack = internalTrackFromDatabaseId(oneModifiedTrack.databaseId()).isValid() ||
                (internalTrackIdFromFileName(oneModifiedTrack.resourceURI()) != 0);

        auto originTrackId = oneModifiedTrack.databaseId();
        if (!originTrackId) {
            originTrackId = internalTrackIdFromFileName(oneModifiedTrack.resourceURI());
        }

        if (!modifyExistingTrack) {
            insertTrackOrigin(oneModifiedTrack.resourceURI(), oneModifiedTrack.fileModificationTime(), insertMusicSource(musicSource));
        } else {
            updateTrackOrigin(originTrackId, oneModifiedTrack.resourceURI(), oneModifiedTrack.fileModificationTime());
        }

        const auto insertedTrackId = internalInsertTrack(oneModifiedTrack, covers, (modifyExistingTrack ? originTrackId : 0),
                                                         (modifyExistingTrack ? TrackFileInsertType::ModifiedTrackFileInsert : TrackFileInsertType::NewTrackFileInsert));

        if (!modifyExistingTrack && insertedTrackId != 0) {
            d->mInsertedTracks.insert(insertedTrackId);
        }
    }

    QList<MusicAlbum> newAlbums;
    for (auto albumId : qAsConst(d->mInsertedAlbums)) {
        d->mModifiedAlbumIds.remove(albumId);
        newAlbums.push_back(internalAlbumFromId(albumId));
    }

    if (!newAlbums.isEmpty()) {
        Q_EMIT albumsAdded(newAlbums);
    }

    for (auto albumId : qAsConst(d->mModifiedAlbumIds)) {
        Q_EMIT albumModified(internalAlbumFromId(albumId), albumId);
    }

    for (auto trackId : qAsConst(d->mModifiedTrackIds)) {
        Q_EMIT trackModified(internalTrackFromDatabaseId(trackId));
    }

    QList<MusicAudioTrack> newTracks;
    for (auto trackId : qAsConst(d->mInsertedTracks)) {
        newTracks.push_back(internalTrackFromDatabaseId(trackId));
    }

    if (!newTracks.isEmpty()) {
        Q_EMIT tracksAdded(newTracks);
    }

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return;
    }
}

bool DatabaseInterface::startTransaction() const
{
    auto result = false;

    auto transactionResult = d->mTracksDatabase.transaction();
    if (!transactionResult) {
        qDebug() << "transaction failed" << d->mTracksDatabase.lastError() << d->mTracksDatabase.lastError().driverText();

        return result;
    }

    result = true;

    return result;
}

bool DatabaseInterface::finishTransaction() const
{
    auto result = false;

    auto transactionResult = d->mTracksDatabase.commit();

    if (!transactionResult) {
        qDebug() << "commit failed" << d->mTracksDatabase.lastError() << d->mTracksDatabase.lastError().nativeErrorCode();

        return result;
    }

    result = true;

    return result;
}

bool DatabaseInterface::rollBackTransaction() const
{
    auto result = false;

    auto transactionResult = d->mTracksDatabase.rollback();

    if (!transactionResult) {
        qDebug() << "commit failed" << d->mTracksDatabase.lastError() << d->mTracksDatabase.lastError().nativeErrorCode();

        return result;
    }

    result = true;

    return result;
}

void DatabaseInterface::initDatabase()
{
    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return;
    }

    auto listTables = d->mTracksDatabase.tables();

    if (!listTables.contains(QStringLiteral("DatabaseVersionV6"))) {
        auto oldTables = QStringList{QStringLiteral("DatabaseVersionV2"),
                QStringLiteral("DatabaseVersionV3"),
                QStringLiteral("DatabaseVersionV4"),
                QStringLiteral("DatabaseVersionV5"),
                QStringLiteral("AlbumsArtists"),
                QStringLiteral("TracksArtists"),
                QStringLiteral("TracksMapping"),
                QStringLiteral("Tracks"),
                QStringLiteral("Composer"),
                QStringLiteral("Genre"),
                QStringLiteral("Lyricist"),
                QStringLiteral("Albums"),
                QStringLiteral("DiscoverSource"),
                QStringLiteral("Artists"),};
        for (const auto &oneTable : oldTables) {
            if (listTables.indexOf(oneTable) == -1) {
                continue;
            }

            QSqlQuery createSchemaQuery(d->mTracksDatabase);

            auto result = createSchemaQuery.exec(QStringLiteral("DROP TABLE ") + oneTable);

            if (!result) {
                qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastQuery();
                qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastError();

                Q_EMIT databaseError();
            }
        }

        listTables = d->mTracksDatabase.tables();
    }

    if (!listTables.contains(QStringLiteral("DatabaseVersionV6"))) {
        QSqlQuery createSchemaQuery(d->mTracksDatabase);

        const auto &result = createSchemaQuery.exec(QStringLiteral("CREATE TABLE `DatabaseVersionV6` (`Version` INTEGER PRIMARY KEY NOT NULL)"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    if (!listTables.contains(QStringLiteral("DiscoverSource"))) {
        QSqlQuery createSchemaQuery(d->mTracksDatabase);

        const auto &result = createSchemaQuery.exec(QStringLiteral("CREATE TABLE `DiscoverSource` (`ID` INTEGER PRIMARY KEY NOT NULL, "
                                                                   "`Name` VARCHAR(55) NOT NULL, "
                                                                   "UNIQUE (`Name`))"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    if (!listTables.contains(QStringLiteral("Artists"))) {
        QSqlQuery createSchemaQuery(d->mTracksDatabase);

        const auto &result = createSchemaQuery.exec(QStringLiteral("CREATE TABLE `Artists` (`ID` INTEGER PRIMARY KEY NOT NULL, "
                                                                   "`Name` VARCHAR(55) NOT NULL, "
                                                                   "UNIQUE (`Name`))"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    if (!listTables.contains(QStringLiteral("Composer"))) {
        QSqlQuery createSchemaQuery(d->mTracksDatabase);

        const auto &result = createSchemaQuery.exec(QStringLiteral("CREATE TABLE `Composer` (`ID` INTEGER PRIMARY KEY NOT NULL, "
                                                                   "`Name` VARCHAR(55) NOT NULL, "
                                                                   "UNIQUE (`Name`))"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    if (!listTables.contains(QStringLiteral("Genre"))) {
        QSqlQuery createSchemaQuery(d->mTracksDatabase);

        const auto &result = createSchemaQuery.exec(QStringLiteral("CREATE TABLE `Genre` (`ID` INTEGER PRIMARY KEY NOT NULL, "
                                                                   "`Name` VARCHAR(85) NOT NULL, "
                                                                   "UNIQUE (`Name`))"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    if (!listTables.contains(QStringLiteral("Lyricist"))) {
        QSqlQuery createSchemaQuery(d->mTracksDatabase);

        const auto &result = createSchemaQuery.exec(QStringLiteral("CREATE TABLE `Lyricist` (`ID` INTEGER PRIMARY KEY NOT NULL, "
                                                                   "`Name` VARCHAR(55) NOT NULL, "
                                                                   "UNIQUE (`Name`))"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    if (!listTables.contains(QStringLiteral("Albums"))) {
        QSqlQuery createSchemaQuery(d->mTracksDatabase);

        const auto &result = createSchemaQuery.exec(QStringLiteral("CREATE TABLE `Albums` ("
                                                                   "`ID` INTEGER PRIMARY KEY NOT NULL, "
                                                                   "`Title` VARCHAR(55) NOT NULL, "
                                                                   "`ArtistName` VARCHAR(55), "
                                                                   "`AlbumPath` VARCHAR(255) NOT NULL, "
                                                                   "`CoverFileName` VARCHAR(255) NOT NULL, "
                                                                   "`AlbumInternalID` VARCHAR(55), "
                                                                   "UNIQUE (`Title`, `ArtistName`, `AlbumPath`), "
                                                                   "CONSTRAINT fk_artists FOREIGN KEY (`ArtistName`) REFERENCES `Artists`(`Name`) "
                                                                   "ON DELETE CASCADE)"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastError();
        }
    }

    if (!listTables.contains(QStringLiteral("Tracks"))) {
        QSqlQuery createSchemaQuery(d->mTracksDatabase);

        const auto &result = createSchemaQuery.exec(QStringLiteral("CREATE TABLE `Tracks` ("
                                                                   "`ID` INTEGER PRIMARY KEY NOT NULL, "
                                                                   "`Title` VARCHAR(85) NOT NULL, "
                                                                   "`ArtistName` VARCHAR(55), "
                                                                   "`AlbumTitle` VARCHAR(55), "
                                                                   "`AlbumArtistName` VARCHAR(55), "
                                                                   "`AlbumPath` VARCHAR(255), "
                                                                   "`TrackNumber` INTEGER DEFAULT -1, "
                                                                   "`DiscNumber` INTEGER DEFAULT -1, "
                                                                   "`Duration` INTEGER NOT NULL, "
                                                                   "`Rating` INTEGER NOT NULL DEFAULT 0, "
                                                                   "`GenreID` INTEGER DEFAULT -1, "
                                                                   "`ComposerID` INTEGER, "
                                                                   "`LyricistID` INTEGER, "
                                                                   "`Comment` VARCHAR(85) DEFAULT '', "
                                                                   "`Year` INTEGER DEFAULT 0, "
                                                                   "`Channels` INTEGER DEFAULT -1, "
                                                                   "`BitRate` INTEGER DEFAULT -1, "
                                                                   "`SampleRate` INTEGER DEFAULT -1, "
                                                                   "UNIQUE ("
                                                                   "`Title`, `AlbumTitle`, `AlbumArtistName`, "
                                                                   "`AlbumPath`, `TrackNumber`, `DiscNumber`"
                                                                   "), "
                                                                   "CONSTRAINT fk_artist FOREIGN KEY (`ArtistName`) REFERENCES `Artists`(`Name`), "
                                                                   "CONSTRAINT fk_tracks_composer FOREIGN KEY (`ComposerID`) REFERENCES `Composer`(`ID`), "
                                                                   "CONSTRAINT fk_tracks_lyricist FOREIGN KEY (`LyricistID`) REFERENCES `Lyricist`(`ID`), "
                                                                   "CONSTRAINT fk_tracks_genre FOREIGN KEY (`GenreID`) REFERENCES `Genre`(`ID`), "
                                                                   "CONSTRAINT fk_tracks_album FOREIGN KEY ("
                                                                   "`AlbumTitle`, `AlbumArtistName`, `AlbumPath`)"
                                                                   "REFERENCES `Albums`(`Title`, `ArtistName`, `AlbumPath`))"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    if (!listTables.contains(QStringLiteral("TracksMapping"))) {
        QSqlQuery createSchemaQuery(d->mTracksDatabase);

        const auto &result = createSchemaQuery.exec(QStringLiteral("CREATE TABLE `TracksMapping` ("
                                                                   "`TrackID` INTEGER NULL, "
                                                                   "`DiscoverID` INTEGER NOT NULL, "
                                                                   "`FileName` VARCHAR(255) NOT NULL, "
                                                                   "`Priority` INTEGER NOT NULL, "
                                                                   "`FileModifiedTime` DATETIME NOT NULL, "
                                                                   "PRIMARY KEY (`FileName`), "
                                                                   "CONSTRAINT TracksUnique UNIQUE (`TrackID`, `Priority`), "
                                                                   "CONSTRAINT fk_tracksmapping_trackID FOREIGN KEY (`TrackID`) REFERENCES `Tracks`(`ID`) ON DELETE CASCADE, "
                                                                   "CONSTRAINT fk_tracksmapping_discoverID FOREIGN KEY (`DiscoverID`) REFERENCES `DiscoverSource`(`ID`))"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createSchemaQuery.lastError();
        }
    }

    {
        QSqlQuery createTrackIndex(d->mTracksDatabase);

        const auto &result = createTrackIndex.exec(QStringLiteral("CREATE INDEX "
                                                                  "IF NOT EXISTS "
                                                                  "`TitleAlbumsIndex` ON `Albums` "
                                                                  "(`Title`)"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createTrackIndex.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createTrackIndex.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        QSqlQuery createTrackIndex(d->mTracksDatabase);

        const auto &result = createTrackIndex.exec(QStringLiteral("CREATE INDEX "
                                                                  "IF NOT EXISTS "
                                                                  "`ArtistNameAlbumsIndex` ON `Albums` "
                                                                  "(`ArtistName`)"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createTrackIndex.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createTrackIndex.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        QSqlQuery createTrackIndex(d->mTracksDatabase);

        const auto &result = createTrackIndex.exec(QStringLiteral("CREATE INDEX "
                                                                  "IF NOT EXISTS "
                                                                  "`TracksAlbumIndex` ON `Tracks` "
                                                                  "(`AlbumTitle`, `AlbumArtistName`, `AlbumPath`)"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createTrackIndex.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createTrackIndex.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        QSqlQuery createTrackIndex(d->mTracksDatabase);

        const auto &result = createTrackIndex.exec(QStringLiteral("CREATE INDEX "
                                                                  "IF NOT EXISTS "
                                                                  "`TracksFileNameIndex` ON `TracksMapping` "
                                                                  "(`FileName`)"));

        if (!result) {
            qDebug() << "DatabaseInterface::initDatabase" << createTrackIndex.lastQuery();
            qDebug() << "DatabaseInterface::initDatabase" << createTrackIndex.lastError();

            Q_EMIT databaseError();
        }
    }

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return;
    }
}

void DatabaseInterface::initRequest()
{
    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return;
    }

    {
        auto selectAlbumQueryText = QStringLiteral("SELECT "
                                                   "album.`ID`, "
                                                   "album.`Title`, "
                                                   "album.`AlbumInternalID`, "
                                                   "album.`ArtistName`, "
                                                   "album.`AlbumPath`, "
                                                   "album.`CoverFileName`, "
                                                   "("
                                                   "SELECT "
                                                   "COUNT(*) "
                                                   "FROM "
                                                   "`Tracks` tracks "
                                                   "WHERE "
                                                   "tracks.`AlbumTitle` = album.`Title` AND "
                                                   "(tracks.`AlbumArtistName` = album.`ArtistName` OR "
                                                   "(tracks.`AlbumArtistName` IS NULL AND "
                                                   "album.`ArtistName` IS NULL"
                                                   ")"
                                                   ") AND "
                                                   "tracks.`AlbumPath` = album.`AlbumPath` "
                                                   ") as `TracksCount`, "
                                                   "("
                                                   "SELECT "
                                                   "COUNT(DISTINCT tracks2.DiscNumber) <= 1 "
                                                   "FROM "
                                                   "`Tracks` tracks2 "
                                                   "WHERE "
                                                   "tracks2.`AlbumTitle` = album.`Title` AND "
                                                   "(tracks2.`AlbumArtistName` = album.`ArtistName` OR "
                                                   "(tracks2.`AlbumArtistName` IS NULL AND "
                                                   "album.`ArtistName` IS NULL"
                                                   ")"
                                                   ") AND "
                                                   "tracks2.`AlbumPath` = album.`AlbumPath` "
                                                   ") as `IsSingleDiscAlbum` "
                                                   "FROM `Albums` album "
                                                   "WHERE "
                                                   "album.`ID` = :albumId");

        auto result = prepareQuery(d->mSelectAlbumQuery, selectAlbumQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAlbumQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAlbumQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAllAlbumsText = QStringLiteral("SELECT "
                                                  "album.`ID`, "
                                                  "album.`Title`, "
                                                  "album.`AlbumInternalID`, "
                                                  "album.`ArtistName`, "
                                                  "album.`AlbumPath`, "
                                                  "album.`CoverFileName`, "
                                                  "("
                                                  "SELECT "
                                                  "COUNT(*) "
                                                  "FROM "
                                                  "`Tracks` tracks "
                                                  "WHERE "
                                                  "tracks.`AlbumTitle` = album.`Title` AND "
                                                  "(tracks.`AlbumArtistName` = album.`ArtistName` OR "
                                                  "(tracks.`AlbumArtistName` IS NULL AND "
                                                  "album.`ArtistName` IS NULL"
                                                  ")"
                                                  ") AND "
                                                  "tracks.`AlbumPath` = album.`AlbumPath` "
                                                  ") as `TracksCount`, "
                                                  "("
                                                  "SELECT "
                                                  "COUNT(DISTINCT tracks2.DiscNumber) <= 1 "
                                                  "FROM "
                                                  "`Tracks` tracks2 "
                                                  "WHERE "
                                                  "tracks2.`AlbumTitle` = album.`Title` AND "
                                                  "(tracks2.`AlbumArtistName` = album.`ArtistName` OR "
                                                  "(tracks2.`AlbumArtistName` IS NULL AND "
                                                  "album.`ArtistName` IS NULL"
                                                  ")"
                                                  ") AND "
                                                  "tracks2.`AlbumPath` = album.`AlbumPath` "
                                                  ") as `IsSingleDiscAlbum` "
                                                  "FROM `Albums` album "
                                                  "ORDER BY album.`Title` COLLATE NOCASE");

        auto result = prepareQuery(d->mSelectAllAlbumsQuery, selectAllAlbumsText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllAlbumsQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllAlbumsQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAllGenresText = QStringLiteral("SELECT "
                                                  "genre.`ID`, "
                                                  "genre.`Name` "
                                                  "FROM `Genre` genre "
                                                  "ORDER BY genre.`Name` COLLATE NOCASE");

        auto result = prepareQuery(d->mSelectAllGenresQuery, selectAllGenresText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllAlbumsQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllAlbumsQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAllAlbumsText = QStringLiteral("SELECT "
                                                  "album.`ID`, "
                                                  "album.`Title`, "
                                                  "album.`ArtistName` "
                                                  "FROM `Albums` album "
                                                  "ORDER BY album.`Title` COLLATE NOCASE");

        auto result = prepareQuery(d->mSelectAllAlbumsShortQuery, selectAllAlbumsText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllAlbumsShortQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllAlbumsShortQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAllArtistsWithFilterText = QStringLiteral("SELECT `ID`, "
                                                             "`Name` "
                                                             "FROM `Artists` "
                                                             "ORDER BY `Name` COLLATE NOCASE");

        auto result = prepareQuery(d->mSelectAllArtistsQuery, selectAllArtistsWithFilterText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllArtistsQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllArtistsQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAllComposersWithFilterText = QStringLiteral("SELECT `ID`, "
                                                               "`Name` "
                                                               "FROM `Artists` "
                                                               "ORDER BY `Name` COLLATE NOCASE");

        auto result = prepareQuery(d->mSelectAllComposersQuery, selectAllComposersWithFilterText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllComposersQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllComposersQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAllLyricistsWithFilterText = QStringLiteral("SELECT `ID`, "
                                                               "`Name` "
                                                               "FROM `Lyricist` "
                                                               "ORDER BY `Name` COLLATE NOCASE");

        auto result = prepareQuery(d->mSelectAllLyricistsQuery, selectAllLyricistsWithFilterText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllLyricistsQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllLyricistsQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAllTracksText = QStringLiteral("SELECT "
                                                  "tracks.`ID`, "
                                                  "tracks.`Title`, "
                                                  "album.`ID`, "
                                                  "tracks.`ArtistName`, "
                                                  "tracks.`AlbumArtistName`, "
                                                  "tracksMapping.`FileName`, "
                                                  "tracksMapping.`FileModifiedTime`, "
                                                  "tracks.`TrackNumber`, "
                                                  "tracks.`DiscNumber`, "
                                                  "tracks.`Duration`, "
                                                  "tracks.`AlbumTitle`, "
                                                  "tracks.`Rating`, "
                                                  "album.`CoverFileName`, "
                                                  "("
                                                  "SELECT "
                                                  "COUNT(DISTINCT tracks2.DiscNumber) <= 1 "
                                                  "FROM "
                                                  "`Tracks` tracks2 "
                                                  "WHERE "
                                                  "tracks2.`AlbumTitle` = album.`Title` AND "
                                                  "(tracks2.`AlbumArtistName` = album.`ArtistName` OR "
                                                  "(tracks2.`AlbumArtistName` IS NULL AND "
                                                  "album.`ArtistName` IS NULL"
                                                  ")"
                                                  ") AND "
                                                  "tracks2.`AlbumPath` = album.`AlbumPath` "
                                                  ") as `IsSingleDiscAlbum`, "
                                                  "trackGenre.`Name`, "
                                                  "trackComposer.`Name`, "
                                                  "trackLyricist.`Name`, "
                                                  "tracks.`Comment`, "
                                                  "tracks.`Year`, "
                                                  "tracks.`Channels`, "
                                                  "tracks.`BitRate`, "
                                                  "tracks.`SampleRate` "
                                                  "FROM "
                                                  "`Tracks` tracks, "
                                                  "`TracksMapping` tracksMapping "
                                                  "LEFT JOIN "
                                                  "`Albums` album "
                                                  "ON "
                                                  "tracks.`AlbumTitle` = album.`Title` AND "
                                                  "(tracks.`AlbumArtistName` = album.`ArtistName` OR tracks.`AlbumArtistName` IS NULL ) AND "
                                                  "tracks.`AlbumPath` = album.`AlbumPath` "
                                                  "LEFT JOIN `Genre` trackGenre ON trackGenre.`ID` = tracks.`GenreID` "
                                                  "LEFT JOIN `Composer` trackComposer ON trackComposer.`ID` = tracks.`ComposerID` "
                                                  "LEFT JOIN `Lyricist` trackLyricist ON trackLyricist.`ID` = tracks.`LyricistID` "
                                                  "WHERE "
                                                  "tracksMapping.`TrackID` = tracks.`ID` AND "
                                                  "tracksMapping.`Priority` = (SELECT MIN(`Priority`) FROM `TracksMapping` WHERE `TrackID` = tracks.`ID`)");

        auto result = prepareQuery(d->mSelectAllTracksQuery, selectAllTracksText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllTracksQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllTracksQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAllTracksShortText = QStringLiteral("SELECT "
                                                       "tracks.`ID`, "
                                                       "tracks.`Title`, "
                                                       "tracks.`AlbumArtistName` "
                                                       "FROM "
                                                       "`Tracks` tracks ");

        auto result = prepareQuery(d->mSelectAllTracksShortQuery, selectAllTracksShortText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllTracksShortQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllTracksShortQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAllTracksFromSourceQueryText = QStringLiteral("SELECT "
                                                                 "tracks.`ID`, "
                                                                 "tracks.`Title`, "
                                                                 "album.`ID`, "
                                                                 "tracks.`ArtistName`, "
                                                                 "tracks.`AlbumArtistName`, "
                                                                 "tracksMapping.`FileName`, "
                                                                 "tracksMapping.`FileModifiedTime`, "
                                                                 "tracks.`TrackNumber`, "
                                                                 "tracks.`DiscNumber`, "
                                                                 "tracks.`Duration`, "
                                                                 "tracks.`AlbumTitle`, "
                                                                 "tracks.`Rating`, "
                                                                 "album.`CoverFileName`, "
                                                                 "("
                                                                 "SELECT "
                                                                 "COUNT(DISTINCT tracks2.DiscNumber) <= 1 "
                                                                 "FROM "
                                                                 "`Tracks` tracks2 "
                                                                 "WHERE "
                                                                 "tracks2.`AlbumTitle` = album.`Title` AND "
                                                                 "(tracks2.`AlbumArtistName` = album.`ArtistName` OR "
                                                                 "(tracks2.`AlbumArtistName` IS NULL AND "
                                                                 "album.`ArtistName` IS NULL"
                                                                 ")"
                                                                 ") AND "
                                                                 "tracks2.`AlbumPath` = album.`AlbumPath` "
                                                                 ") as `IsSingleDiscAlbum`, "
                                                                 "trackGenre.`Name`, "
                                                                 "trackComposer.`Name`, "
                                                                 "trackLyricist.`Name`, "
                                                                 "tracks.`Comment`, "
                                                                 "tracks.`Year`, "
                                                                 "tracks.`Channels`, "
                                                                 "tracks.`BitRate`, "
                                                                 "tracks.`SampleRate` "
                                                                 "FROM "
                                                                 "`Tracks` tracks, "
                                                                 "`TracksMapping` tracksMapping, "
                                                                 "`DiscoverSource` source "
                                                                 "LEFT JOIN "
                                                                 "`Albums` album "
                                                                 "ON "
                                                                 "tracks.`AlbumTitle` = album.`Title` AND "
                                                                 "(tracks.`AlbumArtistName` = album.`ArtistName` OR tracks.`AlbumArtistName` IS NULL ) AND "
                                                                 "tracks.`AlbumPath` = album.`AlbumPath` "
                                                                 "LEFT JOIN `Composer` trackComposer ON trackComposer.`ID` = tracks.`ComposerID` "
                                                                 "LEFT JOIN `Lyricist` trackLyricist ON trackLyricist.`ID` = tracks.`LyricistID` "
                                                                 "LEFT JOIN `Genre` trackGenre ON trackGenre.`ID` = tracks.`GenreID` "
                                                                 "WHERE "
                                                                 "source.`Name` = :source AND "
                                                                 "source.`ID` = tracksMapping.`DiscoverID` AND "
                                                                 "tracksMapping.`TrackID` = tracks.`ID` AND "
                                                                 "tracksMapping.`Priority` = (SELECT MIN(`Priority`) FROM `TracksMapping` WHERE `TrackID` = tracks.`ID`)");

        auto result = prepareQuery(d->mSelectAllTracksFromSourceQuery, selectAllTracksFromSourceQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllTracksFromSourceQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllTracksFromSourceQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectArtistByNameText = QStringLiteral("SELECT `ID`, "
                                                     "`Name` "
                                                     "FROM `Artists` "
                                                     "WHERE "
                                                     "`Name` = :name");

        auto result = prepareQuery(d->mSelectArtistByNameQuery, selectArtistByNameText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectArtistByNameQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectArtistByNameQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectComposerByNameText = QStringLiteral("SELECT `ID`, "
                                                       "`Name` "
                                                       "FROM `Composer` "
                                                       "WHERE "
                                                       "`Name` = :name");

        auto result = prepareQuery(d->mSelectComposerByNameQuery, selectComposerByNameText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectComposerByNameQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectComposerByNameQuery.lastError();
        }
    }

    {
        auto selectLyricistByNameText = QStringLiteral("SELECT `ID`, "
                                                       "`Name` "
                                                       "FROM `Lyricist` "
                                                       "WHERE "
                                                       "`Name` = :name");

        auto result = prepareQuery(d->mSelectLyricistByNameQuery, selectLyricistByNameText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectLyricistByNameQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectLyricistByNameQuery.lastError();
        }
    }

    {
        auto selectGenreByNameText = QStringLiteral("SELECT `ID`, "
                                                    "`Name` "
                                                    "FROM `Genre` "
                                                    "WHERE "
                                                    "`Name` = :name");

        auto result = prepareQuery(d->mSelectGenreByNameQuery, selectGenreByNameText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectGenreByNameQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectGenreByNameQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto insertArtistsText = QStringLiteral("INSERT INTO `Artists` (`ID`, `Name`) "
                                                "VALUES (:artistId, :name)");

        auto result = prepareQuery(d->mInsertArtistsQuery, insertArtistsText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertArtistsQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertArtistsQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto insertGenreText = QStringLiteral("INSERT INTO `Genre` (`ID`, `Name`) "
                                              "VALUES (:genreId, :name)");

        auto result = prepareQuery(d->mInsertGenreQuery, insertGenreText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertGenreQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertGenreQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto insertComposerText = QStringLiteral("INSERT INTO `Composer` (`ID`, `Name`) "
                                                 "VALUES (:composerId, :name)");

        auto result = prepareQuery(d->mInsertComposerQuery, insertComposerText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertComposerQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertComposerQuery.lastError();
        }
    }

    {
        auto insertLyricistText = QStringLiteral("INSERT INTO `Lyricist` (`ID`, `Name`) "
                                                 "VALUES (:lyricistId, :name)");

        auto result = prepareQuery(d->mInsertLyricistQuery, insertLyricistText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertLyricistQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertLyricistQuery.lastError();
        }
    }

    {
        auto selectTrackQueryText = QStringLiteral("SELECT "
                                                   "tracks.`ID`, "
                                                   "tracks.`Title`, "
                                                   "album.`ID`, "
                                                   "tracks.`ArtistName`, "
                                                   "tracks.`AlbumArtistName`, "
                                                   "tracksMapping.`FileName`, "
                                                   "tracksMapping.`FileModifiedTime`, "
                                                   "tracks.`TrackNumber`, "
                                                   "tracks.`DiscNumber`, "
                                                   "tracks.`Duration`, "
                                                   "tracks.`AlbumTitle`, "
                                                   "tracks.`Rating`, "
                                                   "album.`CoverFileName`, "
                                                   "("
                                                   "SELECT "
                                                   "COUNT(DISTINCT tracks2.DiscNumber) <= 1 "
                                                   "FROM "
                                                   "`Tracks` tracks2 "
                                                   "WHERE "
                                                   "tracks2.`AlbumTitle` = album.`Title` AND "
                                                   "(tracks2.`AlbumArtistName` = album.`ArtistName` OR "
                                                   "(tracks2.`AlbumArtistName` IS NULL AND "
                                                   "album.`ArtistName` IS NULL"
                                                   ")"
                                                   ") AND "
                                                   "tracks2.`AlbumPath` = album.`AlbumPath` "
                                                   ") as `IsSingleDiscAlbum`, "
                                                   "trackGenre.`Name`, "
                                                   "trackComposer.`Name`, "
                                                   "trackLyricist.`Name`, "
                                                   "tracks.`Comment`, "
                                                   "tracks.`Year`, "
                                                   "tracks.`Channels`, "
                                                   "tracks.`BitRate`, "
                                                   "tracks.`SampleRate` "
                                                   "FROM "
                                                   "`Tracks` tracks, "
                                                   "`TracksMapping` tracksMapping "
                                                   "LEFT JOIN "
                                                   "`Albums` album "
                                                   "ON "
                                                   "tracks.`AlbumTitle` = album.`Title` AND "
                                                   "(tracks.`AlbumArtistName` = album.`ArtistName` OR tracks.`AlbumArtistName` IS NULL ) AND "
                                                   "tracks.`AlbumPath` = album.`AlbumPath` "
                                                   "LEFT JOIN `Composer` trackComposer ON trackComposer.`ID` = tracks.`ComposerID` "
                                                   "LEFT JOIN `Lyricist` trackLyricist ON trackLyricist.`ID` = tracks.`LyricistID` "
                                                   "LEFT JOIN `Genre` trackGenre ON trackGenre.`ID` = tracks.`GenreID` "
                                                   "WHERE "
                                                   "tracksMapping.`TrackID` = tracks.`ID` AND "
                                                   "album.`ID` = :albumId AND "
                                                   "tracksMapping.`Priority` = (SELECT MIN(`Priority`) FROM `TracksMapping` WHERE `TrackID` = tracks.`ID`) "
                                                   "ORDER BY tracks.`DiscNumber` ASC, "
                                                   "tracks.`TrackNumber` ASC");

        auto result = prepareQuery(d->mSelectTrackQuery, selectTrackQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTrackQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTrackQuery.lastError();

            Q_EMIT databaseError();
        }
    }
    {
        auto selectTrackFromIdQueryText = QStringLiteral("SELECT "
                                                         "tracks.`Id`, "
                                                         "tracks.`Title`, "
                                                         "album.`ID`, "
                                                         "tracks.`ArtistName`, "
                                                         "tracks.`AlbumArtistName`, "
                                                         "tracksMapping.`FileName`, "
                                                         "tracksMapping.`FileModifiedTime`, "
                                                         "tracks.`TrackNumber`, "
                                                         "tracks.`DiscNumber`, "
                                                         "tracks.`Duration`, "
                                                         "tracks.`AlbumTitle`, "
                                                         "tracks.`Rating`, "
                                                         "album.`CoverFileName`, "
                                                         "("
                                                         "SELECT "
                                                         "COUNT(DISTINCT tracks2.DiscNumber) <= 1 "
                                                         "FROM "
                                                         "`Tracks` tracks2 "
                                                         "WHERE "
                                                         "tracks2.`AlbumTitle` = album.`Title` AND "
                                                         "(tracks2.`AlbumArtistName` = album.`ArtistName` OR "
                                                         "(tracks2.`AlbumArtistName` IS NULL AND "
                                                         "album.`ArtistName` IS NULL"
                                                         ")"
                                                         ") AND "
                                                         "tracks2.`AlbumPath` = album.`AlbumPath` "
                                                         ") as `IsSingleDiscAlbum`, "
                                                         "trackGenre.`Name`, "
                                                         "trackComposer.`Name`, "
                                                         "trackLyricist.`Name`, "
                                                         "tracks.`Comment`, "
                                                         "tracks.`Year`, "
                                                         "tracks.`Channels`, "
                                                         "tracks.`BitRate`, "
                                                         "tracks.`SampleRate` "
                                                         "FROM "
                                                         "`Tracks` tracks, "
                                                         "`TracksMapping` tracksMapping "
                                                         "LEFT JOIN "
                                                         "`Albums` album "
                                                         "ON "
                                                         "tracks.`AlbumTitle` = album.`Title` AND "
                                                         "(tracks.`AlbumArtistName` = album.`ArtistName` OR tracks.`AlbumArtistName` IS NULL ) AND "
                                                         "tracks.`AlbumPath` = album.`AlbumPath` "
                                                         "LEFT JOIN `Composer` trackComposer ON trackComposer.`ID` = tracks.`ComposerID` "
                                                         "LEFT JOIN `Lyricist` trackLyricist ON trackLyricist.`ID` = tracks.`LyricistID` "
                                                         "LEFT JOIN `Genre` trackGenre ON trackGenre.`ID` = tracks.`GenreID` "
                                                         "WHERE "
                                                         "tracks.`ID` = :trackId AND "
                                                         "tracksMapping.`TrackID` = tracks.`ID` AND "
                                                         "tracksMapping.`Priority` = (SELECT MIN(`Priority`) FROM `TracksMapping` WHERE `TrackID` = tracks.`ID`)");

        auto result = prepareQuery(d->mSelectTrackFromIdQuery, selectTrackFromIdQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTrackFromIdQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTrackFromIdQuery.lastError();

            Q_EMIT databaseError();
        }
    }
    {
        auto selectCountAlbumsQueryText = QStringLiteral("SELECT count(*) "
                                                         "FROM `Albums` album "
                                                         "WHERE album.`ArtistName` = :artistName ");

        const auto result = prepareQuery(d->mSelectCountAlbumsForArtistQuery, selectCountAlbumsQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectCountAlbumsForArtistQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectCountAlbumsForArtistQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectGenreForArtistQueryText = QStringLiteral("SELECT DISTINCT trackGenre.`Name` "
                                                            "FROM "
                                                            "`Tracks` tracks "
                                                            "LEFT JOIN "
                                                            "`Albums` album "
                                                            "ON "
                                                            "tracks.`AlbumTitle` = album.`Title` AND "
                                                            "(tracks.`AlbumArtistName` = album.`ArtistName` OR tracks.`AlbumArtistName` IS NULL ) AND "
                                                            "tracks.`AlbumPath` = album.`AlbumPath` "
                                                            "LEFT JOIN `Genre` trackGenre ON trackGenre.`ID` = tracks.`GenreID` "
                                                            "WHERE "
                                                            "album.`ArtistName` = :artistName");

        const auto result = prepareQuery(d->mSelectGenreForArtistQuery, selectGenreForArtistQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectGenreForArtistQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectGenreForArtistQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectGenreForAlbumQueryText = QStringLiteral("SELECT DISTINCT trackGenre.`Name` "
                                                           "FROM "
                                                           "`Tracks` tracks "
                                                           "LEFT JOIN "
                                                           "`Albums` album "
                                                           "ON "
                                                           "tracks.`AlbumTitle` = album.`Title` AND "
                                                           "(tracks.`AlbumArtistName` = album.`ArtistName` OR tracks.`AlbumArtistName` IS NULL ) AND "
                                                           "tracks.`AlbumPath` = album.`AlbumPath` "
                                                           "LEFT JOIN `Genre` trackGenre ON trackGenre.`ID` = tracks.`GenreID` "
                                                           "WHERE "
                                                           "album.`ID` = :albumId");

        const auto result = prepareQuery(d->mSelectGenreForAlbumQuery, selectGenreForAlbumQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectGenreForAlbumQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectGenreForAlbumQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectCountAlbumsQueryText = QStringLiteral("SELECT distinct count(album.`ID`) "
                                                         "FROM "
                                                         "`Tracks` tracks, "
                                                         "`Albums` album "
                                                         "LEFT JOIN `Composer` albumComposer ON albumComposer.`ID` = tracks.`ComposerID` "
                                                         "WHERE "
                                                         "(tracks.`AlbumTitle` = album.`Title` OR tracks.`AlbumTitle` IS NULL ) AND "
                                                         "(tracks.`AlbumArtistName` = album.`ArtistName` OR tracks.`AlbumArtistName` IS NULL ) AND "
                                                         "(tracks.`AlbumPath` = album.`AlbumPath` OR tracks.`AlbumPath` IS NULL ) AND "
                                                         "albumComposer.`Name` = :artistName");

        const auto result = prepareQuery(d->mSelectCountAlbumsForComposerQuery, selectCountAlbumsQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectCountAlbumsForComposerQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectCountAlbumsForComposerQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectCountAlbumsQueryText = QStringLiteral("SELECT distinct count(album.`ID`) "
                                                         "FROM "
                                                         "`Tracks` tracks, "
                                                         "`Albums` album "
                                                         "LEFT JOIN `Lyricist` albumLyricist ON albumLyricist.`ID` = tracks.`LyricistID` "
                                                         "WHERE "
                                                         "(tracks.`AlbumTitle` = album.`Title` OR tracks.`AlbumTitle` IS NULL ) AND "
                                                         "(tracks.`AlbumArtistName` = album.`ArtistName` OR tracks.`AlbumArtistName` IS NULL ) AND "
                                                         "(tracks.`AlbumPath` = album.`AlbumPath` OR tracks.`AlbumPath` IS NULL ) AND "
                                                         "albumLyricist.`Name` = :artistName");

        const auto result = prepareQuery(d->mSelectCountAlbumsForLyricistQuery, selectCountAlbumsQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectCountAlbumsForLyricistQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectCountAlbumsForLyricistQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAlbumIdFromTitleQueryText = QStringLiteral("SELECT "
                                                              "album.`ID` "
                                                              "FROM "
                                                              "`Albums` album "
                                                              "WHERE "
                                                              "album.`ArtistName` = :artistName AND "
                                                              "album.`Title` = :title");

        auto result = prepareQuery(d->mSelectAlbumIdFromTitleQuery, selectAlbumIdFromTitleQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAlbumIdFromTitleQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAlbumIdFromTitleQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAlbumIdFromTitleAndArtistQueryText = QStringLiteral("SELECT "
                                                                       "album.`ID` "
                                                                       "FROM "
                                                                       "`Albums` album, "
                                                                       "`Artists` albumArtist "
                                                                       "WHERE "
                                                                       "album.`ArtistName` = albumArtist.`Name` AND "
                                                                       "album.`Title` = :title AND "
                                                                       "album.`AlbumPath` = :albumPath AND "
                                                                       "albumArtist.`ID` = :artistId");

        auto result = prepareQuery(d->mSelectAlbumIdFromTitleAndArtistQuery, selectAlbumIdFromTitleAndArtistQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAlbumIdFromTitleAndArtistQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAlbumIdFromTitleAndArtistQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAlbumIdFromTitleWithoutArtistQueryText = QStringLiteral("SELECT "
                                                                           "album.`ID` "
                                                                           "FROM "
                                                                           "`Albums` album "
                                                                           "WHERE "
                                                                           "album.`AlbumPath` = :albumPath AND "
                                                                           "album.`Title` = :title AND "
                                                                           "album.`ArtistName` IS NULL");

        auto result = prepareQuery(d->mSelectAlbumIdFromTitleWithoutArtistQuery, selectAlbumIdFromTitleWithoutArtistQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAlbumIdFromTitleWithoutArtistQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAlbumIdFromTitleWithoutArtistQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto insertAlbumQueryText = QStringLiteral("INSERT INTO `Albums` "
                                                   "(`ID`, "
                                                   "`Title`, "
                                                   "`ArtistName`, "
                                                   "`AlbumPath`, "
                                                   "`CoverFileName`) "
                                                   "VALUES "
                                                   "(:albumId, "
                                                   ":title, "
                                                   ":albumArtist, "
                                                   ":albumPath, "
                                                   ":coverFileName)");

        auto result = prepareQuery(d->mInsertAlbumQuery, insertAlbumQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertAlbumQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertAlbumQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto insertTrackMappingQueryText = QStringLiteral("INSERT INTO "
                                                          "`TracksMapping` "
                                                          "(`FileName`, "
                                                          "`DiscoverID`, "
                                                          "`Priority`, "
                                                          "`FileModifiedTime`) "
                                                          "VALUES (:fileName, :discoverId, :priority, :mtime)");

        auto result = prepareQuery(d->mInsertTrackMapping, insertTrackMappingQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertTrackMapping.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertTrackMapping.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto initialUpdateTracksValidityQueryText = QStringLiteral("UPDATE `TracksMapping` "
                                                                   "SET "
                                                                   "`TrackID` = :trackId, "
                                                                   "`Priority` = :priority, "
                                                                   "`FileModifiedTime` = :mtime "
                                                                   "WHERE `FileName` = :fileName");

        auto result = prepareQuery(d->mUpdateTrackMapping, initialUpdateTracksValidityQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mUpdateTrackMapping.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mUpdateTrackMapping.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto removeTracksMappingFromSourceQueryText = QStringLiteral("DELETE FROM `TracksMapping` "
                                                                     "WHERE `FileName` = :fileName AND `DiscoverID` = :sourceId");

        auto result = prepareQuery(d->mRemoveTracksMappingFromSource, removeTracksMappingFromSourceQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mRemoveTracksMappingFromSource.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mRemoveTracksMappingFromSource.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto removeTracksMappingQueryText = QStringLiteral("DELETE FROM `TracksMapping` "
                                                           "WHERE `FileName` = :fileName");

        auto result = prepareQuery(d->mRemoveTracksMapping, removeTracksMappingQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mRemoveTracksMapping.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mRemoveTracksMapping.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectTracksWithoutMappingQueryText = QStringLiteral("SELECT "
                                                                  "tracks.`Id`, "
                                                                  "tracks.`Title`, "
                                                                  "album.`ID`, "
                                                                  "tracks.`ArtistName`, "
                                                                  "tracks.`AlbumArtistName`, "
                                                                  "\"\" as FileName, "
                                                                  "NULL as FileModifiedTime, "
                                                                  "tracks.`TrackNumber`, "
                                                                  "tracks.`DiscNumber`, "
                                                                  "tracks.`Duration`, "
                                                                  "tracks.`AlbumTitle`, "
                                                                  "tracks.`Rating`, "
                                                                  "album.`CoverFileName`, "
                                                                  "("
                                                                  "SELECT "
                                                                  "COUNT(DISTINCT tracks2.DiscNumber) <= 1 "
                                                                  "FROM "
                                                                  "`Tracks` tracks2 "
                                                                  "WHERE "
                                                                  "tracks2.`AlbumTitle` = album.`Title` AND "
                                                                  "(tracks2.`AlbumArtistName` = album.`ArtistName` OR "
                                                                  "(tracks2.`AlbumArtistName` IS NULL AND "
                                                                  "album.`ArtistName` IS NULL"
                                                                  ")"
                                                                  ") AND "
                                                                  "tracks2.`AlbumPath` = album.`AlbumPath` "
                                                                  ") as `IsSingleDiscAlbum`, "
                                                                  "trackGenre.`Name`, "
                                                                  "trackComposer.`Name`, "
                                                                  "trackLyricist.`Name`, "
                                                                  "tracks.`Comment`, "
                                                                  "tracks.`Year`, "
                                                                  "tracks.`Channels`, "
                                                                  "tracks.`BitRate`, "
                                                                  "tracks.`SampleRate` "
                                                                  "FROM "
                                                                  "`Tracks` tracks "
                                                                  "LEFT JOIN "
                                                                  "`Albums` album "
                                                                  "ON "
                                                                  "tracks.`AlbumTitle` = album.`Title` AND "
                                                                  "(tracks.`AlbumArtistName` = album.`ArtistName` OR tracks.`AlbumArtistName` IS NULL ) AND "
                                                                  "tracks.`AlbumPath` = album.`AlbumPath` "
                                                                  "LEFT JOIN `Composer` trackComposer ON trackComposer.`ID` = tracks.`ComposerID` "
                                                                  "LEFT JOIN `Lyricist` trackLyricist ON trackLyricist.`ID` = tracks.`LyricistID` "
                                                                  "LEFT JOIN `Genre` trackGenre ON trackGenre.`ID` = tracks.`GenreID` "
                                                                  "WHERE "
                                                                  "tracks.`ID` NOT IN (SELECT tracksMapping2.`TrackID` FROM `TracksMapping` tracksMapping2)");

        auto result = prepareQuery(d->mSelectTracksWithoutMappingQuery, selectTracksWithoutMappingQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTracksWithoutMappingQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTracksWithoutMappingQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectTracksMappingQueryText = QStringLiteral("SELECT "
                                                           "`TrackID`, "
                                                           "`FileName`, "
                                                           "`DiscoverID`, "
                                                           "`Priority`, "
                                                           "`FileModifiedTime` "
                                                           "FROM "
                                                           "`TracksMapping` "
                                                           "WHERE "
                                                           "`FileName` = :fileName");

        auto result = prepareQuery(d->mSelectTracksMapping, selectTracksMappingQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTracksMapping.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTracksMapping.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectTracksMappingPriorityQueryText = QStringLiteral("SELECT "
                                                                   "`Priority` "
                                                                   "FROM "
                                                                   "`TracksMapping` "
                                                                   "WHERE "
                                                                   "`TrackID` = :trackId AND "
                                                                   "`FileName` = :fileName");

        auto result = prepareQuery(d->mSelectTracksMappingPriority, selectTracksMappingPriorityQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTracksMappingPriority.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTracksMappingPriority.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectTracksMappingPriorityQueryByTrackIdText = QStringLiteral("SELECT "
                                                                            "MAX(`Priority`) "
                                                                            "FROM "
                                                                            "`TracksMapping` "
                                                                            "WHERE "
                                                                            "`TrackID` = :trackId");

        auto result = prepareQuery(d->mSelectTracksMappingPriorityByTrackId, selectTracksMappingPriorityQueryByTrackIdText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTracksMappingPriorityByTrackId.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTracksMappingPriorityByTrackId.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAllTrackFilesFromSourceQueryText = QStringLiteral("SELECT "
                                                                     "tracksMapping.`FileName`, "
                                                                     "tracksMapping.`FileModifiedTime` "
                                                                     "FROM "
                                                                     "`TracksMapping` tracksMapping "
                                                                     "WHERE "
                                                                     "tracksMapping.`DiscoverID` = :discoverId");

        auto result = prepareQuery(d->mSelectAllTrackFilesFromSourceQuery, selectAllTrackFilesFromSourceQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllTrackFilesFromSourceQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAllTrackFilesFromSourceQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto insertMusicSourceQueryText = QStringLiteral("INSERT OR IGNORE INTO `DiscoverSource` (`ID`, `Name`) "
                                                         "VALUES (:discoverId, :name)");

        auto result = prepareQuery(d->mInsertMusicSource, insertMusicSourceQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertMusicSource.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertMusicSource.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectMusicSourceQueryText = QStringLiteral("SELECT `ID` FROM `DiscoverSource` WHERE `Name` = :name");

        auto result = prepareQuery(d->mSelectMusicSource, selectMusicSourceQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectMusicSource.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectMusicSource.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectTrackQueryText = QStringLiteral("SELECT "
                                                   "tracks.`ID`,  tracksMapping.`FileName` "
                                                   "FROM "
                                                   "`Tracks` tracks, "
                                                   "`Albums` album, "
                                                   "`TracksMapping` tracksMapping "
                                                   "WHERE "
                                                   "tracks.`Title` = :title AND "
                                                   "album.`ID` = :album AND "
                                                   "(tracks.`AlbumTitle` = album.`Title` OR tracks.`AlbumTitle` IS NULL ) AND "
                                                   "(tracks.`AlbumArtistName` = album.`ArtistName` OR tracks.`AlbumArtistName` IS NULL ) AND "
                                                   "(tracks.`AlbumPath` = album.`AlbumPath` OR tracks.`AlbumPath` IS NULL ) AND "
                                                   "tracks.`ArtistName` = :artist AND "
                                                   "tracksMapping.`TrackID` = tracks.`ID` AND "
                                                   "tracksMapping.`Priority` = (SELECT MIN(`Priority`) FROM `TracksMapping` WHERE `TrackID` = tracks.`ID`)");

        auto result = prepareQuery(d->mSelectTrackIdFromTitleAlbumIdArtistQuery, selectTrackQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTrackIdFromTitleAlbumIdArtistQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTrackIdFromTitleAlbumIdArtistQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto insertTrackQueryText = QStringLiteral("INSERT INTO `Tracks` "
                                                   "("
                                                   "`ID`, "
                                                   "`Title`, "
                                                   "`ArtistName`, "
                                                   "`AlbumTitle`, "
                                                   "`AlbumArtistName`, "
                                                   "`AlbumPath`, "
                                                   "`GenreID`, "
                                                   "`ComposerID`, "
                                                   "`LyricistID`, "
                                                   "`Comment`, "
                                                   "`TrackNumber`, "
                                                   "`DiscNumber`, "
                                                   "`Channels`, "
                                                   "`BitRate`, "
                                                   "`SampleRate`, "
                                                   "`Year`,  "
                                                   "`Duration`, "
                                                   "`Rating` ) "
                                                   "VALUES "
                                                   "("
                                                   ":trackId, "
                                                   ":title, "
                                                   ":artistName, "
                                                   ":albumTitle, "
                                                   ":albumArtistName, "
                                                   ":albumPath, "
                                                   ":genreId, "
                                                   ":composerId, "
                                                   ":lyricistId, "
                                                   ":comment, "
                                                   ":trackNumber, "
                                                   ":discNumber, "
                                                   ":channels, "
                                                   ":bitRate, "
                                                   ":sampleRate, "
                                                   ":year, "
                                                   ":trackDuration, "
                                                   ":trackRating)");

        auto result = prepareQuery(d->mInsertTrackQuery, insertTrackQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertTrackQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mInsertTrackQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto updateTrackQueryText = QStringLiteral("UPDATE `Tracks` "
                                                   "SET "
                                                   "`Title` = :title, "
                                                   "`ArtistName` = :artistName, "
                                                   "`AlbumTitle` = :albumTitle, "
                                                   "`AlbumArtistName` = :albumArtistName, "
                                                   "`AlbumPath` = :albumPath, "
                                                   "`GenreID` = :genreId, "
                                                   "`ComposerID` = :composerId, "
                                                   "`LyricistID` = :lyricistId, "
                                                   "`Comment` = :comment, "
                                                   "`TrackNumber` = :trackNumber, "
                                                   "`DiscNumber` = :discNumber, "
                                                   "`Channels` = :channels, "
                                                   "`BitRate` = :bitRate, "
                                                   "`SampleRate` = :sampleRate, "
                                                   "`Year` = :year, "
                                                   " `Duration` = :trackDuration, "
                                                   "`Rating` = :trackRating "
                                                   "WHERE "
                                                   "`ID` = :trackId");

        auto result = prepareQuery(d->mUpdateTrackQuery, updateTrackQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mUpdateTrackQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mUpdateTrackQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto updateAlbumArtistQueryText = QStringLiteral("UPDATE `Albums` "
                                                         "SET "
                                                         "`ArtistName` = :artistName "
                                                         "WHERE "
                                                         "`ID` = :albumId");

        auto result = prepareQuery(d->mUpdateAlbumArtistQuery, updateAlbumArtistQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mUpdateAlbumArtistQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mUpdateAlbumArtistQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto updateAlbumArtistInTracksQueryText = QStringLiteral("UPDATE `Tracks` "
                                                                 "SET "
                                                                 "`AlbumArtistName` = :artistName "
                                                                 "WHERE "
                                                                 "`AlbumTitle` = :albumTitle AND "
                                                                 "`AlbumPath` = :albumPath AND "
                                                                 "`AlbumArtistName` IS NULL");

        auto result = prepareQuery(d->mUpdateAlbumArtistInTracksQuery, updateAlbumArtistInTracksQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mUpdateAlbumArtistInTracksQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mUpdateAlbumArtistInTracksQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectTrackQueryText = QStringLiteral("SELECT "
                                                   "tracks.ID "
                                                   "FROM "
                                                   "`Tracks` tracks "
                                                   "WHERE "
                                                   "tracks.`Title` = :title AND "
                                                   "tracks.`AlbumTitle` = :album AND "
                                                   "tracks.`TrackNumber` = :trackNumber AND "
                                                   "tracks.`DiscNumber` = :discNumber AND "
                                                   "tracks.`ArtistName` = :artist");

        auto result = prepareQuery(d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery, selectTrackQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectTrackQueryText = QStringLiteral("SELECT "
                                                   "tracks.ID "
                                                   "FROM "
                                                   "`Tracks` tracks, "
                                                   "`Albums` albums "
                                                   "WHERE "
                                                   "tracks.`Title` = :title AND "
                                                   "tracks.`AlbumTitle` = albums.`Title` AND "
                                                   "tracks.`AlbumArtistName` = albums.`ArtistName` AND "
                                                   "tracks.`AlbumPath` = albums.`AlbumPath` AND "
                                                   "albums.`Title` = :album AND "
                                                   "albums.`AlbumPath` = :albumPath AND "
                                                   "tracks.`TrackNumber` = :trackNumber AND "
                                                   "tracks.`DiscNumber` = :discNumber AND "
                                                   "( "
                                                   "( albums.`ArtistName` IS NULL) OR "
                                                   "( albums.`ArtistName` = :albumArtist) "
                                                   ")");

        auto result = prepareQuery(d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery, selectTrackQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAlbumArtUriFromAlbumIdQueryText = QStringLiteral("SELECT `CoverFileName`"
                                                                    "FROM "
                                                                    "`Albums` "
                                                                    "WHERE "
                                                                    "`ID` = :albumId");

        auto result = prepareQuery(d->mSelectAlbumArtUriFromAlbumIdQuery, selectAlbumArtUriFromAlbumIdQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAlbumArtUriFromAlbumIdQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAlbumArtUriFromAlbumIdQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto updateAlbumArtUriFromAlbumIdQueryText = QStringLiteral("UPDATE `Albums` "
                                                                    "SET `CoverFileName` = :coverFileName "
                                                                    "WHERE "
                                                                    "`ID` = :albumId");

        auto result = prepareQuery(d->mUpdateAlbumArtUriFromAlbumIdQuery, updateAlbumArtUriFromAlbumIdQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mUpdateAlbumArtUriFromAlbumIdQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mUpdateAlbumArtUriFromAlbumIdQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectTracksFromArtistQueryText = QStringLiteral("SELECT "
                                                              "tracks.`ID`, "
                                                              "tracks.`Title`, "
                                                              "album.`ID`, "
                                                              "tracks.`ArtistName`, "
                                                              "tracks.`AlbumArtistName`, "
                                                              "tracksMapping.`FileName`, "
                                                              "tracksMapping.`FileModifiedTime`, "
                                                              "tracks.`TrackNumber`, "
                                                              "tracks.`DiscNumber`, "
                                                              "tracks.`Duration`, "
                                                              "tracks.`AlbumTitle`, "
                                                              "tracks.`Rating`, "
                                                              "album.`CoverFileName`, "
                                                              "("
                                                              "SELECT "
                                                              "COUNT(DISTINCT tracks2.DiscNumber) <= 1 "
                                                              "FROM "
                                                              "`Tracks` tracks2 "
                                                              "WHERE "
                                                              "tracks2.`AlbumTitle` = album.`Title` AND "
                                                              "(tracks2.`AlbumArtistName` = album.`ArtistName` OR "
                                                              "(tracks2.`AlbumArtistName` IS NULL AND "
                                                              "album.`ArtistName` IS NULL"
                                                              ")"
                                                              ") AND "
                                                              "tracks2.`AlbumPath` = album.`AlbumPath` "
                                                              ") as `IsSingleDiscAlbum`, "
                                                              "trackGenre.`Name`, "
                                                              "trackComposer.`Name`, "
                                                              "trackLyricist.`Name`, "
                                                              "tracks.`Comment`, "
                                                              "tracks.`Year`, "
                                                              "tracks.`Channels`, "
                                                              "tracks.`BitRate`, "
                                                              "tracks.`SampleRate` "
                                                              "FROM "
                                                              "`Tracks` tracks, "
                                                              "`TracksMapping` tracksMapping "
                                                              "LEFT JOIN "
                                                              "`Albums` album "
                                                              "ON "
                                                              "tracks.`AlbumTitle` = album.`Title` AND "
                                                              "(tracks.`AlbumArtistName` = album.`ArtistName` OR tracks.`AlbumArtistName` IS NULL ) AND "
                                                              "tracks.`AlbumPath` = album.`AlbumPath` "
                                                              "LEFT JOIN `Composer` trackComposer ON trackComposer.`ID` = tracks.`ComposerID` "
                                                              "LEFT JOIN `Lyricist` trackLyricist ON trackLyricist.`ID` = tracks.`LyricistID` "
                                                              "LEFT JOIN `Genre` trackGenre ON trackGenre.`ID` = tracks.`GenreID` "
                                                              "WHERE "
                                                              "tracks.`ArtistName` = :artistName AND "
                                                              "tracksMapping.`TrackID` = tracks.`ID` AND "
                                                              "tracksMapping.`Priority` = (SELECT MIN(`Priority`) FROM `TracksMapping` WHERE `TrackID` = tracks.`ID`) "
                                                              "ORDER BY tracks.`Title` ASC, "
                                                              "album.`Title` ASC");

        auto result = prepareQuery(d->mSelectTracksFromArtist, selectTracksFromArtistQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTracksFromArtist.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTracksFromArtist.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectAlbumIdsFromArtistQueryText = QStringLiteral("SELECT "
                                                                "album.`ID` "
                                                                "FROM "
                                                                "`Albums` album "
                                                                "WHERE "
                                                                "album.`ArtistName` = :artistName");

        auto result = prepareQuery(d->mSelectAlbumIdsFromArtist, selectAlbumIdsFromArtistQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAlbumIdsFromArtist.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectAlbumIdsFromArtist.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectArtistQueryText = QStringLiteral("SELECT `ID`, "
                                                    "`Name` "
                                                    "FROM `Artists` "
                                                    "WHERE "
                                                    "`ID` = :artistId");

        auto result = prepareQuery(d->mSelectArtistQuery, selectArtistQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectArtistQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectArtistQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectGenreQueryText = QStringLiteral("SELECT `ID`, "
                                                   "`Name` "
                                                   "FROM `Genre` "
                                                   "WHERE "
                                                   "`ID` = :genreId");

        auto result = prepareQuery(d->mSelectGenreQuery, selectGenreQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectGenreQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectGenreQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto selectComposerQueryText = QStringLiteral("SELECT `ID`, "
                                                      "`Name` "
                                                      "FROM `Composer` "
                                                      "WHERE "
                                                      "`ID` = :composerId");

        auto result = prepareQuery(d->mSelectComposerQuery, selectComposerQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectComposerQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectComposerQuery.lastError();
        }
    }

    {
        auto selectLyricistQueryText = QStringLiteral("SELECT `ID`, "
                                                      "`Name` "
                                                      "FROM `Lyricist` "
                                                      "WHERE "
                                                      "`ID` = :lyricistId");

        auto result = prepareQuery(d->mSelectLyricistQuery, selectLyricistQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectLyricistQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectLyricistQuery.lastError();
        }
    }

    {
        auto selectTrackFromFilePathQueryText = QStringLiteral("SELECT "
                                                               "tracks.`ID`, "
                                                               "tracks.`Title`, "
                                                               "album.`ID`, "
                                                               "tracks.`ArtistName`, "
                                                               "tracks.`AlbumArtistName`, "
                                                               "tracksMapping.`FileName`, "
                                                               "tracksMapping.`FileModifiedTime`, "
                                                               "tracks.`TrackNumber`, "
                                                               "tracks.`DiscNumber`, "
                                                               "tracks.`Duration`, "
                                                               "tracks.`AlbumTitle`, "
                                                               "tracks.`Rating`, "
                                                               "album.`CoverFileName`, "
                                                               "("
                                                               "SELECT "
                                                               "COUNT(DISTINCT tracks2.DiscNumber) <= 1 "
                                                               "FROM "
                                                               "`Tracks` tracks2 "
                                                               "WHERE "
                                                               "tracks2.`AlbumTitle` = album.`Title` AND "
                                                               "(tracks2.`AlbumArtistName` = album.`ArtistName` OR "
                                                               "(tracks2.`AlbumArtistName` IS NULL AND "
                                                               "album.`ArtistName` IS NULL"
                                                               ")"
                                                               ") AND "
                                                               "tracks2.`AlbumPath` = album.`AlbumPath` "
                                                               ") as `IsSingleDiscAlbum`, "
                                                               "trackGenre.`Name`, "
                                                               "trackComposer.`Name`, "
                                                               "trackLyricist.`Name`, "
                                                               "tracks.`Comment`, "
                                                               "tracks.`Year`, "
                                                               "tracks.`Channels`, "
                                                               "tracks.`BitRate`, "
                                                               "tracks.`SampleRate` "
                                                               "FROM "
                                                               "`Tracks` tracks, "
                                                               "`TracksMapping` tracksMapping "
                                                               "LEFT JOIN "
                                                               "`Albums` album "
                                                               "ON "
                                                               "tracks.`AlbumTitle` = album.`Title` AND "
                                                               "(tracks.`AlbumArtistName` = album.`ArtistName` OR tracks.`AlbumArtistName` IS NULL ) AND "
                                                               "tracks.`AlbumPath` = album.`AlbumPath` "
                                                               "LEFT JOIN `Composer` trackComposer ON trackComposer.`ID` = tracks.`ComposerID` "
                                                               "LEFT JOIN `Lyricist` trackLyricist ON trackLyricist.`ID` = tracks.`LyricistID` "
                                                               "LEFT JOIN `Genre` trackGenre ON trackGenre.`ID` = tracks.`GenreID` "
                                                               "WHERE "
                                                               "tracksMapping.`TrackID` = tracks.`ID` AND "
                                                               "tracksMapping.`FileName` = :filePath AND "
                                                               "tracksMapping.`Priority` = (SELECT MIN(`Priority`) FROM `TracksMapping` WHERE `TrackID` = tracks.`ID`)");

        auto result = prepareQuery(d->mSelectTrackFromFilePathQuery, selectTrackFromFilePathQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTrackFromFilePathQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mSelectTrackFromFilePathQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto removeTrackQueryText = QStringLiteral("DELETE FROM `Tracks` "
                                                   "WHERE "
                                                   "`ID` = :trackId");

        auto result = prepareQuery(d->mRemoveTrackQuery, removeTrackQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mRemoveTrackQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mRemoveTrackQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto removeAlbumQueryText = QStringLiteral("DELETE FROM `Albums` "
                                                   "WHERE "
                                                   "`ID` = :albumId");

        auto result = prepareQuery(d->mRemoveAlbumQuery, removeAlbumQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mRemoveAlbumQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mRemoveAlbumQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    {
        auto removeAlbumQueryText = QStringLiteral("DELETE FROM `Artists` "
                                                   "WHERE "
                                                   "`ID` = :artistId");

        auto result = prepareQuery(d->mRemoveArtistQuery, removeAlbumQueryText);

        if (!result) {
            qDebug() << "DatabaseInterface::initRequest" << d->mRemoveArtistQuery.lastQuery();
            qDebug() << "DatabaseInterface::initRequest" << d->mRemoveArtistQuery.lastError();

            Q_EMIT databaseError();
        }
    }

    finishTransaction();

    d->mInitFinished = true;
    Q_EMIT requestsInitDone();
}

qulonglong DatabaseInterface::insertAlbum(const QString &title, const QString &albumArtist, const QString &trackArtist,
                                          const QString &trackPath, const QUrl &albumArtURI)
{
    auto result = qulonglong(0);

    if (title.isEmpty()) {
        return result;
    }

    if (!albumArtist.isEmpty() || !trackArtist.isEmpty()) {
        d->mSelectAlbumIdFromTitleAndArtistQuery.bindValue(QStringLiteral(":title"), title);
        d->mSelectAlbumIdFromTitleAndArtistQuery.bindValue(QStringLiteral(":albumPath"), trackPath);
        if (!albumArtist.isEmpty()) {
            d->mSelectAlbumIdFromTitleAndArtistQuery.bindValue(QStringLiteral(":artistId"), insertArtist(albumArtist));
        } else {
            d->mSelectAlbumIdFromTitleAndArtistQuery.bindValue(QStringLiteral(":artistId"), insertArtist(trackArtist));
        }

        auto queryResult = d->mSelectAlbumIdFromTitleAndArtistQuery.exec();

        if (!queryResult || !d->mSelectAlbumIdFromTitleAndArtistQuery.isSelect() || !d->mSelectAlbumIdFromTitleAndArtistQuery.isActive()) {
            Q_EMIT databaseError();

            qDebug() << "DatabaseInterface::insertAlbum" << d->mSelectAlbumIdFromTitleAndArtistQuery.lastQuery();
            qDebug() << "DatabaseInterface::insertAlbum" << d->mSelectAlbumIdFromTitleAndArtistQuery.boundValues();
            qDebug() << "DatabaseInterface::insertAlbum" << d->mSelectAlbumIdFromTitleAndArtistQuery.lastError();

            d->mSelectAlbumIdFromTitleAndArtistQuery.finish();

            return result;
        }

        if (d->mSelectAlbumIdFromTitleAndArtistQuery.next()) {
            result = d->mSelectAlbumIdFromTitleAndArtistQuery.record().value(0).toULongLong();

            d->mSelectAlbumIdFromTitleAndArtistQuery.finish();

            return result;
        }

        d->mSelectAlbumIdFromTitleAndArtistQuery.finish();
    }

    if (result == 0) {
        d->mSelectAlbumIdFromTitleWithoutArtistQuery.bindValue(QStringLiteral(":title"), title);
        d->mSelectAlbumIdFromTitleWithoutArtistQuery.bindValue(QStringLiteral(":albumPath"), trackPath);

        auto queryResult = d->mSelectAlbumIdFromTitleWithoutArtistQuery.exec();

        if (!queryResult || !d->mSelectAlbumIdFromTitleWithoutArtistQuery.isSelect() || !d->mSelectAlbumIdFromTitleWithoutArtistQuery.isActive()) {
            Q_EMIT databaseError();

            qDebug() << "DatabaseInterface::insertAlbum" << d->mSelectAlbumIdFromTitleWithoutArtistQuery.lastQuery();
            qDebug() << "DatabaseInterface::insertAlbum" << d->mSelectAlbumIdFromTitleWithoutArtistQuery.boundValues();
            qDebug() << "DatabaseInterface::insertAlbum" << d->mSelectAlbumIdFromTitleWithoutArtistQuery.lastError();

            d->mSelectAlbumIdFromTitleWithoutArtistQuery.finish();

            return result;
        }

        if (d->mSelectAlbumIdFromTitleWithoutArtistQuery.next()) {
            result = d->mSelectAlbumIdFromTitleWithoutArtistQuery.record().value(0).toULongLong();

            d->mSelectAlbumIdFromTitleWithoutArtistQuery.finish();

            return result;
        }

        d->mSelectAlbumIdFromTitleWithoutArtistQuery.finish();
    }

    d->mInsertAlbumQuery.bindValue(QStringLiteral(":albumId"), d->mAlbumId);
    d->mInsertAlbumQuery.bindValue(QStringLiteral(":title"), title);
    if (!albumArtist.isEmpty()) {
        insertArtist(albumArtist);
        d->mInsertAlbumQuery.bindValue(QStringLiteral(":albumArtist"), albumArtist);
    }
    d->mInsertAlbumQuery.bindValue(QStringLiteral(":albumPath"), trackPath);
    d->mInsertAlbumQuery.bindValue(QStringLiteral(":coverFileName"), albumArtURI);

    auto queryResult = d->mInsertAlbumQuery.exec();

    if (!queryResult || !d->mInsertAlbumQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertAlbum" << d->mInsertAlbumQuery.lastQuery();
        qDebug() << "DatabaseInterface::insertAlbum" << d->mInsertAlbumQuery.boundValues();
        qDebug() << "DatabaseInterface::insertAlbum" << d->mInsertAlbumQuery.lastError();

        d->mInsertAlbumQuery.finish();

        return result;
    }

    result = d->mAlbumId;

    d->mInsertAlbumQuery.finish();

    ++d->mAlbumId;

    d->mInsertedAlbums.insert(result);

    return result;
}

bool DatabaseInterface::updateAlbumFromId(qulonglong albumId, const QUrl &albumArtUri,
                                          const MusicAudioTrack &currentTrack, const QString &albumPath)
{
    auto modifiedAlbum = false;
    modifiedAlbum = true;

    if (!albumArtUri.isValid()) {
        return modifiedAlbum;
    }

    auto storedAlbumArtUri = internalAlbumArtUriFromAlbumId(albumId);

    if (!storedAlbumArtUri.isValid() || storedAlbumArtUri != albumArtUri) {
        d->mUpdateAlbumArtUriFromAlbumIdQuery.bindValue(QStringLiteral(":albumId"), albumId);
        d->mUpdateAlbumArtUriFromAlbumIdQuery.bindValue(QStringLiteral(":coverFileName"), albumArtUri);

        auto result = d->mUpdateAlbumArtUriFromAlbumIdQuery.exec();

        if (!result || !d->mUpdateAlbumArtUriFromAlbumIdQuery.isActive()) {
            Q_EMIT databaseError();

            qDebug() << "DatabaseInterface::updateAlbumFromId" << d->mUpdateAlbumArtUriFromAlbumIdQuery.lastQuery();
            qDebug() << "DatabaseInterface::updateAlbumFromId" << d->mUpdateAlbumArtUriFromAlbumIdQuery.boundValues();
            qDebug() << "DatabaseInterface::updateAlbumFromId" << d->mUpdateAlbumArtUriFromAlbumIdQuery.lastError();

            d->mUpdateAlbumArtUriFromAlbumIdQuery.finish();

            return modifiedAlbum;
        }

        d->mUpdateAlbumArtUriFromAlbumIdQuery.finish();

        modifiedAlbum = true;
    }

    if (!isValidArtist(albumId) && currentTrack.isValidAlbumArtist()) {
        updateAlbumArtist(albumId, currentTrack.albumName(), albumPath, currentTrack.albumArtist());

        modifiedAlbum = true;
    }

    return modifiedAlbum;
}

qulonglong DatabaseInterface::insertArtist(const QString &name)
{
    auto result = qulonglong(0);

    if (name.isEmpty()) {
        return result;
    }

    d->mSelectArtistByNameQuery.bindValue(QStringLiteral(":name"), name);

    auto queryResult = d->mSelectArtistByNameQuery.exec();

    if (!queryResult || !d->mSelectArtistByNameQuery.isSelect() || !d->mSelectArtistByNameQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertArtist" << d->mSelectArtistByNameQuery.lastQuery();
        qDebug() << "DatabaseInterface::insertArtist" << d->mSelectArtistByNameQuery.boundValues();
        qDebug() << "DatabaseInterface::insertArtist" << d->mSelectArtistByNameQuery.lastError();

        d->mSelectArtistByNameQuery.finish();

        return result;
    }

    if (d->mSelectArtistByNameQuery.next()) {
        result = d->mSelectArtistByNameQuery.record().value(0).toULongLong();

        d->mSelectArtistByNameQuery.finish();

        return result;
    }

    d->mSelectArtistByNameQuery.finish();

    d->mInsertArtistsQuery.bindValue(QStringLiteral(":artistId"), d->mArtistId);
    d->mInsertArtistsQuery.bindValue(QStringLiteral(":name"), name);

    queryResult = d->mInsertArtistsQuery.exec();

    if (!queryResult || !d->mInsertArtistsQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertArtist" << d->mInsertArtistsQuery.lastQuery();
        qDebug() << "DatabaseInterface::insertArtist" << d->mInsertArtistsQuery.boundValues();
        qDebug() << "DatabaseInterface::insertArtist" << d->mInsertArtistsQuery.lastError();

        d->mInsertArtistsQuery.finish();

        return result;
    }

    result = d->mArtistId;

    ++d->mArtistId;

    d->mInsertedArtists.insert(result);

    d->mInsertArtistsQuery.finish();

    return result;
}

qulonglong DatabaseInterface::insertComposer(const QString &name)
{
    auto result = qulonglong(0);

    if (name.isEmpty()) {
        return result;
    }

    d->mSelectComposerByNameQuery.bindValue(QStringLiteral(":name"), name);

    auto queryResult = d->mSelectComposerByNameQuery.exec();

    if (!queryResult || !d->mSelectComposerByNameQuery.isSelect() || !d->mSelectComposerByNameQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertComposer" << d->mSelectComposerByNameQuery.lastQuery();
        qDebug() << "DatabaseInterface::insertComposer" << d->mSelectComposerByNameQuery.boundValues();
        qDebug() << "DatabaseInterface::insertComposer" << d->mSelectComposerByNameQuery.lastError();

        d->mSelectComposerByNameQuery.finish();

        return result;
    }


    if (d->mSelectComposerByNameQuery.next()) {
        result = d->mSelectComposerByNameQuery.record().value(0).toULongLong();

        d->mSelectComposerByNameQuery.finish();

        return result;
    }

    d->mSelectComposerByNameQuery.finish();

    d->mInsertComposerQuery.bindValue(QStringLiteral(":composerId"), d->mComposerId);
    d->mInsertComposerQuery.bindValue(QStringLiteral(":name"), name);

    queryResult = d->mInsertComposerQuery.exec();

    if (!queryResult || !d->mInsertComposerQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertComposer" << d->mInsertComposerQuery.lastQuery();
        qDebug() << "DatabaseInterface::insertComposer" << d->mInsertComposerQuery.boundValues();
        qDebug() << "DatabaseInterface::insertComposer" << d->mInsertComposerQuery.lastError();

        d->mInsertComposerQuery.finish();

        return result;
    }

    result = d->mComposerId;

    ++d->mComposerId;

    d->mInsertComposerQuery.finish();

    Q_EMIT composersAdded({internalComposerFromId(d->mComposerId - 1)});

    return result;
}

qulonglong DatabaseInterface::insertGenre(const QString &name)
{
    auto result = qulonglong(0);

    if (name.isEmpty()) {
        return result;
    }

    d->mSelectGenreByNameQuery.bindValue(QStringLiteral(":name"), name);

    auto queryResult = d->mSelectGenreByNameQuery.exec();

    if (!queryResult || !d->mSelectGenreByNameQuery.isSelect() || !d->mSelectGenreByNameQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertGenre" << d->mSelectGenreByNameQuery.lastQuery();
        qDebug() << "DatabaseInterface::insertGenre" << d->mSelectGenreByNameQuery.boundValues();
        qDebug() << "DatabaseInterface::insertGenre" << d->mSelectGenreByNameQuery.lastError();

        d->mSelectGenreByNameQuery.finish();

        return result;
    }

    if (d->mSelectGenreByNameQuery.next()) {
        result = d->mSelectGenreByNameQuery.record().value(0).toULongLong();

        d->mSelectGenreByNameQuery.finish();

        return result;
    }

    d->mSelectGenreByNameQuery.finish();

    d->mInsertGenreQuery.bindValue(QStringLiteral(":genreId"), d->mGenreId);
    d->mInsertGenreQuery.bindValue(QStringLiteral(":name"), name);

    queryResult = d->mInsertGenreQuery.exec();

    if (!queryResult || !d->mInsertGenreQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertGenre" << d->mInsertGenreQuery.lastQuery();
        qDebug() << "DatabaseInterface::insertGenre" << d->mInsertGenreQuery.boundValues();
        qDebug() << "DatabaseInterface::insertGenre" << d->mInsertGenreQuery.lastError();

        d->mInsertGenreQuery.finish();

        return result;
    }

    result = d->mGenreId;

    ++d->mGenreId;

    d->mInsertGenreQuery.finish();

    Q_EMIT genresAdded({internalGenreFromId(d->mGenreId - 1)});

    return result;
}

MusicAudioGenre DatabaseInterface::internalGenreFromId(qulonglong genreId)
{
    auto result = MusicAudioGenre{};

    if (!d || !d->mTracksDatabase.isValid() || !d->mInitFinished) {
        return result;
    }

    d->mSelectGenreQuery.bindValue(QStringLiteral(":genreId"), genreId);

    auto queryResult = d->mSelectGenreQuery.exec();

    if (!queryResult || !d->mSelectGenreQuery.isSelect() || !d->mSelectGenreQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::internalGenreFromId" << d->mSelectGenreQuery.lastQuery();
        qDebug() << "DatabaseInterface::internalGenreFromId" << d->mSelectGenreQuery.boundValues();
        qDebug() << "DatabaseInterface::internalGenreFromId" << d->mSelectGenreQuery.lastError();

        d->mSelectGenreQuery.finish();

        return result;
    }

    if (!d->mSelectGenreQuery.next()) {
        d->mSelectGenreQuery.finish();

        return result;
    }

    const auto &currentRecord = d->mSelectGenreQuery.record();

    result.setDatabaseId(currentRecord.value(0).toULongLong());
    result.setName(currentRecord.value(1).toString());

    d->mSelectGenreQuery.finish();

    return result;
}

void DatabaseInterface::insertTrackOrigin(const QUrl &fileNameURI, const QDateTime &fileModifiedTime,
                                          qulonglong discoverId)
{
    d->mInsertTrackMapping.bindValue(QStringLiteral(":discoverId"), discoverId);
    d->mInsertTrackMapping.bindValue(QStringLiteral(":fileName"), fileNameURI);
    d->mInsertTrackMapping.bindValue(QStringLiteral(":priority"), 1);
    d->mInsertTrackMapping.bindValue(QStringLiteral(":mtime"), fileModifiedTime);

    auto queryResult = d->mInsertTrackMapping.exec();

    if (!queryResult || !d->mInsertTrackMapping.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertArtist" << d->mInsertTrackMapping.lastQuery();
        qDebug() << "DatabaseInterface::insertArtist" << d->mInsertTrackMapping.boundValues();
        qDebug() << "DatabaseInterface::insertArtist" << d->mInsertTrackMapping.lastError();

        d->mInsertTrackMapping.finish();

        return;
    }

    d->mInsertTrackMapping.finish();
}

void DatabaseInterface::updateTrackOrigin(qulonglong trackId, const QUrl &fileName, const QDateTime &fileModifiedTime)
{
    d->mUpdateTrackMapping.bindValue(QStringLiteral(":trackId"), trackId);
    d->mUpdateTrackMapping.bindValue(QStringLiteral(":fileName"), fileName);
    d->mUpdateTrackMapping.bindValue(QStringLiteral(":priority"), computeTrackPriority(trackId, fileName));
    d->mUpdateTrackMapping.bindValue(QStringLiteral(":mtime"), fileModifiedTime);

    auto queryResult = d->mUpdateTrackMapping.exec();

    if (!queryResult || !d->mUpdateTrackMapping.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::updateTrackOrigin" << d->mUpdateTrackMapping.lastQuery();
        qDebug() << "DatabaseInterface::updateTrackOrigin" << d->mUpdateTrackMapping.boundValues();
        qDebug() << "DatabaseInterface::updateTrackOrigin" << d->mUpdateTrackMapping.lastError();

        d->mUpdateTrackMapping.finish();

        return;
    }

    d->mInsertTrackMapping.finish();
}

int DatabaseInterface::computeTrackPriority(qulonglong trackId, const QUrl &fileName)
{
    auto result = int(1);

    if (!d) {
        return result;
    }

    d->mSelectTracksMappingPriority.bindValue(QStringLiteral(":trackId"), trackId);
    d->mSelectTracksMappingPriority.bindValue(QStringLiteral(":fileName"), fileName);

    auto queryResult = d->mSelectTracksMappingPriority.exec();

    if (!queryResult || !d->mSelectTracksMappingPriority.isSelect() || !d->mSelectTracksMappingPriority.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::internalTrackIdFromFileName" << d->mSelectTracksMappingPriority.lastQuery();
        qDebug() << "DatabaseInterface::internalTrackIdFromFileName" << d->mSelectTracksMappingPriority.boundValues();
        qDebug() << "DatabaseInterface::internalTrackIdFromFileName" << d->mSelectTracksMappingPriority.lastError();

        d->mSelectTracksMappingPriority.finish();

        return result;
    }

    if (d->mSelectTracksMappingPriority.next()) {
        result = d->mSelectTracksMappingPriority.record().value(0).toInt();

        d->mSelectTracksMappingPriority.finish();

        return result;
    }

    d->mSelectTracksMappingPriority.finish();

    d->mSelectTracksMappingPriorityByTrackId.bindValue(QStringLiteral(":trackId"), trackId);

    queryResult = d->mSelectTracksMappingPriorityByTrackId.exec();

    if (!queryResult || !d->mSelectTracksMappingPriorityByTrackId.isSelect() || !d->mSelectTracksMappingPriorityByTrackId.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::internalTrackIdFromFileName" << d->mSelectTracksMappingPriorityByTrackId.lastQuery();
        qDebug() << "DatabaseInterface::internalTrackIdFromFileName" << d->mSelectTracksMappingPriorityByTrackId.boundValues();
        qDebug() << "DatabaseInterface::internalTrackIdFromFileName" << d->mSelectTracksMappingPriorityByTrackId.lastError();

        d->mSelectTracksMappingPriorityByTrackId.finish();

        return result;
    }

    if (d->mSelectTracksMappingPriorityByTrackId.next()) {
        result = d->mSelectTracksMappingPriorityByTrackId.record().value(0).toInt() + 1;
    }

    d->mSelectTracksMappingPriorityByTrackId.finish();

    return result;
}

qulonglong DatabaseInterface::internalInsertTrack(const MusicAudioTrack &oneTrack, const QHash<QString, QUrl> &covers,
                                                  qulonglong originTrackId, TrackFileInsertType insertType)
{
    qulonglong resultId = 0;

    if (oneTrack.title().isEmpty()) {
        return resultId;
    }

    auto oldTrack = oneTrack;

    QUrl::FormattingOptions currentOptions = QUrl::PreferLocalFile |
            QUrl::RemoveAuthority | QUrl::RemoveFilename | QUrl::RemoveFragment |
            QUrl::RemovePassword | QUrl::RemovePort | QUrl::RemoveQuery |
            QUrl::RemoveScheme | QUrl::RemoveUserInfo;

    const auto &trackPath = oneTrack.resourceURI().toString(currentOptions);

    auto albumId = insertAlbum(oneTrack.albumName(), (oneTrack.isValidAlbumArtist() ? oneTrack.albumArtist() : QString()),
                               oneTrack.artist(), trackPath, covers[oneTrack.resourceURI().toString()]);

    const auto &currentAlbum = internalAlbumFromId(albumId);

    auto otherTrackId = getDuplicateTrackIdFromTitleAlbumTrackDiscNumber(oneTrack.title(), oneTrack.albumName(), oneTrack.albumArtist(),
                                                                         trackPath, oneTrack.trackNumber(), oneTrack.discNumber());
    bool isModifiedTrack = (otherTrackId != 0) || (insertType == TrackFileInsertType::ModifiedTrackFileInsert);
    bool isSameTrack = false;

    qulonglong oldAlbumId = 0;

    if (isModifiedTrack) {
        if (otherTrackId == 0) {
            otherTrackId = internalTrackIdFromFileName(oneTrack.resourceURI());
        }

        originTrackId = otherTrackId;

        oldTrack = internalTrackFromDatabaseId(originTrackId);

        isSameTrack = (oldTrack.title() == oneTrack.title());
        isSameTrack = isSameTrack && (oldTrack.albumName() == oneTrack.albumName());
        isSameTrack = isSameTrack && (oldTrack.artist() == oneTrack.artist());
        isSameTrack = isSameTrack && (oldTrack.trackNumber() == oneTrack.trackNumber());
        isSameTrack = isSameTrack && (oldTrack.discNumber() == oneTrack.discNumber());
        isSameTrack = isSameTrack && (oldTrack.duration() == oneTrack.duration());
        isSameTrack = isSameTrack && (oldTrack.rating() == oneTrack.rating());
        isSameTrack = isSameTrack && (oldTrack.resourceURI() == oneTrack.resourceURI());
        isSameTrack = isSameTrack && (oldTrack.genre() == oneTrack.genre());
        isSameTrack = isSameTrack && (oldTrack.composer() == oneTrack.composer());
        isSameTrack = isSameTrack && (oldTrack.lyricist() == oneTrack.lyricist());
        isSameTrack = isSameTrack && (oldTrack.comment() == oneTrack.comment());
        isSameTrack = isSameTrack && (oldTrack.year() == oneTrack.year());
        isSameTrack = isSameTrack && (oldTrack.channels() == oneTrack.channels());
        isSameTrack = isSameTrack && (oldTrack.bitRate() == oneTrack.bitRate());
        isSameTrack = isSameTrack && (oldTrack.sampleRate() == oneTrack.sampleRate());


        oldAlbumId = internalAlbumIdFromTitleAndArtist(oldTrack.albumName(), oldTrack.albumArtist());

        if (!isSameTrack) {
            auto newTrack = oneTrack;
            newTrack.setDatabaseId(oldTrack.databaseId());
            updateTrackInDatabase(newTrack, trackPath);
            updateTrackOrigin(newTrack.databaseId(), oneTrack.resourceURI(), oneTrack.fileModificationTime());
            updateAlbumFromId(albumId, oneTrack.albumCover(), oneTrack, trackPath);

            recordModifiedTrack(originTrackId);
            if (albumId != 0) {
                recordModifiedAlbum(albumId);
            }
            if (oldAlbumId != 0) {
                recordModifiedAlbum(oldAlbumId);
            }

            isSameTrack = true;
        }
    } else {
        originTrackId = d->mTrackId;
    }

    resultId = originTrackId;

    if (!isSameTrack) {
        d->mInsertTrackQuery.bindValue(QStringLiteral(":trackId"), originTrackId);
        d->mInsertTrackQuery.bindValue(QStringLiteral(":title"), oneTrack.title());
        insertArtist(oneTrack.artist());
        d->mInsertTrackQuery.bindValue(QStringLiteral(":artistName"), oneTrack.artist());
        d->mInsertTrackQuery.bindValue(QStringLiteral(":albumTitle"), currentAlbum.title());
        if (currentAlbum.isValidArtist()) {
            d->mInsertTrackQuery.bindValue(QStringLiteral(":albumArtistName"), currentAlbum.artist());
        } else {
            d->mInsertTrackQuery.bindValue(QStringLiteral(":albumArtistName"), {});
        }
        d->mInsertTrackQuery.bindValue(QStringLiteral(":albumPath"), currentAlbum.albumPath());
        d->mInsertTrackQuery.bindValue(QStringLiteral(":trackNumber"), oneTrack.trackNumber());
        d->mInsertTrackQuery.bindValue(QStringLiteral(":discNumber"), oneTrack.discNumber());
        d->mInsertTrackQuery.bindValue(QStringLiteral(":trackDuration"), QVariant::fromValue<qlonglong>(oneTrack.duration().msecsSinceStartOfDay()));
        d->mInsertTrackQuery.bindValue(QStringLiteral(":trackRating"), oneTrack.rating());
        auto genreId = insertGenre(oneTrack.genre());
        if (genreId != 0) {
            d->mInsertTrackQuery.bindValue(QStringLiteral(":genreId"), genreId);
        } else {
            d->mInsertTrackQuery.bindValue(QStringLiteral(":genreId"), {});
        }
        auto composerId = insertComposer(oneTrack.composer());
        if (composerId != 0) {
            d->mInsertTrackQuery.bindValue(QStringLiteral(":composerId"), composerId);
        } else {
            d->mInsertTrackQuery.bindValue(QStringLiteral(":composerId"), {});
        }
        auto lyricistId = insertLyricist(oneTrack.lyricist());
        if (lyricistId != 0) {
            d->mInsertTrackQuery.bindValue(QStringLiteral(":lyricistId"), lyricistId);
        } else {
            d->mInsertTrackQuery.bindValue(QStringLiteral(":lyricistId"), {});
        }
        d->mInsertTrackQuery.bindValue(QStringLiteral(":comment"), oneTrack.comment());
        d->mInsertTrackQuery.bindValue(QStringLiteral(":year"), oneTrack.year());
        d->mInsertTrackQuery.bindValue(QStringLiteral(":channels"), oneTrack.channels());
        d->mInsertTrackQuery.bindValue(QStringLiteral(":bitRate"), oneTrack.bitRate());
        d->mInsertTrackQuery.bindValue(QStringLiteral(":sampleRate"), oneTrack.sampleRate());

        auto result = d->mInsertTrackQuery.exec();

        if (result && d->mInsertTrackQuery.isActive()) {
            d->mInsertTrackQuery.finish();

            if (!isModifiedTrack) {
                ++d->mTrackId;
            }

            updateTrackOrigin(originTrackId, oneTrack.resourceURI(), oneTrack.fileModificationTime());

            if (isModifiedTrack) {
                recordModifiedTrack(originTrackId);
                if (albumId != 0) {
                    recordModifiedAlbum(albumId);
                }
                if (oldAlbumId != 0) {
                    recordModifiedAlbum(oldAlbumId);
                }
            }

            if (albumId != 0) {
                if (updateAlbumFromId(albumId, covers[oneTrack.resourceURI().toString()], oneTrack, trackPath)) {
                    auto modifiedTracks = fetchTrackIds(albumId);
                    for (auto oneModifiedTrack : modifiedTracks) {
                        if (oneModifiedTrack != resultId) {
                            recordModifiedTrack(oneModifiedTrack);
                        }
                    }
                }
                recordModifiedAlbum(albumId);
            }
        } else {
            d->mInsertTrackQuery.finish();

            Q_EMIT databaseError();

            qDebug() << "DatabaseInterface::internalInsertTrack" << oneTrack << oneTrack.resourceURI();
            qDebug() << "DatabaseInterface::internalInsertTrack" << d->mInsertTrackQuery.lastQuery();
            qDebug() << "DatabaseInterface::internalInsertTrack" << d->mInsertTrackQuery.boundValues();
            qDebug() << "DatabaseInterface::internalInsertTrack" << d->mInsertTrackQuery.lastError();
        }
    }

    return resultId;
}

MusicAudioTrack DatabaseInterface::buildTrackFromDatabaseRecord(const QSqlRecord &trackRecord) const
{
    auto id = trackRecord.value(0).toULongLong();

    auto &result = d->mTracksCache[id];

    if (result.isValid()) {
        return result;
    }

    result.setDatabaseId(id);
    result.setTitle(trackRecord.value(1).toString());
    result.setParentId(trackRecord.value(2).toString());
    result.setArtist(trackRecord.value(3).toString());

    if (trackRecord.value(4).isValid()) {
        result.setAlbumArtist(trackRecord.value(4).toString());
    }

    result.setResourceURI(trackRecord.value(5).toUrl());
    result.setFileModificationTime(trackRecord.value(6).toDateTime());
    result.setTrackNumber(trackRecord.value(7).toInt());
    result.setDiscNumber(trackRecord.value(8).toInt());
    result.setDuration(QTime::fromMSecsSinceStartOfDay(trackRecord.value(9).toInt()));
    result.setAlbumName(trackRecord.value(10).toString());
    result.setRating(trackRecord.value(11).toInt());
    result.setAlbumCover(trackRecord.value(12).toUrl());
    result.setIsSingleDiscAlbum(trackRecord.value(13).toBool());
    result.setGenre(trackRecord.value(14).toString());
    result.setComposer(trackRecord.value(15).toString());
    result.setLyricist(trackRecord.value(16).toString());
    result.setComment(trackRecord.value(17).toString());
    result.setYear(trackRecord.value(18).toInt());
    result.setChannels(trackRecord.value(19).toInt());
    result.setBitRate(trackRecord.value(20).toInt());
    result.setSampleRate(trackRecord.value(21).toInt());
    result.setAlbumId(trackRecord.value(2).toULongLong());

    result.setValid(true);

    return result;
}

void DatabaseInterface::internalRemoveTracksList(const QList<QUrl> &removedTracks)
{
    for (const auto &removedTrackFileName : removedTracks) {
        d->mRemoveTracksMapping.bindValue(QStringLiteral(":fileName"), removedTrackFileName.toString());

        auto result = d->mRemoveTracksMapping.exec();

        if (!result || !d->mRemoveTracksMapping.isActive()) {
            Q_EMIT databaseError();

            qDebug() << "DatabaseInterface::internalRemoveTracksList" << d->mRemoveTracksMapping.lastQuery();
            qDebug() << "DatabaseInterface::internalRemoveTracksList" << d->mRemoveTracksMapping.boundValues();
            qDebug() << "DatabaseInterface::internalRemoveTracksList" << d->mRemoveTracksMapping.lastError();

            continue;
        }

        d->mRemoveTracksMapping.finish();
    }

    internalRemoveTracksWithoutMapping();
}

void DatabaseInterface::internalRemoveTracksList(const QHash<QUrl, QDateTime> &removedTracks, qulonglong sourceId)
{
    for (auto itRemovedTrack = removedTracks.begin(); itRemovedTrack != removedTracks.end(); ++itRemovedTrack) {
        d->mRemoveTracksMappingFromSource.bindValue(QStringLiteral(":fileName"), itRemovedTrack.key().toString());
        d->mRemoveTracksMappingFromSource.bindValue(QStringLiteral(":sourceId"), sourceId);

        auto result = d->mRemoveTracksMappingFromSource.exec();

        if (!result || !d->mRemoveTracksMappingFromSource.isActive()) {
            Q_EMIT databaseError();

            qDebug() << "DatabaseInterface::removeTracksList" << d->mRemoveTracksMappingFromSource.lastQuery();
            qDebug() << "DatabaseInterface::removeTracksList" << d->mRemoveTracksMappingFromSource.boundValues();
            qDebug() << "DatabaseInterface::removeTracksList" << d->mRemoveTracksMappingFromSource.lastError();

            continue;
        }

        d->mRemoveTracksMappingFromSource.finish();
    }

    internalRemoveTracksWithoutMapping();
}

void DatabaseInterface::internalRemoveTracksWithoutMapping()
{
    auto queryResult = d->mSelectTracksWithoutMappingQuery.exec();

    if (!queryResult || !d->mSelectTracksWithoutMappingQuery.isSelect() || !d->mSelectTracksWithoutMappingQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertArtist" << d->mSelectTracksWithoutMappingQuery.lastQuery();
        qDebug() << "DatabaseInterface::insertArtist" << d->mSelectTracksWithoutMappingQuery.boundValues();
        qDebug() << "DatabaseInterface::insertArtist" << d->mSelectTracksWithoutMappingQuery.lastError();

        d->mSelectTracksWithoutMappingQuery.finish();

        return;
    }

    QList<MusicAudioTrack> willRemoveTrack;

    while (d->mSelectTracksWithoutMappingQuery.next()) {
        const auto &currentRecord = d->mSelectTracksWithoutMappingQuery.record();

        willRemoveTrack.push_back(buildTrackFromDatabaseRecord(currentRecord));
    }

    d->mSelectTracksWithoutMappingQuery.finish();

    QSet<qulonglong> modifiedAlbums;

    QUrl::FormattingOptions currentOptions = QUrl::PreferLocalFile |
            QUrl::RemoveAuthority | QUrl::RemoveFilename | QUrl::RemoveFragment |
            QUrl::RemovePassword | QUrl::RemovePort | QUrl::RemoveQuery |
            QUrl::RemoveScheme | QUrl::RemoveUserInfo;

    for (const auto &oneRemovedTrack : willRemoveTrack) {
        removeTrackInDatabase(oneRemovedTrack.databaseId());

        Q_EMIT trackRemoved(oneRemovedTrack.databaseId());

        const auto &modifiedAlbumId = internalAlbumIdFromTitleAndArtist(oneRemovedTrack.albumName(), oneRemovedTrack.albumArtist());
        const auto &allTracksFromArtist = internalTracksFromAuthor(oneRemovedTrack.artist());
        const auto &allAlbumsFromArtist = internalAlbumIdsFromAuthor(oneRemovedTrack.artist());
        const auto &removedArtistId = internalArtistIdFromName(oneRemovedTrack.artist());
        const auto &removedArtist = internalArtistFromId(removedArtistId);
        const auto &trackPath = oneRemovedTrack.resourceURI().toString(currentOptions);

        recordModifiedAlbum(modifiedAlbumId);
        modifiedAlbums.insert(modifiedAlbumId);
        updateAlbumFromId(modifiedAlbumId, oneRemovedTrack.albumCover(), oneRemovedTrack, trackPath);

        if (removedArtistId != 0 && allTracksFromArtist.isEmpty() && allAlbumsFromArtist.isEmpty()) {
            removeArtistInDatabase(removedArtistId);
            Q_EMIT artistRemoved(removedArtist);
        }
    }

    for (auto modifiedAlbumId : modifiedAlbums) {
        auto modifiedAlbum = internalAlbumFromId(modifiedAlbumId);

        if (modifiedAlbum.isValid() && !modifiedAlbum.isEmpty()) {
            Q_EMIT albumModified(modifiedAlbum, modifiedAlbumId);
        } else {
            removeAlbumInDatabase(modifiedAlbum.databaseId());
            Q_EMIT albumRemoved(modifiedAlbum, modifiedAlbumId);

            const auto &allTracksFromArtist = internalTracksFromAuthor(modifiedAlbum.artist());
            const auto &allAlbumsFromArtist = internalAlbumIdsFromAuthor(modifiedAlbum.artist());
            const auto &removedArtistId = internalArtistIdFromName(modifiedAlbum.artist());
            const auto &removedArtist = internalArtistFromId(removedArtistId);

            if (removedArtistId != 0 && allTracksFromArtist.isEmpty() && allAlbumsFromArtist.isEmpty()) {
                removeArtistInDatabase(removedArtistId);
                Q_EMIT artistRemoved(removedArtist);
            }
        }
    }
}

QUrl DatabaseInterface::internalAlbumArtUriFromAlbumId(qulonglong albumId)
{
    auto result = QUrl();

    d->mSelectAlbumArtUriFromAlbumIdQuery.bindValue(QStringLiteral(":albumId"), albumId);

    auto queryResult = d->mSelectAlbumArtUriFromAlbumIdQuery.exec();

    if (!queryResult || !d->mSelectAlbumArtUriFromAlbumIdQuery.isSelect() || !d->mSelectAlbumArtUriFromAlbumIdQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertArtist" << d->mSelectAlbumArtUriFromAlbumIdQuery.lastQuery();
        qDebug() << "DatabaseInterface::insertArtist" << d->mSelectAlbumArtUriFromAlbumIdQuery.boundValues();
        qDebug() << "DatabaseInterface::insertArtist" << d->mSelectAlbumArtUriFromAlbumIdQuery.lastError();

        d->mSelectAlbumArtUriFromAlbumIdQuery.finish();

        return result;
    }

    if (!d->mSelectAlbumArtUriFromAlbumIdQuery.next()) {
        d->mSelectAlbumArtUriFromAlbumIdQuery.finish();

        return result;
    }

    result = d->mSelectAlbumArtUriFromAlbumIdQuery.record().value(0).toUrl();

    d->mSelectAlbumArtUriFromAlbumIdQuery.finish();

    return result;
}

bool DatabaseInterface::isValidArtist(qulonglong albumId)
{
    auto result = false;

    d->mSelectAlbumQuery.bindValue(QStringLiteral(":albumId"), albumId);

    auto queryResult = d->mSelectAlbumQuery.exec();

    if (!queryResult || !d->mSelectAlbumQuery.isSelect() || !d->mSelectAlbumQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::internalAlbumFromId" << d->mSelectAlbumQuery.lastQuery();
        qDebug() << "DatabaseInterface::internalAlbumFromId" << d->mSelectAlbumQuery.boundValues();
        qDebug() << "DatabaseInterface::internalAlbumFromId" << d->mSelectAlbumQuery.lastError();

        d->mSelectAlbumQuery.finish();

        return result;
    }

    if (!d->mSelectAlbumQuery.next()) {
        d->mSelectAlbumQuery.finish();

        return result;
    }

    const auto &currentRecord = d->mSelectAlbumQuery.record();

    result = !currentRecord.value(3).toString().isEmpty();

    return result;
}

qulonglong DatabaseInterface::internalSourceIdFromName(const QString &sourceName)
{
    qulonglong sourceId = 0;

    d->mSelectMusicSource.bindValue(QStringLiteral(":name"), sourceName);

    auto queryResult = d->mSelectMusicSource.exec();

    if (!queryResult || !d->mSelectMusicSource.isSelect() || !d->mSelectMusicSource.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertMusicSource" << d->mSelectMusicSource.lastQuery();
        qDebug() << "DatabaseInterface::insertMusicSource" << d->mSelectMusicSource.boundValues();
        qDebug() << "DatabaseInterface::insertMusicSource" << d->mSelectMusicSource.lastError();

        d->mSelectMusicSource.finish();

        return sourceId;
    }

    if (!d->mSelectMusicSource.next()) {
        return sourceId;
    }

    sourceId = d->mSelectMusicSource.record().value(0).toULongLong();

    d->mSelectMusicSource.finish();

    return sourceId;
}

QHash<QUrl, QDateTime> DatabaseInterface::internalAllFileNameFromSource(qulonglong sourceId)
{
    QHash<QUrl, QDateTime> allFileNames;

    d->mSelectAllTrackFilesFromSourceQuery.bindValue(QStringLiteral(":discoverId"), sourceId);

    auto queryResult = d->mSelectAllTrackFilesFromSourceQuery.exec();

    if (!queryResult || !d->mSelectAllTrackFilesFromSourceQuery.isSelect() || !d->mSelectAllTrackFilesFromSourceQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertMusicSource" << d->mSelectAllTrackFilesFromSourceQuery.lastQuery();
        qDebug() << "DatabaseInterface::insertMusicSource" << d->mSelectAllTrackFilesFromSourceQuery.boundValues();
        qDebug() << "DatabaseInterface::insertMusicSource" << d->mSelectAllTrackFilesFromSourceQuery.lastError();

        d->mSelectAllTrackFilesFromSourceQuery.finish();

        return allFileNames;
    }

    while(d->mSelectAllTrackFilesFromSourceQuery.next()) {
        auto fileName = d->mSelectAllTrackFilesFromSourceQuery.record().value(0).toUrl();
        auto fileModificationTime = d->mSelectAllTrackFilesFromSourceQuery.record().value(1).toDateTime();

        allFileNames[fileName] = fileModificationTime;
    }

    d->mSelectAllTrackFilesFromSourceQuery.finish();

    return allFileNames;
}

QList<QMap<DatabaseInterface::PropertyType, QVariant> > DatabaseInterface::internalAllGenericPartialData(QSqlQuery &query,
                                                                                                         int nbFields)
{
    auto result = QList<QMap<PropertyType, QVariant>>{};

    auto queryResult = query.exec();

    if (!queryResult || !query.isSelect() || !query.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::allArtists" << query.lastQuery();
        qDebug() << "DatabaseInterface::allArtists" << query.boundValues();
        qDebug() << "DatabaseInterface::allArtists" << query.lastError();

        query.finish();

        auto transactionResult = finishTransaction();
        if (!transactionResult) {
            return result;
        }

        return result;
    }

    while(query.next()) {
        auto newData = QMap<PropertyType, QVariant>{};

        const auto &currentRecord = query.record();

        newData[DatabaseId] = currentRecord.value(0);
        newData[DisplayRole] = currentRecord.value(1);
        if (nbFields == 2) {
            newData[SecondaryRole] = currentRecord.value(2);
        }

        result.push_back(newData);
    }

    query.finish();

    return result;
}

MusicArtist DatabaseInterface::internalComposerFromId(qulonglong composerId)
{
    auto &result = d->mArtistsCache[composerId];

    if (result.isValid()) {
        return result;
    }

    if (!d || !d->mTracksDatabase.isValid() || !d->mInitFinished) {
        return result;
    }

    d->mSelectComposerQuery.bindValue(QStringLiteral(":composerId"), composerId);

    auto queryResult = d->mSelectComposerQuery.exec();

    if (!queryResult || !d->mSelectComposerQuery.isSelect() || !d->mSelectComposerQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::internalComposerFromId" << d->mSelectComposerQuery.lastQuery();
        qDebug() << "DatabaseInterface::internalComposerFromId" << d->mSelectComposerQuery.boundValues();
        qDebug() << "DatabaseInterface::internalComposerFromId" << d->mSelectComposerQuery.lastError();

        d->mSelectComposerQuery.finish();

        return result;
    }

    if (!d->mSelectComposerQuery.next()) {
        d->mSelectComposerQuery.finish();

        return result;
    }

    const auto &currentRecord = d->mSelectComposerQuery.record();

    result.setDatabaseId(currentRecord.value(0).toULongLong());
    result.setName(currentRecord.value(1).toString());
    result.setValid(true);

    d->mSelectComposerQuery.finish();

    return result;
}

qulonglong DatabaseInterface::insertLyricist(const QString &name)
{
    auto result = qulonglong(0);

    if (name.isEmpty()) {
        return result;
    }

    d->mSelectLyricistByNameQuery.bindValue(QStringLiteral(":name"), name);

    auto queryResult = d->mSelectLyricistByNameQuery.exec();

    if (!queryResult || !d->mSelectLyricistByNameQuery.isSelect() || !d->mSelectLyricistByNameQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertLyricist" << d->mSelectLyricistByNameQuery.lastQuery();
        qDebug() << "DatabaseInterface::insertLyricist" << d->mSelectLyricistByNameQuery.boundValues();
        qDebug() << "DatabaseInterface::insertLyricist" << d->mSelectLyricistByNameQuery.lastError();

        d->mSelectLyricistByNameQuery.finish();

        return result;
    }

    if (d->mSelectLyricistByNameQuery.next()) {
        result = d->mSelectLyricistByNameQuery.record().value(0).toULongLong();

        d->mSelectLyricistByNameQuery.finish();

        return result;
    }

    d->mSelectLyricistByNameQuery.finish();

    d->mInsertLyricistQuery.bindValue(QStringLiteral(":lyricistId"), d->mLyricistId);
    d->mInsertLyricistQuery.bindValue(QStringLiteral(":name"), name);

    queryResult = d->mInsertLyricistQuery.exec();

    if (!queryResult || !d->mInsertLyricistQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertLyricist" << d->mInsertLyricistQuery.lastQuery();
        qDebug() << "DatabaseInterface::insertLyricist" << d->mInsertLyricistQuery.boundValues();
        qDebug() << "DatabaseInterface::insertLyricist" << d->mInsertLyricistQuery.lastError();

        d->mInsertLyricistQuery.finish();

        return result;
    }

    result = d->mLyricistId;

    ++d->mLyricistId;

    d->mInsertLyricistQuery.finish();

    Q_EMIT lyricistsAdded({internalLyricistFromId(d->mLyricistId - 1)});

    return result;
}

MusicArtist DatabaseInterface::internalLyricistFromId(qulonglong lyricistId)
{
    auto &result = d->mArtistsCache[lyricistId];

    if (result.isValid()) {
        return result;
    }

    if (!d || !d->mTracksDatabase.isValid() || !d->mInitFinished) {
        return result;
    }

    d->mSelectLyricistQuery.bindValue(QStringLiteral(":lyricistId"), lyricistId);

    auto queryResult = d->mSelectLyricistQuery.exec();

    if (!queryResult || !d->mSelectLyricistQuery.isSelect() || !d->mSelectLyricistQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::internalLyricistFromId" << d->mSelectLyricistQuery.lastQuery();
        qDebug() << "DatabaseInterface::internalLyricistFromId" << d->mSelectLyricistQuery.boundValues();
        qDebug() << "DatabaseInterface::internalLyricistFromId" << d->mSelectLyricistQuery.lastError();

        d->mSelectLyricistQuery.finish();

        return result;
    }

    if (!d->mSelectLyricistQuery.next()) {
        d->mSelectLyricistQuery.finish();

        return result;
    }

    const auto &currentRecord = d->mSelectLyricistQuery.record();

    result.setDatabaseId(currentRecord.value(0).toULongLong());
    result.setName(currentRecord.value(1).toString());
    result.setValid(true);

    d->mSelectLyricistQuery.finish();

    return result;
}

qulonglong DatabaseInterface::internalArtistIdFromName(const QString &name)
{
    auto result = qulonglong(0);

    if (name.isEmpty()) {
        return result;
    }

    d->mSelectArtistByNameQuery.bindValue(QStringLiteral(":name"), name);

    auto queryResult = d->mSelectArtistByNameQuery.exec();

    if (!queryResult || !d->mSelectArtistByNameQuery.isSelect() || !d->mSelectArtistByNameQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertArtist" << d->mSelectArtistByNameQuery.lastQuery();
        qDebug() << "DatabaseInterface::insertArtist" << d->mSelectArtistByNameQuery.boundValues();
        qDebug() << "DatabaseInterface::insertArtist" << d->mSelectArtistByNameQuery.lastError();

        d->mSelectArtistByNameQuery.finish();

        return result;
    }

    if (!d->mSelectArtistByNameQuery.next()) {
        d->mSelectArtistByNameQuery.finish();

        return result;
    }

    result = d->mSelectArtistByNameQuery.record().value(0).toULongLong();

    d->mSelectArtistByNameQuery.finish();

    return result;
}

void DatabaseInterface::removeTrackInDatabase(qulonglong trackId)
{
    d->mRemoveTrackQuery.bindValue(QStringLiteral(":trackId"), trackId);

    d->mTracksCache.remove(trackId);

    auto result = d->mRemoveTrackQuery.exec();

    if (!result || !d->mRemoveTrackQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::removeTrackInDatabase" << d->mRemoveTrackQuery.lastQuery();
        qDebug() << "DatabaseInterface::removeTrackInDatabase" << d->mRemoveTrackQuery.boundValues();
        qDebug() << "DatabaseInterface::removeTrackInDatabase" << d->mRemoveTrackQuery.lastError();
    }

    d->mRemoveTrackQuery.finish();
}

void DatabaseInterface::updateTrackInDatabase(const MusicAudioTrack &oneTrack, const QString &albumPath)
{
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":trackId"), oneTrack.databaseId());
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":title"), oneTrack.title());
    insertArtist(oneTrack.artist());
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":artistName"), oneTrack.artist());
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":albumTitle"), oneTrack.albumName());
    if (oneTrack.isValidAlbumArtist()) {
        d->mUpdateTrackQuery.bindValue(QStringLiteral(":albumArtistName"), oneTrack.albumArtist());
    } else {
        d->mUpdateTrackQuery.bindValue(QStringLiteral(":albumArtistName"), {});
    }
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":albumPath"), albumPath);
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":trackNumber"), oneTrack.trackNumber());
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":discNumber"), oneTrack.discNumber());
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":trackDuration"), QVariant::fromValue<qlonglong>(oneTrack.duration().msecsSinceStartOfDay()));
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":trackRating"), oneTrack.rating());
    auto genreId = insertGenre(oneTrack.genre());
    if (genreId != 0) {
        d->mUpdateTrackQuery.bindValue(QStringLiteral(":genreId"), genreId);
    } else {
        d->mUpdateTrackQuery.bindValue(QStringLiteral(":genreId"), {});
    }
    auto composerId = insertComposer(oneTrack.composer());
    if (composerId != 0) {
        d->mUpdateTrackQuery.bindValue(QStringLiteral(":composerId"), composerId);
    } else {
        d->mUpdateTrackQuery.bindValue(QStringLiteral(":composerId"), {});
    }
    auto lyricistId = insertLyricist(oneTrack.lyricist());
    if (lyricistId != 0) {
        d->mUpdateTrackQuery.bindValue(QStringLiteral(":lyricistId"), lyricistId);
    } else {
        d->mUpdateTrackQuery.bindValue(QStringLiteral(":lyricistId"), {});
    }
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":comment"), oneTrack.comment());
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":year"), oneTrack.year());
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":channels"), oneTrack.channels());
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":bitRate"), oneTrack.bitRate());
    d->mUpdateTrackQuery.bindValue(QStringLiteral(":sampleRate"), oneTrack.sampleRate());

    auto result = d->mUpdateTrackQuery.exec();

    if (!result || !d->mUpdateTrackQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::updateTrackInDatabase" << d->mUpdateTrackQuery.lastQuery();
        qDebug() << "DatabaseInterface::updateTrackInDatabase" << d->mUpdateTrackQuery.boundValues();
        qDebug() << "DatabaseInterface::updateTrackInDatabase" << d->mUpdateTrackQuery.lastError();
    }

    d->mUpdateTrackQuery.finish();
}

void DatabaseInterface::removeAlbumInDatabase(qulonglong albumId)
{
    d->mRemoveAlbumQuery.bindValue(QStringLiteral(":albumId"), albumId);

    d->mAlbumsCache.remove(albumId);

    auto result = d->mRemoveAlbumQuery.exec();

    if (!result || !d->mRemoveAlbumQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::removeAlbumInDatabase" << d->mRemoveAlbumQuery.lastQuery();
        qDebug() << "DatabaseInterface::removeAlbumInDatabase" << d->mRemoveAlbumQuery.boundValues();
        qDebug() << "DatabaseInterface::removeAlbumInDatabase" << d->mRemoveAlbumQuery.lastError();
    }

    d->mRemoveAlbumQuery.finish();
}

void DatabaseInterface::removeArtistInDatabase(qulonglong artistId)
{
    d->mRemoveArtistQuery.bindValue(QStringLiteral(":artistId"), artistId);

    d->mArtistsCache.remove(artistId);

    auto result = d->mRemoveArtistQuery.exec();

    if (!result || !d->mRemoveArtistQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::removeArtistInDatabase" << d->mRemoveArtistQuery.lastQuery();
        qDebug() << "DatabaseInterface::removeArtistInDatabase" << d->mRemoveArtistQuery.boundValues();
        qDebug() << "DatabaseInterface::removeArtistInDatabase" << d->mRemoveArtistQuery.lastError();
    }

    d->mRemoveArtistQuery.finish();
}

void DatabaseInterface::reloadExistingDatabase()
{
    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return;
    }

    //d->mInitialUpdateTracksValidity.exec();
    qDebug() << "DatabaseInterface::reloadExistingDatabase";

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return;
    }

    const auto restoredArtists = allArtists();
    for (const auto &oneArtist : restoredArtists) {
        d->mArtistId = std::max(d->mArtistId, oneArtist.databaseId());
    }
    if (!restoredArtists.isEmpty()) {
        ++d->mArtistId;
        Q_EMIT artistsAdded(restoredArtists);
    }

    const auto restoredComposers = allComposers();
    for (const auto &oneComposer : restoredComposers) {
        d->mComposerId = std::max(d->mComposerId, oneComposer.databaseId());
    }
    if (!restoredComposers.isEmpty()) {
        ++d->mComposerId;
        Q_EMIT composersAdded(restoredComposers);
    }

    const auto restoredLyricists = allLyricists();
    for (const auto &oneLyricist : restoredLyricists) {
        d->mLyricistId = std::max(d->mLyricistId, oneLyricist.databaseId());
    }
    if (!restoredLyricists.isEmpty()) {
        ++d->mLyricistId;
        Q_EMIT lyricistsAdded(restoredLyricists);
    }

    const auto restoredAlbums = allAlbums();
    for (const auto &oneAlbum : restoredAlbums) {
        d->mAlbumId = std::max(d->mAlbumId, oneAlbum.databaseId());
    }
    if (!restoredAlbums.isEmpty()) {
        ++d->mAlbumId;
        Q_EMIT albumsAdded(restoredAlbums);
    }

    const auto restoredTracks = allTracks();
    for (const auto &oneTrack : restoredTracks) {
        d->mTrackId = std::max(d->mTrackId, oneTrack.databaseId());
    }
    if (!restoredTracks.isEmpty()) {
        ++d->mTrackId;
        Q_EMIT tracksAdded(restoredTracks);
    }

    const auto restoredGenres = allGenres();
    for (const auto &oneGenre : restoredGenres) {
        d->mGenreId = std::max(d->mGenreId, oneGenre.databaseId());
    }
    if (!restoredGenres.isEmpty()) {
        ++d->mGenreId;
        Q_EMIT genresAdded(restoredGenres);
    }
}

qulonglong DatabaseInterface::insertMusicSource(const QString &name)
{
    qulonglong result = 0;

    d->mSelectMusicSource.bindValue(QStringLiteral(":name"), name);

    auto queryResult = d->mSelectMusicSource.exec();

    if (!queryResult || !d->mSelectMusicSource.isSelect() || !d->mSelectMusicSource.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertMusicSource" << d->mSelectMusicSource.lastQuery();
        qDebug() << "DatabaseInterface::insertMusicSource" << d->mSelectMusicSource.boundValues();
        qDebug() << "DatabaseInterface::insertMusicSource" << d->mSelectMusicSource.lastError();

        d->mSelectMusicSource.finish();

        return result;
    }

    if (d->mSelectMusicSource.next()) {
        result = d->mSelectMusicSource.record().value(0).toULongLong();

        d->mSelectMusicSource.finish();

        return result;
    }

    d->mSelectMusicSource.finish();

    d->mInsertMusicSource.bindValue(QStringLiteral(":discoverId"), d->mDiscoverId);
    d->mInsertMusicSource.bindValue(QStringLiteral(":name"), name);

    queryResult = d->mInsertMusicSource.exec();

    if (!queryResult || !d->mInsertMusicSource.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::insertMusicSource" << d->mInsertMusicSource.lastQuery();
        qDebug() << "DatabaseInterface::insertMusicSource" << d->mInsertMusicSource.boundValues();
        qDebug() << "DatabaseInterface::insertMusicSource" << d->mInsertMusicSource.lastError();

        d->mInsertMusicSource.finish();

        return d->mDiscoverId;
    }

    d->mInsertMusicSource.finish();

    ++d->mDiscoverId;

    return d->mDiscoverId - 1;
}

QList<MusicAudioTrack> DatabaseInterface::fetchTracks(qulonglong albumId)
{
    auto allTracks = QList<MusicAudioTrack>();

    d->mSelectTrackQuery.bindValue(QStringLiteral(":albumId"), albumId);

    auto result = d->mSelectTrackQuery.exec();

    if (!result || !d->mSelectTrackQuery.isSelect() || !d->mSelectTrackQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::fetchTracks" << d->mSelectTrackQuery.lastQuery();
        qDebug() << "DatabaseInterface::fetchTracks" << d->mSelectTrackQuery.boundValues();
        qDebug() << "DatabaseInterface::fetchTracks" << d->mSelectTrackQuery.lastError();
    }

    while (d->mSelectTrackQuery.next()) {
        const auto &currentRecord = d->mSelectTrackQuery.record();

        allTracks.push_back(buildTrackFromDatabaseRecord(currentRecord));
    }

    d->mSelectTrackQuery.finish();

    return allTracks;
}

QList<qulonglong> DatabaseInterface::fetchTrackIds(qulonglong albumId)
{
    auto allTracks = QList<qulonglong>();

    d->mSelectTrackQuery.bindValue(QStringLiteral(":albumId"), albumId);

    auto result = d->mSelectTrackQuery.exec();

    if (!result || !d->mSelectTrackQuery.isSelect() || !d->mSelectTrackQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::fetchTracks" << d->mSelectTrackQuery.lastQuery();
        qDebug() << "DatabaseInterface::fetchTracks" << d->mSelectTrackQuery.boundValues();
        qDebug() << "DatabaseInterface::fetchTracks" << d->mSelectTrackQuery.lastError();
    }

    while (d->mSelectTrackQuery.next()) {
        const auto &currentRecord = d->mSelectTrackQuery.record();

        allTracks.push_back(currentRecord.value(0).toULongLong());
    }

    d->mSelectTrackQuery.finish();

    return allTracks;
}

MusicAlbum DatabaseInterface::internalAlbumFromId(qulonglong albumId)
{
    auto &retrievedAlbum = d->mAlbumsCache[albumId];

    if (retrievedAlbum.isValid()) {
        return retrievedAlbum;
    }

    d->mSelectAlbumQuery.bindValue(QStringLiteral(":albumId"), albumId);

    auto result = d->mSelectAlbumQuery.exec();

    if (!result || !d->mSelectAlbumQuery.isSelect() || !d->mSelectAlbumQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::internalAlbumFromId" << d->mSelectAlbumQuery.lastQuery();
        qDebug() << "DatabaseInterface::internalAlbumFromId" << d->mSelectAlbumQuery.boundValues();
        qDebug() << "DatabaseInterface::internalAlbumFromId" << d->mSelectAlbumQuery.lastError();

        d->mSelectAlbumQuery.finish();

        return retrievedAlbum;
    }

    if (!d->mSelectAlbumQuery.next()) {
        d->mSelectAlbumQuery.finish();

        return retrievedAlbum;
    }

    const auto &currentRecord = d->mSelectAlbumQuery.record();

    retrievedAlbum.setDatabaseId(currentRecord.value(0).toULongLong());
    retrievedAlbum.setTitle(currentRecord.value(1).toString());
    retrievedAlbum.setId(currentRecord.value(2).toString());
    retrievedAlbum.setArtist(currentRecord.value(3).toString());
    retrievedAlbum.setAlbumPath(currentRecord.value(4).toUrl());
    retrievedAlbum.setAlbumArtURI(currentRecord.value(5).toUrl());
    retrievedAlbum.setTracksCount(currentRecord.value(6).toInt());
    retrievedAlbum.setIsSingleDiscAlbum(currentRecord.value(7).toBool());
    retrievedAlbum.setTracks(fetchTracks(albumId));
    retrievedAlbum.setValid(true);

    d->mSelectAlbumQuery.finish();

    d->mSelectGenreForAlbumQuery.bindValue(QStringLiteral(":albumId"), albumId);

    result = d->mSelectGenreForAlbumQuery.exec();

    if (!result || !d->mSelectGenreForAlbumQuery.isSelect() || !d->mSelectGenreForAlbumQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::internalAlbumFromId" << d->mSelectGenreForAlbumQuery.lastQuery();
        qDebug() << "DatabaseInterface::internalAlbumFromId" << d->mSelectGenreForAlbumQuery.boundValues();
        qDebug() << "DatabaseInterface::internalAlbumFromId" << d->mSelectGenreForAlbumQuery.lastError();

        d->mSelectGenreForAlbumQuery.finish();

        return retrievedAlbum;
    }

    QStringList allGenres;
    while(d->mSelectGenreForAlbumQuery.next()) {
        allGenres.push_back(d->mSelectGenreForAlbumQuery.record().value(0).toString());
    }

    retrievedAlbum.setGenres(allGenres);

    d->mSelectGenreForAlbumQuery.finish();

    return retrievedAlbum;
}

MusicAlbum DatabaseInterface::internalAlbumFromTitleAndArtist(const QString &title, const QString &artist)
{
    auto result = MusicAlbum();

    auto albumId = internalAlbumIdFromTitleAndArtist(title, artist);

    if (albumId == 0) {
        return result;
    }

    result = internalAlbumFromId(albumId);

    return result;
}

qulonglong DatabaseInterface::internalAlbumIdFromTitleAndArtist(const QString &title, const QString &artist)
{
    auto result = qulonglong(0);

    d->mSelectAlbumIdFromTitleQuery.bindValue(QStringLiteral(":title"), title);
    d->mSelectAlbumIdFromTitleQuery.bindValue(QStringLiteral(":artistName"), artist);

    auto queryResult = d->mSelectAlbumIdFromTitleQuery.exec();

    if (!queryResult || !d->mSelectAlbumIdFromTitleQuery.isSelect() || !d->mSelectAlbumIdFromTitleQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::internalAlbumIdFromTitleAndArtist" << d->mSelectAlbumIdFromTitleQuery.lastQuery();
        qDebug() << "DatabaseInterface::internalAlbumIdFromTitleAndArtist" << d->mSelectAlbumIdFromTitleQuery.boundValues();
        qDebug() << "DatabaseInterface::internalAlbumIdFromTitleAndArtist" << d->mSelectAlbumIdFromTitleQuery.lastError();

        d->mSelectAlbumIdFromTitleQuery.finish();

        return result;
    }

    if (d->mSelectAlbumIdFromTitleQuery.next()) {
        result = d->mSelectAlbumIdFromTitleQuery.record().value(0).toULongLong();
    }

    d->mSelectAlbumIdFromTitleQuery.finish();

    if (result == 0) {
        d->mSelectAlbumIdFromTitleWithoutArtistQuery.bindValue(QStringLiteral(":title"), title);

        auto queryResult = d->mSelectAlbumIdFromTitleWithoutArtistQuery.exec();

        if (!queryResult || !d->mSelectAlbumIdFromTitleWithoutArtistQuery.isSelect() || !d->mSelectAlbumIdFromTitleWithoutArtistQuery.isActive()) {
            Q_EMIT databaseError();

            qDebug() << "DatabaseInterface::internalAlbumIdFromTitleAndArtist" << d->mSelectAlbumIdFromTitleWithoutArtistQuery.lastQuery();
            qDebug() << "DatabaseInterface::internalAlbumIdFromTitleAndArtist" << d->mSelectAlbumIdFromTitleWithoutArtistQuery.boundValues();
            qDebug() << "DatabaseInterface::internalAlbumIdFromTitleAndArtist" << d->mSelectAlbumIdFromTitleWithoutArtistQuery.lastError();

            d->mSelectAlbumIdFromTitleWithoutArtistQuery.finish();

            return result;
        }

        if (d->mSelectAlbumIdFromTitleWithoutArtistQuery.next()) {
            result = d->mSelectAlbumIdFromTitleWithoutArtistQuery.record().value(0).toULongLong();
        }

        d->mSelectAlbumIdFromTitleWithoutArtistQuery.finish();
    }

    return result;
}

MusicAudioTrack DatabaseInterface::internalTrackFromDatabaseId(qulonglong id)
{
    auto result = MusicAudioTrack();

    if (result.isValid()) {
        return result;
    }

    if (!d || !d->mTracksDatabase.isValid() || !d->mInitFinished) {
        return result;
    }

    d->mSelectTrackFromIdQuery.bindValue(QStringLiteral(":trackId"), id);

    auto queryResult = d->mSelectTrackFromIdQuery.exec();

    if (!queryResult || !d->mSelectTrackFromIdQuery.isSelect() || !d->mSelectTrackFromIdQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::internalTrackFromDatabaseId" << d->mSelectAlbumQuery.lastQuery();
        qDebug() << "DatabaseInterface::internalTrackFromDatabaseId" << d->mSelectAlbumQuery.boundValues();
        qDebug() << "DatabaseInterface::internalTrackFromDatabaseId" << d->mSelectAlbumQuery.lastError();

        d->mSelectTrackFromIdQuery.finish();

        return result;
    }

    if (!d->mSelectTrackFromIdQuery.next()) {
        d->mSelectTrackFromIdQuery.finish();

        return result;
    }

    const auto &currentRecord = d->mSelectTrackFromIdQuery.record();

    result = buildTrackFromDatabaseRecord(currentRecord);

    d->mSelectTrackFromIdQuery.finish();

    return result;
}

qulonglong DatabaseInterface::internalTrackIdFromTitleAlbumTracDiscNumber(const QString &title, const QString &artist, const QString &album,
                                                                          int trackNumber, int discNumber)
{
    auto result = qulonglong(0);

    if (!d) {
        return result;
    }

    d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.bindValue(QStringLiteral(":title"), title);
    d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.bindValue(QStringLiteral(":artist"), artist);
    d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.bindValue(QStringLiteral(":album"), album);
    d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.bindValue(QStringLiteral(":trackNumber"), trackNumber);
    d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.bindValue(QStringLiteral(":discNumber"), discNumber);

    auto queryResult = d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.exec();

    if (!queryResult || !d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.isSelect() || !d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::trackIdFromTitleAlbumArtist" << d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.lastQuery();
        qDebug() << "DatabaseInterface::trackIdFromTitleAlbumArtist" << d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.boundValues();
        qDebug() << "DatabaseInterface::trackIdFromTitleAlbumArtist" << d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.lastError();

        d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.finish();

        return result;
    }

    if (d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.next()) {
        result = d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.record().value(0).toULongLong();
    }

    d->mSelectTrackIdFromTitleArtistAlbumTrackDiscNumberQuery.finish();

    return result;
}

qulonglong DatabaseInterface::getDuplicateTrackIdFromTitleAlbumTrackDiscNumber(const QString &title, const QString &album,
                                                                               const QString &albumArtist, const QString &trackPath,
                                                                               int trackNumber, int discNumber)
{
    auto result = qulonglong(0);

    if (!d) {
        return result;
    }

    d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.bindValue(QStringLiteral(":title"), title);
    d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.bindValue(QStringLiteral(":album"), album);
    d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.bindValue(QStringLiteral(":albumPath"), trackPath);
    d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.bindValue(QStringLiteral(":albumArtist"), albumArtist);
    d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.bindValue(QStringLiteral(":trackNumber"), trackNumber);
    d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.bindValue(QStringLiteral(":discNumber"), discNumber);

    auto queryResult = d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.exec();

    if (!queryResult || !d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.isSelect() || !d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::trackIdFromTitleAlbumArtist" << d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.lastQuery();
        qDebug() << "DatabaseInterface::trackIdFromTitleAlbumArtist" << d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.boundValues();
        qDebug() << "DatabaseInterface::trackIdFromTitleAlbumArtist" << d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.lastError();

        d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.finish();

        return result;
    }

    if (d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.next()) {
        result = d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.record().value(0).toULongLong();
    }

    d->mSelectTrackIdFromTitleAlbumTrackDiscNumberQuery.finish();

    return result;
}

qulonglong DatabaseInterface::internalTrackIdFromFileName(const QUrl &fileName)
{
    auto result = qulonglong(0);

    if (!d) {
        return result;
    }

    d->mSelectTracksMapping.bindValue(QStringLiteral(":fileName"), fileName);

    auto queryResult = d->mSelectTracksMapping.exec();

    if (!queryResult || !d->mSelectTracksMapping.isSelect() || !d->mSelectTracksMapping.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::internalTrackIdFromFileName" << d->mSelectTracksMapping.lastQuery();
        qDebug() << "DatabaseInterface::internalTrackIdFromFileName" << d->mSelectTracksMapping.boundValues();
        qDebug() << "DatabaseInterface::internalTrackIdFromFileName" << d->mSelectTracksMapping.lastError();

        d->mSelectTracksMapping.finish();

        return result;
    }

    if (d->mSelectTracksMapping.next()) {
        const auto &currentRecordValue = d->mSelectTracksMapping.record().value(0);
        if (currentRecordValue.isValid()) {
            result = currentRecordValue.toULongLong();
        }
    }

    d->mSelectTracksMapping.finish();

    return result;
}

QList<MusicAudioTrack> DatabaseInterface::internalTracksFromAuthor(const QString &ArtistName)
{
    auto allTracks = QList<MusicAudioTrack>();

    d->mSelectTracksFromArtist.bindValue(QStringLiteral(":artistName"), ArtistName);

    auto result = d->mSelectTracksFromArtist.exec();

    if (!result || !d->mSelectTracksFromArtist.isSelect() || !d->mSelectTracksFromArtist.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::tracksFromAuthor" << d->mSelectTracksFromArtist.lastQuery();
        qDebug() << "DatabaseInterface::tracksFromAuthor" << d->mSelectTracksFromArtist.boundValues();
        qDebug() << "DatabaseInterface::tracksFromAuthor" << d->mSelectTracksFromArtist.lastError();

        return allTracks;
    }

    while (d->mSelectTracksFromArtist.next()) {
        const auto &currentRecord = d->mSelectTracksFromArtist.record();

        allTracks.push_back(buildTrackFromDatabaseRecord(currentRecord));
    }

    d->mSelectTracksFromArtist.finish();

    return allTracks;
}

QList<qulonglong> DatabaseInterface::internalAlbumIdsFromAuthor(const QString &ArtistName)
{
    auto allAlbumIds = QList<qulonglong>();

    d->mSelectAlbumIdsFromArtist.bindValue(QStringLiteral(":artistName"), ArtistName);

    auto result = d->mSelectAlbumIdsFromArtist.exec();

    if (!result || !d->mSelectAlbumIdsFromArtist.isSelect() || !d->mSelectAlbumIdsFromArtist.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::tracksFromAuthor" << d->mSelectAlbumIdsFromArtist.lastQuery();
        qDebug() << "DatabaseInterface::tracksFromAuthor" << d->mSelectAlbumIdsFromArtist.boundValues();
        qDebug() << "DatabaseInterface::tracksFromAuthor" << d->mSelectAlbumIdsFromArtist.lastError();

        return allAlbumIds;
    }

    while (d->mSelectAlbumIdsFromArtist.next()) {
        const auto &currentRecord = d->mSelectAlbumIdsFromArtist.record();

        allAlbumIds.push_back(currentRecord.value(0).toULongLong());
    }

    d->mSelectAlbumIdsFromArtist.finish();

    return allAlbumIds;
}

QList<QMap<DatabaseInterface::PropertyType, QVariant>> DatabaseInterface::internalAllArtistsPartialData()
{
    return internalAllGenericPartialData(d->mSelectAllArtistsQuery, 1);
}

QList<QMap<DatabaseInterface::PropertyType, QVariant> > DatabaseInterface::internalAllAlbumsPartialData()
{
    return internalAllGenericPartialData(d->mSelectAllAlbumsShortQuery, 2);
}

QList<QMap<DatabaseInterface::PropertyType, QVariant> > DatabaseInterface::internalAllTracksPartialData()
{
    return internalAllGenericPartialData(d->mSelectAllTracksShortQuery, 2);
}

QList<QMap<DatabaseInterface::PropertyType, QVariant> > DatabaseInterface::internalAllGenresPartialData()
{
    return internalAllGenericPartialData(d->mSelectAllGenresQuery, 1);
}

QList<QMap<DatabaseInterface::PropertyType, QVariant> > DatabaseInterface::internalAllComposersPartialData()
{
    return internalAllGenericPartialData(d->mSelectAllComposersQuery, 1);
}

QList<QMap<DatabaseInterface::PropertyType, QVariant> > DatabaseInterface::internalAllLyricistsPartialData()
{
    return internalAllGenericPartialData(d->mSelectAllLyricistsQuery, 1);
}

QList<MusicArtist> DatabaseInterface::internalAllPeople(QSqlQuery allPeopleQuery,
                                                        QSqlQuery selectCountAlbumsForPeopleQuery)
{
    auto result = QList<MusicArtist>();

    if (!d) {
        return result;
    }

    auto transactionResult = startTransaction();
    if (!transactionResult) {
        return result;
    }

    auto queryResult = allPeopleQuery.exec();

    if (!queryResult || !allPeopleQuery.isSelect() || !allPeopleQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::allArtists" << allPeopleQuery.lastQuery();
        qDebug() << "DatabaseInterface::allArtists" << allPeopleQuery.boundValues();
        qDebug() << "DatabaseInterface::allArtists" << allPeopleQuery.lastError();

        allPeopleQuery.finish();

        transactionResult = finishTransaction();
        if (!transactionResult) {
            return result;
        }

        return result;
    }

    while(allPeopleQuery.next()) {
        const auto &currentRecord = allPeopleQuery.record();

        auto artistId = currentRecord.value(0).toULongLong();

        auto &newArtist = d->mArtistsCache[artistId];

        if (newArtist.isValid()) {
            result.push_back(newArtist);
            continue;
        }

        newArtist.setDatabaseId(artistId);
        newArtist.setName(currentRecord.value(1).toString());
        newArtist.setValid(true);

        selectCountAlbumsForPeopleQuery.bindValue(QStringLiteral(":artistName"), newArtist.name());

        auto queryResult = selectCountAlbumsForPeopleQuery.exec();

        if (!queryResult || !selectCountAlbumsForPeopleQuery.isSelect() || !selectCountAlbumsForPeopleQuery.isActive() || !selectCountAlbumsForPeopleQuery.next()) {
            Q_EMIT databaseError();

            qDebug() << "DatabaseInterface::allArtists" << selectCountAlbumsForPeopleQuery.lastQuery();
            qDebug() << "DatabaseInterface::allArtists" << selectCountAlbumsForPeopleQuery.boundValues();
            qDebug() << "DatabaseInterface::allArtists" << selectCountAlbumsForPeopleQuery.lastError();

            selectCountAlbumsForPeopleQuery.finish();

            transactionResult = finishTransaction();
            if (!transactionResult) {
                return result;
            }

            return result;
        }

        newArtist.setAlbumsCount(selectCountAlbumsForPeopleQuery.record().value(0).toInt());

        selectCountAlbumsForPeopleQuery.finish();

        d->mSelectGenreForArtistQuery.bindValue(QStringLiteral(":artistName"), newArtist.name());

        queryResult = d->mSelectGenreForArtistQuery.exec();

        if (!queryResult || !d->mSelectGenreForArtistQuery.isSelect() || !d->mSelectGenreForArtistQuery.isActive()) {
            Q_EMIT databaseError();

            qDebug() << "DatabaseInterface::allArtists" << d->mSelectGenreForArtistQuery.lastQuery();
            qDebug() << "DatabaseInterface::allArtists" << d->mSelectGenreForArtistQuery.boundValues();
            qDebug() << "DatabaseInterface::allArtists" << d->mSelectGenreForArtistQuery.lastError();

            d->mSelectGenreForArtistQuery.finish();

            transactionResult = finishTransaction();
            if (!transactionResult) {
                return result;
            }

            return result;
        }

        QStringList allGenres;
        while(d->mSelectGenreForArtistQuery.next()) {
            allGenres.push_back(d->mSelectGenreForArtistQuery.record().value(0).toString());
        }

        newArtist.setGenres(allGenres);

        d->mSelectGenreForArtistQuery.finish();

        result.push_back(newArtist);
    }

    allPeopleQuery.finish();

    transactionResult = finishTransaction();
    if (!transactionResult) {
        return result;
    }

    return result;
}

bool DatabaseInterface::prepareQuery(QSqlQuery &query, const QString &queryText) const
{
    query.setForwardOnly(true);
    return query.prepare(queryText);
}

void DatabaseInterface::updateAlbumArtist(qulonglong albumId, const QString &title,
                                          const QString &albumPath,
                                          const QString &artistName)
{
    d->mUpdateAlbumArtistQuery.bindValue(QStringLiteral(":albumId"), albumId);
    insertArtist(artistName);
    d->mUpdateAlbumArtistQuery.bindValue(QStringLiteral(":artistName"), artistName);

    auto queryResult = d->mUpdateAlbumArtistQuery.exec();

    if (!queryResult || !d->mUpdateAlbumArtistQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::updateAlbumArtist" << d->mUpdateAlbumArtistQuery.lastQuery();
        qDebug() << "DatabaseInterface::updateAlbumArtist" << d->mUpdateAlbumArtistQuery.boundValues();
        qDebug() << "DatabaseInterface::updateAlbumArtist" << d->mUpdateAlbumArtistQuery.lastError();

        d->mUpdateAlbumArtistQuery.finish();

        return;
    }

    d->mUpdateAlbumArtistQuery.finish();

    d->mUpdateAlbumArtistInTracksQuery.bindValue(QStringLiteral(":albumTitle"), title);
    d->mUpdateAlbumArtistInTracksQuery.bindValue(QStringLiteral(":albumPath"), albumPath);
    d->mUpdateAlbumArtistInTracksQuery.bindValue(QStringLiteral(":artistName"), artistName);

    queryResult = d->mUpdateAlbumArtistInTracksQuery.exec();

    if (!queryResult || !d->mUpdateAlbumArtistInTracksQuery.isActive()) {
        Q_EMIT databaseError();

        qDebug() << "DatabaseInterface::updateAlbumArtist" << d->mUpdateAlbumArtistInTracksQuery.lastQuery();
        qDebug() << "DatabaseInterface::updateAlbumArtist" << d->mUpdateAlbumArtistInTracksQuery.boundValues();
        qDebug() << "DatabaseInterface::updateAlbumArtist" << d->mUpdateAlbumArtistInTracksQuery.lastError();

        d->mUpdateAlbumArtistInTracksQuery.finish();

        return;
    }

    d->mUpdateAlbumArtistInTracksQuery.finish();
}


#include "moc_databaseinterface.cpp"
