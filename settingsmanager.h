#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

// Qt
#include <QSettings>

// Local
#include "base.h"

class SettingsManager : public Base
{
    Q_OBJECT

private:
    static const QString m_SOURCE_PATH_MODEL_KEY;
    static const QStringList m_FALL_BACK_SOURCE_PATH_MODEL;
    static const QString m_SECURE_PATH_MODEL_KEY;
    static const QStringList m_FALL_BACK_SECURE_PATH_MODEL;
    static const QString m_DESTINATION_PATH_MODEL_KEY;
    static const QStringList m_FALL_BACK_DESTINATION_PATH_MODEL;
    static const QString m_OPEN_FILE_KEY;
    static const bool m_FALL_BACK_OPEN_FILE;
    QSettings m_settings;

public:
    explicit SettingsManager(QObject *parent = NULL);
    ~SettingsManager();

private:
    QStringList sourcePathModel() const;
    QStringList securePathModel() const;
    QStringList destinationPathModel() const;
    bool openFile() const;

public slots:
    void onSourcePathModel(QStringList *sourcePathModel);
    void onSourcePathModelChanged(const QStringList &sourcePathModel);
    void onSecurePathModel(QStringList *securePathModel);
    void onSecurePathModelChanged(const QStringList &securePathModel);
    void onDestinationPathModel(QStringList *destinationPathModel);
    void onDestinationPathModelChanged(const QStringList &destinationPathModel);
    void onOpenFile(bool *openFile);
    void onOpenFileChanged(bool openFile);
};

#endif // SETTINGSMANAGER_H
