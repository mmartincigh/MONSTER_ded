#ifndef APPLICATIONMANAGER_H
#define APPLICATIONMANAGER_H

// Qt
#include <QObject>
#include <QDebug>

// Local
#include "base.h"
#include "sourcemanager.h"
#include "destinationmanager.h"
#include "filemanager.h"
#include "encryptionmanager.h"
#include "decryptionmanager.h"
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
    DecryptionManager m_decryptionManager;
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
    DecryptionManager *decryptionManager();
    WindowManager *windowManager();

public slots:
    void onCompleted();
};

#endif // APPLICATIONMANAGER_H
