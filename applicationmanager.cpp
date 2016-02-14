// Local
#include "applicationmanager.h"

ApplicationManager::ApplicationManager(QObject *parent) :
    Base("AM", parent),
    m_sourceManager(this),
    m_secureManager(this),
    m_destinationManager(this),
    m_fileManager(this),
    m_secureFileManager(this),
    m_encryptionManager(this),
    m_decryptionManager(this),
    m_processManager(this),
    m_settingsManager(this),
    m_windowManager(this)
{
    QObject::connect(&m_sourceManager, SIGNAL(sourcePathModel(QStringList*)), &m_settingsManager, SLOT(onSourcePathModel(QStringList*)));
    QObject::connect(&m_sourceManager, SIGNAL(sourcePathModelChanged(QStringList)), &m_settingsManager, SLOT(onSourcePathModelChanged(QStringList)));
    QObject::connect(&m_sourceManager, SIGNAL(sourcePathChanged(QString)), &m_fileManager, SLOT(onSourcePathChanged(QString)));
    QObject::connect(&m_sourceManager, SIGNAL(isSourcePathUrlValidChanged(bool)), &m_encryptionManager, SLOT(onIsSourcePathUrlValidChanged(bool)));

    QObject::connect(&m_secureManager, SIGNAL(securePathModel(QStringList*)), &m_settingsManager, SLOT(onSecurePathModel(QStringList*)));
    QObject::connect(&m_secureManager, SIGNAL(securePathModelChanged(QStringList)), &m_settingsManager, SLOT(onSecurePathModelChanged(QStringList)));
    QObject::connect(&m_secureManager, SIGNAL(securePathChanged(QString)), &m_secureFileManager, SLOT(onSecurePathChanged(QString)));
    QObject::connect(&m_secureManager, SIGNAL(isSecurePathUrlValidChanged(bool)), &m_decryptionManager, SLOT(onIsSecurePathUrlValidChanged(bool)));

    QObject::connect(&m_destinationManager, SIGNAL(destinationPathModel(QStringList*)), &m_settingsManager, SLOT(onDestinationPathModel(QStringList*)));
    QObject::connect(&m_destinationManager, SIGNAL(destinationPathModelChanged(QStringList)), &m_settingsManager, SLOT(onDestinationPathModelChanged(QStringList)));
    QObject::connect(&m_destinationManager, SIGNAL(isDestinationPathUrlValidChanged(bool)), &m_encryptionManager, SLOT(onIsDestinationPathUrlValidChanged(bool)));
    QObject::connect(&m_destinationManager, SIGNAL(isDestinationPathUrlValidChanged(bool)), &m_decryptionManager, SLOT(onIsDestinationPathUrlValidChanged(bool)));

    QObject::connect(&m_fileManager, SIGNAL(sourcePath(QString*)), &m_sourceManager, SLOT(onSourcePath(QString*)));

    QObject::connect(&m_secureFileManager, SIGNAL(securePath(QString*)), &m_secureManager, SLOT(onSecurePath(QString*)));

    QObject::connect(&m_encryptionManager, SIGNAL(isSourcePathUrlValid(bool*)), &m_sourceManager, SLOT(onIsSourcePathUrlValid(bool*)));
    QObject::connect(&m_encryptionManager, SIGNAL(isDestinationPathUrlValid(bool*)), &m_destinationManager, SLOT(onIsDestinationPathUrlValid(bool*)));
    QObject::connect(&m_encryptionManager, SIGNAL(sourcePath(QString*)), &m_sourceManager, SLOT(onSourcePath(QString*)));
    QObject::connect(&m_encryptionManager, SIGNAL(destinationPath(QString*)), &m_destinationManager, SLOT(onDestinationPath(QString*)));
    QObject::connect(&m_encryptionManager, SIGNAL(inputFiles(QStringList*)), &m_fileManager, SLOT(onInputFiles(QStringList*)));
    QObject::connect(&m_encryptionManager, SIGNAL(overwriteOutputFiles(bool*)), &m_fileManager, SLOT(onOverwriteOutputFiles(bool*)));
    QObject::connect(&m_encryptionManager, SIGNAL(stateChanged(Enums::ProcessState)), &m_processManager, SLOT(onEncryptionStateChanged(Enums::ProcessState)));

    QObject::connect(&m_decryptionManager, SIGNAL(isSecurePathUrlValid(bool*)), &m_secureManager, SLOT(onIsSecurePathUrlValid(bool*)));
    QObject::connect(&m_decryptionManager, SIGNAL(isDestinationPathUrlValid(bool*)), &m_destinationManager, SLOT(onIsDestinationPathUrlValid(bool*)));
    QObject::connect(&m_decryptionManager, SIGNAL(securePath(QString*)), &m_secureManager, SLOT(onSecurePath(QString*)));
    QObject::connect(&m_decryptionManager, SIGNAL(destinationPath(QString*)), &m_destinationManager, SLOT(onDestinationPath(QString*)));
    QObject::connect(&m_decryptionManager, SIGNAL(inputFiles(QStringList*)), &m_secureFileManager, SLOT(onInputFiles(QStringList*)));
    QObject::connect(&m_decryptionManager, SIGNAL(overwriteOutputFiles(bool*)), &m_secureFileManager, SLOT(onOverwriteOutputFiles(bool*)));
    QObject::connect(&m_decryptionManager, SIGNAL(stateChanged(Enums::ProcessState)), &m_processManager, SLOT(onDecryptionStateChanged(Enums::ProcessState)));

    QObject::connect(&m_processManager, SIGNAL(pauseEncryption()), &m_encryptionManager, SLOT(onPause()));
    QObject::connect(&m_processManager, SIGNAL(pauseDecryption()), &m_decryptionManager, SLOT(onPause()));
    QObject::connect(&m_processManager, SIGNAL(resumeEncryption()), &m_encryptionManager, SLOT(onResume()));
    QObject::connect(&m_processManager, SIGNAL(resumeDecryption()), &m_decryptionManager, SLOT(onResume()));
    QObject::connect(&m_processManager, SIGNAL(stopEncryption()), &m_encryptionManager, SLOT(onStop()));
    QObject::connect(&m_processManager, SIGNAL(stopDecryption()), &m_decryptionManager, SLOT(onStop()));

    this->debug("Application manager created");
}

ApplicationManager::~ApplicationManager()
{
    this->debug("Application manager disposed of");
}

void ApplicationManager::initialize()
{
    m_sourceManager.initialize();
    m_secureManager.initialize();
    m_destinationManager.initialize();
    m_fileManager.initialize();
    m_secureFileManager.initialize();
    m_encryptionManager.initialize();
    m_decryptionManager.initialize();
    m_processManager.initialize();

    this->debug("Initialized");
}

SourceManager *ApplicationManager::sourceManager()
{
    return &m_sourceManager;
}

SecureManager *ApplicationManager::secureManager()
{
    return &m_secureManager;
}

DestinationManager *ApplicationManager::destinationManager()
{
    return &m_destinationManager;
}

FileManager *ApplicationManager::fileManager()
{
    return &m_fileManager;
}

SecureFileManager *ApplicationManager::secureFileManager()
{
    return &m_secureFileManager;
}

EncryptionManager *ApplicationManager::encryptionManager()
{
    return &m_encryptionManager;
}

DecryptionManager *ApplicationManager::decryptionManager()
{
    return &m_decryptionManager;
}

ProcessManager *ApplicationManager::processManager()
{
    return &m_processManager;
}

WindowManager *ApplicationManager::windowManager()
{
    return &m_windowManager;
}

void ApplicationManager::onCompleted()
{
    this->debug("GUI Completed");
}
