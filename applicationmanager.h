#ifndef APPLICATIONMANAGER_H
#define APPLICATIONMANAGER_H

// Qt
#include <QObject>
#include <QDebug>

// Local
#include "base.h"
#include "sourcepathmanager.h"
#include "securepathmanager.h"
#include "destinationpathmanager.h"
#include "sourcefilemanager.h"
#include "securefilemanager.h"
#include "encryptionmanager.h"
#include "decryptionmanager.h"
#include "processmanager.h"
#include "statsmanager.h"
#include "settingsmanager.h"
#include "windowmanager.h"
#include "systemtrayiconmanager.h"

class ApplicationManager : public Base
{
    Q_OBJECT

private:
    SourcePathManager m_sourcePathManager;
    SecurePathManager m_securePathManager;
    DestinationPathManager m_destinationPathManager;
    SourceFileManager m_sourceFileManager;
    SecureFileManager m_secureFileManager;
    EncryptionManager m_encryptionManager;
    DecryptionManager m_decryptionManager;
    ProcessManager m_processManager;
    StatsManager m_statsManager;
    SettingsManager m_settingsManager;
    WindowManager m_windowManager;
    SystemTrayIconManager m_systemTrayIconManager;
    QStringList m_arguments;

public:
    explicit ApplicationManager(const QString &applicationDirPath, const QStringList &arguments, QObject *parent = NULL);
    ~ApplicationManager();

public:
    void initialize();
    SourcePathManager *sourcePathManager();
    SecurePathManager *securePathManager();
    DestinationPathManager *destinationPathManager();
    SourceFileManager *sourceFileManager();
    SecureFileManager *secureFileManager();
    EncryptionManager *encryptionManager();
    DecryptionManager *decryptionManager();
    ProcessManager *processManager();
    StatsManager *statsManager();
    WindowManager *windowManager();
    SystemTrayIconManager *systemTrayIconManager();

private:
    void parseArguments(QStringList &arguments);

public slots:
    void onGuiCompleted();
    void onMessageReceived(const QString &message);
};

#endif // APPLICATIONMANAGER_H
