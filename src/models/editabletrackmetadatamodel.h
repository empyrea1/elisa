/*
   SPDX-FileCopyrightText: 2020 (c) Matthieu Gallien <matthieu_gallien@yahoo.fr>

   SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef EDITABLETRACKMETADATAMODEL_H
#define EDITABLETRACKMETADATAMODEL_H

#include "elisaLib_export.h"

#include "trackmetadatamodel.h"

class ELISALIB_EXPORT EditableTrackMetadataModel : public TrackMetadataModel
{
    Q_OBJECT

    Q_PROPERTY(bool isDataValid
               READ isDataValid
               NOTIFY isDataValidChanged)

    Q_PROPERTY(QString errorMessage
               READ errorMessage
               NOTIFY errorMessageChanged)

    Q_PROPERTY(bool isDirty
               READ isDirty
               NOTIFY isDirtyChanged)

    Q_PROPERTY(QStringList extraMetadata
               READ extraMetadata
               NOTIFY extraMetadataChanged)

public:
    enum EditableColumnRoles
    {
        ReadOnlyRole = TrackMetadataModel::ItemTypeRole + 1,
        RemovableFieldRole,
    };

    Q_ENUM(EditableColumnRoles)

    explicit EditableTrackMetadataModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool isDataValid() const
    {
        return mIsDataValid;
    }

    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    bool isDirty() const
    {
        return mIsDirty;
    }

    QString errorMessage() const
    {
        return mErrorMessage;
    }

    QHash<int, QByteArray> roleNames() const override;

    QStringList extraMetadata() const
    {
        return mExtraMetadata;
    }

Q_SIGNALS:
    void isDataValidChanged();

    void isDirtyChanged();

    void errorMessageChanged();

    void saveTrackModified(const DataTypes::ListTrackDataType &trackDataType, const QHash<QString, QUrl> &covers);

    void deleteRadioData(qulonglong radioId);

    void extraMetadataChanged();

public Q_SLOTS:

    void saveData();

    void deleteRadio();

    void removeData(int index);

    void addData(const QString &name);

protected:

    void fillDataFromTrackData(const TrackMetadataModel::TrackDataType &trackData,
                               const QList<DataTypes::ColumnsRoles> &fieldsForTrack) override;

    void filterDataFromTrackData() override;

    void fillLyricsDataFromTrack() override;

    void fillDataForNewRadio() override;

    void initialize(MusicListenersManager *newManager,
                    DatabaseInterface *trackDatabase) override;
private:

    void validData();

    void updateExtraMetadata();

    void modelHasBeenModified();

    bool mIsNewRadio = false;

    bool mIsDataValid = false;

    bool mIsDirty = false;

    QString mErrorMessage;

    QStringList mExtraMetadata;
};



#endif // EDITABLETRACKMETADATAMODEL_H
