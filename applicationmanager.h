#ifndef APPLICATIONMANAGER_H
#define APPLICATIONMANAGER_H

#include <QObject>
#include <QDebug>

#include "base.h"
#include "sourcemanager.h"
#include "destinationmanager.h"
#include "filemanager.h"
#include "encryptionmanager.h"
#include "settingsmanager.h"
#include "windowmanager.h"

class ApplicationManager : public Base
{
    Q_OBJECT

private:
    SourceManager m_sourceManager;
    DestinationManager m_destinationManager;
    FileManager m_fileManager;
    EncryptionManager m_encryptionManager;
    SettingsManager m_settingsManager;
    WindowManager m_windowManager;

public:
    explicit ApplicationManager(QObject *parent = NULL);
    ~ApplicationManager();

public:
    void initialize();
    SourceManager *sourceManager();
    DestinationManager *destinationManager();
    FileManager *fileManager();
    EncryptionManager *encryptionManager();
    WindowManager *windowManager();

public slots:
    void onCompleted();
};

#endif // APPLICATIONMANAGER_H
