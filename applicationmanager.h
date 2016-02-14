#ifndef APPLICATIONMANAGER_H
#define APPLICATIONMANAGER_H

// Qt
#include <QObject>
#include <QDebug>

// Local
#include "base.h"
#include "sourcemanager.h"
#include "securemanager.h"
#include "destinationmanager.h"
#include "filemanager.h"
#include "securefilemanager.h"
#include "encryptionmanager.h"
#include "decryptionmanager.h"
#include "processmanager.h"
#include "settingsmanager.h"
#include "windowmanager.h"

class ApplicationManager : public Base
{
    Q_OBJECT

private:
    SourceManager m_sourceManager;
    SecureManager m_secureManager;
    DestinationManager m_destinationManager;
    FileManager m_fileManager;
    SecureFileManager m_secureFileManager;
    EncryptionManager m_encryptionManager;
    DecryptionManager m_decryptionManager;
    ProcessManager m_processManager;
    SettingsManager m_settingsManager;
    WindowManager m_windowManager;

public:
    explicit ApplicationManager(QObject *parent = NULL);
    ~ApplicationManager();

public:
    void initialize();
    SourceManager *sourceManager();
    SecureManager *secureManager();
    DestinationManager *destinationManager();
    FileManager *fileManager();
    SecureFileManager *secureFileManager();
    EncryptionManager *encryptionManager();
    DecryptionManager *decryptionManager();
    ProcessManager *processManager();
    WindowManager *windowManager();

public slots:
    void onCompleted();
};

#endif // APPLICATIONMANAGER_H
